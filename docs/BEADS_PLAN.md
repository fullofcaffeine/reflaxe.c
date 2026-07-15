# Beads execution plan

`docs/PRD.md` is the source of product intent. After materialization, Beads is the source of execution state: ownership, readiness, discoveries, and completion evidence. The checked-in JSON plan remains the reproducible bootstrap and recovery map.

## Plan shape

The bootstrap contains **11 epics, 122 tasks, and 274 explicit blocking edges**. Dependencies are intentionally selective: tasks under one epic can proceed in parallel unless a semantic or tooling prerequisite is named.

| Epic | Milestone | P | Tasks | Outcome |
|---|---:|---:|---:|---|
| `E0` Repository and governance foundation | `M0` | 0 | 10 | Ratify contracts, make the scaffold build reproducibly, and establish the project rules Codex must follow. |
| `E1` Reflaxe compiler core and C AST | `M1` | 0 | 10 | Build the deterministic AST/IR/output foundation before broad language lowering. |
| `E2` Primitive semantics, control flow, and UB discipline | `M2` | 0 | 11 | Deliver the first correct executable Haxe-to-C vertical slice with explicit evaluation order and no C undefined behavior. |
| `E3` Aggregates, generics, classes, and dispatch | `M3` | 1 | 10 | Lower Haxe's structured type system into readable C layouts and calls without defaulting to a universal box. |
| `E4` Runtime, memory, dynamic values, and exceptions | `M4` | 1 | 12 | Implement selective runtime capabilities and full portable object-graph semantics while preserving a proven runtime-free lane. |
| `E5` Haxe standard-library parity | `M5` | 1 | 12 | Systematically support the Haxe standard library with direct lowering, target-owned adapters, and narrow runtime slices. |
| `E6` C interop and deterministic bindgen | `M6` | 1 | 12 | Make existing C libraries easy and safe to consume from exact headers, flags, and ABI facts. |
| `E7` Exported C ABI and shared libraries | `M7` | 1 | 10 | Make Haxe-authored code a first-class producer of stable C headers, static libraries, and shared objects. |
| `E8` CLI, build adapters, diagnostics, and inspection | `M8` | 1 | 11 | Turn compiler components into the coherent hxc product and an automation-friendly build surface. |
| `E9` Examples, todo CLI, documentation, and adoption | `M9` | 2 | 12 | Prove the product through honest, polished examples and a macro-powered SQLite todo CLI. |
| `E10` Quality, performance, security, and releases | `M10` | 1 | 12 | Continuously harden correctness, ABI, security, performance, compatibility, and release provenance. |

Priority uses Beads convention: `0` is urgent/foundational, then `1`, `2`, and `3`. Effort is a planning hint, not a time promise.

## Validate without changing Beads

```text
python3 scripts/beads/validate_plan.py
python3 scripts/beads/bootstrap.py
python3 scripts/beads/bootstrap.py --json
```

The validator checks schema contracts, unique stable keys, parent ownership, dependency references, self-edges, cycles, and deterministic topological order. The default bootstrap mode is read-only and does not require `bd`.

## Materialize

```text
bd init --quiet                 # optional; bootstrap initializes if missing
python3 scripts/beads/bootstrap.py --apply
bd prime
bd ready --json
```

The apply path creates epics first, then parented tasks, then `child needs blocker` dependency edges. It writes only `.hxc/beads-plan-state.json`, an atomic stable-key-to-Beads-ID recovery map; it never edits Beads database internals. Exact issue titles also provide recovery when that state file is absent.

Rerunning `--apply` is intended to be idempotent. Existing state mappings and exact-title matches are reused; duplicate dependency reports are treated as existing edges. Any other Beads error stops the run.

## Work loop

```text
bd ready --json
bd update <id> --claim
# implement and collect acceptance evidence
bd close <id> --reason "Acceptance criteria satisfied; tests: <commands>"
bd dolt push
```

Only `E0.T01` is initially ready. That is deliberate: it ratifies the license, C baseline, string encoding, memory-management, exception, and platform decisions that would otherwise harden accidentally in implementation.

## Stable plan keys

- Epics use `E0` … `E10`.
- Tasks use `E<epic>.T<sequence>`, such as `E6.T03`.
- Materialized titles include the compact key, for example `[HXC-E6-T03] …`.
- Code, docs, and capability ledgers may cite stable plan keys; Beads IDs remain execution-system details.

## Discoveries and scope

Implementation will expose work not knowable from the bootstrap. Create a Beads issue linked with a `discovered-from` relationship, attach it to the correct epic, and state whether it blocks the claimed issue. Do not hide new scope in TODO comments or silently widen acceptance criteria.

## Source files

- `docs/specs/beads-plan.json`: exhaustive execution graph and acceptance criteria.
- `docs/specs/beads-plan.schema.json`: portable structural schema.
- `scripts/beads/validate_plan.py`: semantic graph validation and summary.
- `scripts/beads/bootstrap.py`: dry-run and idempotent materialization.
- `.hxc/beads-plan-state.json`: local generated recovery map after apply; ignored by Git.
