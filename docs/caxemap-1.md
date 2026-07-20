# CAXEMAP 1 scenario format

Status: accepted format contract; parser, validator, persistence, and editor are
delivered by the ordered `haxe_c-xge.19.*` implementation slices.

CAXEMAP is Caxecraft's public map and story format. Creative, Adventure, the
in-game editor, Eval tests, and generated C all use the same typed model. Built-
in content does not get a private source-code-only representation.

This reference uses a few technical terms:

- A **scalar** is one Unicode character value, excluding UTF-16 surrogate
  halves. CAXEMAP text is valid UTF-8 and follows haxe.c's scalar-string
  contract.
- **Canonical** means there is one required byte spelling for a validated map.
  Saving the same model twice therefore produces identical bytes.
- **RLE**, or run-length encoding, stores “repeat block 2 for 30 cells” instead
  of writing the same cell 30 times.
- A **deferred** event is queued for the next rule boundary instead of starting
  another rule recursively in the middle of the current action list.

## Goals and boundaries

Version 1 is deliberately a small, line-oriented language rather than JSON or
an embedded scripting language. It is easy to diff, has a bounded parser, and
maps directly to closed Haxe enums and records. A map cannot run shell commands,
load native code, access the network, name an arbitrary host path, or embed Lua,
JavaScript, C, or Haxe code.

The format carries data and CaxeFlow rules. Reusable game mechanics remain
ordinary typed Haxe registered at compile time. Unknown required features make
loading fail; unknown optional features may be reported and ignored only when
their complete records use the extension envelope described below.

## Bytes, lines, and tokens

A document is at most 16,777,216 bytes and 131,072 logical records. Input may
use LF or CRLF line endings. The canonical writer always emits UTF-8 without a
byte-order mark, LF endings, and one final LF.

Blank lines and lines whose first non-space character is `#` are comments.
They are accepted for authoring but are not semantic and are not reproduced by
the canonical writer. Canonical files use no indentation except two spaces for
records inside a block.

Tokens are separated by one or more ASCII spaces. A token is one of:

- a signed base-10 integer with no leading `+` and no unnecessary leading zero;
- a bare identifier or logical path using the grammars below; or
- a quoted scalar string.

Quoted strings preserve printable Unicode scalars directly. The only escapes
are `\"`, `\\`, `\n`, `\r`, `\t`, and `\u{HEX}` with one to six uppercase hex
digits. `\u{HEX}` must denote a Unicode scalar, must not denote NUL, and is used
canonically only for non-tab C0 controls and DEL. Strings are never normalized,
so canonically equivalent spellings remain different author data. One string is
limited to 8,192 Unicode scalars.

Identifiers use lowercase ASCII so identity does not depend on locale:

```text
id          = segment, { ("." | "_" | "-"), segment } ;
segment     = letter, { letter | digit } ;
content-id  = id, ":", id ;
logical-path = path-segment, { "/", path-segment } ;
```

`letter` is `a` through `z`. A logical path is project-relative, uses `/`, and
contains no empty, `.` or `..` segment, backslash, drive prefix, or NUL. IDs,
tags, content IDs, message IDs, and paths are different nominal Haxe types even
though all have readable text spellings.

## Document shape and canonical order

The first record is exactly:

```text
CAXEMAP 1
```

The remaining top-level records appear in this canonical order:

1. required features, sorted by content ID;
2. optional features, sorted by content ID;
3. `map`, `asset-pack`, `title`, `mode`, and `world` exactly once;
4. palette entries sorted by numeric code;
5. chunks sorted by origin `z`, then `y`, then `x`, then ID;
6. objects sorted by ID;
7. dialogues, journal entries, objectives, and routes, each sorted by ID;
8. variables sorted by scope then ID;
9. sequences sorted by ID;
10. rules sorted by ID; and
11. `end-map`.

Ordering that carries meaning is never sorted: voxel runs, object tags,
dialogue lines, route objectives, predicate children, sequence parameters,
action lists, call arguments, and seeded choices retain source order. Rule
**execution** uses priority then stable rule ID even though storage is ID-sorted.

## Header and world records

```text
feature required <content-id>
feature optional <content-id>
map <id>
asset-pack <logical-path>
title message <message-id>
title literal "<user text>"
mode creative
mode adventure
world <width> <height> <depth>
palette <code> <block-content-id>
```

`feature required caxecraft:core` is mandatory in version 1. Duplicate feature
records are invalid. World dimensions are positive and no larger than 128 by 64
by 128; their checked product is at most 1,048,576 cells. Palette codes are
unique integers from 0 through 255. Code 0 must map to the registered air block.

