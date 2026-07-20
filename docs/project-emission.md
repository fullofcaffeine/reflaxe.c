# Deterministic project-emission boundary

`CProjectEmitter` is the typed, filesystem-independent packaging boundary for
multi-file C projects. It accepts logical compiler facts and finalized text
artifacts; it never receives an output directory. `ReflaxeOutputWriter` is the
separate ownership adapter that validates the destination and delegates every
artifact write and stale deletion to Reflaxe's `OutputManager`.

This boundary still does not make the scaffold a general Haxe-to-C compiler.
E2.T03 admits one production status only for a validated, reachable primitive
static-function graph, and E2.T05 extends that same status with UB-safe
primitive operations and request-local helpers. That path emits its structural
private header, one or more deterministic sources, and `int main(void)` entry
with analyzed empty runtime/ABI/stdlib results.
E3.T01 admits a second bounded status for the same executable boundary when the
reachable graph also contains direct closed anonymous-record values. It emits
dependency-first private structs and layout assertions and retains an analyzed
empty runtime plan. It is not the generic `lowered-program` status and does not
claim public exports.
E3.T02 and E3.T03 use that same direct-value status for bounded private enum
layouts and closed generic function/type instances. Generic projects add a
validated specialization sidecar and remain runtime-free; they do not admit the
broader catch-all `lowered-program` state.
E3.T04-E3.T06 use the same status for private concrete class layouts, bounded
nonescaping construction, ordinary instance methods, and reachable closed-world
virtual dispatch. Dispatch projects conditionally add a validated explanation
sidecar and remain runtime-free with no public exports.
The bounded E6 direct-import slice uses that executable boundary for exact
header-owned scalar/typedef/enum/constant/by-value-struct calls and literal
borrowed C strings. Reached includes and logical library/pkg-config/framework
facts enter the neutral build plan with declaration provenance; the program
remains runtime-free and the generated project never owns the foreign header or
library.
Unsupported nodes stop at exact `HXC1001` without output. The broader generic
`lowered-program` status remains rejected until real semantic runtime, ABI, and
stdlib analyses exist for those programs. The checked-in structural project
corpus remains independently constructed by a test macro and retains honest
placeholders; it is not relabeled as generated-Haxe semantic evidence.

## Typed model and layout

Compiler state does not flow through open JSON objects. `CProjectEmissionPlan`,
`CBuildPlanSnapshot`, their configuration and build facts, content-address
records, and every sidecar record are explicit Haxe types. Closed states use
enum abstracts. The one
inherently untyped operation is decoding Reflaxe's existing JSON ownership
file; it is isolated in a private decoder, checked as schema 1, and converted
immediately to `ReflaxeOwnershipMetadata`.

Every `GeneratedFile` has:

- a normalized POSIX path relative to the output root;
- a closed `GeneratedFileKind`;
- canonical LF-only text with no NUL byte;
- a SHA-256 digest computed at construction and verified again before writing.

Payload kinds have fixed locations:

| Kind | Required layout |
| --- | --- |
| Public/private header | `include/**/*.h` |
| C source | `src/**/*.c` |
| Runtime header | `runtime/include/**/*.h` |
| Runtime source | `runtime/src/**/*.c` |

### Source-shaped and unity generated C

The executable emitter first builds one target-owned semantic declaration plan:
program-wide includes and representation declarations, module-owned globals,
module-owned function prototypes and definitions, layout/dispatch support, and
the hosted entry wrapper. That plan consumes already validated HxcIR and
finalized symbol/representation decisions. Only then does the pure
`CProjectLayoutPlan` assign declarations to files; `CASTPrinter` receives the
result and remains unaware of Haxe modules or layout policy.

The closed `projectLayout` setting has two variants:

| Layout | Compiler-owned shape | Intended use |
| --- | --- | --- |
| `split` (default) | `include/hxc/detail/program_types.h`, `include/hxc/modules/<package>/<Module>.h`, `src/modules/<package>/<Module>.c`, stable `include/hxc/program.h`, small `src/hxc/main.c`, and conditional `src/hxc/support.c` | Source-shaped navigation, review, debugging, and incremental native builds. |
| `unity` | Stable `include/hxc/program.h` plus ordinary `src/program.c` | Tiny programs, embedding, archival inspection, and build systems that prefer amalgamation. |

File ownership alone does not make generated C takeover-ready. The later
structured-control-flow, readable-name/temporary-cleanup, and
[maintainability-rubric](generated-c-maintainability.md) gates now pass for the
fixed body, evaluation-order, and Caxecraft corpora. That is intentionally a
bounded result; broader arbitrary-program handoff claims still require their
own evidence.

