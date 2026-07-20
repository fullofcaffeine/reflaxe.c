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
literal whose storage lasts for the call.

Begin/End calls stay explicit. A callback-style scope helper would need proven
closure inlining and cleanup on every exit before making the same claim.

## Why the source uses `#if c`

The current facade represents raylib's C ABI directly, so it exists only when
Haxe selects the `c` custom target. Haxe removes the inactive branch while
compiling; generated C contains no run-time target check.

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

## Verification

Run `npm run test:c-import`, `npm run test:raylib-provisioning`, and
`npm run test:all-sources`. The suite keeps separate raw and semantic snapshots,
rejects mixed enum domains, embedded-NUL titles, out-of-range color literals,
invalid dimension types, and deliberately omitted resource APIs, and proves
deterministic runtime-free output.
