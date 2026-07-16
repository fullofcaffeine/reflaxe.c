# Architecture decision records

Accepted ADRs refine the PRD and are normative for their scope. Change an
accepted contract with a superseding ADR; do not silently rewrite its behavior
in code or examples.

| ADR | Status | Decision |
| --- | --- | --- |
| [0001](0001-direct-c-and-selective-runtime.md) | Accepted | Direct C/local specialization before selective `hxrt` |
| [0002](0002-haxe-first-typed-c-authoring.md) | Accepted | Haxe-first typed C declarations, macros, and DSL admission |
| 0003 | Reserved | C++ `extern "C"` shim scope remains owned by E6.T10 |
| [0004](0004-utf8-scalar-string-contract.md) | Accepted | Valid UTF-8 with Unicode-scalar Haxe indices |
| [0005](0005-precise-nonmoving-collector.md) | Accepted | Selective precise non-moving tracing collector |
| [0006](0006-explicit-failure-edges-and-contained-unwinding.md) | Accepted | Result lowering plus contained strict-C unwinding |
| [0007](0007-strict-c11-target-and-platform-baseline.md) | Accepted | C11 target identity and version 1.0 support lanes |
| [0008](0008-primitive-representations-and-conversions.md) | Accepted | Exact primitive representation, conversion, operation, and nullability semantics |

Numbers are stable once referenced by the PRD or Beads plan, so a reserved
number can legitimately appear as a gap in the checked-in files.
