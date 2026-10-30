# ADR 0010: `hxc` diagnostic event framing

- Status: Accepted
- Date: 2026-08-30
- Decision owners: project owner and compiler maintainers
- Related Beads: `haxe_c-gx7`, `haxe_c-6qn`, and `haxe_c-huo`

## Context

The first `hxc` JSON response carried only a diagnostic code, message, and
remediation. That shape omitted source ranges, severity, phase, related
locations, compiler profile, and exact native-tool text. IDEs and agents could
not recover those facts safely from human prose.

The compiler already owns stable `HXCdddd` IDs and structured schema-1 records.
The CLI also owns `HXC-CLI-dddd` failures for routing and command behavior. A
public format must preserve both systems without copying compiler policy into
the CLI.

The CLI already promises one JSON value on stdout. Changing that stream to
JSON Lines would break existing response parsing and separate diagnostics from
their command result.

## Decision

JSON mode emits one schema-2 `hxc` response followed by one line terminator.
Its ordered `diagnostics` array is the event stream. Every array item uses the
standalone schema-1 diagnostic-event contract.

An event contains its stable ID, severity, phase, message, remediation, source
range, notes, related locations, and exact command context. Compiler events
also retain profile, kind, and context. Those compiler-owned values are null
for events that do not come from the compiler.

The typed CLI adapter consumes `CDiagnosticRecord` directly. It does not define
a second compiler catalog or translate a compiler ID. CLI-owned diagnostics
use the reserved `HXC-CLI-dddd` namespace.

Exact compiler and native-tool text travels in typed attachments. An attachment
names its producer, channel, and tool. Its text preserves every line break and
the trailing newline. Attachments remain data inside the response and never
create another stdout value.

Human rendering remains the existing one-line diagnostic on stderr. Child
stderr also remains available in the response-level `stderr` field. Keeping
both surfaces preserves interactive use and exact process evidence.

Response and event schemas have independent integer versions. Any change to a
required field, field meaning, allowed value, or closed object shape increases
the affected version. Consumers reject unknown versions, and one response does
not mix event versions. The CLI does not provide a legacy schema-1 response
mode.

Exact command arguments and attachments can contain sensitive values. Tools
that store or forward machine responses must apply their normal secret and
privacy controls.

## Consequences

Consumers parse one response and then process diagnostics in order. They can
show concise human text while retaining structured source and native details.
Future build, test, and ABI commands can publish the same event contract.

Adding a field to these closed schemas is a versioned compatibility change.
This cost is deliberate: silent shape growth would make strict automation
unreliable.

JSON Lines and prose-only native errors are rejected. JSON Lines would break
the single-response contract. Prose-only errors would lose structure or force
consumers to parse unstable text.
