"""Executable reference model for per-trip fuel measurement, pricing,
settlement eligibility and lifecycle behaviour (v0.18 design).

Pure Python standard library. No game offsets, no hooks, no live billing, no
money-moving. Passing tests prove only this model's internal behaviour, not
production correctness or real-game association.

State separation, per vehicle:
  - tank baseline availability            (baseline_valid / baseline_tons)
  - window anchored at a readable event    (window_open / window_usable)
  - window usability                       (window_usable)
  - persistent contamination reasons      (contamination: set[str])
  - observed depletion                    (observed_depletion, tonnes)
  - association quality                   (association)
  - freight eligibility                   (freight_eligible -- independent input)
  - actual pricing source                 (pricing_source)
  - requested / simulated / refused / actual payment outcomes
  - movement/settlement lifecycle + dedup identity

Window semantics:
  - A readable event with a usable slot anchors/opens a window (sets baseline).
  - Each subsequent readable event measures the interval delta from the
    baseline and accumulates it into observed_depletion; the baseline then
    advances. Loads accumulate; deliveries accumulate then settle the FULL
    delivery-to-delivery window and close it.
  - A first readable LOAD does NOT establish a complete delivery-window anchor
    for settlement; it only sets a baseline.
  - A first readable DELIVERY anchors the window AND may settle via fallback
    pricing (CPI/constant) if freight-eligible and fallback is enabled.
  - An unreadable event closes the old window, cannot anchor a usable next one.
    Baseline recovery never repairs missing coverage of the closed window.
  - Contamination persists until the window closes.
  - Interval measurement, window usability, and invoice eligibility are three
    separate concerns.

Pricing policy:
  - trip_tariff=1 (measured):
      has_depletion + fuel_price -> ACTUAL_QUANTITY
         = depletion * fuel_price * (1+margin) + wear
      no_depletion + fallback.cpi -> CPI = baskets(cargo) * cpi_basket
      no_depletion + fallback.constant -> CONSTANT
         = constant_fuel_tariff * density * fuel_price * (1+margin) + wear
      no_depletion + no fallback -> NONE (unpriceable; fail-closed)
  - trip_tariff=0 (value):
      cargo_tons * unit_price * freight_share -> VALUE
      independent of fuel/CPI/fallback; requires cargo + unit price.

  Wear = wear_litres * diesel_density * fuel_price, applied once per trip cycle.
  CPI cargo scaling uses cargo tonnes, NOT fuel-depletion tonnes.
  Constant tariff is in litres: constant_fuel_tariff * density = tonnes.

Fallback configuration (explicit):
  - fallback_mode: "none" | "cpi" | "constant" | "both"
  - fallback_enabled derived from fallback_mode + available parameters.

Settlement policy (single policy for immediate and timeout invoices):
  - VERIFIED + freight_eligible -> may pay live (subject to treasury/min/cap)
  - HEURISTIC -> simulate only (dry-run); refuse live
  - AMBIGUOUS/UNMATCHED -> cannot pay (refuse association), even in dry-run
  - source == NONE -> refuse unpriceable (never invent)
  - amount <= 0 or < min_payment -> refuse minimum
  - treasury insufficient -> refuse treasury (cumulative tracking)
  - daily cap exceeded -> refuse cap (global across vehicles)

Actual and simulated amounts are disjoint; refusals are distinct from both.

Lifecycle:
  - Pending-invoice timeout is separate from vehicle-window expiry.
  - Vehicle-window expiry without an invoice creates NO payment.
  - Expiry checks real stored timestamps against an injected clock.
  - Identity is session-scoped; save/load cannot correlate reused raw identities.
  - Rehoming cannot silently transfer old-owner consumption.
  - A generated observation ID is not proof of movement deduplication.
  - Terminal outcomes: settled_actual, settled_simulated, refused, skipped.
  - Dedup ring eviction cannot turn a settled invoice into a new one; exactly-once
    cannot be guaranteed under bounded retention; fail closed.
  - save/load invalidates tank baselines, carrier identities, dedup ring, and
    pending invoices before any consumer runs.
"""

