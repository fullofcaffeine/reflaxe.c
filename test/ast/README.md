# AST and IR fixtures

Cases here construct target-owned HxcIR or C AST directly. They require a
deterministic structural result and validation; emitted C/header text also
requires strict native compilation. See [`docs/testing.md`](../../docs/testing.md).

These fixtures do not by themselves prove typed Haxe-to-C lowering.
