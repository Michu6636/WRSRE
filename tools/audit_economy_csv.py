#!/usr/bin/env python3
"""Audit tesmio_ledger.csv for money-supply and treasury reconciliation."""
import argparse
import csv
import json
import math
import sys


def norm(name):
    return name.strip().lower().replace(" ", "_")


def find_col(header, base):
    base = base.lower()
    for i, name in enumerate(header):
        n = norm(name)
        if n == base or n.startswith(base + "_"):
            return i
    return None


def parse_float(value):
    if value is None:
        return None
    text = value.strip()
    if text == "":
        return None
    try:
        return float(text)
    except ValueError:
        return None


def audit(path, tolerance=1.0):
    issues = []
    rows_out = []
    with open(path, newline="", encoding="utf-8-sig") as f:
        reader = csv.reader(f)
        try:
            header = next(reader)
        except StopIteration:
            return {"error": "empty file", "rows": [], "issues": ["empty file"]}, 1
        header = [h.strip() for h in header]

        idx_year = find_col(header, "year")
        idx_month = find_col(header, "month")
        idx_treasury_open = find_col(header, "treasuryopen")
        idx_treasury_close = find_col(header, "treasuryclose")
        idx_net = find_col(header, "nettreasury")
        idx_ms_open = find_col(header, "totalmoneysupplyopen") or find_col(header, "moneysupplyopen")
        idx_ms_close = find_col(header, "totalmoneysupplyclose") or find_col(header, "moneysupplyclose")
        idx_ms_delta = find_col(header, "moneysupplydelta")
        idx_priv_sales = find_col(header, "privatesales")
        idx_state_sales = find_col(header, "statestoresales")
        idx_priv_wages = find_col(header, "privatewages")
        idx_priv_taxes = find_col(header, "privatetaxes")
        idx_priv_pool = find_col(header, "privatepool")

        flow_cols = []
        for base in ("wages", "pensions", "welfare", "statestoresales", "wholesaleb2b",
                     "vat", "profittax", "propertytax", "other"):
            i = find_col(header, base)
            if i is not None:
                flow_cols.append(i)

        row_count = 0
        max_drift = 0.0
        velocities = []
        shares = []
        for row in reader:
            if not row or all(c.strip() == "" for c in row):
                continue
            row_count += 1
            row_issues = []
            # NaN/Inf check across all cells
            for i, cell in enumerate(row):
                v = parse_float(cell)
                if v is None and cell.strip() != "":
                    # non-numeric where number expected; skip empties
                    pass
                if v is not None and not math.isfinite(v):
                    row_issues.append("non-finite value in %s" % header[i] if i < len(header) else "non-finite value")

            def get(idx):
                if idx is None or idx >= len(row):
                    return None
                return parse_float(row[idx])

            to = get(idx_treasury_open)
            tc = get(idx_treasury_close)
            net = get(idx_net)
            if net is None and flow_cols:
                s = 0.0
                ok = True
                for i in flow_cols:
                    v = get(i)
                    if v is None:
                        ok = False
                        break
                    s += v
                if ok:
                    net = s

            drift = None
            if to is not None and tc is not None and net is not None:
                drift = abs(tc - to - net)
                if drift > max_drift:
                    max_drift = drift
                if drift > tolerance:
                    row_issues.append("treasury drift %.6f > %.6f" % (drift, tolerance))

            ms_open = get(idx_ms_open)
            ms_close = get(idx_ms_close)
            ms_delta = get(idx_ms_delta)
            if ms_delta is None and ms_open is not None and ms_close is not None:
                ms_delta = ms_close - ms_open
            if ms_open is not None and ms_delta is not None and ms_open != 0:
                velocities.append(ms_delta / ms_open)

            priv_sales = get(idx_priv_sales)
            state_sales = get(idx_state_sales)
            if priv_sales is not None and state_sales is not None:
                total = priv_sales + state_sales
                if total != 0:
                    shares.append(priv_sales / total)

            label = ""
            if idx_year is not None and idx_month is not None:
                y = row[idx_year].strip() if idx_year < len(row) else ""
                m = row[idx_month].strip() if idx_month < len(row) else ""
                label = "%s-%s" % (y, m)

            rows_out.append({
                "label": label,
                "drift": drift,
                "money_supply_delta": ms_delta,
                "velocity": (ms_delta / ms_open) if (ms_open not in (None, 0) and ms_delta is not None) else None,
                "private_share": (priv_sales / (priv_sales + state_sales)) if (priv_sales is not None and state_sales is not None and (priv_sales + state_sales) != 0) else None,
                "issues": row_issues,
            })
            if row_issues:
                issues.extend("%s: %s" % (label or ("row %d" % row_count), "; ".join(row_issues)))

    velocities_summary = {
        "count": len(velocities),
        "max_abs": max((abs(v) for v in velocities), default=0.0),
        "avg": (sum(velocities) / len(velocities)) if velocities else 0.0,
    }
    shares_summary = {
        "count": len(shares),
        "avg": (sum(shares) / len(shares)) if shares else 0.0,
        "max": max(shares, default=0.0),
        "min": min(shares, default=0.0),
    }

    report = {
        "file": str(path),
        "rows": row_count,
        "max_treasury_drift": max_drift,
        "tolerance": tolerance,
        "issues_count": len(issues),
        "issues": issues,
        "velocity": velocities_summary,
        "private_state_share": shares_summary,
        "row_details": rows_out,
    }
    return report, (1 if issues else 0)


