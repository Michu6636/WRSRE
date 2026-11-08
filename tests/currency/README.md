# Currency transaction regressions

Run from the repository root:

```sh
PYTHONPATH=. PYTHONDONTWRITEBYTECODE=1 python3 tests/currency/test_transactions.py
```

The portable C++17 harness includes the **production** transaction headers used
by `currency.cpp`, rather than duplicating a financial model. It substitutes game
memory, clock, locks and observer delivery. Seven executable scenarios cover:

- Full charges refuse insufficient funds without changing wallet or spent total.
- Partial charges collect only the available balance; empty wallets retain liveness.
- Paired transfers conserve money, route RUB/USD correctly, and report committed legs.
- NaN/infinity, finite non-positive no-ops, invalid recipients and zero IDs.
- Missing/unreadable native memory, existing/missing wallets, and failure recovery.
- Overflow, corrupt stored balances, and changes rounded entirely away.
- Eight concurrent spenders cannot overdraw one wallet.

Additional Python checks ensure the plugin includes those implementations,
continues publishing both ABI v3/v4 tables, and the state checkout uses a full
payment rather than a partial payment followed by a dispense refusal.

`native_fault_test.cpp` is a separate **Windows/MSVC** test that uses real SEH,
`VirtualAlloc` and `VirtualProtect`. It makes the treasury read-only, forces a
store failure after the spent-counter write, and checks rollback. It also tests
a spent-counter write fault and inaccessible person memory. The Python runner
runs it when Windows and `cl` are available; otherwise it explicitly skips it.
It can also be run from an x64 Native Tools prompt:

```bat
cl /nologo /std:c++17 /EHsc /MT /W4 /WX tests\currency\native_fault_test.cpp /Fe:build\currency_native_fault.exe /Fo:build\currency_native_fault.obj
build\currency_native_fault.exe
```

Create `build` first if absent. Portable tests need `g++` or `clang++`; the MSVC
fault test does not. No binary or generated game data belongs in Git.

## Boundaries and compatibility

- Currency plugin implementation version is 4.1; the API table layout, service
  versions (3/4), save format and INIs are unchanged.
- Currency's lock serializes service callers, not unrelated native game writes.
  Observers run after lock release and may see subsequent concurrent operations;
  they must use event amounts for accounting, not infer deltas from snapshots.
- Float wallets/double treasuries retain their existing representation and
  ordinary rounding. Mutations that overflow or make no representable change
  are refused. This is not a fixed-point-money migration.
- Corrupt wallet balances no longer regenerate a starting grant on read or
  transaction. Reads return zero and transactions refuse the corrupt entry.
- Allocation happens before native commits. Failed native commits erase a newly
  prepared wallet node. Native rollback is best-effort if game memory itself
  becomes unavailable during rollback; that exceptional failure is logged.
- This change does not implement save-switching support, alter pruning policy,
  or redesign retail's existing fail-open native-hook exception policy.

Portable tests cannot validate Windows SEH, native hook sites, game threading,
or UI fulfillment. Build `currency` and `citizen_retail` on Windows and check a
state purchase, refused purchase, payroll and ledger flow in a disposable save
before deployment. The enterprise source change is comment-only.
