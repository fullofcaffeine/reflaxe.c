# Caxecraft deterministic game pilot

Status: named input scripts, real native Raylib execution, framebuffer capture,
structural image checks, a versioned semantic report, repeated-run comparison,
and bounded automatic exit are implemented under `haxe_c-xge.31`. The Linux
desktop lane is wired to run this proof under a virtual display and preserve
the report plus review image. A live command connection remains later work.

## What the pilot does

The pilot is a small supported way for a test—or an engineering agent—to play
a known part of Caxecraft without asking a person to press keys at the right
time:

```text
ordinary build: Raylib devices -> immutable GameInputFrame -> named intent
pilot build:    fixed script -----------------------------> named intent
                                                               |
                          movement / edit / UI / render <-------+
                                                               |
                    semantic report + review screenshot + clean exit
```

`GameInputFrame` describes intent rather than hardware buttons: move, look,
jump, use the selected item's primary or secondary action, interact, pause,
recapture, quit, and hotbar selection. A secondary action places a block or
uses a selected consumable. The real
Raylib adapter samples keyboard and mouse once per rendered frame. A pilot
selects one closed `PilotAction` for that frame. After that small provider
choice, both paths use the same gameplay and presentation code.

Pilot builds also provide exactly one 50 ms game step per rendered frame.
Interactive builds use Raylib's measured elapsed time and may run zero or
several fixed steps to catch up. The exact pilot clock makes enemy warnings,
damage, weapon cooldowns, and screenshots repeatable even when the host or GPU
is busy; it changes only the time source, not the gameplay transition code.

This is **Playwright-like** in one limited sense. Playwright drives a program
through named actions and inspects deliberate checkpoints. Caxecraft does the
same for a native game: it compiles a named action script into a test build,
runs the real generated C executable, flushes and captures the final frame,
presents it through `EndDrawing`, validates visible scene roles, and exits by
itself. It is not a
general macro recorder or a replacement for manual checks of mouse feel,
focus, resizing, controller ergonomics, animation, or artistic quality.

## Available scripts

Run these from the repository root:

```sh
# Text-only, sub-second contract for every exact action/checkpoint.
npm run test:caxecraft-pilot

# Title art, first presented frames, and bounded quit.
npm run caxecraft:play -- --smoke

# Movement, look, jump, mine, place, hotbar cycle, Nia interaction, scene
# capture, and quit.
npm run caxecraft:play -- --pilot move-jump-edit

# Pause, recapture without click-through mining, scene capture, and quit.
npm run caxecraft:play -- --pilot pause-recapture

# Survive one warned attack, pace three sword hits, prove the drop, and quit.
npm run caxecraft:play -- --pilot combat-drop

# Start one heart below full, consume berries, prove recovery feedback, and quit.
npm run caxecraft:play -- --pilot recovery-use

# Fill the berry stack, ask Nia for her gift, prove it remains retryable, and quit.
npm run caxecraft:play -- --pilot full-inventory-gift

# Fill all block stacks, try to mine, prove the block is not discarded, and quit.
npm run caxecraft:play -- --pilot full-inventory-mining

# Resize the real native window to 960x540, prove the title reflows, and quit.
npm run caxecraft:play -- --pilot resize-layout
```

The eight closed script names are `LaunchSmoke`, `MoveJumpEdit`,
`PauseRecapture`, `CombatDrop`, `RecoveryUse`, `FullInventoryGift`, and
`FullInventoryMining`, plus `ResizeLayout`. Each has a small fixed frame limit
below the absolute 120-frame policy. Its final and every later action is
`Quit`, which protects against a script accidentally becoming an unattended
interactive session. The Python runner adds an independent 15-second
wall-clock timeout.

The launch image check requires the staged panorama and wordmark by semantic
color/region evidence. Gameplay checks require independent sky, terrain,
heads-up display, and light UI. The stationary full-inventory interaction also
requires small color fingerprints from Nia's and the Mossling's reviewed atlas
cells. That is semantic evidence that the native billboard path presented both
original actors, not a whole-image golden. The checks do not compare whole GPU
images byte-for-byte because drivers and platforms may rasterize the same
scene differently. Exact pixel goldens belong to the separately pinned
software renderer.

