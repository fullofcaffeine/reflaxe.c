# ${PROJECT_NAME}

This is a hosted haxe.c application starter. Type-check it first:

```sh
haxe build.hxml --no-output
```

Generate C after `reflaxe.c` is installed:

```sh
haxe build.hxml --custom-target c=build/c
```

The generated C still needs a native C compile and link step. The `hxc build`
and `hxc run` product commands are planned but are not available yet, so this
template does not claim a one-command native build. Platform support is limited
to the runtime and target combinations documented by your installed haxe.c.

The selected SPDX identifier is `${LICENSE}`. Read `LICENSE.txt` before you
distribute the project.
