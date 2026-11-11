# Global Almanac: product and delivery plan

**Status:** The product Almanac is not implemented. Research and planning are documented, and a separate draw-only ninth-backdrop prototype has been merged with its default on. The user performed one visual-only check in a disposable game/profile with a copied save and cloud sync off; they report no intentional save, time advance, policy change, or button click, and a normal close. The extra tile appears beside the Economy & Trade row in the supplied view, but the user reports that it repeats the crime-button artwork. The log contains 259 `game.ERROR` lines, many for missing save/workshop content; the user says the `saved_last` errors at close are expected when exiting without saving. These errors are not attributed to the prototype. The screenshot/log do not identify the exact DLL hash or source revision. The prototype has no click handling or Almanac page and does not meet the product acceptance criteria below. Any further game run needs separate approval. For design reference, see the [City Manager static UI review](../export/almanac-evidence/city-manager-ui-review.md); it is inspiration only, not code/assets to reuse. For technical findings and open questions see [Technical findings](almanac-technical-findings.md). Earlier investigations may contain superseded conclusions.

## 1. Purpose

Build a Tropico-inspired global Almanac for Workers & Resources: Soviet Republic: a single place to understand government finances and adjust economic policy. It is global, not a City Hall building panel.

### Placement and navigation

- The Almanac must have its **own button alongside the current Economy & Trade buttons, in the same row**, without overlapping them or moving to a separate bottom line. The current screenshot shows eight existing tiles.
- Prefer a distinct global sidebar section. If that cannot be integrated safely, an Almanac page inside Economy & Trade is acceptable, **while retaining its own same-row button**.
- Position the button dynamically with the row's real layout and scale, not at a fixed screen coordinate. Account for visible native buttons, spacing, changes in resolution/UI scale, and other supported layout modes.
- Reuse the mod's established button and interaction machinery where appropriate. The City Hall TAX control is a useful **interaction precedent**, not a placement model; this work does not request moving that existing control.
- If the same-row requirement cannot be met without overlap or an unintended second row, stop and bring the trade-off to the project owner rather than silently changing the placement.

## 2. First release

The first acceptable release must provide **government finance information and directly adjustable taxes in the Almanac**. A read-only page or controls available only through City Hall do not satisfy this requirement.

### Fiscal information

- Show the current treasury balance and currency when the currency service is available.
- Show meaningful recent government income and spending from the ledger, with an explicit period and currency where supported.
- Distinguish recorded financial flows from the observed change in the treasury. If they differ, expose or clearly account for the unexplained difference; do not label the flow sum as the entire treasury change.
- If a service or period is unavailable, display an unavailable state rather than an invented zero. Agree on the exact set of finance rows based on data the owner can reliably provide.

### Direct tax controls

- Provide controls for VAT, corporate profit tax, and property tax, including their enabled/suspended status and configured values.
- Preserve the existing tax service as the authority for validation, settlement, saved policy, and scheduling. Do not implement a second set of tax rules in the Almanac.
- Label VAT and profit as rates; the property-tax control sets a **base amount per enterprise**, not a percentage. In the existing service the actual charge can be scaled by land attractiveness when the land-value option is enabled (the shipped default); when it is disabled the base is a flat amount. Show the applicable basis accurately rather than changing tax behavior. Explain when changes take effect and the behavior of suspended taxes and outstanding liabilities according to the existing service.
- Keep existing City Hall behavior usable during a safe migration; do not remove it until compatibility and the replacement have been verified and removal is authorized.

### Later expansion

Interest rates, subsidies, and global industry wages are intended to migrate into the Almanac over time. Graphs, historical trends, and native graph controls are later work, not prerequisites for the first release. The exact sequencing beyond v1 remains a planning decision.

## 3. Product principles

1. **A global, native-feeling surface:** open the Almanac without selecting a building, and return to normal sidebar content predictably.
2. **One authority per value:** use existing economic services; the Almanac presents and adjusts policy rather than duplicating the simulation.
3. **Honest data:** label periods, currencies, unavailable values, and the difference between tracked flows and actual balance changes.
4. **Preserve vanilla and saves:** do not disrupt existing buttons, close behavior, controls, or stored policy.
5. **Responsive placement:** reuse the existing interaction model and fit the current row dynamically rather than assuming a fixed screen position.

## 4. Delivery stages

These stages are planning gates, not authorization to implement.

1. **Resolve integration:** identify how a mod-owned button joins the active row and how its global page opens, replaces or suppresses native content, closes, and returns. Investigate relevant third-party mod DLLs if supplied; see [DLL research](almanac-dll-research.md).
2. **Design the data contracts:** expose configured tax values even while suspended, and make a trustworthy ledger snapshot available to the Almanac, with version and availability handling.
3. **Propose the implementation:** choose the least invasive viable integration, assess hook safety and compatibility, and request explicit implementation approval.
4. **Build the first release after approval:** implement the button, global page, finance view, and adjustable taxes together; retain existing policies and save behavior.
5. **Verify and expand:** test native coexistence and financial accuracy, then consider migration of other controls and, later, graphs.

## 5. Acceptance checklist for an authorized implementation

- [ ] A dedicated Almanac button sits in the **same row** beside the existing Economy & Trade buttons, without overlapping them or creating a second row, across supported layouts and scales.
- [ ] Clicking it opens the global Almanac. The existing button interaction can be reused; verify that the new placement receives input in this sidebar context and does not steal or duplicate native clicks.
- [ ] Vanilla buttons, selection, page content, close, and return behavior still work. Almanac content does not mix with native content when it should replace it.
- [ ] Treasury and recent financial information are correctly labeled by period and currency; tracked flows are not confused with the total treasury change.
- [ ] VAT, profit tax, and the property-tax base can be adjusted and enabled/suspended from the Almanac, with the active land-value basis accurately represented. Configured values survive suspension and normal save/load behavior.
- [ ] Missing or incompatible services show a safe unavailable state; no unrelated economic policy, saves, or vanilla state are changed.
- [ ] Integration is validated for the supported game build, layout modes, window sizes and UI scales, including relevant compatibility and regression testing.

The checklist describes the desired result, not claims that these tests have already passed.