The combat pilot intentionally has a different actor check. Its 40 fixed
frames let the Mossling warn and land one bounded attack, then space three
sword actions across the four-tick cooldown. The captured frame requires the
exact damage-feedback and berry-drop colors, while the defeated Mossling must
be absent. This makes the screenshot a small state assertion rather than
merely another nonblack frame.

The recovery pilot begins at two of three hearts through a typed fixture fact,
selects berries, performs the same secondary action as a real right click, and
requires the unique successful-recovery color in the presented framebuffer.
Because a full-health or empty-stack decision cannot produce that feedback,
the screenshot proves the shared recovery transition reached the renderer.

The full-inventory pilot begins with exactly 64 berries, advances Nia to the
gift step, and tries the interaction again. Its frame must show both the unique
capacity-warning color and Nia still offering the gift. It is also the stable
native frame that proves both original actor sprites. The renderer-free test
separately proves that partial world pickup leaves the remainder active.

The full-inventory mining pilot is deliberately separate. It begins with all
three collectable block stacks at 64, follows the same approach and mining
actions as the successful edit pilot, and requires the capacity warning in a
presented native frame. The shared `Mining.attempt` operation checks capacity
before it changes the world. The renderer-free test proves that free capacity
moves exactly one block, while a full matching stack leaves both the block and
inventory unchanged. Creative mode still removes blocks directly because its
building inventory is intentionally unlimited.

The resize pilot makes one target-neutral logical-size request. The C/raylib
application adapter performs the actual window call, waits for later presented
frames, and captures the title at exactly 960x540 (or its admitted 2x Retina
framebuffer). The same responsive title layout and asset ownership code runs at
both sizes; the scripted request and screenshot path are absent from release
builds.

## How semantic state leaves the native game

A screenshot can prove that the game drew a scene, but it cannot reliably tell
us an exact player coordinate, world hash, or rejected edit count. Pilot builds
therefore draw a tiny machine-readable strip along the bottom edge of a second
frame capture. Each colored mark represents one hexadecimal digit. Thirty-two
fixed-width words carry a magic number, protocol version, script identity and
input hash, completed frames and fixed ticks, player motion, world selection
and edit counts, render counters, inventory/actor state, and presentation flags.

This strip is test instrumentation, not game content or a public save format.
It is used because it exercises the same real Raylib framebuffer without adding
a JSON writer, filesystem service, socket, or general runtime dependency to the
generated game. The host runner accepts only the exact opaque colors, word
count, version, logical framebuffer scale, and closed field ranges. Unknown
colors, changed magic, unsupported versions, impossible counts, and unknown
flags fail before a report is written.

The edit and render counters that feed the strip are also inside the
`caxecraft_pilot` compile-time boundary. An interactive/release build therefore
does not merely hide the strip: it contains no pilot counter storage,
counter-increment branches, explicit batch flush, state screenshot, or report
transport. The ordinary playable snapshot is the fail-closed check for that
separation.

Raylib normally submits its queued draw work and swaps front/back buffers in
`EndDrawing`. A screenshot taken after that swap can read the previous back
buffer on some desktop systems. The pilot therefore calls the reviewed rlgl
batch flush only at explicit review/state capture points, captures the
now-complete back buffer, and then calls `EndDrawing` to present that same
frame. Ordinary builds contain no extra flush or state strip; frequent manual
flushes would split useful GPU batches and hurt rendering performance.

The runner converts the validated strip to
`caxecraft-pilot-report.json`. It runs the same executable twice and compares
the complete normalized report, including native compiler and pinned Raylib
identity. Checkout paths and temporary directories are deliberately absent;
the report names the review screenshot only by its packaged filename. The
whole screenshot remains a broad visual/scene check rather than a
cross-driver pixel golden.

### Why this carrier is temporary

The colored strip is acceptable for one small final observation because all of
its behavior is written in Haxe, compiled through haxe.c, removed from release
builds, and carried by Raylib APIs the game already proves. It adds no hidden C
helper, file path, port, socket lifecycle, JSON runtime, or second gameplay
implementation. It also forces the native graphical test to prove that a
complete frame really reached the framebuffer.

It would be a poor general control protocol. It has little bandwidth, spends
framebuffer pixels, depends on capture timing and display scale, and cannot
carry interactive requests back into the game. Adding more colored words would
make the test harder to understand without improving the compiler. The strip
therefore freezes at this bounded version and must not become the console,
CaxeTest language, save format, or live agent bridge.