from __future__ import annotations

import math
from dataclasses import dataclass, field
from enum import Enum
from typing import Optional


class AssocQuality(Enum):
    VERIFIED = "verified"
    HEURISTIC = "heuristic"
    AMBIGUOUS = "ambiguous"
    UNMATCHED = "unmatched"


class PricingSource(Enum):
    NONE = "none"
    ACTUAL_QUANTITY = "actual_quantity"
    CONSTANT = "constant"
    VALUE = "value"
    CPI = "cpi"


class RefusalReason(Enum):
    NONE = "none"
    MINIMUM = "minimum"
    TREASURY = "treasury"
    CAP = "cap"
    ASSOCIATION = "association"
    UNPRICEABLE = "unpriceable"
    INELIGIBLE = "ineligible"


# Terminal lifecycle outcomes
LIFECYCLE_SETTLED_ACTUAL = "settled_actual"
LIFECYCLE_SETTLED_SIMULATED = "settled_simulated"
LIFECYCLE_REFUSED = "refused"
LIFECYCLE_SKIPPED = "skipped"
LIFECYCLE_DUPLICATE = "duplicate"
LIFECYCLE_REHOMED = "rehomed"
LIFECYCLE_NO_SLOT = "no_slot"
LIFECYCLE_CLOSED_UNREADABLE = "closed_unreadable"
LIFECYCLE_MEASURED = "measured"
LIFECYCLE_ACCUMULATED = "accumulated"
LIFECYCLE_ALREADY_SETTLED = "already_settled"
LIFECYCLE_UNUSABLE_WINDOW = "unusable_window"
LIFECYCLE_EXPIRED_ACTUAL = "expired_actual"
LIFECYCLE_EXPIRED_SIMULATED = "expired_simulated"
LIFECYCLE_EXPIRED_REFUSED = "expired_refused"


@dataclass
class ModelParams:
    trip_tariff: int = 1
    freight_share: float = 0.08
    tariff_margin: float = 0.35
    wear_litres: float = 5.0
    diesel_density: float = 0.001  # tonnes per litre (game uses 1 kg/L)
    constant_fuel_tariff: float = 0.0  # configured per-trip constant (litres)
    min_payment: float = 0.01
    daily_pay_cap: float = 5000.0  # 0 = no cap
    fuel_price: Optional[float] = None  # None/unresolved -> fail-closed in trip mode
    cpi_basket: Optional[float] = None  # None -> CPI unavailable
    baskets_per_trip: float = 0.80
    baskets_per_tonne: float = 0.02
    dry_run: bool = True
    refill_threshold: float = 0.001  # tonnes; only sub-kilo noise is ignored
    fallback_mode: str = "both"  # "none" | "cpi" | "constant" | "both"
    ttl_seconds: float = 2.0  # pending-invoice timeout
    treasury_balance: float = 0.0
    dedup_capacity: int = 16


@dataclass
class TankEvent:
    vehicle_id: str
    kind: str  # "delivery" or "load"
    tank_tons: Optional[float]
    readable: bool
    association: AssocQuality = AssocQuality.UNMATCHED
    carrier_id: Optional[str] = None
    cargo_tons: Optional[float] = None
    unit_price: Optional[float] = None
    # INDEPENDENT adapter input: freight eligibility is NOT inferred from
    # tank readability + positive cargo.
    freight_eligible: bool = False
    timestamp: float = 0.0
    event_id: Optional[str] = None


@dataclass
class EventOutcome:
    vehicle_id: str
    kind: str
    timestamp: float
    duplicate: bool = False
    lifecycle: str = "ignored"
    # interval measurement
    baseline_was_valid: bool = False
    measured_depletion: Optional[float] = None
    # window usability
    window_usable: bool = False
    contamination: set = field(default_factory=set)
    # invoice eligibility and settlement
    pricing_source: PricingSource = PricingSource.NONE
    requested_amount: float = 0.0
    simulated_amount: float = 0.0
    actual_amount: float = 0.0
    refused: bool = False
    refusal_reason: RefusalReason = RefusalReason.NONE
    freight_eligible: bool = False
    association_quality: AssocQuality = AssocQuality.UNMATCHED


