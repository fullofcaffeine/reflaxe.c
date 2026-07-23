# Caxecraft deterministic game pilot

Status: named input scripts, real native Raylib execution, framebuffer capture,
structural image checks, a versioned semantic report, repeated-run comparison,
and bounded automatic exit are implemented under `haxe_c-xge.31`. Linux CI
runs the title proof on a virtual desktop and the movement/edit proof through
Raylib's pinned in-memory software renderer. Both preserve the report plus a
review image. A live command connection remains later work.

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

That exact one-tick-per-frame clock is not evidence that display-rate motion is
smooth. Interactive builds now sample a presentation-only position between the
previous and current committed bodies. The focused Eval/native presentation
test owns its arithmetic and reset rules; a real interactive movement pass (or
a future pilot with an explicitly authored variable render cadence) owns feel
at representative refresh rates. Collision, raycasts, edits, and telemetry
remain tied to the committed body in both cases.

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

# Run that same game pilot without a desktop or GPU. This builds the pinned
# PLATFORM=Memory/software Raylib configuration when it is not cached.
npm run caxecraft:play -- \
  --raylib-configuration memory-software \
  --pilot move-jump-edit

# Linux-only generated-C safety profile used by the required GCC/Clang jobs.
npm run caxecraft:play -- \
  --raylib-configuration memory-software \
  --pilot move-jump-edit \
  --sanitizers

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

# Walk the authored Adventure level, collect Tideweave through the ordinary
# world-item path, equip its aquatic profile, show the HUD result, and quit.
npm run caxecraft:play -- --pilot aquatic-gear

