# ADR 0009: RaylibHx companion and extraction boundary

- Status: Accepted
- Date: 2026-07-17
- Decision owners: project owner and compiler maintainers
- Related Beads: `haxe_c-xge`, `haxe_c-xge.1`, `haxe_c-zcj.1`,
  `haxe_c-ode.1`, `haxe_c-xge.3`, `haxe_c-xge.4`, `haxe_c-xge.5`,
  `haxe_c-xge.10`, and `haxe_c-xge.11`

## Context

Raylib is a useful proof that this target can consume a substantial real C
framework without hiding the native API or routing ordinary calls through a
parallel Haxe runtime. Caxecraft, a finite voxel sandbox, will exercise that
companion as a production custom-target consumer rather than as separately
authored C.

The companion needs two qualities that can pull in different directions. Its
raw declarations must be exact enough that raylib documentation, ABI probes,
and generated C remain authoritative. Its public Haxe surface should also use
the type checker, abstracts, constructors, and explicit ownership to remove
avoidable C mistakes. Mixing both concerns in one namespace would either make
generated declarations hand-curated or make ergonomic policy part of bindgen.

The compiler does not yet admit every C form used by raylib. In particular,
raylib uses C `float` throughout while ordinary Haxe `Float` is the binary64
`double` fixed by ADR 0008. Raylib also includes pointers, callbacks,
variadics, retained resources, aliases, platform-conditioned declarations, and
native build facts. A coherent Caxecraft subset must not be presented as full
header coverage.

Several existing projects occupy nearby names and source conventions:

