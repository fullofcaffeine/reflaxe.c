# Dynamic value Oracle disposition (2026-08-26)

This review defines the safe implementation boundary for `haxe_c-ec5`.
Dynamic values need an internal carrier, checked operations, and exact lifetime
rules. They must not become the representation for ordinary typed Haxe values.

The Oracle request was `orq_20260826T212601Z_b3e5e4cd`. GPT-5.6 Pro completed
a planning review of revision `340cef2f26cda54df783665a830723db66561d6e`.
The local processor used GPT-5.6 Sol at xhigh reasoning.

## Local baseline

The current compiler has `IRTDynamic`, `IRCBox`, `IRCUnbox`, and
`IRCDIntrinsic`. These names do not yet define a complete Dynamic contract.

The following gaps remain:

- `CBodyAggregate` does not classify `TDynamic`.
- HxcIR does not define Dynamic type identities or operation availability.
- The validator does not validate box and unbox operations.
- `CBodyEmitter` has no C representation for `IRTDynamic`.
- Managed-root planning cannot find a managed pointer inside a Dynamic value.
- The runtime catalog reserves `dynamic`, but it has no implementation.

The existing object descriptor has a different job. It records object size,
alignment, tracing, and finalization. It is not reflection or Dynamic metadata.

## Oracle claim matrix

| Oracle recommendation | Disposition | Local reason |
| --- | --- | --- |
| Use one private tagged `hxc_value` carrier. | Retained | This carrier gives Dynamic one representation without changing ordinary typed values. |
| Keep null, Bool, Int, and Float inline. | Retained | These values need no allocation or garbage-collector dependency. |
| Use a separate immutable Dynamic type or operation descriptor. | Retained | Dynamic behavior and managed object layout have different owners. |
| Leave `hxc_type_descriptor` unchanged. | Retained | Its source and documentation explicitly exclude names, methods, and Dynamic construction. |
| Put Dynamic type identities and operations in a program-level HxcIR plan. | Retained | Bare `IRCBox` and `IRCUnbox` do not carry enough facts for validation. |
| Add dedicated box, unbox, field, call, member-call, and equality instructions. | Retained | Each operation needs an exact type token, failure edge, and runtime reason. |
| Generate one exact adapter for each reachable source type. | Retained | One adapter avoids repeated site switches and works across generated C files. |
| Use numeric member tokens and omit runtime field names. | Retained | Static member access does not require reflection metadata. |
| Add `IRMRPDynamicPayload` for exact managed roots. | Retained | The collector accepts only null or an exact allocation base. |
| Add managed Dynamic tracing for fields and wrapper cells. | Retained | A Dynamic value can otherwise hide a live managed object from the collector. |
| Add a general managed-global-root plan now. | Deferred | The first slice will reject managed Dynamic globals. A general root plan needs its own reusable owner. |
| Orthogonalize all class headers for polymorphic Dynamic boxing now. | Deferred | The first slice will admit exact concrete classes and reject unresolved base or interface identities. |
| Promote mutable anonymous objects before aliases split. | Retained | A box-site copy changes visible identity and mutation behavior. |
| Support direct Dynamic member calls but reject bound-method extraction. | Retained | Direct calls preserve the receiver. Bound values need the separate closure lifetime model. |
| Add a general status-plus-output HxcIR convention. | Modified | Use dedicated checked Dynamic instructions first. Generalize only when a second consumer proves the shared need. |
| Add reflection names, computed fields, or constructors. | Rejected | Those operations belong to the later reflection task. |
| Extend the object descriptor with Dynamic callbacks. | Rejected | This change mixes source behavior with managed storage facts. |
| Use one heap box for every Dynamic value. | Rejected | Scalar-only Dynamic programs must not select allocation, objects, or the collector. |
| Use per-use-site switches as the main identity mechanism. | Rejected | A value can cross functions and C files, so identity must travel with the value. |
| Reuse hidden collector headers as Haxe type identity. | Rejected | The collector owns storage lookup, not source-level Dynamic behavior. |
| Infer equality and casts from C representations. | Rejected | Haxe behavior must come from reference-target differential evidence. |
| Support computed names, open registration, or a public Dynamic ABI. | Rejected | `haxe_c-ec5` owns a private closed-world feature only. |