| Carrier | Useful property | Cost or risk | Decision |
| --- | --- | --- | --- |
| framebuffer strip | works through the already admitted Haxe/Raylib path and proves a rendered frame | low bandwidth and tied to graphics | keep only for the current one-shot bootstrap |
| Haxe standard output | simplest final structured report; the launcher already owns the child process | needs general `haxe.io.Output`, strings/bytes, and framing | next replacement for final telemetry |
| Haxe standard input/output pipes | private two-way channel with no port, firewall, discovery, or remote bind | needs framing, limits, backpressure, and clean end-of-file behavior | preferred live agent transport when the runner launches the game |
| Haxe file output | easy to inspect after a crash | introduces paths, stale files, permissions, and atomic-write policy | use for ordinary evidence/save artifacts after filesystem support, not live control |
| loopback socket | permits attaching to an existing process or multiple tools | port allocation, authentication, firewall, timeout, and platform socket policy | add only if a real attach/multi-client need justifies it |

The preferred live path is therefore a versioned, length-bounded protocol over
the child process's inherited standard input and output. The encoder, decoder,
command dispatch, and observations stay Haxe. A host runner may remain Python
because it launches compilers and processes, but it cannot own or duplicate game
semantics. `haxe_c-8al` owns standard Haxe streams, `haxe_c-0bx` owns general
JSON support if JSON Lines remains the selected message spelling, and
`haxe_c-xge.19.8.1` owns the disabled-by-default Caxecraft agent bridge. A
Caxecraft-specific C function or application-owned `extern` shim is explicitly
not an alternative.

## Why the native path has one compile-time condition

Interactive releases must contain no scripted-input channel. The application
therefore makes one compile-time provider choice:

- an ordinary build samples `RaylibGameInput` into `GameInputFrame`;
- a build with the internal `caxecraft_pilot` define asks `PilotScript` for
  the current closed action and any explicitly declared initial fixture fact.

Haxe removes the inactive branch before haxe.c sees the program. This is an
appropriate `#if` boundary: it selects a test adapter with different external
inputs; it does not duplicate movement, world, inventory, or rendering rules,
and there is no per-frame target test in generated C. The damaged recovery
fixture is selected at this same boundary and is removed from ordinary builds;
it is not a hidden user command or duplicated health rule. The provider
immediately projects to common scalar intent so the native loop adds neither an allocated
input object nor interface dispatch.

The broader conditional-light adapter experiment is tracked by
`haxe_c-xge.34`. A future source-set or composition-root design may localize
this choice further, but it must retain the important property that ordinary
builds contain no pilot behavior.

## Why named scripts come before live IPC

Inter-process communication (IPC) means a runner sends commands while the game
is running and receives observations back. That will be useful when an agent
needs to decide its next action from the latest state. It is deliberately not
the first authority:

- the action vocabulary and ordering are closed and typed;
- no JSON parser, socket, port, queue, or timing race can obscure an input bug;
- the complete input is known before launch and is easy to reproduce; and
- CI can establish deterministic semantics before transport is introduced.

A later live adapter should translate validated versioned messages into this
same intent vocabulary. It must not become a second game implementation.

The pilot is not the cutscene system either. A cutscene is shipped CaxeFlow
content that sequences authorized world/presentation actions and defines its
skip result. A pilot supplies player input such as start, advance, or skip and
then observes the real cutscene result. They may share the fixed clock and
engine driver, but never the same script authority or story source.

## Current evidence and remaining work

The target-neutral probe runs under pinned Haxe Eval in two installed locales.
It proves exact repeated samples, every action/checkpoint, hotbar intent,
permanent bounded quit, and absence of C/Raylib/untyped leakage.

The native commands compile through production `--custom-target c`, require an
empty `hxrt` plan, build and launch pinned Raylib, stage only hash-verified
assets, inspect a real RGBA framebuffer, decode the versioned state record, run
the same executable twice, and require equal reports plus clean process exits.
The generated-C gate also rejects `goto`, allocation calls, and hidden `hxrt`,
and checks exact texture load/validity/unload ownership counts.

The Linux desktop workflow runs the launch pilot through Xvfb, a small virtual
display server used when CI has no physical monitor. It uploads the normalized
JSON report and human-review screenshot as one fail-closed artifact. This lane
proves the compiled application can open, draw, report state, and quit on the
hosted Linux image; it does not replace the separate macOS/Windows compile/link
matrix or manual input-quality review.
