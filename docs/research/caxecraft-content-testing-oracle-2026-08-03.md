# Caxecraft content-testing Oracle disposition

## Outcome

Caxecraft content can now be changed and checked without turning every map,
campaign, pack, or UI edit into an engine test run. The retained rule is:

1. reusable mechanics and trust boundaries belong to focused Haxe tests, with
   generated strict C and native execution wherever that boundary is claimed;
2. every edited package must pass the production parser, validator, resolver,
   and generation builder using the exact bytes the game will read;
3. one relevant Piloscript journey protects the player-visible outcome;
4. broad composition, prose, pacing, visual appeal, and play feel remain human
   review; and
5. when a journey exposes a reusable engine or compiler defect, retain the
   journey and add a focused regression at the smallest stable owner.

This is the “double lock”: the focused test explains the reusable rule, while
the real journey proves that the complete product path still works. A misplaced
prop remains a content fix. A collision, parser, publication, security, save, or
compiler defect does not become “just content” because a Piloscript found it.

## Why this review was requested

Western Falls initially accumulated Haxe assertions for exact coordinates,
object counts, authored dialogue/objective counts, and fixed file hashes. Those
checks made a valid content expansion look like an engine regression. At the
same time, a prior graphical journey had missed the real title-to-Adventure UI
path, and the destination currently publishes new world data while retaining
Evergrove's objective text. Removing all strict checks would therefore have
been equally unsafe.

The local baseline before consultation proposed a split between focused
foundation tests and Piloscript content journeys. Oracle request
orq_20260803T013204Z_f116b611 reviewed the actual campaign probe, launcher,
scorecards, package boundary, Western Falls evidence, and open Beads. The
requested model was GPT-5.6 Pro and the captured browser surface reported Pro.

## Recommendation disposition

| Oracle recommendation | Disposition | Local result or owner |
|---|---|---|
| Validate the exact current runtime bytes through production owners before a journey | Retained | caxecraft:content:validate reuses a qualified native launch-smoke executable, restages current bytes, and runs the shortest existing pilot. A miss fails instead of building. |
| Add an explicit no-compiler/no-linker creator path | Retained | caxecraft:content supports free play or one selected pilot. A focused governance test makes a missing variant fail before compile_haxe or compile_native. |
| Use one representative journey, not every journey, for an ordinary content edit | Retained | Content feedback runs one sample. Formal system commands retain two identical runs and remain the determinism/backstop evidence. |
| Keep real package loading, receipt rejection, atomic publication, Eval/native agreement, and sanitizers | Retained | CampaignRuntimeProbe.hx keeps these foundation claims. |
| Remove decorative campaign literals from the strict tracer | Retained | The probe now derives the selected entry, sole required transition, destination path, byte length, and hash from the current manifest. It no longer requires exact coordinates, actor/item/dialogue/objective counts, or empty CaxeFlow collections. |
| Keep closed campaign-schema negatives independently authored | Retained | Schema mutations now start from a small manual two-level fixture rather than shipped campaign decoration. |
| Pair the visible waterfall with renderer evidence and allow horizontal relocation | Retained with a local adjustment | The Adventure pilot requires the third water-render batch and substantial water-colored rows across the whole gameplay viewport. A connected-pixel requirement was tested and rejected because normal texture variation fragmented the real falls despite correct rendering. |
| Add semantic checkpoint IDs for active campaign, level, objective, and transition outcome | Deferred | haxe_c-xge.20.2.2 owns the currently red destination HUD objective and focused-plus-journey proof. |
| Runtime-select pilots from one executable and remove duplicated host metadata | Deferred | haxe_c-xge.20.6 already owns Haxe-authoritative pilot metadata. It should include the runtime-selection question rather than expanding this review into another compiler/application refactor. |
| Distinguish logical “continue” from physical traversal through an authored exit | Retained as a claim limit | Current evidence proves a typed logical transition only. Physical exits, branching, return travel, and save rules remain under haxe_c-xge.20.2. |
| Treat save compatibility and untrusted mods as strict contracts | Retained as carve-outs | Campaign/save compatibility remains under haxe_c-xge.20.2 and haxe_c-4my; package/ZIP confinement and receipt owners remain strict. They are not governed by the malleable-decoration rule. |
| Add broad new CI lanes, Gherkin, content assertion quotas, or Python product semantics | Rejected | Existing R0–R5 ownership remains. No new Python file, Gherkin layer, or CI topology was added. Python only verifies build state, runs processes, and inspects portable native artifacts. |

## Independent oracles and red evidence

The exact package and campaign receipts are authored in manifests and checked
against independently read bytes. The minimal malformed campaign document is
manually authored. Generated-C execution is compared with pinned Haxe Eval, and
the native C harness independently observes only the exported scalar ABI.
Piloscript actions and semantic checkpoints remain Haxe-owned; the host reads
native telemetry and framebuffers but does not implement game behavior.

The representative pre-change command was:

~~~
python3 examples/caxecraft/play.py --content-feedback --pilot adventure-journey --raylib-configuration memory-software
~~~

It failed because --content-feedback did not exist. During implementation, a
first hidden validation-switch design failed at Sys.getEnv with HXC1001; that
test-shaped compiler workaround was removed. The content validator instead
reuses the existing shortest Haxe-owned pilot. A first connected-water visual
rule then failed despite a real water batch because texture colors formed only
small connected components; the retained whole-viewport row rule protects the
visible landmark without dictating texture continuity or horizontal placement.

## Verified feedback and remaining limits

On the observed macOS host:

- final current-byte validation: 8.59 seconds, no build;
- final one-run title-to-Adventure feedback: 17.01 seconds, no build;
- corresponding ordinary Adventure requalification: 69.80 seconds, generated-C
  planning plus cached native objects/link and two determinism runs; and
- campaign foundation owner: Eval, generated strict C, native execution, and
  sanitizers all passed.

The content path is intentionally not hot reload inside one process. A creator
relaunches the existing executable. A changed engine, compiler input, launcher,
native library, or executable requires one explicit normal qualification build.
The current title-to-Adventure journey still proves a logical transition rather
than physical exit traversal, and it must not claim the correct destination
objective until haxe_c-xge.20.2.2 closes.
