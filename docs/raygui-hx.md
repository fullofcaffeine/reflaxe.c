# RayguiHx: typed immediate-mode tools for Caxecraft

## What exists now

RayguiHx is the Haxe-facing boundary for the pinned raygui 5.0 header. It is
being built first for Caxecraft's visual editor, menus, settings, pause screen,
inventory panels, and loading screens. The first checked slice is deliberately
small: panels, labels, buttons, a Boolean toggle, a selectable scrolling list,
status bars, shared state, and shared style.

This is not a claim that all of raygui is supported. Text boxes, extended lists
whose items arrive as a `char **`, sliders, file-loaded styles, and other
pointer- or resource-owning controls are still omitted. Each will be added only
with its element type, buffer size, lifetime, ownership, failure, and cleanup
rules made explicit in Haxe.

## Why there are two Haxe layers

`raygui.raw.Raygui` mirrors the selected C declarations exactly. It uses C ABI
types such as `c.Int32`, `c.Float32`, `c.CString`, and raylib's by-value
`Rectangle`. This layer is useful when checking the foreign interface, but it
is intentionally not the normal application API.

`raygui.Raygui` is the small semantic facade used by ordinary Haxe code. It
uses named result, state, control, and style types and performs explicit number
conversion at the boundary. Its methods are inline, so the generated program
contains the direct raygui C calls without wrapper allocation or an `hxrt`
runtime dependency.

The facade is a static namespace on purpose. Raygui is an *immediate-mode GUI*:
the program describes each visible control again every frame and receives what
happened during that frame. A persistent Haxe `Button` object would suggest an
owned widget and lifetime that raygui does not actually have. Higher-level
Caxecraft screens may still own durable editor state; they simply render that
state through stateless facade calls and small typed state owners such as the
list below.

`GuiToggleState` is one example of that division. The Haxe class owns one
Boolean across frames. `Raygui.Toggle` lends the address of that Boolean to
raygui for one native call through `c.Ref.to(state.active)`. In the generated C,
that becomes an ordinary `bool *` pointing at the caller-owned field. The
pointer cannot be stored in a Haxe value or returned from a function, and the
binding selection records that raygui may use it only until `GuiToggle`
returns. This is a narrow out-parameter contract, not a general C pointer or
borrow checker.

`GuiListViewState` applies the same rule to two C integers: the first visible
row and the selected row. Its `draw` method keeps those exact-width ABI values
private, exposes ordinary Haxe `Int` queries, and lends both addresses only
until `GuiListView` returns. A class is useful for this control because its
small state has a real lifetime across frames; stateless buttons remain simple
facade calls and do not become pretend widget objects.

The extra binding policy matters because Clang can prove that the header says
`bool *` or `int *`, but a C type does not say whether the library keeps that pointer.
[`raygui-core-selection.json`](specs/raygui-core-selection.json) therefore
names every admitted mutable parameter together with its call-only lifetime.
The generator fails if the pinned header no longer matches that reviewed
contract.

## Why the package is `#if c`

RayguiHx describes a C library ABI, so the package exists only for haxe.c.
`#if c` is evaluated while Haxe compiles the program. It does not generate a
target check in the game loop. A later target-neutral Caxecraft UI boundary can
choose a JavaScript, Rust, or another target's renderer without pretending
that `c.CString` and C struct layout are portable game-domain values.

## Header-only implementation ownership

Raygui publishes declarations and implementation in `raygui.h`. The function
bodies are compiled only when one C translation unit defines
`RAYGUI_IMPLEMENTATION`. Defining it twice produces duplicate symbols; never
defining it produces missing symbols at link time.

[`scripts/raygui/provision.py`](../scripts/raygui/provision.py) is the sole
implementation owner. It verifies the immutable source tree, generates this
content-addressed implementation unit:

```c
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
```

It then compiles that unit and creates a deterministic `libraygui.a`. The
archive metadata is normalized because BSD `ar` records the current time and
local user IDs even when the object bytes are identical. The normalizer changes
only standard archive member metadata; it never rewrites C machine code.

Raygui 5.0 itself produces warnings for signed/unsigned style colors, unused
parameters/functions, one shadowed name, and one narrowed file size under the
strict Clang lane. Those exact warning families remain visible but non-fatal
for the unmodified upstream header. Other enabled warnings remain errors. This
exception is third-party build evidence, not permission for generated or
repository-owned C to introduce the same warnings.

## Reproducible declarations

The reviewed function list lives in
[`raygui-core-selection.json`](specs/raygui-core-selection.json). Clang parses
the exact pinned raygui header together with the exact pinned raylib 6.0 header;
the resulting C types and source lines are recorded in
[`raygui-core-binding-lock.json`](specs/raygui-core-binding-lock.json). The raw
Haxe declaration is generated from that lock, so it is not a second handwritten
description of the ABI.

Run the focused check with:

```bash
npm run test:raygui-binding
```

That check proves lock/generator consistency, fail-closed provenance, the one
implementation template, deterministic archive metadata, and a real Haxe-to-C
compile whose generated C calls raygui directly.

The Raylib headless CI lane additionally runs
`test/raygui_binding/native.py`. It builds the one pinned implementation archive
and links two independent consumers: strict C11 and C++17. These small programs
are handwritten deliberately. If they were generated by haxe.c, the compiler
would be checking its own description of the ABI against itself. Independent
source proves that the selected header, C/C++ language guards, archive, Raylib
dependency, and linker order agree outside the Haxe pipeline.

Caxecraft's build command uses the same provisioner and keeps the resulting
archive in a content-derived cache. Generated application C is compiled with
both pinned include directories and linked in this order:

```text
generated C objects -> libraygui.a -> libraylib.a -> platform libraries
```

Static-library order matters because raygui calls Raylib. The generated build
manifest must request exactly that reviewed dependency set; the Python build
driver does not silently add raygui when Haxe did not reach the binding.

Real rendered editor evidence is still required before the visual editor is
described as complete.
