# RaylibHx semantic core

The `raylib` package is the small, hand-reviewed Haxe layer above the generated
[`raylib.raw`](raylib-raw-core.md) declarations. It keeps raylib's familiar
PascalCase names, but uses Haxe types to prevent easy mistakes and make common
values pleasant to create.

This is a zero-cost facade in the literal compiler sense. The checked snapshot
under `test/raylib_provisioning/expected/semantic` shows that:

- value constructors become ordinary C compound literals;
- semantic calls become the same direct raylib calls as raw calls;
- keys, buttons, projection, trace levels, and flags are distinct Haxe types;
- colors such as `Colors.RAYWHITE` remain the original raylib macros; and
- no wrapper, allocation, virtual call, reflection, or `hxrt` is emitted.

`raylib.raw` remains public. Use it when a reviewed helper is absent and the
program can satisfy the raw C lifetime, ownership, and safety rules. The facade
does not pretend callbacks, variadics, pointers, handles, or resource ownership
are safe merely because Haxe can name their C types.

## Constructing imported structs

`c.StructInit.make` is the reusable compiler operation behind constructors:

```haxe
final point:raylib.raw.Vector3 = c.StructInit.make({
  x: c.Float32.fromFloat(1.0),
  y: c.Float32.fromFloat(2.0),
  z: c.Float32.fromFloat(3.0)
});
```

The generic types preserve the closed literal fields and infer the result from
the assignment or return. The compiler checks the exact header-owned field set
and ABI types, evaluates fields in Haxe order, records an HxcIR aggregate
construction, and emits a designated C compound literal. It is not a cast: a
missing, extra, mistyped, or non-literal field set fails at its Haxe position.

Semantic value types are transparent Haxe abstracts over the raw structs. Their
constructors are inline and introduce no second C representation. A constructor
ending in `fromFloat` explicitly rounds Haxe binary64 to C binary32.
`Color.rgba` currently accepts compiler-visible channels that fit `0...255`.
Dynamic conversion needs a future named checked or clamping policy.

## Effects and safety

Admitted helpers allocate nothing, select no Haxe runtime feature, and add no
failure edge. A direct C/Raylib call can still fail or have global effects
according to raylib. Window, input, cursor, and drawing methods remain main or
render-thread operations. `c.CString` currently requires an embedded-NUL-free
static literal or a conditional/switch proven to select only such literals.
Program-local `c.CString` helpers may propagate that carrier; dynamic Haxe
strings and retained foreign pointers remain unsupported.

Begin/End calls stay explicit. A callback-style scope helper would need proven
closure inlining and cleanup on every exit before making the same claim.

### Texture ownership stays visible

`raylib.Texture2D` and `raylib.Rectangle` are curated zero-cost views used to
read dimensions and describe a draw rectangle. They do not turn a native GPU
texture into a garbage-collected Haxe object. The owning calls remain in
`raylib.raw.Raylib` for now, so a source review can see the exact load, validity
check, draw borrow, and unload boundary.

Caxecraft demonstrates the intended temporary pattern in one adapter rather
than spreading raw calls through the game. A successful load has one owner;
copies are non-owning aliases; every valid owner is unloaded once before the
window closes. A future semantic resource wrapper requires haxe.c cleanup
lowering that covers every admitted normal and exceptional exit. Until then,
omitting the convenient owning helper is safer than providing a wrapper whose
syntax suggests automatic cleanup that does not exist.

## Why the source uses `#if c`

The current facade represents raylib's C application binary interface (ABI)—
the exact values and function shapes exchanged with the native library. It
therefore exists only when Haxe selects the `c` custom target. Haxe removes the
inactive branch while compiling; generated C contains no run-time target check.
On another target, `raylib.Rectangle` is deliberately absent instead of being
a similar-looking type with different layout or lifetime rules.

For example, `raylib.Rectangle.fromFloat(...)` creates a value containing four
C `float` fields through `c.StructInit`. JavaScript does not have that C struct
or binary32 field contract, so compiling this same adapter as JavaScript would
be misleading. A JavaScript renderer should translate a portable game
rectangle into the representation its graphics library expects.

This guard is not an instruction to scatter target checks through game logic.
It keeps one target adapter honest while portable Caxecraft code remains
ordinary Haxe. The long-term shape is:

```text
portable Caxecraft rules and data
              |
      small rendering/input port
       /       |       \
  raylib/C   JS/Web   another native backend
```

Another target can implement the same game port with its native framework. It
does not have to imitate C structs or consume `raylib.raw`. Shared Haxe logic,
schemas, localization, world rules, and tests stay above the adapter. This
makes a future port smaller for a person or an LLM while preserving the close-
to-C, performance-oriented path today.

The current per-file guards are an honest incubation boundary, not necessarily
the final package layout. `haxe_c-xge.34` owns a later comparison of
target-specific source roots, one composition-root selection, and narrow
conditional aliases. Its goal is to localize target choice without hiding the
fact that native resources and application binary interfaces have genuinely
different rules on different targets.

## Verification

Run `npm run test:c-import`, `npm run test:raylib-provisioning`, and
`npm run test:all-sources`. The suite keeps separate raw and semantic snapshots,
rejects mixed enum domains, embedded-NUL titles, out-of-range color literals,
invalid dimension types, and the deliberately omitted *semantic* owning
resource API, and proves deterministic runtime-free output.
