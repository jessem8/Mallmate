# Agent 3 — SQL Injection Fixes (Parking Filter)

Goal: eliminate dynamic column concatenation in filter SQL.

Tasks
- Replace column concatenation with a strict whitelist map.
- Keep current UI options and search behavior.
- Update:
  - `parking/parking.cpp`

Acceptance
- Only allowed columns can be used in queries.
- No string concatenation of unchecked column names.

