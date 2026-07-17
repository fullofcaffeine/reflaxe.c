# Hello vertical slice

This is the first repository-owned product example that travels from ordinary
Haxe source through the production C target to a native executable. Its exact
output is:

```text
Hello from hxc
```

From the repository root, install the pinned tools and run the complete proof:

```sh
npm ci
python3 examples/hello/run.py
```

The proof compiles `Main.hx` through both Haxe Eval and the C target, compares
their stdout byte for byte, regenerates the C project twice, and builds it with
every available identity-matching GCC and Clang at O0 and O2. Required CI jobs
run GCC and Clang separately, so neither compiler family may be silently
skipped.

To inspect the direct Haxe/HXML path yourself:

```sh
node_modules/.bin/haxe \
  --cwd examples/hello \
  build.hxml \
  -D hxc_runtime_diagnostics=off \
  --custom-target c=_build/c

cc -std=c11 -Wall -Wextra -Werror -pedantic-errors \
  -Iexamples/hello/_build/c/include \
  -Iexamples/hello/_build/c/runtime/include \
  examples/hello/_build/c/src/program.c \
  examples/hello/_build/c/runtime/src/io.c \
  -o examples/hello/_build/hello

examples/hello/_build/hello
```

The reviewable generated baseline lives under [`expected/`](expected/). Its
runtime plan selects only `runtime-base`, `status`, `string-literal`, and `io`;
every selected record retains the one source reason from `Main.hx`. No object,
GC, dynamic, reflection, exception, allocator, or full-string slice is present.

This example proves only the compiler-known literal `Sys.println` path. It is
not evidence for general String values, general Haxe or standard-library
support, a packaged `hxc` command, a supported platform, or a stable public ABI.
The later E9 work owns polished installation and first-run guidance.
