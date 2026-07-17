# Differential fixtures

Cases name the reference Haxe target/oracle, store normalized oracle and C-target
traces, declare every permitted normalization, and retain reproducible seeds.
See [`docs/testing.md`](../../docs/testing.md).

The HxcIR indexing oracle remains a pre-emission semantic seed. Generated E2
fixtures compare evaluation order, arithmetic, and static initialization with
pinned Haxe oracles. The E4.T03 string-runtime case compares the independent
native UTF-8 slice's scalar-visible trace with Eval, while separately asserting
malformed-input, allocation, and CString rules that ADR 0004 defines directly.
The E4.T04 array-runtime case compares a common primitive mutation trace while
its native side independently forces relocation aliasing, reference lifecycle,
overflow, allocator failure, and rollback behavior.