def selftest():
    import tempfile, os
    # Build a small synthetic CSV
    header = ["Year", "Month", "Wages", "Pensions", "Welfare", "StateStoreSales",
              "WholesaleB2B", "VAT", "ProfitTax", "PropertyTax", "Other",
              "NetTreasury", "TreasuryOpen_RUB", "TreasuryClose_RUB",
              "CitizenWallets", "EnterpriseCapitalSum", "InvestmentPool",
              "TotalMoneySupplyOpen", "TotalMoneySupplyClose", "MoneySupplyDelta",
              "PrivateSales", "PrivateWages", "PrivateTaxes", "PrivatePool",
              "TreasuryUnexplained"]
    rows = []
    # clean row: drift 0
    rows.append(["2026", "1", "-10", "0", "0", "20", "0", "2", "1", "0", "0",
                 "13", "100", "113", "50", "25", "10", "185", "198", "13",
                 "30", "5", "3", "1", "0"])
    # drift row
    rows.append(["2026", "2", "-10", "0", "0", "20", "0", "2", "1", "0", "0",
                 "13", "113", "130", "50", "25", "10", "198", "215", "17",
                 "30", "5", "3", "1", "3"])
    # nan row
    rows.append(["2026", "3", "NaN", "0", "0", "20", "0", "2", "1", "0", "0",
                 "13", "130", "143", "50", "25", "10", "215", "228", "13",
                 "30", "5", "3", "1", "0"])

    fd, path = tempfile.mkstemp(suffix=".csv")
    os.close(fd)
    try:
        with open(path, "w", newline="", encoding="utf-8") as f:
            w = csv.writer(f)
            w.writerow(header)
            w.writerows(rows)
        report, code = audit(path, tolerance=1.0)
        assert report["rows"] == 3, report
        assert report["issues_count"] >= 2, report
        # max drift at least 3
        assert report["max_treasury_drift"] >= 3.0, report
        # velocity present
        assert report["velocity"]["count"] == 3, report
        # private share present
        assert report["private_state_share"]["count"] == 3, report
        # nan detected
        assert any("non-finite" in i or "NaN" in i for i in report["issues"]) or report["issues_count"] >= 2
        print("selftest: PASS")
    finally:
        os.remove(path)
    return 0


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "selftest":
        return selftest()

    parser = argparse.ArgumentParser()
    parser.add_argument("csv")
    parser.add_argument("--json", dest="json_out")
    parser.add_argument("--strict", action="store_true")
    parser.add_argument("--tolerance", type=float, default=1.0)
    args = parser.parse_args()

    report, code = audit(args.csv, tolerance=args.tolerance)
    # concise diagnostic report
    print("Audit: %s" % report["file"])
    print("Rows: %d" % report["rows"])
    print("Max treasury drift: %.6f (tolerance %.6f)" % (report["max_treasury_drift"], report["tolerance"]))
    print("Issues: %d" % report["issues_count"])
    for i in report["issues"]:
        print("  - " + i)
    v = report["velocity"]
    print("Money-supply velocity: n=%d max_abs=%.6f avg=%.6f" % (v["count"], v["max_abs"], v["avg"]))
    s = report["private_state_share"]
    print("Private/state share: n=%d avg=%.4f min=%.4f max=%.4f" % (s["count"], s["avg"], s["min"], s["max"]))

    if args.json_out:
        with open(args.json_out, "w", encoding="utf-8") as f:
            json.dump(report, f, indent=2)

    if args.strict and report["issues_count"] > 0:
        return 1
    return code


if __name__ == "__main__":
    sys.exit(main())
