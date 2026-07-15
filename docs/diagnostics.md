# Diagnostic contract

Every target-owned compiler diagnostic enters through `reflaxe.c.CDiagnostic`.
The stable automation key is its `HXCdddd` ID; Haxe's surrounding error text,
English detail, and source-display formatting are not APIs.

The canonical machine surfaces are:

- `docs/specs/diagnostics.json`: allocated IDs, defaults, allowed severity
  changes, phases, kinds, summaries, and remediation;
- `docs/specs/diagnostics.schema.json`: catalog schema;
- `docs/specs/diagnostic-event.schema.json`: one structured diagnostic record;
- `CDiagnosticId`: the typed Haxe call-site registry, checked byte-for-byte
  against the catalog.

The catalog is schema 2 because the earlier schema-1 seed had no formal schema
or drift contract. A diagnostic record is schema 1. E8.T09 will frame those
records into the public `hxc` JSON/event stream and attach native-tool and
command context; it must extend or version the record rather than invent a
second ID system.

## Reserved ranges

| Range | Owner |
| --- | --- |
| `HXC0000`–`HXC0999` | Bootstrap and configuration |
| `HXC1000`–`HXC1999` | Language lowering and unsupported source semantics |
| `HXC2000`–`HXC2999` | Runtime and memory policy |
| `HXC3000`–`HXC3999` | C ABI, export, layout, and ownership boundaries |
| `HXC4000`–`HXC4999` | Interop, unsafe boundaries, bindgen, and platform capabilities |
| `HXC5000`–`HXC5999` | Target API, metadata, macros, and DSL contracts |
| `HXC6000`–`HXC6999` | Native build, toolchain, and packaging (reserved) |
| `HXC7000`–`HXC7999` | Determinism, incremental state, and quality gates (reserved) |
| `HXC8000`–`HXC8999` | CLI, workspace, inspection, and orchestration (reserved) |
| `HXC9000`–`HXC9999` | Internal compiler failures |

An allocation uses the lowest suitable free ID in its owning range and updates
the JSON catalog, typed Haxe registry, tests, and affected documentation in one
change. An emitted ID is never reused. Retirement leaves a catalog tombstone
and, where applicable, names its replacement.

## Severity, source, and profile

Each catalog entry declares a default severity and the complete set of allowed
severities. Most entries are errors only. `HXC2001` defaults to `info` and may
be promoted to `warning` by the runtime-diagnostics policy without changing its
meaning or ID. A caller cannot choose an undeclared severity.

Every compiler diagnostic has a primary Haxe source range. Structured paths
are normalized project/repository-relative UTF-8 paths with `/` separators;
lines and columns are one-based and the end is exclusive. If no user construct
exists, use the compilation/configuration anchor. Never serialize the agent's
checkout path. The profile is `portable`, `metal`, or `unresolved`; the last is
only for configuration failure before a valid profile exists.

Human output includes the ID, profile, and canonical remediation. Tests should
assert the ID and whichever structured or semantic fields matter to the case.
Full prose belongs in a reviewed golden only when formatting itself is the
subject of that test.

## Failure taxonomy

Unsupported source and compiler defects are deliberately different:

- `HXC1001` is `lowering` / `unsupported-source`: the user program contains a
  typed construct whose lowering is not yet admitted.
- `HXC9000` is `internal` / `internal-compiler-failure`: a compiler invariant
  or internal model is missing or malformed.
- `HXC1000` is `compiler-capability`: the honest scaffold boundary before the
  first production lowering slice, not an internal crash and not executable
  output.

Do not turn malformed internal IR into `HXC1001`, and do not use `HXC9000` to
hide a known unsupported source construct.

## Contributor workflow

Production Haxe code uses `CDiagnosticId` plus `CDiagnostic.error`,
`CDiagnostic.fatal`, or another typed emission method. Raw `"HXCdddd: ..."`
prefixes outside `CDiagnostic.hx` are rejected. Exception/report models may use
`CDiagnostic.codeMessage` while retaining the typed ID.

Run:

```bash
npm run test:diagnostics
```

The gate checks exhaustive, non-overlapping ranges; catalog/schema validity;
typed-registry parity; every repository reference; raw production prefixes;
two byte-identical Haxe renders; structured event fields; severity admission;
relative source paths; and the structural distinction between `HXC1001` and
`HXC9000`.
