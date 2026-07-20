# Generated-C maintainability evidence

This document defines the bounded quality check used for generated C. The check
answers a narrow question: **did a reviewed example become harder for a C
programmer to navigate or read?** It does not decide whether the program is
semantically correct, fast, portable, or ready for release. Those claims keep
their own stronger tests.

## Why this is a separate check

Exact snapshots catch every changed byte, and the native lanes prove that GCC
and Clang can compile and run the result. Neither test explains whether a change
turned one clear function into hundreds of temporary variables, one enormous
file, or a low-level graph made of labels and `goto` statements.

The first Caxecraft counter scanned every file as one text string and counted
globally unique temporary names. That was useful while names were being cleaned
up, but it was not a maintainability contract. The same temporary spelling is
reused in different functions, so a project with many declarations could look
smaller than it was. Text inside comments and string literals could also look
like C syntax to a regular expression.

The maintained check therefore uses an independent, small C lexer. A lexer is a
reader that separates identifiers and punctuation from comments and string
literals. It does not compile C and it does not reuse `CASTPrinter` rules, so it
cannot manufacture its expected answer from the code generator it is checking.

## What the report measures

The report records normalized input paths and SHA-256 hashes, then measures each
file and function separately:

- **ownership** says which Haxe module, generated entry point, or compiler
  support component owns the file and each source-backed function;
- **source mapping** records source-backed function identity and any structural
  `#line` mapping. `#line` is a C directive that tells native diagnostics which
  Haxe file and line produced the following C line;
- **name readability** records the longest compiler-owned identifier and counts
  encoded bytes, long semantic digests, and hash suffixes. A collision or
  length hash is explained only when `hxc.symbols.json` records that reason;
- **temporary pressure** counts compiler-generated `hxc_tmp_` declarations and
  references per function. Its density is declarations per 1,000 function code
  lines, and declarations and references also have independent whole-corpus
  ceilings, so reusing the same spelling elsewhere cannot hide work;
- **control flow** records branches, nesting, labels, and every `goto` with its
  generated coordinate and typed lowering reason; and
- **size** records physical and code-bearing lines for files and functions.

Every reviewed limit is copied into the report. Measurements remain attached to
their file and function, the longest identifier retains its coordinate, and a
failed rule records the first exact path, line, and column that explains it.
Reports and violations are sorted by UTF-8 path bytes so filesystem discovery
order and locale cannot change the result.

The machine contracts are the closed [report
schema](specs/generated-c-maintainability.schema.json), [policy
schema](specs/generated-c-maintainability-policy.schema.json), and reviewed
[corpus policy](specs/generated-c-maintainability-policy.json). The policy is a
separate input: regenerating a report cannot silently raise its own limits.

## `goto` is fail-closed

The analyzer never approves a jump because its label happens to have a familiar
prefix. The caller must provide provenance from the typed control-flow plan.
The two currently admitted reasons are:

- `loop-break-through-switch`: Haxe `break` exits a loop even when it appears
  inside a switch, while a plain C `break` would exit only the switch; and
- `irreducible-cfg`: validation proved that the control-flow graph has more than
  one entry and cannot be represented by ordinary nested C constructs without
  changing behavior.

An occurrence that does not match exactly one typed authority is a failure.
Caxecraft admits neither reason: its current domain graph is reducible and must
remain free of compiler labels and jumps. The synthetic evaluation-order fixture
keeps the two exceptional forms visible and native-tested.

## Representative corpus

The initial corpus is intentionally small but varied:

| Corpus | What it protects |
| --- | --- |
| Body lowering | Small functions, shadowed-name collision proof, ordinary temporary density, and optional structural `#line` mapping. |
| Evaluation order | Legitimate temporaries required by Haxe sequencing, structured loops and switches, and the complete admitted-`goto` taxonomy. |
| Caxecraft domain, split | A real multi-module program whose C files mirror Haxe ownership. |
| Caxecraft domain, unity | The same semantic plan in one implementation file, with a layout-specific file-size limit rather than pretending it is a split tree. |
| Handwritten kernels | Small independently authored C files under `test/governance/fixtures/generated_c_maintainability/` that preserve call order and structured control flow. Governance generates and schema-validates their reports directly; they calibrate readability only and are never presented as generated-output or runtime-equivalence evidence. |

The handwritten kernels matter because “fewer temporaries” is not always an
improvement. C does not specify the order in which function arguments are
evaluated. A disciplined C programmer may introduce local variables to preserve
the order Haxe requires. The rubric must not reward deleting those variables and
silently creating an evaluation-order bug.

Large QA-only functions and the unity implementation file may have explicit,
named policy overrides. An override identifies one exact file or function and
keeps the measured limit visible; it is not a wildcard exemption. Updating a
snapshot alone cannot raise a policy limit.

## Correctness always wins

Maintainability analysis runs after typed lowering and is additive. A report
within its budgets cannot make any of these failures pass:

- invalid HxcIR or CAST structure;
- an Eval/native output difference;
- a GCC or Clang warning or strict-C error;
- an ASan or UBSan failure;
- a runtime, ABI, ownership, or allocation-policy mismatch; or
- nondeterministic generated bytes.

Likewise, the compiler must retain a temporary, helper, check, cleanup edge, or
bounded jump whenever removing it would weaken Haxe semantics. The right response
to a readability limit is a semantics-preserving compiler improvement or an
explicitly reviewed policy change, never unsafe emission.

## Claims and future work

Passing this rubric means only that the named, fixed corpus is within its
reviewed maintainability budgets. It does not prove that arbitrary Haxe produces
handwritten-quality C, and it does not change the capability inventory's
unsupported performance/marketing claim. The broader generated-versus-
handwritten benchmark methodology, runtime and code-size comparisons, and agent
comprehension studies remain owned by the later E9/E10 evidence work.

The generated-corpus reports are snapshot-owned. Refresh them only through the
registered snapshot updater, inspect the C and the policy difference, then run
the focused body-lowering, evaluation-order, project-layout, and Caxecraft
gates. Governance generates the two handwritten calibration reports directly
from their checked-in C files. The normal strict native and differential lanes
remain the final authority.
