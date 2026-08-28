# hxc command router

The `hxc` bootstrap now gives people and automation one stable command entry
point. It provides help, version reporting, process-exit categories, and a
versioned JSON response. Product commands such as `build`, `run`, and `doctor`
are recognized but fail as unavailable until their separate implementation
tasks land. Direct Haxe and HXML invocation remains authoritative.

Run the development entry point through Haxe Eval:

```sh
haxe -cp src --run Run help
haxe -cp src --run Run version
haxe -cp src --run Run build --json
```

The last command currently exits with code 69 (`unavailable`). Recognition is
not an implementation claim.

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
