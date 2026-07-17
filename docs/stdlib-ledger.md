# Standard-library ownership and parity ledger

The standard-library ledger is now a generated planning contract over the exact
pinned Haxe `5.0.0-preview.1` source tree. It is not a claim that the C target
implements the standard library.

[`docs/specs/stdlib-ledger.json`](specs/stdlib-ledger.json) is the machine
authority. It contains the applicability policy, compiler-first lowering rule,
diagnostic policies, module ownership rules, exact API exceptions, source-tree
fingerprints, and generated public-API rows. The adjacent
[`stdlib-ledger.csv`](specs/stdlib-ledger.csv) is a generated review view. Do not
edit the CSV by hand.

## What is inventoried

The typed probe resolves every applicable common `*.hx`, `haxe/**`, and
`sys/**` module through the pinned compiler. It records non-private types,
constructors, public instance/static fields, overloads, enum constructors,
anonymous typedef fields, abstract forwards, operators, array/resolve access,
and implicit conversions. `@:noCompletion` declarations are not public product
surface, except for the compiler's hidden carrier for a source-authored abstract
constructor. Every entry retains its upstream module, normalized source path,
source SHA-256, typed signature, and stable `hxc-stdlib-api-v1` identity.

Haxe's macro model stores abstract implementation members as C-like static
fields. The probe restores the public Haxe view: the typed leading `this`
parameter identifies instance methods, the exact declaration position recovers
the otherwise omitted `static` fact for properties, and the compiler's hidden
`_hx_new` carrier becomes `new`. This narrow source check reads only the typed
field's own byte range; it does not parse bodies or infer API names from text.

Other targets' implementation directories are excluded because they are not C
API declarations. `haxe.macro`, display, and HXB modules execute in the Haxe
compiler rather than in generated C. The JavaScript/Node HTTP implementations
are likewise excluded while the portable `haxe.Http` and `sys` contracts remain
included. These exclusions and every discovered top-level std root are part of
the checked snapshot, so a Haxe upgrade cannot silently change the boundary.

The normal inventory runs on the real `CustomTarget(c)` carrier. C correctly
does not advertise `target.threaded` or `target.atomics` yet, and upstream
atomic/thread modules deliberately reject such a target. A second, test-only
custom target exposes those two facts solely while reading the 16 pinned common
declaration modules. It has no generator or target override tree. Rows from that
probe are marked `capability-gated`; this is not C typing, execution evidence,
or permission to expose either C target capability. E5.T11 still owns the C
adapter, memory-model, collector-root, and race evidence.

At this snapshot the ledger contains 183 source modules and 2,311 public API
records. Exactly the typed `Std.int(Float):Int` row is `conformant`, based on the
existing ADR 0008 differential, strict-C, optimization, and UBSan suite. The
other 2,310 rows are `planned`. This deliberately preserves the current narrow
compiler claim.

## Per-row contract

Each generated row names:

- primary implementation ownership: direct compiler lowering, thin C/OS
  wrapper, target source override, runtime slice, platform adapter, or explicit
  unsupported target-specific API;
- portable/metal and hosted/freestanding/WASI/Emscripten applicability;
- candidate runtime feature ownership, which never means eager selection;
- implementation and test Beads owners;
- current parity status and executable tests;
- diagnostic policy while a path is unsupported or runtime-constrained.

Every `planned` or `partial` row maps to `planned-unsupported`, which means the
current compiler stops at source-positioned `HXC1001`; a placeholder return is
never allowed. Rows with a possible later runtime fallback also map to the
`HXC2000`/`HXC2001` policy that applies only after the API has a real lowering.
Stable plan keys are the reproducible Beads links. E5.T12 owns the eventual
upstream test sweep unless a row names a narrower test owner.

## Hard lowering rule

The ledger repeats ADR 0001's required selection order for every stdlib API:

1. emit direct, idiomatic, strictly defined C or use a compatible C
   library/intrinsic;
2. generate the smallest program-local specialization;
3. request the smallest dependency-closed, optimized `hxrt` slice only after
   the first two choices are proven infeasible;
4. issue a source-positioned diagnostic when policy forbids the remaining
   implementation.

Representation, specialization, constant/static dispatch, escape and lifetime
analysis, layout/table construction, and dead-feature elimination belong in
`hxc` whenever sound. A `runtimeFeatures` field records candidate ownership; it
does not let mentioning an API select runtime code. A runtime-free build still
means no `hxrt` include, source, define, library, or symbol.

The global ownership ledger is distinct from a generated program's
`hxc.stdlib-report.json`. The ledger says who must implement and test the pinned
surface. The per-program report says which reachable stdlib operations were
analyzed for one build; its first bounded selected form lists only literal
`Sys.println` and default `haxe.Log.trace`. Neither artifact can substitute for
generated-C behavior evidence.

## Review and update workflow

Check the current pin and both generated views with:

```text
npm run test:stdlib-ledger
npm run snapshots:check -- --suite stdlib-ledger
```

After intentionally changing a module rule, exact API override, or Haxe pin,
regenerate through the central snapshot owner:

```text
npm run snapshots:update -- --suite stdlib-ledger
```

The updater renders twice, prints the semantic JSON and exact CSV diffs before
writing, and reruns the owning suite. A pin update also requires the full
toolchain/source API audit from ADR 0007. Generation fails on an unowned module,
overlapping ownership, a stale or multiply matched API override, an unknown
runtime feature or Beads key, a planned row without a diagnostic, or a
`partial`/`conformant` row without an existing test. CI and pre-commit run the
same check; CI never updates either file.
