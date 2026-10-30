# Caxecraft Eval-assisted loop experiment

## Outcome

Caxecraft will not add a second runtime that sends every game frame from Eval to a native Raylib helper.

Creators already have a faster and simpler path. Reloadable content changes can reach one persistent native game process without a rebuild.

Engine changes still use the incremental haxe.c and native build. Renderer-free rules can use Eval for focused feedback before that authoritative build.

This decision keeps one game state owner. It also keeps Raylib resources, input, rendering, and cleanup in the existing native application.

## Question

This experiment asked whether Eval can run shared gameplay while a small native process handles Raylib presentation.

The proposed split had to improve edit-to-frame time. It also had to preserve debugging, cleanup, failure recovery, and the direct C release path.

## Small mental model

The supported feedback paths have separate jobs:

```text
content edit -> persistent native game -> JSON state and a review frame
shared rule edit -> Eval probe -> semantic result
engine edit -> incremental haxe.c build -> authoritative native game
```

An Eval-to-Raylib frame bridge adds another path:

```text
Eval game state -> frame messages -> native Raylib helper -> input and resource messages -> Eval
```

That path needs two lifecycle owners and a versioned message model for changing game state. No completed measurement supports this cost.

## Executable comparison

Run the portable comparison with:

```sh
npm run profile:caxecraft-eval-assisted-loop
```

The command runs five independent samples by default. Each sample measures these requests:

- a cold Eval request.
- the first Eval server request.
- a repeated unchanged Eval request.
- an edited shared-rule Eval request.
- the same four compile shapes for JavaScript.

The JavaScript result is a compile-to-artifact measurement. It is not a browser frame or a complete game port.

Add the native session only on a representative host:

```sh
python3 examples/caxecraft/profile_eval_assisted_loop.py \
  --samples 5 \
  --native-session \
  --representative-host
```

The native sample uses the existing `--agent-session` path. One haxe.c-built game stays open while bounded Piloscript batches request two frames.

Each response contains Haxe-owned JSON state and `caxecraft-agent-session.png`. The profiler records request-to-screenshot time and always stops its process group.

The report records the Haxe version, host load, sample distributions, and source-sharing metric. The metric counts Haxe lines in the narrow adapter probe.

The source-sharing percentage does not describe the complete game. The report states this limit beside the value.

## Comparison

| Option | Shared behavior | Frame evidence | Lifecycle and failure result | Decision |
| --- | --- | --- | --- | --- |
| Eval probe | The adapter probe uses shared Haxe rules and portable target carriers. | None. It prints one semantic result. | The Haxe process owns all probe state and exits after one request. | Keep for focused rule feedback. |
| JavaScript target | The target compiles the same narrow shared probe. | None. No browser presentation adapter exists. | The compiler emits one artifact. Runtime input and resource cleanup remain unproved. | Keep as compile-only portability evidence. |
| Native agent session | The production Haxe game runs through haxe.c. Reloadable content supplies actions. | Each accepted batch returns JSON state and one real review image. | The launcher owns one child process, bounded messages, end-of-file, timeouts, and cleanup. | Keep for content and creator feedback. |
| Eval native plugin | No Caxecraft game code uses this path. | None. | `eval.vm.Context.loadPlugin` loads an OCaml plugin tied to the exact Haxe and OCaml builds. | Reject as a Raylib development boundary. |
| Eval frame bridge | No implementation exists. | None. | It needs frame serialization, input return messages, compatibility, resource ownership, and two crash-recovery paths. | Reject until measured evidence changes this result. |

## Why the native plugin is not a Raylib adapter

The pinned Haxe `eval.vm.Context.loadPlugin` API loads `.cmxs` or `.cmo` modules. These modules use the Eval virtual machine and OCaml value API.

They must match the exact Haxe and OCaml versions. Raylib exposes a C application binary interface instead.

A plugin therefore needs a new OCaml binding and a second resource-lifecycle policy. It does not reuse the typed `c.*` Raylib boundary.

## Why content reload is the preferred creator loop

Maps, dialogue, objectives, encounters, CaxeFlow rules, and Piloscript actions already belong to reloadable data.

The native game can validate these bytes while it stays open. A creator receives state and a screenshot without recompiling the engine.

This path also preserves one source of game truth. Eval does not simulate a second copy of the world beside the visible native world.

## Limits and required evidence

Loaded-host samples are diagnostics only. The profiler marks latency evidence as representative only after an explicit operator declaration and a bounded load check.

The current loaded-host attempts reached the unchanged 120-second Haxe-to-C timeout before the first native session. They produced no native latency claim.

A clean five-sample native report remains required before this research task can close. The report must use a current validated build receipt.

Final compiler, generated-C, native, sanitizer, packaging, and release evidence still comes from the complete haxe.c build.