# Move and jump through the ordinary game loop at deterministic 8/17/25 ms
# display intervals and prove the camera rendered between simulation states.
npm run caxecraft:play -- --pilot smooth-motion
```

The ten closed script names are `LaunchSmoke`, `MoveJumpEdit`,
`PauseRecapture`, `CombatDrop`, `RecoveryUse`, `FullInventoryGift`, and
`FullInventoryMining`, plus `ResizeLayout`, `AquaticGear`, and `SmoothMotion`.
Each has a small fixed frame limit below the absolute 120-frame policy. Its
final and every later action is `Quit`, which protects against a script
accidentally becoming an unattended interactive session. The Python runner
adds an independent 15-second wall-clock timeout.

The launch image check requires the staged panorama and wordmark by semantic
color/region evidence. Gameplay checks require independent terrain, heads-up
display, light UI, and enough color variety. Pilots whose camera remains near
the horizon also require open sky and the sun. `MoveJumpEdit` deliberately aims
at a reachable block, so its stronger evidence is a visible selected terrain
view plus telemetry proving one removal, one placement, no rejection, and a
final ray hit; it is not rejected merely because the player looked away from
the sun. The stationary full-inventory interaction also
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

The aquatic-gear pilot starts in Adventure mode and follows deterministic
movement intent toward the Tideweave placement authored by the level data. The
ordinary fixed tick, collision, generic world-item range check, content-pack
item profile, aquatic-profile adoption, item removal, renderer, and HUD paths
perform the result. Protocol version 5 records the `aquaticGearEquipped` flag
and appends exposed-face, current-rebuild, cumulative-rebuild, and cache-valid
counters. A gameplay frame must report a valid initial sixteen-chunk build, and
the edit pilot must report later rebuild work while its final steady frame
reports no unnecessary dirty rebuild. These are semantic renderer facts, not
timing claims. The host also rejects a completed journey when the equipment
flag is absent. This is a narrow
integration proof for pickup and equipment. It does not replace the Eval/native
aquatics tests that own buoyancy, breath, drowning, and profile rules, nor does
it yet claim that a deep underwater Adventure route is authored.

The smooth-motion pilot walks and jumps while keeping gameplay at exact 50 ms
ticks and feeding the
real outer game loop a deterministic sequence of 8, 17, and 25 ms render
intervals. The application sets a pilot-only observation bit only after its
actual camera sample lies strictly between two different committed positions.
The native report must also finish above the authored spawn with positive
vertical speed and `grounded=false`; this proves the jump reached gameplay,
not merely that the script requested one.
The host requires that bit and also requires fewer fixed ticks than rendered
frames. This proves that variable display cadence reaches the shipped
interpolation path without feeding visual coordinates back into gameplay. A
human playtest still owns the subjective question of whether movement feels
pleasant.

## How semantic state leaves the native game

A screenshot can prove that the game drew a scene, but it cannot reliably tell
us an exact player coordinate, world hash, or rejected edit count. Pilot builds
therefore draw a tiny machine-readable strip along the bottom edge of a second
frame capture. Each colored mark represents one hexadecimal digit. Forty
fixed-width words carry a magic number, protocol version, script identity and
input hash, completed frames and fixed ticks, player motion, world selection
and edit counts, render counters, inventory/actor state, presentation flags,
and four benchmark-only timing counters. Version 5 also records exposed terrain
faces, dirty-chunk rebuilds, cumulative rebuilds, and whether the fixed cache
capacity remained valid. Ordinary pilots require every timing counter to be
zero, proving that their executable does not accidentally retain benchmark
instrumentation.

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
`caxecraft-pilot-report.json`. An ordinary pilot runs the same executable twice and compares
the complete normalized report, including native compiler and pinned Raylib
identity. Checkout paths and temporary directories are deliberately absent;
the report names the review screenshot only by its packaged filename. The
whole screenshot remains a broad visual/scene check rather than a
cross-driver pixel golden.

`--benchmark-renderer` is a separate opt-in form of the `move-jump-edit`
pilot. It discards the first two frames as warmup, measures the next twelve
terrain submissions with Raylib's monotonic clock, and repeats the bounded
native run seven times. Semantic reports and screenshots must still match;
only the timing samples may vary. The report retains every sample plus its
median and per-frame median. The paired `--renderer immediate-baseline` mode
compiles the former full-world face-discovery loop while sharing the same
world, atlas, submission, pilot, compiler, and Raylib path.

The pinned Raylib 6.0 Memory backend originally returned a constant `GetTime()`
on macOS because its platform implementation omitted Apple. Provisioning now
applies a narrowly scoped, content-addressed patch to a private build copy and
runs a positive monotonic-clock probe; the verified upstream cache remains
untouched. Linux remains the required CI comparison lane, while macOS can run
the same headless benchmark locally. See
[Raylib 6.0 provisioning](raylib-provisioning.md#reviewed-macos-memory-clock-patch)
for the patch and provenance contract.

On Linux or macOS, `npm run benchmark:caxecraft-renderer` runs both admitted
Haxe renderer variants, requires the same gameplay report and byte-identical
final image, then runs a focused handwritten-C reference. It writes one
generated comparison report under
`examples/caxecraft/_build/benchmarks/`. The report records the scene and input
hashes, viewport, pinned Raylib and native compiler, warmup and sampling method,
terrain/update/preparation times, draw and rebuild counts, explicit cache
payload, generated-source size, binary size, and zero generated allocator,
`goto`, and `hxrt` references.

The three comparison points answer different questions:

1. The retained Haxe immediate renderer shows the old algorithm after haxe.c
   lowering.
2. The Haxe chunk cache shows the new algorithm after the same lowering.
3. [`renderer_immediate.c`](../examples/caxecraft/benchmarks/renderer_immediate.c)
   is an independent native reference for the narrow terrain operation.

The third algorithm could also be written in Haxe, and haxe.c could compile it.
That would make it another useful generated-Haxe comparison, but it would no
longer be an independent C reference: both sides would share haxe.c's lowering
choices and any lowering defect. The direct C is therefore an intentional,
narrow benchmark/test exception, never game implementation or an escape route
around a compiler limitation. The runner generates only immutable cell bytes
from the same CAXEMAP named by the Haxe adapter, then the reviewed C owns
discovery, tinting, two atlas submissions, edit replay, timing, and cleanup. It
uses the same C compiler, `-O2`, Raylib library, clock, warmup, sample count, and
final face count as the Haxe paths.

The C reference excludes simulation, water, actors, HUD, and telemetry because
those are outside the measured terrain operation. Its binary size is recorded
but is not compared as though it were a second complete game. Results remain
measurement evidence—not a general “faster than C” or zero-cost claim—and the
required Linux GCC/Clang lanes remain the publication authority.

Raylib 6.0's pinned software readback has one backend-specific byte convention.
Its software `swReadPixels` path already returns top-down BGRA bytes, while the
shared screenshot helper assumes bottom-up RGBA and flips them again. The host
runner converts only those memory/software captures back to ordinary top-down
RGBA before visual checks, telemetry decoding, and artifact upload. A focused
four-color fixture locks the direction and channel conversion. Desktop images
are never converted, and no gameplay or expected colors change. Linux CI keeps
the resulting renderer-scoped image and semantic report as review evidence;
this is not yet a checked-in exact-pixel golden.

The memory platform has one other explicit boundary: pinned Raylib 6.0 does
not implement `SetWindowSize` there. The runner therefore rejects the
`resize-layout` pilot before compilation instead of spending a full build and
then accepting the unchanged framebuffer. Live resize remains a desktop/Xvfb
check. Other deterministic pilots continue to use the display-free software
backend.

The Linux headless job has isolated GCC and Clang variants. Both compile every
generated C file and the final link with AddressSanitizer, which detects invalid
memory access, and UndefinedBehaviorSanitizer, which detects invalid C
operations such as bad shifts. Recovery is disabled, frame pointers stay
available for useful stack traces, and leak detection is enabled. The report's
`native.sanitizers` array makes that profile reviewable. The checksum-verified
Raylib static library is executed under the sanitizer runtime but is not itself
rebuilt with instrumentation in this slice; the claim is deliberately limited
to instrumented generated C linked to and running through real Raylib.

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

A macOS desktop executable also needs access to the logged-in graphical
session. A normal Terminal window has that access. A background automation
shell may not: GLFW then reports that it could not initialize the platform
before any gameplay frame exists. Treat that message as a launcher/environment
failure, not as a blank-frame game result. Use `--build-only` for a headless
local compile/link check, run the pilot from a normal Terminal for local visual
evidence, or use the Linux Xvfb lane for unattended graphical evidence.
