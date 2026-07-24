# CAXEMAP 1 scenario format

Status: accepted format contract. The target-neutral Eval codec/validator,
fixed-tick CaxeFlow executor, and renderer-independent editor semantics are
implemented. Native persistence and the child-friendly Raylib editor remain
later ordered `haxe_c-xge.19.*` slices.

CAXEMAP is Caxecraft's public map and story format. Today the target-neutral
codec, validator, editor semantics, and first-playable authored source use the
same typed model. The native first playable currently packages that source and
uses deterministic generated localization lookup, level, and content adapters
derived from validated source data. Localization selects text without owning
Raylib drawing. The level adapter supplies terrain, initial fluids,
fluid presentation, and player spawn to the real game loop. It is a temporary
learning bridge, not the intended loading architecture: it proves validated
authored data can drive the native game while `haxe_c-xge.39` adds runtime
CAXEMAP loading and removes this adapter from the required play path. New
features must not treat the generated module as a durable content API. The
`asset-pack` path resolves through the schema-2
built-in manifest at `packs/caxecraft/base/content.json`; direct native JSON
loading is not implemented yet. Actor construction, starting inventory, item
placement, and encounter wiring remain temporary Haxe scaffolding.
`haxe_c-xge.20.4` owns replacing that coupling with one validated composition
path. Direct native CAXEMAP loading
(`haxe_c-xge.39`), Creative mode (`haxe_c-xge.20`), and the visual in-game
editor (`haxe_c-xge.19.6`) remain planned work. Built-in content must not keep a
private source-code-only representation.

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
ordinary typed Haxe. The current action catalog is a closed compile-time
inventory, not a public plug-in API and not map-supplied code. Unknown required
features make loading fail; unknown optional features may be reported and
ignored only when their complete records use the extension envelope described
below.

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
3. `map` and `asset-pack` exactly once;
4. when localized message IDs are used, one `default-locale` followed by locale
   blocks sorted by locale ID and messages sorted by message ID;
5. `title`, `mode`, and `world` exactly once;
6. palette entries sorted by numeric code;
7. chunks sorted by origin `z`, then `y`, then `x`, then ID;
8. fluids sorted by ID;
9. objects sorted by ID;
10. dialogues, journal entries, objectives, and routes, each sorted by ID;
11. variables sorted by scope (`map`, `player`, `quest`, then `local` by owning
   sequence ID) and then variable ID;
12. sequences sorted by ID;
13. rules sorted by ID;
14. optional extension blocks sorted by feature content ID then record ID; and
15. `end-map`.

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
default-locale <locale-id>
locale <locale-id>
  message <message-id> "<complete translated text>"
end locale
title message <message-id>
title literal "<user text>"
mode creative
mode adventure
world <width> <height> <depth>
palette <code> <block-content-id>
```

`feature required caxecraft:core` is mandatory in version 1. Duplicate feature
records are invalid.

The locale records are optional for a literal-only map. Once the map uses a
`message <message-id>` reference, it must embed one to eight locale blocks and
name exactly one of them as the default. The default locale defines the complete
message-ID set; every other locale translates exactly that set. There is no
fallback chain: lookup tries the requested locale, then the declared default,
so it cannot cycle. Duplicate locales/messages, a missing or extra translation,
an unknown default, or a referenced message absent from the catalog rejects the
scenario before it becomes live. Locale IDs use the same lowercase ID grammar
(`en`, `es-mx`), while display/platform adapters may map them to conventional
spellings such as `es-MX`.

Keeping these small strings inside CaxeMap makes a map edit, message reference,
and its translations one atomic, canonical change. Large binary assets remain
external logical-path dependencies. The current native playable temporarily
derives closed Haxe enum names from message IDs at build time (for example,
`nia_welcome` becomes `NiaWelcome`) so raylib receives static C string literals;
that bridge does not create a second content source.

World dimensions are positive and no larger than 128 by 64
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

Fluids are authored independently of solid terrain:

```text
fluid <id> <fluid-content-id> source <x> <y> <z>
fluid <id> <fluid-content-id> volume <x> <y> <z> <width> <height> <depth>
```

A `source` continually feeds the bounded fluid simulation. A `volume` fills
the half-open box once when the level starts; its cells may then flow or drain.
This distinction prevents every cell in an authored lake from becoming an
infinite source. Fluid IDs are unique, the content ID must resolve to a fluid
in the selected pack, and every point or positive-sized box must fit inside the
world. Version 1 admits at most 4,096 fluid records. A CAXEMAP stores the
designed starting water; a player save stores later cell and flow-queue state.

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
English/Spanish content uses message IDs. `narrator` means that no world object
is speaking. A world object uses the separate `speaker <object-id>` form, so an
object whose ID is literally `narrator` remains unambiguous.

```text
dialogue <id>
  line narrator message <message-id>
  line speaker <speaker-object-id> literal "<user text>"
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
dialogues, and objectives must exist. The reader still accepts the early
`line <speaker-object-id> <text>` spelling and the canonical writer makes it
explicit as `line speaker <speaker-object-id> <text>`. Version 1 admits at most
1,024 dialogues, 128 lines per dialogue, 512 objectives, and 64 routes.

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

