# Typed Haxe boundary policy

Repository-owned compiler code uses Haxe's type system as a correctness
boundary. Compiler state, normalized input, IR, C plans, diagnostics, and
reports use closed target-owned classes, enums, enum abstracts, generics, and
typedefs. `Dynamic`, `Any`, `Reflect`, `untyped`, `DynamicAccess`, unchecked
casts, private-access metadata, and raw `__c__` calls are not substitutes for a
missing model.

The machine-readable authority is
[`docs/specs/typed-boundaries.json`](specs/typed-boundaries.json). Its guard
scans every repository-owned `.hx` file, ignoring only the enumerated
third-party and tool-state roots:

```sh
npm run test:typed-boundaries
```

The allowlist is deliberately source-exact. Each remaining occurrence records
its file, line, column, containing symbol, boundary kind, closed typed model,
why the boundary is unavoidable, and the negative evidence that contains it.
A moved or removed occurrence makes its entry stale; a new occurrence fails
until the implementation removes it or a reviewer accepts a genuinely narrow
boundary with evidence.

## Current inventory

Only four occurrences remain:

| Boundary | Why it remains | Containment |
| --- | --- | --- |
| `Json.parse` in `ReflaxeOutputWriter` | The pinned Haxe JSON parser returns an untyped external value. | The value is admitted only inside `decodeOwnershipMetadata`, checked for schema version and every consumed scalar/array/path invariant, and returned as `ReflaxeOwnershipMetadata`. Malformed and wrong-shape ownership documents fail before any generated write. |
| One `Dynamic` parameter in the aggregate negative fixture | The fixture presents the real language type to prove it cannot masquerade as a structurally known value record. | `CBodyLowering` rejects it before HxcIR construction; the suite requires exact source-positioned `HXC1001` and no generated artifact. |
| One cast in `DiagnosticGolden` | The negative test must construct an ID the `CDiagnosticId` enum abstract makes impossible in normal code. | The forged value is passed directly to the rejecting registry lookup and cannot enter production code. |
| One `Dynamic` parameter in `IRCoverage` | The fixture models Haxe's real `Dynamic` language semantic so HxcIR cannot confuse it with an internal opaque value. | It is test source only. Compiler implementation state remains typed, and HxcIR represents the semantic explicitly as `IRTDynamic`. |

There are no approved repository-owned uses of `Any`, `Reflect`, `untyped`,
`DynamicAccess`, private-access metadata, or raw `__c__` calls. The preview
Haxe `PlatformConfig` and Reflaxe callback carrier are typed directly; their
external origin is not a reason to erase their known contracts.

## Admitting an external boundary

An external API can justify a boundary only when its upstream type is genuinely
untyped. Keep the occurrence in the smallest adapter and:

1. validate every consumed shape and value before returning;
2. convert immediately into a closed target-owned model;
3. prevent the untyped carrier from reaching semantic compiler code;
4. add malformed-input and wrong-shape tests;
5. add the exact occurrence and test evidence to the inventory; and
6. run the boundary guard, affected compiler suites, exhaustive all-source
   gate, compiler-server probes where applicable, snapshots, and `npm test`.

Do not add an allowance merely to make the guard pass. If several sites need
the same escape, that is evidence that a typed adapter or semantic model is
missing.

This policy governs compiler implementation technique. It does not reject the
Haxe language features themselves: user `Dynamic` and reflection support remain
planned semantic work under E4/E5, where they must lower explicitly and select
only the justified runtime slices.
