# hxc command router

The `hxc` bootstrap now gives people and automation one stable command entry
point. It provides help, version reporting, process-exit categories, and a
versioned JSON response. The read-only `inspect` command and the first Clang
semantic-lock stage of `bindgen` are available. Product commands such as
`build`, `run`, and `doctor`
are recognized but fail as unavailable until their separate implementation
tasks land. Direct Haxe and HXML invocation remains authoritative.

Run the development entry point through Haxe Eval:

```sh
haxe -cp src --run Run help
haxe -cp src --run Run version
haxe -cp src --run Run build --json
haxe -cp src --run Run inspect runtime --manifest build/hxc.manifest.json --json
haxe -cp src --run Run bindgen vendor/base.h vendor/widget.h --output bindings/widget --json
```

The `build` command currently exits with code 69 (`unavailable`). Recognition is
not an implementation claim.

## Capture a Clang semantic binding lock

`hxc bindgen` invokes Clang directly with an argument array. It does not use a
shell, and it does not parse declarations with regular expressions. The
current stage records Clang's version, effective target, exact semantic
arguments, transitive input hashes, and a canonical semantic abstract syntax
tree (AST) in `hxc.bindings.lock.json`. One or more entry headers form an
ordered translation unit. Only their transitive declaration closure is
available to later extern and wrapper generation.

```sh
haxe -cp src --run Run bindgen vendor/base.h vendor/widget.h \
  --language c \
  --target arm64-apple-darwin \
  --sysroot /path/to/sdk \
  --include-dir vendor/include \
  --define WIDGET_FEATURE=1 \
  --output bindings/widget
```

Use `--dry-run` to print the lock without writing it. In JSON mode, parse the
CLI response and then parse its `stdout` field as the schema-2 lock. The lock
schema is
[`schemas/hxc-bindings-lock.schema.json`](../schemas/hxc-bindings-lock.schema.json).

The lock separates effective configuration from provenance. Equivalent,
conflict-free definition order produces the same configuration hash, while
entry-header and include-directory order remains exact because preprocessing
order can change declarations. Repeated entry/include paths and repeated
definition names fail before Clang. `--language` accepts `c` and `c++`; C++
capture does not claim that direct C++ calls are available without the later
reviewed C shim.

This stage does not generate Haxe externs or safe wrappers. Later E6 tasks own
type mapping, layout facts, callbacks, wrapper policy, and drift workflows.
Clang errors remain in the child stderr stream with their original source file,
line, and column.

## Inspect compiler decisions

`hxc inspect` reads an existing `hxc.manifest.json`; it never builds or changes
the output. The command checks every manifest-owned artifact path and SHA-256
digest before it reports a fact. This prevents a report from silently mixing
files from different builds.

Use `-D hxc_inspection_reports` when compiling if you need the typed inventory,
HxcIR, structural C abstract syntax tree (AST) summary, declaration effects, or
combined lowering reports. The define adds report sidecars to the manifest. It
does not change generated C or runtime selection. Reports already emitted by a
normal build, such as configuration, runtime, symbols, includes, build facts,
standard-library ownership, and application binary interface (ABI), need no
extra define.

```sh
haxe build.hxml -D hxc_inspection_reports --custom-target c=build/generated
haxe -cp src --run Run inspect lowering \
  --manifest build/generated/hxc.manifest.json
```

The stable report names are `manifest`, `config`, `typed-inventory`, `hxcir`,
`c-ast`, `lowering`, `runtime`, `symbols`, `includes`, `build`, `declarations`,
`macros`, `stdlib`, `abi`, `sizes`, and `all`. The `declarations` and `macros`
views include typed ownership, unsafe-boundary, portability, runtime, metadata,
and source-reason evidence. The `sizes` view reports exact artifact bytes and
the runtime plan used as allocation evidence; it does not invent measured heap
allocation counts.

Host-absolute paths are redacted by default. Use `--show-sensitive` only when
the output is trusted and the exact host paths are necessary. JSON mode keeps
the schema-1 inspect report in the shared response's `stdout` string, including
its terminating newline. Parse the CLI response once, then parse that field.
The report schema is
[`schemas/hxc-inspect-report.schema.json`](../schemas/hxc-inspect-report.schema.json).

## Output contract

Human mode writes command output to stdout. Logs, child stderr, and diagnostics
go to stderr. Diagnostics use one form:

```text
HXC-CLI-0001: unknown command `wat` Remediation: Use `hxc help` to list commands.
```

Add `--json` before the forwarded-argument separator to make stdout contain
exactly one schema-1 JSON object and one line terminator. Human logs and
diagnostics still go to stderr. Child stdout, stderr, exit code, and signal are
also fields in the JSON object, so an agent does not lose native process facts.
The authoritative schema is
[`schemas/hxc-cli-response.schema.json`](../schemas/hxc-cli-response.schema.json).

Stable exit categories are:

| Category | Router-owned code | Meaning |
| --- | ---: | --- |
| `success` | 0 | The router or command completed successfully. |
| `usage` | 64 | The command or option syntax is invalid. |
| `unavailable` | 69 | The command is known, but its implementation is not in this build. |
| `internal` | 70 | The CLI itself failed unexpectedly. |
| `command` | Command-owned | A command or child process failed; its exact exit code is retained. |

Unknown global options and undocumented `--experimental-*` options fail. The
router never silently consumes them. Arguments after `--` are forwarded
unchanged to the selected command owner.

## Extension boundary

New command behavior implements the typed `HxcCliExecutor` seam. A handler
returns an `HxcCliExecution`; it does not write directly to process streams.
This keeps JSON framing under one owner and lets Eval and the future native
haxe.c executable use the same parser and renderer. Command-specific parsers
must reject unknown flags and return every child stream, exit status, and signal
without translating a failure into success.