@dataclass
class VehicleState:
    vehicle_id: str
    slot_available: bool = True
    baseline_tons: Optional[float] = None
    baseline_valid: bool = False
    window_open: bool = False
    window_usable: bool = False
    window_opened_at: Optional[float] = None
    contamination: set = field(default_factory=set)
    observed_depletion: float = 0.0
    wear_charged: bool = False
    association: AssocQuality = AssocQuality.UNMATCHED
    carrier_id: Optional[str] = None
    freight_eligible: bool = False  # independent adapter input
    actual_paid_today: float = 0.0
    settled: bool = False  # current window settled?
    lifecycle: str = "fresh"


@dataclass
class PendingInvoice:
    """A requested invoice awaiting settlement within its timeout window."""
    invoice_id: str
    vehicle_id: str
    amount: float
    source: PricingSource
    association: AssocQuality
    freight_eligible: bool
    requested_at: float
    state: str = "pending"  # pending / settled / expired / refused


def _is_finite(value: Optional[float]) -> bool:
    """Return True if value is a finite float (not None, not NaN, not inf)."""
    if value is None:
        return False
    return math.isfinite(value)


class FuelAccountingModel:
    """Reference model. Clock is injected as a float (seconds); tests advance
    it explicitly so behaviour is deterministic."""

    def __init__(self, clock: float = 0.0, params: Optional[ModelParams] = None):
        self.clock = float(clock)
        self.params = params or ModelParams()
        self._vehicles: dict[str, VehicleState] = {}
        self._dedup: list[str] = []  # FIFO ring of processed event_ids
        self._pending_invoices: dict[str, PendingInvoice] = {}
        self._pending_order: list[str] = []  # insertion order for eviction
        self._simulated_total = 0.0
        self._actual_total = 0.0
        self._refused_total = 0.0
        self._treasury_remaining = float(self.params.treasury_balance)
        self._global_actual_paid_today = 0.0  # daily cap is global across vehicles

    # ------------------------------------------------------------------ clock
    def advance(self, seconds: float) -> None:
        self.clock += float(seconds)

    # --------------------------------------------------------------- vehicle
    def _vehicle(self, vehicle_id: str) -> VehicleState:
        vs = self._vehicles.get(vehicle_id)
        if vs is None:
            vs = VehicleState(vehicle_id=vehicle_id)
            self._vehicles[vehicle_id] = vs
        return vs

    def _invalidate_vehicle(self, vs: VehicleState, keep_carrier: bool) -> None:
        vs.baseline_tons = None
        vs.baseline_valid = False
        vs.window_open = False
        vs.window_usable = False
        vs.window_opened_at = None
        vs.observed_depletion = 0.0
        vs.wear_charged = False
        vs.settled = False
        vs.lifecycle = "fresh"
        if not keep_carrier:
            vs.carrier_id = None
            vs.association = AssocQuality.UNMATCHED

    def _close_window(self, vs: VehicleState) -> None:
        """Close the current measurement window; contamination is cleared
        only here (persists until close)."""
        vs.window_open = False
        vs.window_usable = False
        vs.window_opened_at = None
        vs.observed_depletion = 0.0
        vs.wear_charged = False
        vs.contamination.clear()
        vs.lifecycle = "fresh"

    # -------------------------------------------------------------- fallback
    def _fallback_enabled(self) -> bool:
        """Explicit fallback configuration. Returns True if any fallback
        source is configured and available."""
        p = self.params
        if p.fallback_mode == "none":
            return False
        if p.fallback_mode in ("cpi", "both"):
            if p.cpi_basket is not None and p.cpi_basket > 0.0:
                return True
        if p.fallback_mode in ("constant", "both"):
            if (
                p.constant_fuel_tariff > 0.0
                and _is_finite(p.fuel_price)
                and p.fuel_price > 0.0
            ):
                return True
        return False

    # -------------------------------------------------------------- pricing
    def _price(
        self,
        burn_tons: float,
        cargo_tons: Optional[float],
        unit_price: Optional[float],
        has_depletion: bool,
    ) -> tuple[float, PricingSource]:
        p = self.params
        if p.trip_tariff == 0:
            # Value pricing: independent of fuel/CPI/fallback.
            if (
                _is_finite(cargo_tons)
                and _is_finite(unit_price)
                and cargo_tons > 0.0
                and unit_price > 0.0
            ):
                return cargo_tons * unit_price * p.freight_share, PricingSource.VALUE
            return 0.0, PricingSource.NONE

        # Trip tariff (measured): observed depletion * fuel price per tonne.
        if has_depletion and _is_finite(p.fuel_price) and p.fuel_price > 0.0:
            fuel_cost = burn_tons * p.fuel_price
            wear_cost = p.wear_litres * p.diesel_density * p.fuel_price
            return (
                fuel_cost * (1.0 + p.tariff_margin) + wear_cost,
                PricingSource.ACTUAL_QUANTITY,
            )

        # Fallback path: no measured depletion.
        if self._fallback_enabled():
            # CPI fallback: cargo-scaled baskets, NOT depletion-scaled.
            if p.fallback_mode in ("cpi", "both") and p.cpi_basket is not None and p.cpi_basket > 0.0:
                baskets = p.baskets_per_trip + p.baskets_per_tonne * max(
                    cargo_tons if _is_finite(cargo_tons) else 0.0, 0.0
                )
                return baskets * p.cpi_basket, PricingSource.CPI
            # Constant fuel tariff fallback: litres * density = tonnes.
            if p.fallback_mode in ("constant", "both") and _is_finite(p.fuel_price) and p.fuel_price > 0.0 and p.constant_fuel_tariff > 0.0:
                fuel_cost = p.constant_fuel_tariff * p.diesel_density * p.fuel_price
                wear_cost = p.wear_litres * p.diesel_density * p.fuel_price
                return (
                    fuel_cost * (1.0 + p.tariff_margin) + wear_cost,
                    PricingSource.CONSTANT,
                )

        return 0.0, PricingSource.NONE

    # ------------------------------------------------------------- settlement
    def _settle(
        self,
        amount: float,
        source: PricingSource,
        assoc: AssocQuality,
        freight_eligible: bool,
        vs: VehicleState,
    ) -> tuple[float, float, bool, RefusalReason, float]:
        """Single settlement policy for immediate and timeout invoices.

        Returns (actual, simulated, refused, reason, requested).
        Actual and simulated never overlap. Heuristic never pays live."""
        p = self.params
        requested = amount

        if not freight_eligible:
            return 0.0, 0.0, True, RefusalReason.INELIGIBLE, requested
        if source == PricingSource.NONE:
            # Fail-closed: never invent a figure.
            return 0.0, 0.0, True, RefusalReason.UNPRICEABLE, requested
        if not _is_finite(amount) or amount <= 0.0 or amount < p.min_payment:
            return 0.0, 0.0, True, RefusalReason.MINIMUM, requested
        if assoc in (AssocQuality.AMBIGUOUS, AssocQuality.UNMATCHED):
            return 0.0, 0.0, True, RefusalReason.ASSOCIATION, requested
        if assoc == AssocQuality.HEURISTIC:
            if p.dry_run:
                return 0.0, amount, False, RefusalReason.NONE, requested
            return 0.0, 0.0, True, RefusalReason.ASSOCIATION, requested
        # VERIFIED
        if p.dry_run:
            return 0.0, amount, False, RefusalReason.NONE, requested
        # live: treasury + cap (treasury_remaining tracks cumulative spending;
        # daily cap is global across vehicles)
        if not _is_finite(self._treasury_remaining) or self._treasury_remaining < amount:
            return 0.0, 0.0, True, RefusalReason.TREASURY, requested
        if (
            p.daily_pay_cap > 0.0
            and self._global_actual_paid_today + amount > p.daily_pay_cap
        ):
            return 0.0, 0.0, True, RefusalReason.CAP, requested
        return amount, 0.0, False, RefusalReason.NONE, requested

    def _apply_payment(self, amount: float) -> None:
        """Deduct an actual payment from the remaining treasury (cumulative)
        and increment the global daily-paid counter (cap is global)."""
        self._treasury_remaining -= amount
        self._global_actual_paid_today += amount

    # ------------------------------------------------------------- dedup
    def _is_duplicate(self, event_id: Optional[str]) -> bool:
        if event_id is None:
            return False
        return event_id in self._dedup

    def _record_dedup(self, event_id: Optional[str]) -> None:
        if event_id is None:
            return
        self._dedup.append(event_id)
        cap = self.params.dedup_capacity
        while len(self._dedup) > cap:
            self._dedup.pop(0)

    # -------------------------------------------------------------- events
    def record_event(self, ev: TankEvent) -> EventOutcome:
        p = self.params
        out = EventOutcome(
            vehicle_id=ev.vehicle_id,
            kind=ev.kind,
            timestamp=ev.timestamp,
            association_quality=ev.association,
        )

        # --- finite-value validation ---
        if ev.tank_tons is not None and not _is_finite(ev.tank_tons):
            ev.readable = False  # NaN/inf tank reading is unreadable
        if ev.cargo_tons is not None and not _is_finite(ev.cargo_tons):
            ev.cargo_tons = None
        if ev.unit_price is not None and not _is_finite(ev.unit_price):
            ev.unit_price = None

        # --- duplicate-event identity ---
        if self._is_duplicate(ev.event_id):
            out.duplicate = True
            out.lifecycle = LIFECYCLE_DUPLICATE
            return out
        self._record_dedup(ev.event_id)

        vs = self._vehicle(ev.vehicle_id)
        out.association_quality = ev.association

        # --- rehoming: never charge old-owner consumption to new owner ---
        if ev.carrier_id is not None:
            if vs.carrier_id is not None and ev.carrier_id != vs.carrier_id:
                self._invalidate_vehicle(vs, keep_carrier=False)
                vs.carrier_id = ev.carrier_id
                vs.association = ev.association
                out.lifecycle = LIFECYCLE_REHOMED
            elif vs.carrier_id is None:
                vs.carrier_id = ev.carrier_id
                vs.association = ev.association
            else:
                vs.association = ev.association
        else:
            vs.association = ev.association

        # Freight eligibility is an INDEPENDENT adapter input, not inferred
        # from tank readability + cargo.
        vs.freight_eligible = bool(ev.freight_eligible)
        out.freight_eligible = vs.freight_eligible

        # --- can we measure the tank? ---
        # Value pricing (trip_tariff=0) needs no tank reading, so it can
        # anchor/settle without a usable slot. Trip-tariff requires measurement.
        can_measure = vs.slot_available
        value_pricing_only = p.trip_tariff == 0
        if not vs.slot_available and not value_pricing_only:
            out.lifecycle = LIFECYCLE_NO_SLOT
            out.window_usable = False

        # --- readability (event carries valid tank data) ---
        readable = (
            ev.readable
            and ev.tank_tons is not None
            and math.isfinite(ev.tank_tons)
            and ev.tank_tons >= 0.0
        )
        # Value pricing needs no tank reading; treat as always measurable.
        measurable = readable if p.trip_tariff != 0 else True
        can_measure_for_valuation = can_measure if p.trip_tariff != 0 else True

        # ==================================================================
        # CONCERN 1: INTERVAL MEASUREMENT (per-event delta from baseline)
        # ==================================================================
        measured: Optional[float] = None
        # Record whether the window already existed before this event; a
        # first observation anchors but does not settle.
        window_existed_before = vs.window_open
        if measurable and can_measure_for_valuation:
            if p.trip_tariff != 0:
                level = ev.tank_tons
                out.baseline_was_valid = vs.baseline_valid
                if vs.baseline_valid and vs.window_usable:
                    drop = vs.baseline_tons - level
                    if level > vs.baseline_tons + p.refill_threshold:
                        vs.contamination.add("refill")
                        out.contamination.add("refill")
                        measured = None  # refill: not burn, not charged negative
                    elif drop > p.refill_threshold:
                        measured = drop
                        vs.observed_depletion += measured
                    else:
                        measured = 0.0  # noise / zero depletion
                # Advance baseline and (re)anchor the window.
                vs.baseline_tons = level
                vs.baseline_valid = True
            if not vs.window_open:
                vs.window_open = True
                vs.window_opened_at = ev.timestamp
                vs.settled = False  # new window: not yet settled
                vs.wear_charged = False
                vs.contamination.clear()
            vs.window_usable = True
            vs.lifecycle = "open"
            out.measured_depletion = measured
            out.window_usable = True
        elif measurable and not can_measure_for_valuation:
            # Tank data readable but no slot: cannot measure this event,
            # but keep the existing window open (don't close it).
            out.measured_depletion = None
            out.window_usable = vs.window_usable
        else:
            # Unreadable: closes the old window, cannot anchor a usable next
            # one. Baseline recovery never repairs missing coverage.
            vs.window_open = False
            vs.window_usable = False
            vs.baseline_valid = False
            vs.window_opened_at = None
            vs.observed_depletion = 0.0
            out.measured_depletion = None
            out.window_usable = False
            vs.lifecycle = LIFECYCLE_CLOSED_UNREADABLE
            out.lifecycle = LIFECYCLE_CLOSED_UNREADABLE

        # If the event itself was unreadable AND we need tank data, no settlement.
        # Value pricing does not need tank data, so it continues.
        if not measurable:
            return out

        # ==================================================================
        # CONCERN 3: INVOICE ELIGIBILITY AND SETTLEMENT
        # ==================================================================
        # A delivery settles the FULL accumulated window and closes it.
        # A load accumulates but never settles.
        # A first readable DELIVERY anchors the window AND may settle via
        # fallback pricing if freight-eligible and fallback is enabled.
        # Value pricing (trip_tariff=0) needs no tank window.
        if ev.kind == "delivery" and not vs.settled:
            has_depletion = vs.observed_depletion > 0.0

            # Trip-tariff: an open measurement window (existing before this
            # event) is eligible for settlement (pricing/refusal handled
            # inside _settle/_price).
            # Value pricing: needs cargo + unit price (no tank required).
            if p.trip_tariff != 0:
                eligible = window_existed_before
            else:
                eligible = (
                    _is_finite(ev.cargo_tons)
                    and _is_finite(ev.unit_price)
                    and ev.cargo_tons > 0.0
                    and ev.unit_price > 0.0
                )

            if not eligible:
                # First delivery with no prior window: anchor only. If
                # freight-eligible and fallback-enabled, it may still settle
                # via fallback (zero-depletion pricing).
                if (
                    p.trip_tariff != 0
                    and not window_existed_before
                    and vs.freight_eligible
                    and self._fallback_enabled()
                ):
                    eligible = True
                else:
                    out.lifecycle = LIFECYCLE_MEASURED
                    return out

            # Price the full accumulated window (trip-tariff) or cargo value.
            burn_tons = vs.observed_depletion if p.trip_tariff != 0 else 0.0
            amount, source = self._price(
                burn_tons,
                ev.cargo_tons,
                ev.unit_price,
                has_depletion,
            )
            out.pricing_source = source
            out.requested_amount = amount

            actual, simulated, refused, reason, requested = self._settle(
                amount, source, ev.association, vs.freight_eligible, vs
            )
            out.simulated_amount = simulated
            out.actual_amount = actual
            out.refused = refused
            out.refusal_reason = reason

            if actual > 0.0:
                self._actual_total += actual
                self._apply_payment(actual)
                vs.actual_paid_today += actual
                out.lifecycle = LIFECYCLE_SETTLED_ACTUAL
            elif simulated > 0.0:
                self._simulated_total += simulated
                out.lifecycle = LIFECYCLE_SETTLED_SIMULATED
            elif refused:
                self._refused_total += requested
                out.lifecycle = LIFECYCLE_REFUSED
            else:
                out.lifecycle = LIFECYCLE_SKIPPED

            vs.settled = True
            # Close the window after settlement.
            vs.window_open = False
            vs.window_usable = False
            vs.window_opened_at = None
            out.window_usable = False
            vs.observed_depletion = 0.0
        elif ev.kind == "delivery" and vs.settled:
            out.lifecycle = LIFECYCLE_ALREADY_SETTLED
        elif ev.kind == "delivery" and not vs.window_usable:
            out.lifecycle = LIFECYCLE_UNUSABLE_WINDOW
        elif ev.kind == "load":
            out.lifecycle = LIFECYCLE_ACCUMULATED if readable else "unreadable"
        else:
            out.lifecycle = LIFECYCLE_MEASURED if readable else "unreadable"

        return out

    # -------------------------------------------------------------- save/load
    def save_load(self) -> None:
        """Invalidates identities and baselines before any consumer runs.
        Identity is session-scoped: save/load cannot correlate reused raw
        identities. Persistent contamination reasons describe physical
        accounting history, not identity. The dedup ring and pending invoices
        are also invalidated (session-scoped)."""
        for vs in self._vehicles.values():
            vs.baseline_tons = None
            vs.baseline_valid = False
            vs.window_open = False
            vs.window_usable = False
            vs.window_opened_at = None
            vs.observed_depletion = 0.0
            vs.wear_charged = False
            vs.settled = False
            vs.carrier_id = None
            vs.association = AssocQuality.UNMATCHED
            vs.freight_eligible = False
            vs.lifecycle = "fresh"
        # Session-scoped invalidation: clear dedup ring and pending invoices.
        self._dedup.clear()
        self._pending_invoices.clear()
        self._pending_order.clear()

    # --------------------------------------------------------- daily census
    def daily_close(self) -> None:
        """End of day: reset per-day paid counters (cap window)."""
        self._global_actual_paid_today = 0.0
        for vs in self._vehicles.values():
            vs.actual_paid_today = 0.0
            vs.settled = False

    # ------------------------------------------------------- pending invoices
    def request_pending_invoice(
        self,
        invoice_id: str,
        vehicle_id: str,
        amount: float,
        source: PricingSource,
        association: AssocQuality,
        freight_eligible: bool,
    ) -> None:
        """Register a pending invoice subject to a separate timeout TTL."""
        p = self.params
        inv = PendingInvoice(
            invoice_id=invoice_id,
            vehicle_id=vehicle_id,
            amount=amount,
            source=source,
            association=association,
            freight_eligible=freight_eligible,
            requested_at=self.clock,
        )
        self._pending_invoices[invoice_id] = inv
        self._pending_order.append(invoice_id)
        # Bound retention: evict oldest if over capacity.
        cap = p.dedup_capacity
        while len(self._pending_order) > cap:
            old_id = self._pending_order.pop(0)
            if old_id in self._pending_invoices:
                old_inv = self._pending_invoices.pop(old_id)
                # Eviction of a never-settled pending invoice: fail closed.
                old_inv.state = "expired"

    def settle_pending_invoice(
        self, invoice_id: str, vs: VehicleState
    ) -> Optional[EventOutcome]:
        """Settle a pending invoice using the same settlement policy."""
        p = self.params
        inv = self._pending_invoices.get(invoice_id)
        if inv is None or inv.state != "pending":
            return None
        # Timeout check: pending-invoice TTL separate from vehicle-window expiry.
        if self.clock - inv.requested_at > p.ttl_seconds:
            inv.state = "expired"
            out = EventOutcome(
                vehicle_id=inv.vehicle_id,
                kind="delivery",
                timestamp=self.clock,
                association_quality=inv.association,
                pricing_source=inv.source,
                requested_amount=inv.amount,
                refused=True,
                refusal_reason=RefusalReason.ASSOCIATION,
                lifecycle=LIFECYCLE_REFUSED,
            )
            self._refused_total += inv.amount
            return out

        actual, simulated, refused, reason, requested = self._settle(
            inv.amount, inv.source, inv.association, inv.freight_eligible, vs
        )
        out = EventOutcome(
            vehicle_id=inv.vehicle_id,
            kind="delivery",
            timestamp=self.clock,
            association_quality=inv.association,
            pricing_source=inv.source,
            requested_amount=inv.amount,
            simulated_amount=simulated,
            actual_amount=actual,
            refused=refused,
            refusal_reason=reason,
        )
        if actual > 0.0:
            self._actual_total += actual
            self._apply_payment(actual)
            vs.actual_paid_today += actual
            out.lifecycle = LIFECYCLE_SETTLED_ACTUAL
        elif simulated > 0.0:
            self._simulated_total += simulated
            out.lifecycle = LIFECYCLE_SETTLED_SIMULATED
        elif refused:
            self._refused_total += requested
            out.lifecycle = LIFECYCLE_REFUSED
        else:
            out.lifecycle = LIFECYCLE_SKIPPED

        inv.state = "settled"
        return out

    def expire_pending_invoices(self, cutoff: float) -> list:
        """Expire pending invoices older than cutoff (pending-invoice timeout,
        separate from vehicle-window expiry). Never invents a freight invoice.
        An invoice is expired only when cutoff - requested_at > ttl_seconds."""
        p = self.params
        outcomes = []
        for inv_id in list(self._pending_order):
            inv = self._pending_invoices.get(inv_id)
            if inv is None or inv.state != "pending":
                continue
            if cutoff - inv.requested_at <= p.ttl_seconds:
                continue  # not yet expired
            inv.state = "expired"
            out = EventOutcome(
                vehicle_id=inv.vehicle_id,
                kind="delivery",
                timestamp=cutoff,
                association_quality=inv.association,
                pricing_source=inv.source,
                requested_amount=inv.amount,
                refused=True,
                refusal_reason=RefusalReason.ASSOCIATION,
                lifecycle=LIFECYCLE_REFUSED,
            )
            self._refused_total += inv.amount
            outcomes.append(out)
        return outcomes

    # ------------------------------------------------------- wall-clock expiry
    def expire_stale_windows(self, cutoff: float) -> list:
        """Vehicle-window expiry: never invents a freight invoice. Closes
        windows that opened before cutoff and were never settled. Does NOT
        create payments -- only pending invoices can be settled/refused via
        expire_pending_invoices. Checks real stored timestamps
        (window_opened_at) against the injected clock."""
        outcomes = []
        for vs in self._vehicles.values():
            if (
                not vs.window_open
                or vs.settled
                or vs.window_opened_at is None
                or vs.window_opened_at >= cutoff
            ):
                continue
            # Close the window without inventing a payment.
            vs.window_open = False
            vs.window_usable = False
            vs.window_opened_at = None
            vs.observed_depletion = 0.0
            vs.wear_charged = False
            vs.contamination.clear()
            vs.settled = True
            vs.lifecycle = "fresh"
            outcomes.append(
                EventOutcome(
                    vehicle_id=vs.vehicle_id,
                    kind="delivery",
                    timestamp=cutoff,
                    association_quality=vs.association,
                    pricing_source=PricingSource.NONE,
                    requested_amount=0.0,
                    actual_amount=0.0,
                    simulated_amount=0.0,
                    refused=False,
                    refusal_reason=RefusalReason.NONE,
                    freight_eligible=vs.freight_eligible,
                    lifecycle="window_closed_no_payment",
                )
            )
        return outcomes

    # --------------------------------------------------------------- totals
    def totals(self) -> dict:
        return {
            "simulated_total": self._simulated_total,
            "actual_total": self._actual_total,
            "refused_total": self._refused_total,
            "treasury_remaining": self._treasury_remaining,
        }
