# Differential fixtures

Cases name the reference Haxe target/oracle, store normalized oracle and C-target
traces, declare every permitted normalization, and retain reproducible seeds.
See [`docs/testing.md`](../../docs/testing.md).

The HxcIR indexing oracle remains a pre-emission semantic seed. The E2.T04
evaluation-order case is active differential evidence: the same typed Haxe
fixture runs under Eval and as generated strict C at `-O0` and `-O2`.
