# Configuration model

`hxc.json` is the optional user-facing orchestration configuration; HXML remains
the authoritative compiler input and transport. The implemented E8.T01 core
strictly parses and resolves the project model. The E8.T02/E8.T04 command and
build-driver work must compile that validated model into deterministic
HXML/argument-array plans and eventually print the exact plan through
`hxc inspect build`.

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

The schema lives at `schemas/hxc.schema.json`. The E8 configuration/build
milestone requires schema validation, path normalization independent of the
current working directory, config provenance in `hxc.manifest.json`, and
secret-free reproducible reports. Manifest integration remains E8.T04 scope.

## Implemented schema-1 core

The target-neutral implementation lives under `src/hxc/config/` and runs on the
Eval bootstrap host without depending on Reflaxe registration. It uses a closed,
location-bearing JSON tree rather than allowing `haxe.Json.parse`, `Dynamic`, or
reflection into configuration semantics. Duplicate keys, unknown keys,
malformed JSON or Unicode escapes, wrong value types, unsupported schema
identity/version, and unsafe paths all fail with `HXC0003` before a Haxe or C
compiler is invoked.

The root object requires `schemaVersion: 1` and admits these settings:

| Key | Default | Direct-define equivalent | Meaning |
| --- | --- | --- | --- |
| `hxml` | `build.hxml` | none | Authoritative Haxe input, relative to the config directory. |
| `output` | `build/c` | custom-target output remains the direct-Haxe authority | Generated-project root intent. |
| `profile` | `portable` | `reflaxe_c_profile` | Source-semantics/fallback preset. |
| `runtime` | profile preset | `hxc_runtime` | Independent runtime fallback policy. |
| `runtimeDiagnostics` | profile preset | `hxc_runtime_diagnostics` | Runtime-reason presentation only. |
| `environment` | `hosted` | `hxc_environment` | Hosted/freestanding/WASI/Emscripten environment. |
| `cStandard` | `c11` | `hxc_c_standard` | C11/C17/C23 source mode. |
| `projectLayout` | `split` | `hxc_project_layout` | Compiler-owned generated-C file arrangement. |
| `cExtensions` | `none` | `hxc_c_extensions` | Explicit GNU/MSVC compatibility family. |
| `build` | `debug` | `hxc_build` | Optimization intent, never a semantic relaxation. |
| `artifact` | `executable` | none | Executable/static-library/shared-library planning intent. |
| `overlays` | empty | none | Named partial settings selected explicitly by the caller. |

`$schema` is optional, but when present it must be the stable schema ID
`https://reflaxe-c.dev/schemas/hxc.schema.json`. A minimal reviewable project is:

```json
{
  "$schema": "https://reflaxe-c.dev/schemas/hxc.schema.json",
  "schemaVersion": 1,
  "hxml": "build.hxml",
  "output": "build/c",
  "overlays": {
    "release": {
      "build": "release",
      "output": "build/release"
    }
  }
}
```

An overlay accepts exactly the eleven setting keys above and cannot recursively
declare schemas or overlays. Names match `[A-Za-z][A-Za-z0-9_-]*`; asking for a
missing overlay is an error rather than a fallback to the base project.

### Resolution and provenance

The resolver applies the six layers in the documented order. Environment
presets and CLI values are typed inputs supplied by the future platform/command
adapters; they are not hidden JSON keys. Direct define collection consumes only
the eight documented configuration defines, rejects duplicate or misspelled
`hxc_*` configuration names, and leaves unrelated Haxe defines outside this
model.

Every effective field is a `HxcResolvedSetting<T>` with a stable origin kind,
detail, and numeric priority. The canonical schema-1 inspection record includes
all eleven fields, the selected overlay, logical config filename, and the complete
precedence list. It never serializes the config root, process cwd, temporary
directory, timestamp, locale value, or map iteration order. Profile-derived
runtime defaults are explicitly reported as `profile-preset:portable` or
`profile-preset:metal`; an explicit runtime value remains independent even if a
higher-precedence layer changes the profile.

### Paths and early combination checks

