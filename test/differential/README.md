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

The first E5.T04 Bytes slice runs one ordinary Haxe binary-storage program under
Eval and generated native C. Its independent C contract then forces overlapping
copies, embedded NUL data, byte masking, bounds failures, reference-count
overflow, allocator rollback, and final-release balance. A declaration in the C
target `_std` override makes the nominal API visible without claiming that every
declared Bytes method is already implemented.
