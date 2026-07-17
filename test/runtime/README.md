# Runtime fixtures

Runtime cases assert exit/stdout/stderr, exact runtime-plan effects, strict
native compilation, failure cleanup, and eligible sanitizer results. See
[`docs/testing.md`](../../docs/testing.md).

Current evidence includes runtime-free execution of the generated primitive
body/span slice plus independent allocator, UTF-8 scalar string, and typed
resizable-array runtime contracts. The string and array fixtures live in the
differential lane because each compares a bounded native trace with pinned Haxe
Eval while also running sanitizer, failure, and ownership assertions. Generated
Haxe currently selects only the separate literal-output closure; general Haxe
String and Array operations remain unsupported.
