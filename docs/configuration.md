# Configuration model

`hxc.json` is the user-facing configuration; HXML remains the compiler-facing
transport. M8 must compile validated JSON into deterministic HXML/command plans
and print the exact plan through `hxc inspect build`.

Precedence, from lowest to highest:

1. compiler defaults;
2. environment preset;
3. checked-in `hxc.json`;
4. named configuration overlay;
5. explicit CLI flags;
6. direct Haxe defines for expert/debug use.

Profile, runtime policy, environment, C standard, and artifact type are distinct
axes. A configuration error is reported before Haxe typing or native compilation
where possible. Unknown JSON keys are errors so typos do not silently change
semantics.

The schema lives at `schemas/hxc.schema.json`. M8 acceptance includes JSON Schema
validation, path normalization independent of the current working directory,
config provenance in `hxc.manifest.json`, and secret-free reproducible reports.

## Profile and runtime resolution

The canonical expert/debug defines are:

```text
-D reflaxe_c_profile=portable|metal
-D hxc_runtime=auto|minimal|none
-D hxc_runtime_diagnostics=off|summary|warn
-D hxc_environment=hosted|freestanding|wasi|emscripten
-D hxc_c_standard=c11|c17|c23
-D hxc_c_extensions=none|gnu|msvc
```

Profiles provide defaults for otherwise independent axes:

| Selected profile | Default runtime | Default runtime diagnostics |
| --- | --- | --- |
| `portable` or omitted | `auto` | `summary` |
| `metal` | `minimal` | `warn` |

An explicit runtime or diagnostic value overrides the preset. Thus portable can
be compiled with `none`, and metal can explicitly use `auto`. The manifest and
runtime plan record the effective value and whether it came from a compiler
default, profile preset, checked-in configuration, overlay, CLI flag, or direct
define.

Runtime policy is evaluated only after direct C representations and program-local
specializations:

- `auto` packages the exact dependency-closed features still required;
- `minimal` rejects features outside the versioned narrow allowlist;
- `none` requires a whole-program proof and permits no `hxrt` header, source,
  define, library, or symbol.

Runtime diagnostics do not change generated behavior:

- `off` keeps evidence only in `hxc.runtime-plan.json`;
- `summary` emits one aggregate notice if `hxrt` is selected;
- `warn` emits deduplicated, source-positioned warnings for root requirements.

Transitive dependencies remain report entries rather than separate warnings.
Policy violations are always errors. See
[ADR 0001](adr/0001-direct-c-and-selective-runtime.md).

## Source contracts versus build configuration

Configuration never silently changes which source API a declaration consumed.
Ordinary Haxe APIs retain portable Haxe semantics. Imports from `c.*`, typed C
externs, and validated C metadata declare explicit C-native boundaries; the
selected profile controls fallback and diagnostic defaults around those facts.

Headers, external includes, target defines, libraries, calling conventions,
layout attributes, and exports flow through one provenance-aware declaration and
build model. Haxe macros receive that resolved model rather than consulting
ambient process state ad hoc. Clang inputs and native probes remain explicit and
reproducible. See [ADR 0002](adr/0002-haxe-first-typed-c-authoring.md).
