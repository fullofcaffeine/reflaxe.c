# Deterministic project-emission boundary

`CProjectEmitter` is the typed, filesystem-independent packaging boundary for
multi-file C projects. It accepts logical compiler facts and finalized text
artifacts; it never receives an output directory. `ReflaxeOutputWriter` is the
separate ownership adapter that validates the destination and delegates every
artifact write and stale deletion to Reflaxe's `OutputManager`.

This boundary does not make the scaffold a usable Haxe-to-C compiler. E2.T02
can lower an admitted typed main body through HxcIR and structural C, but
production still creates no artifact: unsupported nodes stop at exact `HXC1001`,
and a supported body stops later at `HXC1000` before static-function, call,
entry-point, and project emission. The checked-in project corpus is constructed
directly in a test macro, independently of the real body-lowering fixture. A
`lowered-program` emission request is rejected until real semantic runtime and
ABI analyses can replace the honest placeholders.

## Typed model and layout

Compiler state does not flow through open JSON objects. `CProjectEmissionPlan`,
its configuration, build facts, content-address records, and every sidecar
record are explicit Haxe types. Closed states use enum abstracts. The one
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

- `hxc.manifest.json`: resolved logical configuration, typed build
  requirements, layout groups, artifact kinds, and SHA-256 digests;
- `hxc.symbols.json`: the finalized `hxc-c-symbol-v1` table;
- `hxc.runtime-plan.json`: an explicit `placeholder-no-runtime-analysis`
  record, with no feature selection and no fabricated no-runtime proof;
- `hxc.abi.json`: an experimental `placeholder-no-export-analysis` record;
- `hxc.stdlib-report.json`: a `placeholder-no-stdlib-analysis` record.

The compiler manifest cannot contain its own digest without a recursive
definition, so its declared hash scope is every compiler artifact except
`hxc.manifest.json` and Reflaxe's `_GeneratedFiles.json`. The `GeneratedFile`
value for the manifest still carries and verifies its own in-memory SHA-256.
CMake, Meson, direct command plans, and stable runtime/ABI schemas remain owned
by their later Beads issues.

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
counter, or process identity. Payloads and build requirements are sorted by
UTF-8 bytes; identical build facts merge their sorted declaration provenance.
Path layout, artifact hash coverage, and sidecar status are snapshot-owned.
The adversarial comparator orders relative paths by UTF-8 and compares raw
bytes, reporting the first differing artifact and exact byte offset (including
missing-path and end-of-file differences) without normalizing compiler output.

Run:

```sh
npm run test:project-emitter
npm run snapshots:check
npm run test:native
```

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
non-canonical line endings before a partial write, while preserving both the
production exact-`HXC1001` unsupported-body boundary and the later `HXC1000`
no-output capability boundary. The native matrix independently
compiles every emitted header and links/runs the emitted strict-C11 structural
project under GCC and Clang. This is emitted-C shape and ownership evidence,
not generated-Haxe semantic evidence or a public ABI, runtime, standard-library,
or generated-output licensing claim.
