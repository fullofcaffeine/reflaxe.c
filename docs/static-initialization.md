# Static initialization

The admitted primitive executable slice uses one explicit eager initialization
phase. It never relies on C file-scope initializer ordering across translation
units. The phase is planned from the captured whole-program typed AST, lowered
through HxcIR, and called exactly once by the compiler-owned hosted `main`
before the Haxe entry function when the plan is non-empty. An empty plan emits
no wrapper, symbol, or call.

## Selected strategy

The strategy is `eager-haxe-type-order`, following the pinned Haxe compiler's
dependency sort and its JavaScript/Lua generator phase split:

1. derive a dependency-first type order;
2. run retained class `static function __init__()` bodies in that type order;
3. assign retained ordinary static fields in the same type order and each
   class's source field order;
4. invoke the Haxe executable entry.

This ordering matters even for literal fields. A class initializer may write a
field that its ordinary field initializer subsequently replaces, so moving a
literal into a C file-scope initializer would change observable Haxe behavior.
Generated global storage therefore has no C initializer. Explicit Haxe field
initializers become private zero-argument HxcIR functions with one
`initialize-global` instruction, and each global records
`deferred(<initializer-function-id>)`.

The current strategy is eager and executable-scoped. Lazy first-use guards,
library init/shutdown entry points, registration phases, thread-safe concurrent
initialization, source-uninitialized fields, and non-primitive static
representations remain future work.
No type-registration phase is selected for the current primitive representation.

## Dependency graph

The planner starts from every retained non-extern class with a class initializer
or a physical non-inline static-field initializer. It records source-positioned
edges for:

- superclass and implemented-interface requirements;
- typed class references;
- referenced static fields and static functions;
- constructor references.

Static function and referenced-field bodies are traversed recursively, so a
dependency hidden behind `A.make()` remains visible. Edges and roots are sorted
by UTF-8 logical identity before depth-first ordering; callback order, map
iteration, checkout path, and compiler-server history cannot affect the plan.

Self-references stay within their class's Haxe source order. A cross-type cycle
is rejected with `HXC1002`. Its path is chosen from sorted roots and sorted
edges and names every dependency reason and normalized source span, so reversed
typed-module discovery produces the same diagnostic. The compiler emits no
artifact after that failure.

## Inspection and generated C

Every successful primitive executable includes
`hxc.initialization-plan.json`. Schema 1 records:

- the strategy, phase order, entry function, cycle and exact-once policies;
- dependency-first `typeOrder`;
- source-positioned dependency edges and reasons;
- class/field initializer records and their HxcIR function IDs;
- the exact `executionOrder`;
- an empty `runtimeFeatures` list.

`reflaxe_c_static_initialization_report` is an implementation-only test seam
that prints the same plan with the validated HxcIR dump. It is not application
configuration. The production C emitter turns each initializer into a
project-private externally linked C function declared in the compiler-owned
private header tree. That linkage is identical in split and unity layouts; it
lets the small split entry unit call module-owned initializer definitions and
does not create a public export or ABI promise. The emitter turns the ordered
IDs into one file-local `static void` bootstrap function for a non-empty plan.
Hosted `int main(void)` calls that function once, then calls Haxe `main`, then
returns zero. Empty plans are compile-time-elided. The HxcIR validator requires a
deferred initializer to have signature `():Void` and to initialize its named
global exactly once.

Initialization selects no `hxrt` feature, allocation, public export, or build
fact. The runtime plan records `compiler-planned-eager-static-initialization`
as a direct compiler decision while keeping its feature list empty. Portable
and metal emit identical initialization C. `auto`, `minimal`, and explicit
`none` runtime policy therefore retain the analyzed empty runtime feature plan.
This task changes generated private symbols and adds a compiler sidecar; it
does not stabilize or alter a public C ABI. The current implementation is
hosted-executable only because broader environments and library lifecycle are
outside the admitted project-emission slice.

## Evidence

`test/static_initialization` proves:

- a lexically reversed three-type dependency chain and source-ordered fields;
- class-before-field phase behavior against the pinned Haxe JavaScript
  generator oracle;
- deferred globals and exact-once `initialize-global` HxcIR;
- project-private initializer declarations with identical split/unity linkage
  and no public-export/ABI claim;
- byte-identical repeated and reversed-input production roots;
- deterministic source-positioned `HXC1002` with no output;
- portable, metal, and runtime-none zero-runtime plans;
- warning-clean strict C11 execution at `-O0` and `-O2` under the required GCC
  and Clang CI lanes, using an independent harness that checks the initialized
  result.

The native-only CI lane replays the checked-in generated C without invoking
Haxe. Its small `native-symbols.json` snapshot maps the stable Haxe source
identity used by that harness to the compiler-selected C name. This avoids
guessing an internal spelling from generated text while also avoiding a copy of
the complete symbol report when the harness needs only one function.

Run:

```bash
npm run test:static-initialization
npm run snapshots:check
```