The action vocabulary has one typed catalog in
[`CaxeFlowActionRegistry.hx`](../examples/caxecraft/src/caxecraft/scenario/CaxeFlowActionRegistry.hx).
An **action descriptor** is a data record that answers four questions: which
CAXEMAP word names the action, which ordered fields it accepts, which broad
engine family it belongs to, and which authoring surfaces may offer it. For
example, `give-item` declares an object identity, then an item content ID, then
a quantity. The parser uses the stable action ID for admission, the writer uses
that same ID for canonical spelling, and the renderer-independent editor reads
the same ordered roles through
[`EditorActionPalette.hx`](../examples/caxecraft/src/caxecraft/editor/EditorActionPalette.hx).
This prevents three independent string tables from quietly disagreeing.

The descriptor is not executable code. `FlowAction` remains the closed typed
payload, `CaxeFlowValidator` still validates scenario and content references,
and `CaxeFlowExecutor` still exhaustively implements behavior. Content cannot
register a callback, native function, script, or service through this catalog.
All 18 descriptors are currently authorized only for CaxeFlow documents.
Cutscene, developer-console, and CaxeTest consumers are represented so future
work can request authority explicitly, but they remain denied because those
integration and validation paths are not implemented. Editor label/help values
are stable message IDs; translated visual-card copy remains part of the planned
visual editor rather than a shipped UI claim.

The current catalog, parser/writer parity, validator/executor guards, and editor
palette are executable under the pinned Eval oracle. haxe.c now admits the
nominal String IDs and managed Arrays carried by `FlowAction`, as well as the
closed `FlowSequence` records stored in
`ScenarioValidationContext.sequenceTable`. The StringMap implementation keeps
each record unboxed and supplies exact generated copy/assign/destroy callbacks
when its nested Arrays need ownership work; the focused compiler and runtime
evidence is owned by completed issue `haxe_c-djl.7`.

The same focused StringMap evidence now covers two more direct value families:
fieldless Haxe enums such as `FlowValueKind`, represented as native C enums,
and Haxe `Int`, represented as signed `int32_t`. Both stay unboxed and need no
value-lifecycle callbacks. Completed issues `haxe_c-djl.8` and `haxe_c-djl.9`
own those additions.

The complete Caxecraft compile now passes every StringMap boundary above and
the direct generic instance method `ScenarioRecordCursor.failAt<T>`. haxe.c
creates one deterministic closed method body for each concrete
`ScenarioReadResult<T>` used by the parser; completed issue `haxe_c-djl.10`
owns that general capability.

The compiler now passes allocation-free `String.charAt` through its dedicated
String representation and runtime slice; completed issue `haxe_c-djl.11` owns
that shared intrinsic-method classification and lowering. The next unrelated
boundary is `haxe.ds.IntMap.exists`: virtual-slot validation cannot yet settle
the generic standard-library collection operation to a concrete specialization
or intrinsic collection plan. Issue `haxe_c-c3s.1` owns that general IntMap
work. Until it lands, this section is not evidence that the complete CaxeFlow
registry or executor runs in generated C; unrelated DomainProbe output does
not fill that gap.

Arguments are `value flag <bool>`, `value counter <int>`, `value state
<content-id>`, or `variable <variable-id>`. CAXEMAP 1 does not admit an object
argument because its sequence parameters have only the three value kinds above.
Adding object parameters later requires real substitution and execution
semantics; accepting an object spelling before then would create a value that
can never match a parameter.

A `choose` action is followed by exactly its declared number of choice blocks:

```text
choice weight <positive-integer>
  do <action>
end choice
```

The choice uses the named counter variable as an explicit deterministic seed.
There is no wall-clock or implicit randomness. A choice may contain ordinary
actions but not another `choose`. The outer `choose` and every action in all of
its alternatives count toward the containing rule's 64-action limit. This
keeps validation and the visual editor simple while still supporting branching
scenarios for the first version.

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