Voxel data is divided into non-overlapping chunks:

```text
chunk <id> <origin-x> <origin-y> <origin-z> <width> <height> <depth>
  run <palette-code> <count>
  run <palette-code> <count>
end chunk
```

Every chunk edge is 1 through 32 cells, the chunk lies inside the world, and
the checked sum of positive run counts equals its checked volume exactly.
Chunks collectively cover the complete world exactly once. Cells are visited
with `x` changing fastest, then `y`, then `z`; this is called z/y/x order
because z is the outer dimension and x is the inner dimension.

## Object records

An object block gives one stable ID, zero or more author-facing tags, and
exactly one placement:

```text
object <id>
  tag <tag>
  placement player-spawn <x-milli> <y-milli> <z-milli> <yaw-degrees>
end object
```

Tags retain source order but must be unique within an object. Object IDs are
globally unique. Positions use integer thousandths of a block; this avoids
locale-sensitive decimal parsing and cross-target floating-point text drift.
Yaw is an integer from 0 through 359.

The complete placement forms are:

```text
placement player-spawn <transform>
placement checkpoint <transform>
placement item <item-content-id> <quantity> <transform>
placement entity <entity-content-id> <transform>
placement npc <npc-content-id> <dialogue-id> <transform>
placement prefab <prefab-content-id> <transform>
placement trigger-zone <x> <y> <z> <width> <height> <depth>
placement stateful <object-content-id> <initial-state-content-id> <transform>
```

`<transform>` means the four integer fields shown in the player-spawn example.
Quantities are positive and bounded by the registered item. Voxel bounds are
half-open: the origin is included and origin plus size is excluded. Validation
resolves every content ID through the compile-time registry and rejects
overlaps or locations disallowed by the registered placement kind.

## Story records

Text is either `message <message-id>` or `literal "<user text>"`. Built-in
English/Spanish content uses message IDs. A speaker is an object ID or
`narrator`.

```text
dialogue <id>
  line narrator message <message-id>
  line <speaker-object-id> literal "<user text>"
end dialogue

journal <id> title <text>
  body <text>
end journal

objective <id> <hidden|active|complete|failed> title <text>
  body <text>
end objective

route <id> title <text>
  objective <objective-id>
end route
```

Dialogue line and route objective order is semantic. All referenced speakers,
dialogues, and objectives must exist. Version 1 admits at most 1,024 dialogues,
128 lines per dialogue, 512 objectives, and 64 routes.

## CaxeFlow variables and values

A variable has one scope and one type implied by its initial value:

```text
variable <id> map flag <true|false>
variable <id> player counter <integer>
variable <id> quest state <content-id>
variable <id> local <sequence-id> flag <true|false>
```

The scopes are `map`, `player`, `quest`, and `local <sequence-id>`. Assignments
must retain the declared value kind; a counter cannot later receive a flag.
Version 1 admits at most 1,024 variables.

## CaxeFlow rules

A rule is a readable WHEN / IF / DO block:

```text
rule <id> priority <integer> once
  when enter-zone <zone-id>
  if (all (inventory-has <owner-id> <item-content-id> greater-or-equal 1) (object-state <object-id> <state-content-id>))
  do set-object-state <object-id> <state-content-id>
  do signal <signal-content-id>
end rule
```

The repeat policy is `once`, `repeat`, or `cooldown <positive-ticks>`. Exactly
one `when` and one `if` record are required. Use `(always)` when no condition is
needed. Each rule has 0 through 64 ordered actions.

Events are:

```text
enter-zone <zone-id>
leave-zone <zone-id>
interact <object-id>
block-changed <zone-id> <block-content-id>
use-item <item-content-id>
entity-defeated <entity-id>
signal <signal-content-id>
timer <timer-id>
objective-changed <objective-id>
state-changed <variable-id>
```

Predicates use a bounded prefix tree. Parentheses are tokens for the predicate
grammar and nesting is limited to 16:

```text
(always)
(all <predicate> ...)
(any <predicate> ...)
(not <predicate>)
(flag <variable-id> <true|false>)
(counter <variable-id> <comparison> <integer>)
(state <variable-id> <state-content-id>)
(object-state <object-id> <state-content-id>)
(inventory-has <owner-id> <item-content-id> <comparison> <quantity>)
(objective <objective-id> <hidden|active|complete|failed>)
(near <actor-id> <object-id> <maximum-milliblocks>)
(mode <creative|adventure>)
```

