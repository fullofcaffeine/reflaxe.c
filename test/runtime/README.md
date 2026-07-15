# Runtime fixtures

Runtime cases assert exit/stdout/stderr, exact runtime-plan effects, strict
native compilation, failure cleanup, and eligible sanitizer results. See
[`docs/testing.md`](../../docs/testing.md).

Current evidence includes runtime-free execution of the generated primitive
body slice plus independent native runtime seeds. The body case selects no
`hxrt`; it is not proof that generated Haxe selects or links a runtime feature.
