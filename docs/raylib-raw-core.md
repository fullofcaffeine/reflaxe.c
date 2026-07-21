# Raylib 6.0 raw core binding

`raylib.raw` is the ABI-faithful, extraction-ready foundation of RaylibHx. It
is generated from the pinned raylib 6.0 `raylib.h`; it is not a handwritten
facade and it is not a claim that the complete upstream header is supported.
The first lock deliberately selects the coherent dependency closure needed by
Caxecraft:

- 9 by-value records plus the direct `Camera`/`Camera3D` and
  `Texture2D`/`Texture` typedef aliases;
- 5 integer enum domains containing 143 constants; and
- 59 window, timing, input, camera, collision, primitive drawing, texture,
  HUD, and screenshot functions.

The machine authorities are:

- [`raylib-core-selection.json`](specs/raylib-core-selection.json), which names
  the reviewed surface, canonical ABI facts, and every omitted family;
- [`raylib-core-binding-lock.json`](specs/raylib-core-binding-lock.json), which
  records the upstream identity, Clang extraction request, declaration facts,
  source coordinates, generated paths, and declaration digest; and
- their closed JSON schemas beside them.

The raw Haxe files under `src/raylib/raw` and the native ABI probe at
`test/raylib_provisioning/native/core_abi_probe.c` are deterministic derived
views. Change the selection or generator and use the registered snapshot
updater; do not repair those outputs by hand.

The selection digest treats its repository-owned JSON as UTF-8 text and
normalizes LF, CRLF, and legacy CR separators to LF before hashing. This makes
the same closed selection portable across Git checkout policies without
ignoring whitespace or semantic changes. Hashes for upstream raylib headers,
archives, licenses, and other reviewed inputs remain exact raw-byte identities;
line-ending normalization must never be used to make a changed upstream input
look pinned.

## Extraction and verification

Clang's typed C AST is the declaration authority. Extraction always uses the
locked raylib header hash, strict C11, `RAYLIB_NO_DEPRECATED`, and the canonical
`x86_64-unknown-linux-gnu` ABI target. The canonical target makes checked
layout facts independent of the developer host; integration then re-extracts
the declaration model from each provisioned source and compiles the same ABI
probe with the actual lane compiler.

Given a verified raylib 6.0 source extraction:

```sh
python3 scripts/raylib/core_binding.py verify \
  --source-root /explicit/raylib-6.0-source \
  --clang clang

python3 scripts/raylib/core_binding.py render --check
```

To intentionally create a replacement lock for review, write it outside the
checkout first and compare it before updating snapshots:

```sh
python3 scripts/raylib/core_binding.py extract \
  --source-root /explicit/raylib-6.0-source \
  --clang clang \
  --output /explicit/review/raylib-core-binding-lock.json
```

The lock comparison ignores the host Clang executable's display identity but
does not ignore any selected declaration, canonical target, header hash,
source line, type spelling, layout, constant value, or function signature.
Reversing Clang's top-level discovery order must produce the same declaration
model.

## Raw Haxe contract

Every generated declaration uses ordinary typed Haxe plus the public `c.*`
contract:

- C `float` is `c.Float32`, never ordinary binary64 Haxe `Float`;
- exact-width integer fields and parameters use `c.Int32`, `c.UInt32`, or
  `c.UInt8` as appropriate;
- non-retained text parameters use `c.CString` and therefore inherit the
  compiler's checked, embedded-NUL-free literal-borrowing rule;
- records use `@:c.layout(c.Layout.Struct)`, exact field names, and the system
  `raylib.h` include;
- enum domains use exact upstream constant spellings and integer values; and
- fixed-arity functions retain PascalCase C names, C calling convention,
  by-value record arguments/results, and typed build facts.

When a Haxe name is already the exact C name, the generated source relies on
the direct-import identity default instead of repeating `@:c.name`. The same
rule makes the ordinary C calling convention implicit. Explicit metadata is
reserved for a real spelling or convention difference; the resolved identity
is still validated by `CSymbolRegistry` and locked/probed as ABI evidence.

The direct alias is worth calling out:

```c
typedef Camera3D Camera;
```

is represented by `typedef Camera = Camera3D` in Haxe. The imported declaration
plan preserves the exact `Camera` spelling at the foreign boundary, while
HxcIR treats the value as representation-identical to `Camera3D`. This emits no
wrapper, copy, cast, allocation, or runtime helper. Only one-level aliases to
an imported struct are admitted; alias chains, cycles, and aliases to another
semantic family remain fail-closed.

### The first owned resource

Most raw values above are copied plain data. A `Texture2D` is different: its
fields identify image data owned by raylib and usually backed by the graphics
device. Copying the struct copies the handle; it does not create a second
texture or a second owner.

The selected raw slice therefore treats five calls as one closed resource
contract:

1. `LoadTexture` returns one caller-owned handle;
2. `IsTextureValid` proves whether that load produced a usable resource;
3. `DrawTexturePro` borrows the handle for one screen-space draw;
4. `DrawBillboardRec` borrows it for one camera-facing world draw; neither draw
   retains the handle; and
5. `UnloadTexture` consumes the one valid owner exactly once, on the render
   thread, before `CloseWindow`.

The generator records this relationship in both the reviewed selection and
the binding lock, and it puts short ownership instructions beside the raw
functions. This does not make arbitrary raw resource use automatically safe.
Caxecraft keeps all five operations in one typed `CaxecraftTextures` adapter,
checks every load, and unloads five admitted image owners in reverse order.
The higher-level facade deliberately does not expose an owning `LoadTexture`
helper yet because haxe.c cannot prove cleanup on every return or failure edge.

`raylib.raw.Raylib` owns the header and platform-specific logical link facts.
Selecting a platform/configuration define changes only the neutral build plan;
it must not change the generated call semantics or select `hxrt`. The reviewed
[`raylib.*` semantic core](raylib-semantic-core.md) adds zero-cost constructors,
typed domains, named colors, and inline forwarding without removing this exact
escape hatch.

## Executable evidence

The network-free suite checks the lock and generated files, malformed/stale
inputs, unsupported C forms, wrong targets, missing symbols, embedded-NUL
titles, deterministic Haxe compilation, five neutral build-plan snapshots, and
an empty runtime plan:

```sh
npm run test:raylib-provisioning
```

Native integration additionally:

1. provisions the exact raylib source and library;
2. re-extracts the selected declarations with Clang;
3. compiles the generated Haxe consumer through the production C target;
4. compiles and links the independent native ABI probe; and
5. in the memory/software lane, runs both executables and checks exact output.

The probe asserts C `bool`, `float`, and `int` assumptions; every selected
record size, alignment, and field offset; both aliases; all 143 constant
values; and typed function-pointer compatibility for all 60 functions. This is
ABI evidence for the selected target/configuration lanes, not a promise for an
unprobed platform.

## Deliberate omissions

Callbacks, variadics, borrowed or retained pointers, other resource-owning APIs,
compound-literal color macros, and the rest of the public header remain
explicitly omitted with stable Beads owners in the selection document. They
must not be approximated with `Dynamic`, `cpp.*`, unchecked casts, raw C, hidden
allocation, or an unconditional runtime. The semantic facade owns value
constructors and color constants; generic bindgen and precise-or-omitted full
header coverage remain later work.