Normalized Haxe ownership determines module paths. Portable ASCII components
remain readable; short non-portable UTF-8 components are encoded losslessly for
the filesystem. The encoding namespace and Windows device names such as `CON`
are escaped as well. An encoding that would exceed the conservative 200-byte
stem limit is shortened to a readable byte prefix plus its complete SHA-256;
the remaining suffix budget accommodates recursive-cycle source suffixes below
common 255-byte component limits. Case-insensitive path or digest collisions
fail before printing.
Header guards are allocated as one deterministic project set and still pass
through the complete symbol registry. A collision-free path such as
`include/hxc/modules/caxecraft/domain/BlockCoord.h` becomes the readable
`HXC_CAXECRAFT_DOMAIN_BLOCK_COORD_H_INCLUDED`; only paths that normalize to the
same guard (or exceed the length bound) receive a compact SHA-256 suffix. The common
types header owns system/import includes, program-wide ABI assertions and
inline primitive helpers, and dependency-neutral forward declarations. Each
split module header owns that module's complete aggregate,
enum, and class definitions plus its private globals and function prototypes.
It includes module headers only for hard definition-time layout edges and
types, such as embedded by-value fields and native enums, that strict C11
cannot declare incompletely. Aggregate, class, and tagged-enum struct tags are
forward-declared in the common header, so function prototypes and `extern`
objects that name them remain soft declaration edges. Pointer-like storage is
also soft. Every module source includes the umbrella after all complete
definitions exist. This distinction admits a C-valid mixed cycle where module
A embeds B while a prototype in B mentions A, without hiding a real by-value
definition cycle. For a named anonymous-record typedef, the compiler
retains the innermost typedef declaration module before it unwraps aliases for
structural shape deduplication, so a type-only `Point.hx` owns the complete
record in `Point.h` instead of whichever function happened to consume it
first. Shape-identical aliases still intentionally share one representation
and C tag. The umbrella includes module headers in
dependency-first order and owns whole-program dispatch declarations. Sources
include that stable private umbrella so every reached linkage dependency is
declared before use. Generated initializer functions and virtual-table objects
use the same project-private external declarations and definitions in both
layouts; split merely assigns their definitions to the module/support unit
that needs cross-file linkage. Neither choice creates a public ABI.

`hxc.manifest.json.configuration.projectLayout` records the choice, while its
neutral build plan enumerates the exact source and private-header set consumed
by raw arguments, CMake, and Meson. `ReflaxeOutputWriter` treats a layout switch
as an ordinary ownership transaction: the complete old and new sets are
validated before the first write, stale owned paths are deleted by
`OutputManager`, and unowned neighbors are preserved. The
`test/project_layout/` multi-package corpus proves semantic-plan parity,
isolated-root/order/locale/warm-server determinism, exact manifests,
standalone headers, hard/soft mixed-cycle classification, bidirectional stale
cleanup, and GCC/Clang O0/O2 Eval parity. Caxecraft provides the larger
source-tree snapshot and both-layout E2E proof.

The implemented split is per Haxe module and preserves package directories;
it is not a third package-coalesced layout. A distinct mode that groups several
modules into one package translation unit remains future HXC-COMP-011 scope,
owned by `haxe_c-xge.18.5`.

The emitter owns these independently versioned sidecars:

- `hxc.initialization-plan.json`: the admitted direct executable's
  dependency-first type order, source-positioned reasons, class/field phases,
  exact initializer execution order, entry ID, cycle policy, and empty runtime
  feature set. Structural fixtures do not fabricate this semantic sidecar;
- `hxc.manifest.json`: resolved logical configuration, the typed neutral build
  plan, artifact kinds, and SHA-256 digests;
- `hxc.symbols.json`: the finalized schema-2 `hxc-c-symbol-v2` table, retaining
  full semantic keys separately from readable emitted names and explaining
  every collision suffix;
- `hxc.runtime-plan.json`: either the structural fixture's explicit
  `placeholder-no-runtime-analysis`, with no fabricated proof, or the admitted
  direct executable's schema-2 `hxc-runtime-plan-v2`
  analyzed record. Primitive, closed-record, concrete-class, bounded
  constructor, bounded enum, and direct-import graphs use
  `analyzed-runtime-free`; literal
  hosted output uses `analyzed-runtime-features` with exactly `runtime-base`,
  `status`, `string-literal`, and `io`. Both are produced by the typed runtime
  feature planner and contain resolved policy/diagnostic provenance, planning
  purpose and environment, direct decisions, root reasons, manual constraints,
  dependency edges, selected feature/artifact/symbol/library/define sets, and a
  nested `hxc-no-runtime-eligibility-v1` proof only for the empty case. That
  proof records reachable-program counts, zero explicit HxcIR runtime intents,
  exact direct decisions and program-local helpers, and empty runtime feature,
  include, source, define, library, and symbol sets. The
  `selected-program-local-helpers` decision appears only when that compilation
  selected at least one helper. A reachable virtual-dispatch graph additionally
  records `reachable-program-local-virtual-dispatch`; this is a direct compiler
  decision, not an `hxrt` feature. A reached import graph records
  `typed-header-owned-c-imports` plus exact reached function/value/type counts;
  header and linker build facts remain outside the runtime feature sets;