`CaxeFlowExecutor` runs a previously validated `Scenario`. A **fixed tick** is
one numbered simulation step, independent of rendering speed. Tick numbers
begin at one and use two ordinary non-negative `Int` fields: an epoch plus an
offset inside a one-billion-tick epoch. Every carry is checked before addition,
so crossing the 32-bit integer boundary cannot wrap a timer or cooldown. The
final representable value is more than one billion years away at 60 ticks per
second; reaching it stops with `LimitExceeded(FixedTickEpochs, 2147483647)`
instead of corrupting time. Authored delays remain positive 32-bit integers and
are split into epoch and offset parts before addition. A delay of one means “at
the next fixed tick,” not “after some number of milliseconds.” No rule reads a
wall clock, and this representation needs no currently unsupported `haxe.Int64`
lowering in generated C.

One tick has these exact boundaries:

1. Copy the game-supplied object positions into scenario state.
2. Collect due signals, state/objective notifications, and timers in the order
   they were queued. Append the caller's new events in caller order.
3. Find matching rules. Lower numeric priority runs first; equal priorities use
   the rule ID's raw UTF-8 byte order. Within one rule, events keep their order.
4. Evaluate every matching predicate before running any admitted rule action.
   This is the **stable tick view**: all predicates see the state that existed
   at the start of rule planning. An earlier rule cannot change what a later
   rule observed.
5. Run due sequence actions in queue order, then admitted rules in the order
   planned above. Each action list keeps authored source order. Unlike
   predicates, a later action sees changes made by an earlier action.
6. Return typed presentation requests and, if work stopped, one typed runtime
   diagnostic. The portable executor does not call Raylib, play audio, or draw
   UI itself.

Due work is removed from the queue only after rule planning succeeds. Once the
action phase begins, however, that accepted due batch is consumed. If an action
hits a limit or fails, its completed changes remain, later due sequences are
skipped, and that batch is not automatically tried again on the next tick. The
diagnostic identifies the exact sequence or rule that stopped, so the game can
fail visibly instead of repeating a partially completed interaction.

For example, if rule A sets `quest.bridge` and rule B tests that value in the
same tick, B tests the old value. The change queues `StateChanged` for the next
tick; B can respond then. This makes authoring predictable and prevents a
signal from recursively running an unbounded stack of rules.

`once` may fire only once for the lifetime of the executor. `repeat` may fire
once per matching event. `cooldown N` may fire at most once in an eligible tick
and becomes eligible again when exactly N fixed ticks have elapsed. Multiple
matching events in one tick cannot bypass `once` or `cooldown`.

Sequence calls receive positional typed arguments. Parameters and variables
declared `local <sequence-id>` get a fresh call frame and disappear when that
call returns; they cannot be read by ordinary rules or become next-tick
`StateChanged` events. Map, player, and quest variables persist. A scheduled
call captures argument values when the `schedule` action runs, so a later
change to the source variable does not rewrite pending work.

`choose` is deterministic weighted choice, not hidden randomness. The named
counter is reduced modulo the sum of positive weights to select one alternative
in source order, then incremented with explicit 32-bit wraparound. Replaying the
same events and saved seed produces the same choice on every target and locale.

### Runtime work limits

The first executor keeps each tick bounded:

| Work in one tick | Maximum |
| --- | ---: |
| input and ready events | 256 |
| admitted rule executions | 2,048 |
| actions, including selected choice actions | 8,192 |
| sequence calls | 1,024 |
| sequence call depth | 16 |
| spawn actions | 512 |
| new schedules | 1,024 |
| predicate nodes evaluated | 8,192 |
| queued deferred entries across ticks | 4,096 |

A planning-limit failure happens before scenario actions run and leaves ready
deferred work queued for inspection or a later tick. An action-limit failure
keeps the already completed action prefix, stops the remaining sequences and
rules in that accepted batch, and returns the first exact `LimitExceeded`
diagnostic with its work kind, maximum, and owning rule or sequence where one
exists. It never hangs, recursively re-enters the action stack, or pretends the
unknown suffix ran. Save-game state must include persistent variables,
inventory, object/objective state, once/cooldown history, the current fixed
tick, explicit seeds, and deferred work; that persistence belongs to the
separate save-game owner rather than `.caxemap` authoring files.

The fast executable proof is `npm run test:caxecraft-caxeflow`. It executes all
10 events, 12 predicates, and 18 actions, including reverse rule registration,
sequence locals and captured arguments, fixed-tick delays, repeat policies,
the maximum positive delay and the 32-bit tick boundary, every runtime work
limit with its exact owner, the complete typed presentation payload sequence,
and C versus Spanish-locale trace equality. The same source is intentionally
target-neutral so a later generated-C integration uses the tested engine
instead of a C-only copy.

## Renderer-independent editing

