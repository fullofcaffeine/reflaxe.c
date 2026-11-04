# ${PROJECT_NAME}

This starter keeps library code in ordinary typed Haxe. Type-check it first:

```sh
haxe build.hxml --no-output
```

Generate C after `reflaxe.c` is installed:

```sh
haxe build.hxml --custom-target c=build/c
```

The `@:c.export` marker records C export intent, but the stable public C ABI,
header package, ABI comparison, and `hxc export` workflow are not complete yet.
Do not publish the current generated symbol spelling as a stable API. The
generated C also needs an external native archive build until `hxc build` lands.

The selected SPDX identifier is `${LICENSE}`. Read `LICENSE.txt` before you
distribute the project.