- `hxc.abi.json`: either `placeholder-no-export-analysis` or the admitted
  executable's `analyzed-no-public-exports` plus its C `main` entry;
- `hxc.stdlib-report.json`: either `placeholder-no-stdlib-analysis` or the
  executable's exact `analyzed-no-stdlib-use` or bounded
  `analyzed-selected-stdlib-use` record. The latter currently admits only
  literal `Sys.println` and default `haxe.Log.trace` capabilities;
- `hxc.specializations.json`: omitted when no generic instance is reachable;
  otherwise a schema-1 `hxc-generic-specialization-v1` record containing full
  collision-checked semantic keys, normalized arguments, source-rooted reasons,
  recursion, hard limits, and conservative function/enum code-size attribution.
  The emitter revalidates its counts, order, hashes, sources, reason totals, and
  complete payload totals before accepting it.
- `hxc.dispatch.json`: omitted when no instance call is reachable; otherwise a
  schema-1 `hxc-closed-world-virtual-dispatch-v1` report recording each
  source-positioned direct/virtual choice, minimal hierarchy layouts and slots,
  selected concrete tables, representation-checked implementations, finalized
  adapter names, and exact counts. Its closed schema is
  `docs/specs/dispatch-report.schema.json`, and its runtime feature set must be
  empty. See [closed-world virtual dispatch](virtual-dispatch.md).

Two optional non-payload adapters are compiler-owned and content-addressed in
the same manifest: `cmake/CMakeLists.txt` and `meson.build`. They are derived
views, not independent configuration authorities.

The compiler manifest cannot contain its own digest without a recursive
definition, so its declared hash scope is every compiler artifact except
`hxc.manifest.json` and Reflaxe's `_GeneratedFiles.json`. The `GeneratedFile`
value for the manifest still carries and verifies its own in-memory SHA-256.
Floating modulo contributes a provenance-bearing `m` link fact to the neutral
manifest. Reached C imports similarly contribute only their validated include,
logical library, pkg-config, or framework facts. These are ordinary C build
requirements and do not change the empty runtime plan. A nonempty runtime payload is additionally traced to the schema-2
feature catalog: packaging verifies each registered source SHA-256, and this
manifest hashes the emitted copy plus its exact source/include build plan.

## Neutral build plan and adapters

`hxc.manifest.json.build` is the schema-1 authority consumed by all three build
lanes. The M1 seed records one non-installing executable target, strict
`c11|c17|c23-experimental` mode with extensions disabled, warning policy,
owned source/header groups, include directories, source-provenance-bearing
header requirements, typed definitions, logical libraries, pkg-config
packages, and Apple frameworks. A consumer may ignore both generated adapters
and construct an argument array directly from this object; no shell command is
stored or required by the schema.

`CBuildAdapterEmitter` renders the same immutable snapshot into CMake and Meson:

- CMake values use dynamically delimited bracket arguments, which do not
  perform variable substitution or escape processing;
- Meson values use escaped single-quoted literals;
- compile definitions retain their literal source value and a separately
  validated `compilerValue`. String definitions are encoded as C string
  literals with fixed-width octal escapes where punctuation could be
  interpreted by a build language;
- source paths containing `$` or `;` fail closed because those bytes carry
  generator/list meaning. Ordinary spaces and apostrophes remain supported and
  are exercised in real build directories and source names.

This seed intentionally stops at one executable named `hxc_program`. Static or
shared libraries, installed exports, configurable target names, direct/Ninja
orchestration, toolchain/sysroot/cross files, and the full CLI build plan remain
E7/E8 work. The adapters therefore do not imply a public ABI, supported
platform, or release artifact.

`RuntimeFeaturePackager` materializes an already validated non-empty plan as
exact runtime `GeneratedFile` values. The generated literal-output path packages
only the compiler-selectable foundation/status/literal/I/O artifacts; the
alloc/full-string fixtures remain native-seed evidence. `CProjectEmitter`
rejects any unrelated runtime plan or payload. See
[runtime feature planning](runtime-feature-planning.md).

