# ${PROJECT_NAME}

This is a freestanding, runtime-free haxe.c starter. Type-check it first:

```sh
haxe build.hxml --no-output
```

Generate one C translation unit after `reflaxe.c` is installed:

```sh
haxe build.hxml --custom-target c=build/c
```

Only Haxe programs that pass the runtime-none eligibility proof can use this
shape. Adding allocation, exceptions, strings, hosted input/output, or another
runtime-owned feature can make generation fail with a reasoned diagnostic.
Startup code, linker scripts, hardware access, and the final native archive are
owned by your platform build. `hxc build` does not provide that integration yet.

The selected SPDX identifier is `${LICENSE}`. Read `LICENSE.txt` before you
distribute the project.
