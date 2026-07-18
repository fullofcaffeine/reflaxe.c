# Raylib 6.0 provisioning

This repository consumes one exact native Raylib release for the RaylibHx and
Caxecraft work: Raylib `6.0` (`6.0.0`) at commit
`dbc56a87da87d973a9c5baa4e7438a9d20121d28`. The compiler and ordinary Haxe
build never fetch Raylib, search `../raylib`, or consult a home-directory
installation. Native dependency provisioning is an explicit orchestration
step.

The machine authority is
[`raylib-provisioning-lock.json`](specs/raylib-provisioning-lock.json), with its
shape documented by
[`raylib-provisioning-lock.schema.json`](specs/raylib-provisioning-lock.schema.json).
The lock records the immutable archive URL, archive byte size and SHA-256, a
content identity for all 1,384 source-tree files, hashes for the reviewed API
and build inputs, exact CMake configurations, and logical link facts for each
admitted host. Validate it without downloading anything:

```sh
python3 scripts/raylib/provision.py verify-lock
```

## Provisioning authorities

The three authorities are deliberately separate. A failure in one never falls
back to another.

### Checksum-pinned source

This is the canonical CI and example authority. Network access is disabled by
default and can be enabled only for the first immutable archive acquisition:

```sh
python3 scripts/raylib/provision.py build-source \
  --authority pinned-source \
  --configuration memory-software \
  --platform linux \
  --cache-root /explicit/raylib-cache \
  --build-root /explicit/raylib-build \
  --cc gcc \
  --cxx g++ \
  --generator Ninja \
  --allow-network \
  --report /explicit/reports/raylib.json
```

After the locked archive is present, omit `--allow-network`. The cache must
retain both the verified archive and its verified extraction; a source tree
without its archive is rejected. CMake uses the in-tree GLFW source and cannot
silently fetch another dependency.

### User-supplied offline source

An already available extraction can be selected explicitly. It must match the
complete locked source-tree identity, not merely the version macros or a few
reviewed files:

```sh
python3 scripts/raylib/provision.py build-source \
  --authority offline-source \
  --configuration desktop \
  --platform macos \
  --source /explicit/raylib-6.0-source \
  --build-root /explicit/raylib-build \
  --cc clang \
  --cxx clang++ \
  --generator Ninja \
  --report /explicit/reports/raylib.json
```

Offline source rejects `--allow-network`; it is not an alias for pinned-source
provisioning. A developer clone is acceptable only when its non-`.git` files
exactly match the locked tree.

### System pkg-config

The optional system lane accepts only a package reporting version `6.0.0`, an
exact copy of the locked `raylib.h`, and one unambiguous library file. Its
resolved compiler and linker argument arrays, header hash, and library hash are
recorded rather than treated as pinned-source evidence:

```sh
python3 scripts/raylib/provision.py resolve-system \
  --configuration desktop \
  --pkg-config pkg-config \
  --work-root /explicit/raylib-resolution \
  --report /explicit/reports/raylib-system.json
```

Use `--library-file` when a pkg-config library directory contains both static
and shared candidates. System mode is desktop-only: pkg-config metadata cannot
prove the reviewed `PLATFORM=Memory` and software-renderer configuration, so a
headless system request fails instead of changing authority.

## Reviewed build configurations

| Configuration | Raylib platform / graphics API | Native evidence |
| --- | --- | --- |
| `memory-software` | `PLATFORM=Memory`, `OPENGL_VERSION=Software` | Compile, link, and deterministic execution in Linux CI |
| `desktop` | `PLATFORM=Desktop`, `OPENGL_VERSION=3.3` | Compile and link on Linux, macOS, and Windows; no display-dependent run claim |

Both configurations are release static libraries with examples, audio, and
external GLFW disabled. The desktop Linux build explicitly selects X11 and
disables Wayland. Windows source builds explicitly select CMake's
`MultiThreadedDLL` MSVC runtime, and the `clang-cl` integration consumer uses
the matching `/MD` compile and link policy. This keeps Raylib and generated C
on one CRT model without presenting toolchain runtime libraries as Raylib link
facts.

The neutral generated build plan uses logical facts rather than shell text:

| Host | `memory-software` libraries | Desktop libraries/frameworks |
| --- | --- | --- |
| Linux | `raylib`, `m` | `raylib`, `m`, `pthread`, `GL`, `X11`, `dl`, `rt` |
| macOS | `raylib` | `raylib`; frameworks `OpenGL`, `Cocoa`, `IOKit`, `CoreFoundation` |
| Windows | `raylib`, `winmm` | `raylib`, `opengl32`, `gdi32`, `winmm`, `shell32`, `user32` |

These are argument-array facts. The generated `hxc.manifest.json` never stores
`-l...`, `-framework ...`, quoting, command substitution, or another shell
fragment.

## Generated-Haxe integration proof

The integration runner provisions real Raylib, invokes the production custom C
target, verifies the neutral plan and empty `hxrt` plan, compiles the emitted C
under the strict warning policy, and links it with the reviewed native facts.
For example, after acquiring an exact offline source tree:

```sh
python3 test/raylib_provisioning/run.py \
  --integration \
  --authority offline-source \
  --configuration memory-software \
  --platform macos \
  --source /explicit/raylib-6.0-source \
  --build-root /explicit/raylib-integration-build \
  --cc clang \
  --cxx clang++ \
  --generator Ninja \
  --run \
  --report /explicit/reports/raylib-integration.json
```

`--run` is accepted only for `memory-software`. Desktop jobs stop after a real
compile and link so they do not imply that CI opened a window or exercised a
display server.

The smoke source is intentionally a test-only, dependency-closed direct-C
import subset. It proves by-value `Color`, a closed trace-level enum, direct
Raylib calls, generated C, and zero `hxrt`; it is not the public RaylibHx raw
binding promised by the next task. A fixture-local header supplies two macro
constants because aggregate C macros are still outside the admitted compiler
slice. Public RaylibHx coverage must add typed binding ownership rather than
promoting this support header.

## Evidence and reproducibility

Every native report records the authority, upstream identity, target
architecture, compiler family/version/target, C++ compiler, CMake and build
tool identities, configuration definitions, normalized command argument
arrays, reviewed inputs, output hashes, exact libraries/frameworks, and the
scope of compile/link/run claims. Source reports hash normalized
`compile_commands.json` content so temporary roots do not perturb that field.

Host checkout, source, cache, build, generated, and temporary paths are replaced
with declared placeholders before serialization. Reports contain no timestamp,
locale-formatted value, or shell command string. `cacheAuthorityExplicit` says
which authority was selected; it does not claim a cache was initially empty.
CI establishes the separate clean-cache fact by rejecting a pre-existing job
cache before the pinned acquisition.

The ordinary test remains network-free:

```sh
npm run test:raylib-provisioning
```

It runs adversarial archive/authority/pkg-config/path tests, compiles the Haxe
smoke repeatedly, compares generated bytes across unrelated roots, validates
five centrally managed build-plan snapshots, and proves no `hxrt` artifact or
symbol is selected. Update those snapshots only through:

```sh
npm run snapshots:update -- --suite raylib-provisioning
```