`EditorSession` applies a closed `EditorCommand` enum to a draft without
importing Raylib or the C target API. Commands cover world resize, palette and
voxel edits, bounded selection/fill, fluid sources and initial volumes, prefab
stamps, placements, dialogue,
objectives, rules, and explicit last-playable recovery. The draft may be
temporarily invalid; a separate last-playable snapshot changes only when the
complete model passes `ScenarioValidator`.

Undo and redo store exact bounded in-memory CAXEMAP snapshots plus selection
state. Both stacks share deterministic entry and byte limits, and an oversized
entry is rejected before the draft changes. Test play validates and deep-copies
the current draft into a disposable `CaxeFlowExecutor`; leaving test play drops
all simulation changes. See [Caxecraft editor semantics](caxecraft-editor.md)
for the command families, bounds, performance tradeoff, and focused proof.

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

### Reader implementation map

The implementation keeps those stages visible in the source tree:

- `ScenarioLexer` checks bytes and turns each meaningful line into bounded
  tokens.
- Game and editor code passes the records from `ScenarioLexer.read(...)` to
  `ScenarioParser.parse(...)`. That method checks the document's syntax and
  coordinates the internal reader classes listed below.
- `ScenarioDocumentReader` owns the header, top-level fields, and final model
  assembly. It delegates chunks and objects to `ScenarioWorldReader`, story
  records to `ScenarioStoryReader`, and rules to `CaxeFlowReader`.
- `ScenarioTokenGrammar` owns context-free values such as IDs, integers,
  transforms, and localized text references. `CaxeFlowValueReader` does the
  same for events, predicates, arguments, and non-branching actions.
- `ScenarioValidator` is the only semantic entry point. Its short, visible
  pipeline preserves diagnostic order while delegating document features and
  extensions, world geometry and placements, story references, and CaxeFlow
  rules to collaborators named after those responsibilities.
- `ScenarioValidationContext` builds the shared identity indexes and owns
  source-coordinate lookup and diagnostic collection. This prevents the
  smaller validators from disagreeing about whether an ID exists or which
  source record should receive an error.
- `ScenarioWriter` produces the one canonical byte spelling for a structurally
  representable typed model. Saving or test-playing still requires the complete
  candidate to pass `ScenarioValidator`; the editor may use the same bytes only
  in memory to retain an incomplete draft for undo and repair.

The readers share a `ScenarioRecordCursor`. Here, a **cursor** is simply the
current line plus the source locations collected so far. A successful reader
advances it past one complete record or block; a failed reader returns a typed
diagnostic without pretending that a partial object is usable. This small
state object preserves exact error locations without hiding the grammar behind
a parser framework. Predicate parsing is the one recursive part, and its depth
is checked before descending.

Application and editor code should call `ScenarioLexer.read(...)`,
`ScenarioParser.parse(...)`, `ScenarioValidator.validate(...)`, and
`ScenarioWriter.write(...)`. The smaller reader and validator classes are
visible between Haxe source files so they can collaborate, but they are
implementation details. Their `@:noCompletion` marker keeps them out of normal
editor suggestions; it does not make them private at runtime or change Haxe's
type-safety rules.

The validator rejects malformed UTF-8, invalid escapes, overflow, duplicate or
missing singleton records, limit violations, invalid RLE totals, overlapping or
missing chunks, duplicate IDs/tags, unresolved references, type-mismatched
variables/arguments, impossible placements, invalid rule graphs, and unknown
required features.

`npm run test:caxecraft-scenario-model` is the fast edit-time proof for this
pipeline. `npm run test:caxecraft-scenario-determinism` is its separate
reference lane: it runs the complete codec probe twice from cold Haxe, once
under an alternate installed locale, and twice through one pinned compilation
server. Keeping the five-request proof separate makes compiler-cache and locale
drift visible without making every parser edit pay for repeated compiler setup.

The planned map-file persistence layer (`haxe_c-xge.19.4`) will write these
canonical bytes to a validated sibling temporary, flush as required by the
platform adapter, and then replace the destination atomically where the
admitted platform guarantees that operation. A failed create, write, flush,
replace, or cleanup has a distinct typed stage. The last known-good destination
remains live and no partial candidate is accepted.

That operation saves an authored map; it is not a player save. A `.caxemap`
describes the world and scenario as designed. The separate save-game work in
`haxe_c-4my` will record changes made while playing—such as modified blocks,
player inventory and position, objectives, object state, and scheduled rule
work—without rewriting the source map.

## Canonical example

The review fixture is
[`minimal.caxemap`](../examples/caxecraft/test/fixtures/caxemap/minimal.caxemap). It is small
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