## Integrated conclusion

The implementation will use two separate descriptors:

1. `hxc_dynamic_type` identifies one reachable Dynamic source type and its
   checked operations.
2. `hxc_type_descriptor` continues to own only managed storage and lifetime.

`hxc_value` will contain a tag, a Dynamic type pointer, and a C union. The union
will hold direct scalar payloads or one data pointer. Generated wrappers will
store String, enum, Array, function, and promoted-object values in their exact
typed fields. A function pointer will never pass through `void *` or an integer.

HxcIR will define the complete meaning before C emission. The Dynamic plan will
list each admitted type, member token, call shape, storage kind, and operation.
Dedicated instructions will name that plan data and an explicit failure edge.
The validator will reject missing, incompatible, or unsorted plan entries.

Static field access will use numeric tokens. Generated C will not contain a
field-name table. Computed names and field enumeration will remain reflection
features.

Managed Dynamic values will use `IRMRPDynamicPayload`. The projection will
return null for scalar tags. It will return one exact allocation base for a
managed tag. Managed fields will use the same guarded operation during tracing.

The first complete slice will use these boundaries:

- It will support primitives, String, null, Array, exact concrete objects,
  one mutable anonymous-object path, fieldless enums, non-capturing functions,
  and opaque type identity.
- It will reject managed Dynamic globals until a general global-root plan
  exists.
- It will reject unresolved polymorphic class or interface identities.
- It will reject bound-method extraction, computed field names, open generic
  specialization, and Dynamic map keys.
- It will reject payload-enum equality until typed payload-enum equality has
  one shared implementation.
- It will use explicit failure edges. If Haxe requires catch behavior, the
  source form will remain unsupported until the exception task provides it.

This boundary is smaller than the complete Oracle plan. It keeps every
required safety rule and avoids speculative class-layout and global-root work.

## Implementation plan

1. Record reference-Haxe results for equality, casts, calls, failure behavior,
   and left-to-right evaluation.
2. Add the Dynamic plan, dedicated instructions, root projection, dumper, and
   strict validator rules to HxcIR.
3. Classify `TDynamic` as `CBVKDynamic`. Keep existing generic and map-key
   negatives.
4. Add the selective runtime carrier and its native contract tests.
5. Generate exact adapters and wrapper cells for each admitted category.
6. Add managed roots, field tracing, and safe allocation publication.
7. Lower checked field, call, equality, box, and unbox operations.
8. Add source-positioned runtime reasons and policy checks.
9. Run differential, structural, native, sanitizer, split-output, and policy
   tests.

## Required evidence and stop conditions

The change needs these positive cases:

- Int, Float, Bool, String, and null
- one Array
- one mutable anonymous object
- one managed exact concrete class
- one fieldless enum
- one non-capturing function
- one opaque type value.

The change also needs these negative cases:

- wrong unbox or cast
- null receiver and missing field
- non-callable value and wrong call shape
- bound-method extraction
- managed Dynamic globals
- unresolved polymorphic class identity
- payload-enum equality
- existing Dynamic generic specialization and map-key cases.

The runtime report must show every boxing reason. A typed control program must
contain no Dynamic carrier, feature, adapter, symbol, or reason. Scalar-only
Dynamic must not select allocation, objects, or the collector.

Strict C11, C++ header, AddressSanitizer, and UndefinedBehaviorSanitizer checks
must pass. Split and unity output must select the same feature closure. Cold and
warm compiler-server requests must produce identical plans and generated C.

If implementation needs a public Dynamic ABI, runtime field names, collector
header inspection, or a universal box, stop. Those changes violate this
disposition and require a new owner decision.