Because every selected closure includes `runtime-base`, the private generated
program header also carries one structural C11 assertion that the runtime ABI
major matches the compiler's internal 0.5.0 contract. Same-major minor changes
remain compatible; a changed major fails before linking. Runtime-free projects
contain no `hxrt` include, version marker, or compatibility assertion. This is
not a public application ABI: primitive production emission rejects public
headers and records `analyzed-no-public-exports`; E7 owns the future typed export
checker and wrappers.

`examples/hello` is the first product consumer of that same emission path. Its
checked-in baseline retains the private header/source shape and runtime plan,
while the example runner recompiles the manifest-owned generated tree rather
than maintaining a second writer or hand-authored C implementation.

## Ownership and filesystem safety

`_GeneratedFiles.json` remains Reflaxe's stale-file authority. The target never
deletes or rewrites an artifact directly. Before allowing `OutputManager` to
write, the adapter validates the complete old ownership set and the complete
new artifact set:

- paths are normalized, relative, unique, and cannot reserve the ownership
  filename;
- existing path components must resolve exactly beneath the canonical output
  root, and directory entries that exist only as dangling links are rejected,
  so output-root/descendant symlinks and traversal cannot redirect writes;
- an existing destination must have been listed in the prior Reflaxe ownership
  manifest, otherwise it is treated as user-owned and the build fails before
  writing anything;
- malformed ownership JSON and non-string/duplicate/escaping entries fail with
  `HXC9000` before stale deletion can run.

After validation, files are passed to `OutputManager.saveFile` in UTF-8 byte
order. Reflaxe compares contents and skips an artifact write when bytes are
unchanged. Once all current files are recorded, Reflaxe removes only old paths
still listed in its ownership metadata. Unlisted files in the output tree are
left untouched.

Reflaxe rewrites `_GeneratedFiles.json` as invocation/activity state and
increments its `id`; that engine metadata is not a compiler artifact and is
excluded from the unchanged-mtime guarantee. Fresh builds in different roots
still produce byte-identical ownership metadata because both begin with the
same ID.

## Determinism and evidence

The plan contains no output root, timestamp, locale, random value, discovery
counter, or process identity. Artifacts and build-plan collections are sorted
by UTF-8 bytes; identical build facts merge their sorted declaration
provenance.
Path layout, artifact hash coverage, and sidecar status are snapshot-owned.
The adversarial comparator orders relative paths by UTF-8 and compares raw
bytes, reporting the first differing artifact and exact byte offset (including
missing-path and end-of-file differences) without normalizing compiler output.

Run:

```sh
npm run test:project-emitter
npm run test:build-adapters -- --toolchain clang
npm run test:runtime-features
npm run test:arithmetic-semantics
npm run test:c-import
npm run snapshots:check
npm run test:native
```

The required build-adapter command expects CMake 3.21 or newer, Meson 1.4 or
newer, Ninja, and the selected C compiler on `PATH`; CI installs Meson from the
checksum-pinned requirements file and runs separate GCC and Clang lanes. The
ordinary project-emitter gate uses explicit `SKIP` messages for unavailable
optional adapters.

`test/project_emitter` proves byte identity across isolated roots, reversed
discovery, a fixed explicit locale, CRLF source/HXML inputs with an absolute
classpath, and real warm compiler-server requests. It scans normal artifacts
for host paths, carriage returns, timestamp/UUID-shaped values, and unstable
JSON keys; validates content hashes and unchanged-file mtimes; and exercises a
full-to-renamed-symbol transition that removes every prior owned payload while
preserving an unlisted neighboring source. Reflaxe's `_GeneratedFiles.json`
invocation/activity fields are validated separately and excluded from the
normal-artifact server comparison. The suite also rejects unowned collisions,
invalid paths, existing or dangling symlinks, malformed ownership JSON, and
non-canonical line endings before a partial write, while preserving the
production exact-`HXC1001` unsupported boundary and rejection of unproven generic
lowered-program status. The required adapter matrix reads the raw JSON with an
argument-array consumer, then independently configures, builds, and runs the
same strict-C11 structural project and both checked-in C AST/printer translation
units through CMake and Meson. Paths include spaces and apostrophes, and an
adversarial string definition contains semicolon, CMake generator-expression,
quote, and backslash characters. The native matrix also compiles every emitted
header and links/runs the structural project under GCC and Clang. The
function-lowering, arithmetic, aggregate-lowering, enum-lowering, direct-import, and
generic-specialization suites
prove that the narrow direct-value production paths pass through this
ownership boundary with analyzed empty runtime/ABI/stdlib records, a typed static-initialization plan,
optional request-local helpers, and exact reached math/import build facts. The structural
corpus itself remains emitted-C shape
and ownership evidence, not generated-Haxe semantic evidence or a public ABI,
runtime, standard-library, or generated-output licensing claim.
