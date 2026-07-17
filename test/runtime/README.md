# Runtime fixtures

Runtime cases assert exit/stdout/stderr, exact runtime-plan effects, strict
native compilation, failure cleanup, and eligible sanitizer results. See
[`docs/testing.md`](../../docs/testing.md).

Current evidence includes runtime-free execution of the generated primitive
body slice plus independent allocator and UTF-8 scalar string runtime contracts.
The string fixture lives in the differential lane because it compares the
native implementation with pinned Haxe Eval while also running sanitizer and
allocation/lifetime assertions. No current generated Haxe case selects or links
an `hxrt` feature.
