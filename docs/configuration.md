# Configuration model

`hxc.json` is the user-facing configuration; HXML remains the compiler-facing
transport. M8 must compile validated JSON into deterministic HXML/command plans
and print the exact plan through `hxc inspect build`.

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

The schema lives at `schemas/hxc.schema.json`. M8 acceptance includes JSON Schema
validation, path normalization independent of the current working directory,
config provenance in `hxc.manifest.json`, and secret-free reproducible reports.