- [`hxRaylib`](https://lib.haxe.org/p/hxRaylib/),
  [`raylib-haxe`](https://lib.haxe.org/p/raylib-haxe/), and
  [`raylib`](https://lib.haxe.org/p/raylib/) are existing Haxelib projects;
- [`foreignsasquatch/raylib-hx`](https://github.com/foreignsasquatch/raylib-hx)
  is an established hxcpp-oriented project using `@:native`, `cpp.*`, and
  PascalCase raylib spellings; and
- raylib's own [bindings inventory](https://github.com/raysan5/raylib/blob/master/BINDINGS.md)
  lists community bindings but is not a compatibility certification.

Those projects are useful prior art. Their target, ABI, dependency, platform,
and completeness claims are not evidence for this C target, and their source
is not an implementation input unless a later change records provenance and
license obligations explicitly.

## Decision

### The companion incubates here but is a separate logical product

The logical product name is **RaylibHx**. During incubation its public source
lives under this repository's existing `src` class path:

```text
src/raylib/                 curated semantic companion
src/raylib/raw/             ABI-faithful generated or mechanically owned layer
examples/caxecraft/         product consumer and compiler QA example
```

The future Haxelib project ID is `raylibhx`. It deliberately avoids the
established `raylib`, `raylib-haxe`, `hxRaylib`, and `raylib-hx` identities.
Registry state is mutable, so `haxe_c-xge.11` must recheck and reserve the exact
ID before publication. If it is unavailable, extraction stops for a new public
naming decision; tooling must not silently publish under a fallback.

The Haxe package namespace is lowercase `raylib`. The incubated source is part
of the root Haxelib only for development and testing. It must behave as if it
were already external: no public companion source may import `reflaxe.c.*`,
compiler test seams, unpublished sibling code, or repository-private generated
artifacts. Public target-facing types may depend only on Haxe and admitted
public `c.*` contracts.

RaylibHx initially supports only this C target. A clean domain boundary in
Caxecraft is encouraged, but Rust, JavaScript, hxcpp, HashLink, and other Haxe
targets receive no compatibility promise from this decision.

### Raylib 6.0 is the only initial upstream authority

The initial upstream is the raylib
[`6.0` release](https://github.com/raysan5/raylib/releases/tag/6.0) at commit
[`dbc56a87da87d973a9c5baa4e7438a9d20121d28`](https://github.com/raysan5/raylib/commit/dbc56a87da87d973a9c5baa4e7438a9d20121d28).
The reviewed release tree has these SHA-256 identities:

| Input | SHA-256 |
| --- | --- |
| `src/raylib.h` | `047e7255f93f8c34039cab906ad76136706b5c7b4c5b5b065d84141963ee9b6b` |
| `src/rlgl.h` | `1c9b802c219971dbf2ba9725fb8cea3f14616c039891810a2d31aa442aea4be3` |
| `src/raymath.h` | `2b8b88f5b3f748e3cf8bdbfb8b7da23a76c755dc40f9c6e455bfc09b3669d028` |
| `src/config.h` | `0495bce56ae6e1cd5c4e3e1e635fec8915c551e41ee3b62c7841537bed3878d7` |
| `CMakeLists.txt` | `66c4dcfcdb20754e57cfa8eee23473d300c95405512417b09e41652b5b5263bd` |
| `CMakeOptions.txt` | `4f58628ebdcde3a8075ca5a10871ea2901ddf1f0b4486ca9a29b76b3d0c4b844` |
| `LICENSE` | `882a5a819cf562aa3583aae3af3f2211dda15c63de9fc8cc4b399a2f9e78d799` |
| `tools/rlparser/output/raylib_api.json` | `d4e2dd0727a8edf4d4f828e23f86e3bb74e373861edd0eabef4a634ae74936d6` |

The production binding lock created by `haxe_c-xge.3`/`haxe_c-xge.10` must
also record the archive identity, resolved target tuple, Clang identity,
preprocessor definitions, include search, configuration choices, declaration
digests, and compiled ABI-probe results. The hashes above identify reviewed
inputs; they are not a substitute for that target-specific lock.

Repository-authored UTF-8 JSON selection text is hashed after normalizing only
LF/CRLF/legacy-CR line separators to LF, so Git checkout policy cannot change
its identity. Upstream raylib headers, archives, licenses, and parser inputs
retain exact raw-byte hashes; their bytes are never normalized to satisfy a
lock.

`raylib.h` is the authority for the initial public raw API. `raymath.h` and
`rlgl.h` are separate opt-in inventories and namespaces if later admitted;
neither is pulled into the core companion merely because it ships in the same
release.

Clang-derived typed facts plus compiled C probes are authoritative for
layouts, aliases, constants, signatures, calling conventions, and
configuration-dependent declarations. Upstream `rlparser` output is only an
inventory hint. The reviewed 6.0 JSON fails standard JSON parsing at line 4699
because a description contains unescaped quotes, so it cannot be the parsed
or ABI authority even when its hash matches.

No normal compile, test, package, or generated artifact may depend on
`../raylib`, a developer clone, a temporary extraction path, an implicit latest
download, or mutable upstream `master`. The provisioning task may accept an
explicit user-supplied source tree only after it validates the complete lock.

### The public API has raw and semantic layers

`raylib.raw.*` is the typed C escape hatch. It preserves:

- upstream declaration, field, function, enum, and constant spellings;
- C typedef and alias identities even when two aliases share a representation;
- exact scalar carriers such as `c.Float32` rather than ordinary Haxe `Float`;
- pointer depth, `const`, nullability, calling convention, and array facts;
- by-value versus by-reference arguments and returns;
- header/configuration ownership and source declaration coordinates; and
- explicit ownership, retention, thread-affinity, and failure annotations when
  those facts are not expressible by the C type alone.

The central raw function module is `raylib.raw.Raylib`. Raw calls retain the
upstream PascalCase field names, for example `InitWindow`, `BeginDrawing`, and
`GetScreenToWorldRay`. Raw functions do not overload, allocate, translate
errors, or invent default arguments. Because those Haxe spellings are already
the exact C spellings, generated raw source omits redundant `@:c.name` and
default-C-calling-convention metadata; the direct-import registry still
validates and records the resolved identities. Metadata remains present for a
real spelling, convention, ownership, or other policy difference.

`raylib.*` is the curated semantic layer. Its central module is
`raylib.Raylib`. Core function names also remain PascalCase so raylib's manual,
cheatsheet, and C examples transfer directly and generated C remains easy to
compare. Haxe ergonomics are added through types and explicit helpers rather
than by renaming every function:

- enum abstracts and flags prevent mixing unrelated integer domains;
- `Vector2`, `Vector3`, `Color`, `Camera3D`, and similar value carriers gain
  discoverable constructors and constants without heap wrapper objects;
- named colors remain available through `raylib.Colors` with recognizable
  upstream constant names, while value constructors may offer Haxe-style
  helpers such as `Color.rgba`;
- safe inline forwarding may narrow a raw API, but the raw declaration remains
  public when the semantic layer intentionally omits it; and
- a helper reports its allocation, ownership, lifetime, failure, unsafe,
  runtime, and portability effects in its API documentation and tests.

There are no unqualified global raylib functions and no wildcard-generated
facade. Application code chooses `raylib.Raylib` or
`raylib.raw.Raylib` explicitly.

### Precise-or-omitted is the coverage rule

Every upstream public declaration appears exactly once in the eventual
coverage inventory. Its state distinguishes at least:

1. discovered in the locked header;
2. representable as an exact raw Haxe declaration;
3. ABI-probed for the selected target/configuration;
4. compile-tested against the real header;
5. runtime-exercised where execution is meaningful;
6. reviewed for ownership/lifetime/unsafe effects; and
7. admitted through the semantic layer, intentionally raw-only, or omitted.

An omission names the unsupported C form or unresolved policy, the diagnostic
behavior, and its Beads owner. A Caxecraft core fixture cannot promote nearby
raylib functions or a whole declaration family. Bindings must not use
`Dynamic`, `Any`, `cpp.*`, opaque integer stand-ins, unchecked casts, or raw C
injection to manufacture apparent coverage.

The first `raylib.raw` slice is a coherent dependency-closed subset for
Caxecraft, not a miniature claim of full raylib support. Complete
precise-or-omitted header coverage belongs to `haxe_c-xge.10` after generic E6
bindgen and drift machinery is available.

### Ergonomics are zero-cost or explicitly costed

The default semantic helper is an inline/direct representation over the raw C
carrier. Its emitted-code test must prove the same call, value layout, runtime
feature set, and allocation behavior as the raw form. A different cost is
allowed only when the source API makes it visible and the documentation and
tests name it.

In particular:

- a non-retained UTF-8 text parameter may accept ordinary Haxe `String` only
  through an admitted conversion with explicit lifetime and allocation
  behavior; the first zero-allocation path is a validated embedded-NUL-free
  literal borrowed for the call;
- retained C strings never use an implicit temporary conversion;
- ordinary Haxe `Float` does not silently become C `float`; semantic helpers
  perform the explicit ADR-0008/`c.Float32` conversion owned by
  `haxe_c-ode.1`;
- constructors for value structs produce direct C aggregate values and no
  hidden object identity;
- direct Begin/End calls remain first-class; callback-based scope helpers are
  omitted until closure inlining and cleanup on every return/failure edge are
  proven without hidden allocation; and
- no import or wrapper selects an `hxrt` feature merely to make foreign calls
  convenient.

### Ownership, callbacks, variadics, and unsafe access stay visible

Raylib owns its native runtime, window, renderer, platform state, internal
allocators, and resource semantics. RaylibHx does not add a scene graph, ECS,
renderer, allocator, finalizer service, GC ownership layer, or shadow resource
registry.

Resource-returning APIs use distinct typed states where sound, such as owned
resource values, borrowed views, nullable handles, or explicit unsafe raw
values. An owned load has one documented unload operation and cannot rely on a
Haxe finalizer. Copying a C value struct does not duplicate its underlying
native resource ownership. Cleanup must be explicit in HxcIR on every admitted
edge before a semantic owned-resource helper is accepted.

Pointer APIs retain their exact raw pointer types. A semantic helper may expose
a `c.Span`, borrow, or owner only when length, provenance, mutability, escape,
and lifetime are verified. Native window handles, untyped user data, pointer
arithmetic, retained aliases, and similar boundaries remain in an explicit
unsafe namespace or raw-only state; they are never converted to a general
Haxe object.

Callbacks are omitted until E6 admits their exact calling convention and the
compiler can enforce retention, thread, reentrancy, closure, and failure
boundaries. Raylib callback registration may retain a function globally, so a
temporary closure is not an acceptable approximation. Variadics such as
`TraceLog` and `TextFormat` remain omitted unless exact C variadics plus format
validation are admitted; semantic code should prefer nonvariadic raylib APIs.
No Haxe exception or long jump may cross a raylib frame or callback boundary.

Raylib window, input, and drawing operations are main/render-thread-affine
unless locked upstream evidence says otherwise. The companion records that
constraint even when the C signature does not.

### Native dependency and build ownership remain external

Importing RaylibHx selects no runtime feature and does not bundle raylib source
or binaries. The application supplies a lock-compatible native raylib through
one explicit provisioning mode:

- checksum-pinned source provisioning for canonical CI and examples;
- a validated user-supplied/offline source or install; or
- an optional `pkg-config`/system mode whose resolved version, flags, and
  library identity are recorded.

Header includes, libraries, frameworks, defines, and platform facts flow
through the neutral build plan as argument arrays. They are not shell snippets,
Haxe macro side effects, or hard-coded Caxecraft compiler branches. Generated C
and project artifacts remain Reflaxe-owned; dependency sources and build caches
are orchestration inputs and must not leak host paths into deterministic
reports.

The package may document upstream platform reach, but RaylibHx claims a
platform/configuration only after its own compile/link/run obligations pass.
Compile-only, headless software, and native interactive evidence remain
separate states.

### Generated and curated source have separate ownership

Mechanically derived raw source is centrally generated from one binding lock,
contains an ownership banner and generator/schema version, and changes only
through the registered regeneration command. Identical locked input produces
byte-identical Haxe and inventories. Manual corrections change the generator,
typed override data, or lock; they do not patch an emitted raw file silently.

Semantic source under `src/raylib` is curated and reviewed as ordinary Haxe.
It consumes only public raw declarations and `c.*` contracts. It never imports
the Clang adapter or generator internals at application compile time.

Upstream examples and existing Haxe bindings may guide compatibility research,
but new implementation is derived from the accepted architecture and locked C
headers. If source or substantial expression is adapted, the exact origin,
revision, and license notice must be added before commit.

### Licensing is fail-closed before extraction

Raylib 6.0 remains under its
[zlib/libpng license](https://www.raylib.com/license.html), and every staged
package carries the exact upstream copyright/license notice and clearly states
that raylib itself is a separately supplied native dependency.

While incubated here, RaylibHx source inherits this repository's
`GPL-3.0-only` license. This ADR does not grant a separate or permissive
license. Before `haxe_c-xge.11` may publish or authorize extraction, the project
owner must either:

- retain `GPL-3.0-only` and document its consumer implications; or
- explicitly grant and record a different license for the cleanly identified
  companion files, after verifying that their provenance permits it.

The package fails closed if that owner decision, notices, or provenance audit
is absent. Existing binding source is not copied, so its license is not used as
a shortcut for this decision.

### Extraction requires an isolated real consumer

`haxe_c-xge.11` may authorize a later repository split only after all of these
are true:

- the complete public header has a deterministic precise-or-omitted inventory;
- the staged `raylibhx` artifact has deterministic contents, metadata,
  documentation, lock/upgrade workflow, and resolved licensing;
- public source imports no compiler-private module or repository test seam;
- an isolated Haxelib repository can install the packed artifact;
- Caxecraft builds and runs against that installed artifact without a source
  class path, sibling checkout, or unpublished generated file;
- the supported raylib version/configuration and platform matrix matches
  executable evidence; and
- extraction, rollback, generated-source ownership, and post-split maintenance
  boundaries are documented.

Passing the gate authorizes a later move. It does not itself publish, rewrite
history, bundle raylib, or promise another Haxe target.

### Caxecraft is a consumer, not package policy

Caxecraft lives under `examples/caxecraft` and imports only the same public
RaylibHx surface available to a packed consumer. Game-domain helpers, voxel
storage, terrain, collision, DDA picking, chunking, and rendering policy do not
enter the companion merely because the example needs them.

The initial game may deliberately avoid resource-owning mesh APIs and use a
small raw subset. Later measured rendering work can expand the binding through
normal interop issues, but it cannot add example-named compiler or package
special cases.

## Consequences

- Raylib documentation and C examples remain recognizable in Haxe.
- Users retain a typed exact escape hatch instead of falling back to raw text.
- Haxe ergonomics are reviewable separately from ABI generation.
- A playable subset can arrive before full bindgen without overstating
  coverage.
- Exact `c.Float32`, C imports, CString borrowing, build facts, and fixed
  storage are reusable compiler capabilities rather than game hooks.
- Owned resources, callbacks, variadics, and unsafe pointers remain visibly
  incomplete until their semantics are actually enforced.
- The incubated package can be extracted later, but no publication or
  relicensing is implied now.

## Rejected alternatives

- **One flat generated namespace:** it cannot add Haxe ergonomics without
  making generated ABI declarations hand-curated.
- **A high-level game engine facade:** it hides the direct-raylib feasibility
  proof and creates runtime/API ownership outside this project's goal.
- **Global unqualified functions:** they make imports collision-prone and blur
  raw versus semantic ownership.
- **Lower-camel renaming of the entire API:** it makes upstream documentation
  and generated C harder to compare for little semantic gain.
- **Copying an hxcpp binding:** `cpp.*`, build XML, target behavior, and
  platform claims do not model this C target's ABI.
- **Bundling raylib in the Haxelib:** it couples package releases to native
  binaries, platforms, and security updates and conflicts with explicit build
  ownership.
- **Depending on `../raylib`:** it cannot support clean CI, packaged consumers,
  or reproducible provenance.
- **Using `rlparser` JSON as authority:** the reviewed file is malformed and,
  even if repaired, descriptions and parser output do not replace Clang ABI
  facts or compiled probes.
- **Implicit finalizers or hidden GC ownership:** they obscure thread affinity,
  shutdown order, copies, and resource lifetime.
- **Designing the first API around future Rust/JavaScript portability:** it
  would weaken the direct C contract before a second backend proves a useful
  common abstraction.