Project-file and overlay paths use portable `/` spelling and are normalized
against the directory that owns `hxc.json`, not `Sys.getCwd()`. Absolute paths,
Windows drive paths, backslashes, NUL/control characters, empty results, and
parent (`..`) segments or surrounding whitespace fail closed; redundant `.` and
`/` segments normalize away. Host-native absolute paths exist only
inside `HxcLoadedProjectConfig` for later I/O and are excluded from inspection.

The resolved HXML must end in `.hxml`; the output directory cannot equal or own
that input. A freestanding shared-library request and a non-hosted MSVC-extension
request are rejected before planning. Valid independent combinations remain
valid—specifically portable plus `runtime=none` and metal plus `runtime=auto`.
Tuple capability checks that require a compiler, SDK, sysroot, or target triple
remain E8.T04 planning work.

The executable corpus at `test/hxc_config/` keeps the schema and parser key/enum
sets synchronized and covers malformed inputs, all precedence sources, exact
per-field provenance, config-root host resolution, CRLF and locale variance,
define order, repeated cold requests, and warm compiler-server isolation. The
checked-in effective record is centrally owned by the snapshot updater.

No `hxc` command is claimed by this slice. Direct Haxe/HXML remains the recovery
path, and the compiler deliberately does not discover an ambient `hxc.json`.
E8.T04 will feed this resolved model into native build planning and
`hxc.manifest.json`; the separate dev/watch task will reuse the same normalized
input graph so rebuilds and one-shot builds cannot disagree about configuration.

## Target activation and native tuple

Direct Haxe/HXML builds use Haxe's custom-target output:

```text
--custom-target c=<generated-directory>
```

The exact pinned Haxe 5 compiler invokes `c.Init.init()` first. That hook
installs a static, scalar-Unicode platform configuration; initialization then
exposes `c` / `target.name=c`, retains `target.unicode`, and leaves
`target.utf16` disabled. Application source should use `#if c`, not the
path-bearing internal `c_output` transport or the implementation-owned
`reflaxe_c` compatibility marker.

`CompilerInit` derives `c_output` from the custom-target output because the
reviewed Reflaxe registration API still consumes that define. An expert may
supply the same normalized value explicitly, but a mismatch is `HXC0003` and
`-D c_output=...` by itself is not a conforming production invocation.
Likewise, `-D c`, a lifecycle-probe define, Eval, legacy Cross, or a target-name-
only rewrite must never bypass the platform guard.

The build plan resolves a native tuple rather than assuming an OS is sufficient:

```text
environment + target triple + C runtime/SDK + compiler family + capabilities
```

Capability defines such as `target.sys`, `target.threaded`, and
`target.atomics` come from that resolved adapter. A missing or unsupported tuple
is a planning diagnostic. Compile-only, cross-compile, emulated-run, and native
run evidence remain distinct in manifests and release reports. The accepted 1.0
lanes and tiers are in
[ADR 0007](adr/0007-strict-c11-target-and-platform-baseline.md).

Target-contract snapshots include the complete custom-target platform
configuration for auditability. Hosted currently enables `sys`; freestanding,
WASI, and Emscripten do not until their adapters prove the relevant surface.
Threads and atomics remain false in every M0 environment. Eval observations are
oracle/host facts and cannot be copied into a C manifest.

## Profile and runtime resolution

The canonical expert/debug defines are:

```text
-D reflaxe_c_profile=portable|metal
-D hxc_runtime=auto|minimal|none
-D hxc_runtime_diagnostics=off|summary|warn
-D hxc_environment=hosted|freestanding|wasi|emscripten
-D hxc_c_standard=c11|c17|c23
-D hxc_project_layout=split|package|unity
-D hxc_c_extensions=none|gnu|msvc
-D hxc_build=debug|release|minsizerel
```

`projectLayout` changes only compiler-owned file assignment after HxcIR,
representation, symbol, and declaration planning are finalized:

- `split` is the human-facing default. It emits a stable private umbrella at
  `include/hxc/program.h`, program-wide representation declarations at
  `include/hxc/detail/program_types.h`, module headers and sources under
  `include/hxc/modules/<package>/<Module>.h` and
  `src/modules/<package>/<Module>.c`, and a small hosted entry unit at
  `src/hxc/main.c`. A support unit appears only when layout assertions or
  virtual-dispatch objects require one.
