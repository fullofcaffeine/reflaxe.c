# Caxecraft deterministic game pilot

Status: named input scripts, real native Raylib execution, framebuffer capture,
structural image checks, and bounded automatic exit are implemented under
`haxe_c-xge.31`. Exporting complete semantic state and running the hosted
graphical lane in CI remain open.

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
                              presented screenshot + clean exit
```

`GameInputFrame` describes intent rather than hardware buttons: move, look,
jump, use the selected item's primary or secondary action, interact, pause,
recapture, quit, and hotbar selection. A secondary action places a block or
uses a selected consumable. The real
Raylib adapter samples keyboard and mouse once per rendered frame. A pilot
selects one closed `PilotAction` for that frame. After that small provider
choice, both paths use the same gameplay and presentation code.

This is **Playwright-like** in one limited sense. Playwright drives a program
through named actions and inspects deliberate checkpoints. Caxecraft does the
same for a native game: it compiles a named action script into a test build,
runs the real generated C executable, captures a frame only after
`EndDrawing`, validates visible scene roles, and exits by itself. It is not a
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

# Select the Copper Sword, defeat the Mossling, prove its visible drop, and quit.
npm run caxecraft:play -- --pilot combat-drop

# Start one heart below full, consume berries, prove recovery feedback, and quit.
npm run caxecraft:play -- --pilot recovery-use

# Fill the berry stack, ask Nia for her gift, prove it remains retryable, and quit.
npm run caxecraft:play -- --pilot full-inventory-gift
```

The six closed script names are `LaunchSmoke`, `MoveJumpEdit`,
`PauseRecapture`, `CombatDrop`, `RecoveryUse`, and `FullInventoryGift`. Each has a small fixed frame
limit below the absolute 120-frame policy. Its final and every later action is `Quit`, which
protects against a script accidentally becoming an unattended interactive
session. The Python runner adds an independent 15-second wall-clock timeout.

The launch image check requires the staged panorama and wordmark by semantic
color/region evidence. Gameplay checks require independent sky, terrain,
heads-up display, light UI, Nia-coat, and Mossling-crown pixels. These actor
colors are deliberate semantic evidence that the native renderer submitted
both original silhouettes, not a whole-image golden. The checks do not compare whole GPU images
byte-for-byte because drivers and platforms may rasterize the same scene
differently. Exact pixel goldens belong to the separately pinned software
renderer.

The combat pilot intentionally has a different actor check. It requires Nia
and the exact berry-drop color after three sword actions, while requiring the
Mossling crown would prove that the defeat failed. This makes the screenshot a
small state assertion rather than merely another nonblack frame.

The recovery pilot begins at two of three hearts through a typed fixture fact,
selects berries, performs the same secondary action as a real right click, and
requires the unique successful-recovery color in the presented framebuffer.
Because a full-health or empty-stack decision cannot produce that feedback,
the screenshot proves the shared recovery transition reached the renderer.

The full-inventory pilot begins with exactly 64 berries, advances Nia to the
gift step, and tries the interaction again. Its frame must show both the unique
capacity-warning color and Nia still offering the gift. The renderer-free test
separately proves that partial world pickup leaves the remainder active.

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

## Current evidence and remaining work

The target-neutral probe runs under pinned Haxe Eval in two installed locales.
It proves exact repeated samples, every action/checkpoint, hotbar intent,
permanent bounded quit, and absence of C/Raylib/untyped leakage.

The native commands compile through production `--custom-target c`, require an
empty `hxrt` plan, build and launch pinned Raylib, stage only hash-verified
assets, inspect a real RGBA framebuffer, and require a clean process exit. The
generated-C gate also rejects `goto`, allocation calls, and hidden `hxrt`, and
checks exact texture load/validity/unload ownership counts.

The pilot is useful for visual debugging now, but `haxe_c-xge.31` remains open
until native results also export a versioned semantic report containing player
and world state, selected cell, edit outcomes, fixed ticks, render counters,
termination reason, input hash, native compiler/Raylib identity, and normalized
evidence paths. Repeated native runs must then prove identical semantic reports,
and hosted CI must run the graphical executable under its accepted virtual or
software display.
