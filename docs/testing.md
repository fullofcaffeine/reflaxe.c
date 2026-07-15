# Fixture and snapshot policy

This document is the human-readable contract for HXC-COMP-014, HXC-QA-001,
and HXC-QA-007. The machine-readable join point is
[`fixture-taxonomy.json`](specs/fixture-taxonomy.json); individual future cases
use [`fixture-case.schema.json`](specs/fixture-case.schema.json).

The taxonomy describes evidence. It does not promote a seed into a supported
compiler capability. In particular, direct HxcIR/C AST fixtures and independent
native C/C++ inputs do not prove that typed Haxe currently emits C.

## Canonical lanes

| Lane | Canonical directory | Required evidence | Current state |
| --- | --- | --- | --- |
| Positive | `test/positive/` | Success exit plus exact semantic assertions and declared artifacts/effects | Active through mapped M0 suites, including typed-input acceptance before lowering |
| Negative | `test/negative/` | Failure exit, stable diagnostic ID/essential fields/source span, and no plausible output | Active through mapped M0 suites, including the honest `HXC1000` typed-input boundary |
| AST/IR | `test/ast/` | Deterministic structural model, validator result, and native compile/run when C is produced | Active through `c_ast`, `declaration_plan`, and `hxc_ir` |
| Snapshot | `test/snapshot/` | Byte-exact text or semantic JSON, deterministic rerender, and reviewable diff | Active; existing expected trees remain mapped in place |
| Runtime | `test/runtime/` | Exit/stdout/stderr, runtime-plan effects, strict native build, and sanitizers where eligible | Native seed only; no generated-Haxe runtime proof yet |
| Differential | `test/differential/` | Named oracle, normalized oracle/target traces, deterministic seed, and allowed normalizations | HxcIR side-effect oracle seed only |
| ABI | `test/abi/` | Headers, symbols/layouts, ownership/calling convention, and external consumers | Independent native seed only |
| Performance | `test/performance/` | Versioned measurements, units, inputs/toolchain/hardware/variance, baseline, and budget decision | Contract only |

The canonical directories are stable homes for new cases. Existing focused
suites stay in their current paths and are mapped by the catalog; moving them
would create noise without improving their evidence.

The mapped `diagnostics` suite is both positive and negative policy evidence.
It compares the typed Haxe definitions with the machine catalog, validates two
byte-identical structured renders, and proves that unregistered IDs, absolute
paths, and forbidden severity changes are rejected. It asserts stable IDs and
essential fields rather than treating complete prose as an API. See
[diagnostic contract](diagnostics.md).

## Case contract

A new case contains `case.json` conforming to the checked-in case schema. It
declares:

- a stable ID, role (`fixture` or `example`), lane, and current status;
- every Haxe/native/config source input;
- an argument-array runner command, case-relative working directory, timeout,
  and whether network access is required;
- the exact expected exit code;
- at least one semantic assertion: stdout, stderr, diagnostic IDs, artifacts,
  runtime plan, oracle output, or metrics.

Runners execute from the declared working directory, default to no network,
use bounded timeouts, preserve stdout/stderr/exit status separately, and never
silently skip a required toolchain. Normalization is permitted only when the
case names the field and the underlying Haxe contract permits it.

Expected outputs are owned by the test case, not by generated-output caches.
Generated scratch/output trees stay ignored. A snapshot asserts source shape;
runtime or differential behavior requires its own lane assertion as well.

## Snapshot workflow

Check every registered expected tree without writing:

```sh
npm run snapshots:check
```

Regenerate one suite intentionally:

```sh
npm run snapshots:update -- --suite c-ast
```

An intentional full refresh requires the visibly broader command:

```sh
npm run snapshots:update -- --all
```

The updater renders twice, rejects nondeterministic output, prints a unified
semantic diff before any write, and then updates only catalog-owned expected
roots with per-file atomic replacements. JSON comparisons ignore object-key
formatting but display a canonical structural diff. C, headers, and HxcIR remain
exact text. Stale files
inside a managed expected root are shown as deletions. Update mode refuses to
run when `CI` is set.

Never hand-edit an expected file merely to make a failure disappear. Review the
printed semantic change, update through the command, inspect `git diff`, and run
the owning suite plus every required native/oracle gate. CI only checks; it
never blesses output.

The registered snapshot selectors are:

- `bootstrap`
- `typed-c`
- `typed-ast`
- `c-ast`
- `declaration-plan`
- `hxc-ir`

List them from the executable registry with:

```sh
python3 scripts/test/snapshots.py --list
```

The `typed-ast` suite is the compile-backed frontend boundary. It captures real
custom-target input, classifies modules/declarations/externs/typedefs/abstracts,
metadata, fields, expressions, and the entry point, and compares forward,
reversed, repeated, cold, and compiler-server reports. Its expected JSON is an
unsupported-node inventory; it does not prove HxcIR or C lowering. The internal
report and reverse-order defines are test/diagnostic seams, not user-facing
compiler configuration. See [typed-AST input boundary](typed-ast-input.md).

## Examples are product proofs, not implicit tests

The current checkout has no `examples/` directory; that inventory mismatch is
tracked separately. When examples are restored or added, every immediate
`examples/<name>/` directory must contain a schema-valid `case.json` with
`role: "example"` and explicit commands and expected assertions.

A test harness may run an example only through that declared case. It must not
glob `examples/**`, infer success from compilation alone, or treat README prose
as an assertion. Examples may have broader product narratives, but reusable
compiler regressions belong in a focused fixture when that produces a smaller,
more diagnostic proof.

## Policy validation

Run:

```sh
npm run test:fixture-policy
```

The guard validates the catalog/schema shape, all canonical directories,
registered runners and expected roots, complete ownership of existing expected
files, snapshot-registry parity, package/pre-commit/CI wiring, and the example
manifest rule. It is part of `npm test` and the pre-commit path for relevant
changes.

`test/symbol_registry` is the focused positive/negative/snapshot suite for the
schema-1 `hxc.symbols.json` shape. It renders twice, reverses request and typed
declaration discovery order inside the Haxe fixture, validates exact-name
failures and both collision origins, feeds finalized defaults into declaration
planning, rejects host paths, and asserts an empty typed-contract runtime feature
set. It proves naming analysis only; it does not claim production C emission.