- `package` keeps the same common types header, umbrella, entry, and optional
  support unit, but combines reached modules under each normalized Haxe package
  into `include/hxc/packages/<package>/package.h` and
  `src/packages/<package>/package.c`. The root package uses
  `include/hxc/packages/package.h` and `src/packages/package.c`. Complete types
  are ordered inside a package header; only hard cross-package layout edges add
  another package header include. Pointer and prototype edges continue to use
  the common forward declarations.
- `unity` emits the same planned declarations and functions through one
  `src/program.c` and the same stable private umbrella. It is useful for
  embedding, quick inspection, tiny programs, and build systems that prefer an
  amalgamation; it does not select different Haxe or C semantics.

An unknown value fails closed rather than being silently interpreted as one of
the three layouts. Package coalescing is an artifact-assignment choice, not a
semantic fallback: it reuses the same HxcIR, representation, finalized names,
runtime plan, and structural C declarations as split, package, and unity output.

All three modes are strict, deterministic compiler output. The neutral build plan
lists the exact selected sources/headers, and switching modes uses the normal
Reflaxe ownership transaction so stale generated files are removed without
claiming neighboring user files. Compiler-proven closed recursive cycles may
retain narrow safety partitions until their structural trampoline lowering is
implemented; that warning-safety exception is recorded in the function-
lowering contract rather than hidden in the printer.

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

The current generated-Haxe runtime selection is deliberately narrower than the
configuration surface: only compiler-known String literals passed to hosted
`Sys.println` or default `trace` request `io`. Portable defaults report one
summary; metal defaults warn once per source root; `off` stays silent. `none`
rejects all source requirements with one sorted `HXC2000` that retains each
operation, typed surface, source span, dependency chain, and available
alternative. The rejection occurs before any artifact or native-link step.
Non-hosted environments fail planning before any artifact is written.
Primitive and fixed-array/span programs retain a structured whole-program proof
with zero runtime intents and empty feature/include/source/define/library/symbol
sets.

The typed runtime planner also accepts internal manual feature constraints.
They never override semantic inference: `require` can only confirm a feature
already selected by a reachable source reason, and `forbid` fails when that
feature is required. A manual control cannot force unused runtime code into an
`auto` or `minimal` build, cannot bypass the minimal allowlist or environment,
and cannot add anything under `none`. Duplicate or conflicting controls fail
with `HXC2000`. No public feature-level override define or project-file spelling
exists at M0; future configuration work must preserve these rules and record
provenance. See [runtime feature planning](runtime-feature-planning.md).

`hxc_build` is optimization intent, never permission to change source
semantics. It defaults to `debug`; `release` and `minsizerel` may enable only
transformations backed by the same semantic proof. The E2 fixed-array/span
slice records the resolved build mode directly on dynamic bounds policy in
HxcIR and executes the same fail-stop behavior in all three modes. An unknown
value fails with `HXC0003` under the already resolved profile before output.
The complete provenance-aware build model and manifest field remain owned by
E8; no lowering may consult ambient optimization flags as a substitute. See
[fixed arrays and span-based iteration](span-lowering.md).

The accepted semantic implications of those independent axes are:

- portable strings use valid UTF-8 and Unicode-scalar indices; byte and
  `CString` operations are explicit
  ([ADR 0004](adr/0004-utf8-scalar-string-contract.md));
- tracing collection selects the precise non-moving `gc` slice only when
  reachable semantics require it
  ([ADR 0005](adr/0005-precise-nonmoving-collector.md));
- closed exception regions prefer result/status lowering; general exceptions
  select the contained `exception` slice, which `minimal` rejects by default
  ([ADR 0006](adr/0006-explicit-failure-edges-and-contained-unwinding.md)).

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

Source metadata supplies only declaration-local, literal facts through the
canonical namespaced vocabulary in [typed C authoring](typed-c-authoring.md).
It cannot inject compiler flags, shell fragments, a target triple, sysroot, or
ambient environment lookup. The M0 collector normalizes and deduplicates these
facts deterministically; E8.T04 later merges them with resolved project/toolchain
configuration and records provenance or a conflict diagnostic.
