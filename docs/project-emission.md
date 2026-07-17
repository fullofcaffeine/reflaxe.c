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

The emitter owns these schema-1 sidecars:

- `hxc.initialization-plan.json`: the admitted primitive executable's
  dependency-first type order, source-positioned reasons, class/field phases,
  exact initializer execution order, entry ID, cycle policy, and empty runtime
  feature set. Structural fixtures do not fabricate this semantic sidecar;
- `hxc.manifest.json`: resolved logical configuration, the typed neutral build
  plan, artifact kinds, and SHA-256 digests;
- `hxc.symbols.json`: the finalized `hxc-c-symbol-v1` table;
- `hxc.runtime-plan.json`: either the structural fixture's explicit
  `placeholder-no-runtime-analysis`, with no fabricated proof, or the admitted
  primitive executable's schema-2 `hxc-runtime-plan-v2`
  analyzed record. Primitive-only graphs use `analyzed-runtime-free`; literal
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
  selected at least one helper;
- `hxc.abi.json`: either `placeholder-no-export-analysis` or the primitive
  executable's `analyzed-no-public-exports` plus its C `main` entry;
- `hxc.stdlib-report.json`: either `placeholder-no-stdlib-analysis` or the
  executable's exact `analyzed-no-stdlib-use` or bounded
  `analyzed-selected-stdlib-use` record. The latter currently admits only
  literal `Sys.println` and default `haxe.Log.trace` capabilities.

Two optional non-payload adapters are compiler-owned and content-addressed in
the same manifest: `cmake/CMakeLists.txt` and `meson.build`. They are derived
views, not independent configuration authorities.

The compiler manifest cannot contain its own digest without a recursive
definition, so its declared hash scope is every compiler artifact except
`hxc.manifest.json` and Reflaxe's `_GeneratedFiles.json`. The `GeneratedFile`
value for the manifest still carries and verifies its own in-memory SHA-256.
Floating modulo contributes a provenance-bearing `m` link fact to the neutral
manifest. This is an ordinary C build requirement and does not change the empty
runtime plan. A nonempty runtime payload is additionally traced to the schema-2
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
function-lowering and arithmetic suites prove that the narrow
primitive production path passes through this ownership boundary with analyzed
empty runtime/ABI/stdlib records, a typed static-initialization plan, optional
request-local helpers, and the exact
math build fact. The structural corpus itself remains emitted-C shape
and ownership evidence, not generated-Haxe semantic evidence or a public ABI,
runtime, standard-library, or generated-output licensing claim.