Comparisons are `equal`, `not-equal`, `less`, `less-or-equal`, `greater`, and
`greater-or-equal`.

Actions are:

```text
dialogue <dialogue-id>
journal <journal-id>
set-flag <variable-id> <true|false>
set-counter <variable-id> <integer>
add-counter <variable-id> <integer>
set-state <variable-id> <state-content-id>
give-item <owner-id> <item-content-id> <quantity>
take-item <owner-id> <item-content-id> <quantity>
spawn <object-id>
despawn <object-id>
set-object-state <object-id> <state-content-id>
checkpoint <checkpoint-id>
objective <objective-id> <hidden|active|complete|failed>
effect <effect-content-id> [at <object-id>]
signal <signal-content-id>
schedule <timer-id> <positive-ticks> <sequence-id> [<argument> ...]
call <sequence-id> [<argument> ...]
choose <seed-variable-id> <choice-count>
```

Arguments are `value flag <bool>`, `value counter <int>`, `value state
<content-id>`, `variable <variable-id>`, or `object <object-id>`. A `choose`
action is followed by exactly its declared number of choice blocks:

```text
choice weight <positive-integer>
  do <action>
end choice
```

The choice uses the named counter variable as an explicit deterministic seed.
There is no wall-clock or implicit randomness.

## Reusable sequences

Sequences provide reusable ordered actions without an embedded language:

```text
sequence <id>
  parameter <id> flag <true|false>
  parameter <id> counter <integer>
  parameter <id> state <content-id>
  do <action>
end sequence
```

Arguments are positional and must match parameter value kinds. Local variables
name their owning sequence. The validated sequence call graph has a maximum
depth of 16; a static unconditional cycle is invalid.

## Deterministic execution

One fixed simulation tick has these boundaries:

1. accept already queued input events in source order;
2. advance physics and ordinary game state;
3. evaluate matching rules by numeric priority, then stable rule ID;
4. execute each admitted rule's actions in source order against the defined
   tick state;
5. commit deferred signals and scheduled work for a later boundary; and
6. publish diagnostics and presentation events.

Signals never re-enter the current action stack. Once/repeat/cooldown state is
part of the saved scenario state. Per-tick events, deferred events, sequence
calls, spawned objects, scheduled work, and predicate depth have hard limits.
Exhausting a limit stops further rule work for that tick and produces a typed
visible diagnostic; it never hangs or silently drops an unknown suffix.

## Feature extensions

Core records above cannot be redefined. An optional extension is carried as:

```text
extension <feature-content-id> <record-id>
  data "<feature-owned canonical scalar text>"
end extension
```

The complete block is opaque only to a loader that does not implement an
**optional** feature. The block still obeys the global byte, scalar, line, and
record bounds. A required feature must be understood before any semantic model
is produced. Compiler-owned raw target code is never an extension payload.

## Validation and recovery

Loading has three typed stages: bounded token records, a parsed document with
source coordinates, and a fully validated `Scenario`. The live world changes
only after the third stage succeeds. Diagnostics identify line, column, logical
record, closed failure kind, and related typed identity where applicable.

The validator rejects malformed UTF-8, invalid escapes, overflow, duplicate or
missing singleton records, limit violations, invalid RLE totals, overlapping or
missing chunks, duplicate IDs/tags, unresolved references, type-mismatched
variables/arguments, impossible placements, invalid rule graphs, and unknown
required features.

Persistence writes canonical bytes to a validated sibling temporary, flushes
as required by the platform adapter, and then replaces the destination
atomically where the admitted platform guarantees that operation. A failed
create, write, flush, replace, or cleanup has a distinct typed stage. The last
known-good destination remains live and no partial candidate is accepted.

## Canonical example

The review fixture is
[`minimal.caxemap`](../examples/caxecraft/scenarios/minimal.caxemap). It is small
enough to read in one sitting while exercising a palette, RLE chunk, objects,
dialogue, an objective, typed state, and one WHEN / IF / DO rule.

## Haxe ownership

The model lives under `caxecraft.scenario` and uses nominal IDs plus closed
enums/records. It imports no Raylib or `c.*` API. The renderer-independent
editor, Eval oracle, and generated C must consume that model rather than
parallel bags of flags.

Native parsing and saving deliberately depend on the general haxe.c owners for
String/Unicode (`haxe_c-c3s`), collections (`haxe_c-7d0`), Bytes/core I/O
(`haxe_c-8al`), and filesystem adapters (`haxe_c-fwg`). Caxecraft may reduce a
missing feature and retain end-to-end evidence, but it must not bypass those
owners with raw C or a game-specific compiler branch.
