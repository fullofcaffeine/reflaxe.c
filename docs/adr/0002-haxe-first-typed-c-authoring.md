# ADR 0002: Haxe-first typed C authoring

- Status: Accepted
- Date: 2026-07-14
- Decision owners: project owner and compiler maintainers
- Related requirements: HXC-PROD-004, HXC-PROD-008, HXC-PROD-012,
  HXC-MAC-001 through HXC-MAC-012

## Context

The C target is not only a way to run portable Haxe on a C toolchain. C
developers should be able to author C-facing declarations, headers, layouts,
linkage, ownership, and build facts from Haxe while retaining transparent C
output.

Haxe can improve that workflow before native compilation: its type checker,
abstracts, algebraic enums, metadata, and macros can reject invalid layouts,
ownership contracts, declaration graphs, or domain schemas at the originating
Haxe source span. That compile-time layer complements rather than replaces C
compiler checks and Clang-derived ABI facts.

Without an admission policy, however, target APIs can devolve into a stringly
typed C mini-language or a collection of clever macros with hidden costs.

## Decision

### Surface priority

New C-facing capabilities use the first adequate form in this order:

1. ordinary Haxe constructs such as modules, classes, abstracts, enums,
   typedefs, functions, generics, and conditional compilation;
2. typed `c.*` abstractions for concepts Haxe does not model directly, such as
   pointers, spans, ownership, C strings, qualifiers, atomics, function
   pointers, allocators, and results;
3. validated metadata or macros for declaration facts such as header grouping,
   external includes, native names, export visibility, linkage, calling
   conventions, sections, packing, alignment, and bitfields;
4. a narrow typed DSL only when Haxe syntax and metadata cannot express the
   concept clearly;
5. raw C only as an explicit, auditable, unsafe framework or migration escape.

Repeated raw snippets are treated as evidence of a missing typed surface or
compiler lowering.

### Headers are generated from typed declarations

Haxe-authored C headers are a first-class product surface. Ordinary declarations
and their types determine prototypes, structs, enums, forward declarations, and
include dependencies. Explicit metadata may select public/private header groups,
stable C names, visibility, or external includes, but users do not hand-author
include guards or dependency ordering as strings.

The exact API spellings remain subject to the owning implementation issue, but
the surface must cover:

- public and private header grouping;
- imported and exported declarations;
- complete versus incomplete types and forward declarations;
- C scalar widths, enums, structs, unions, opaque handles, arrays, function
  pointers, bitfields, packing, and alignment;
- `const`, `volatile`, `restrict`, atomic and mutability contracts;
- internal/external/static/inline linkage, visibility, calling conventions,
  sections, and platform capability gates;
- typed compile-time constants and assertions;
- external includes, libraries, defines, frameworks, sysroots, and target facts
  in the neutral build manifest.

Haxe conditional compilation should replace generated preprocessor branching
when the choice can be made before C emission. Function-like C macros should
prefer typed inline functions or generated wrappers. Raw preprocessor output is
reserved for ABI or platform boundaries that genuinely require it.

### Compile-time verification is layered

Macros and compiler analyses should reject mistakes as early as possible,
including:

- duplicate or reserved C symbols;
- invalid/incomplete by-value types and impossible declaration cycles;
- target-dependent size, alignment, offset, packing, or bitfield mistakes;
- qualifier, mutability, pointer provenance, borrow escape, ownership,
  nullability, allocator, and callback-lifetime mismatches;
- unsupported variadics, calling conventions, atomics, format strings, and
  section/visibility combinations;
- public headers that leak compiler-private or runtime-private layouts;
- build/header facts that conflict under the resolved target configuration.

Generated C still emits `_Static_assert` where native layout or toolchain facts
must be verified. Imported headers and ABI layouts remain authoritative only
when parsed by Clang and/or checked by compiled native probes. Haxe macros must
not guess them.

### Macro and DSL admission gate

A first-party macro or DSL is accepted only when it has a written reason to
exist and all of the following:

- typed Haxe inputs and outputs, or a precisely constrained parser when text is
  the external format being modeled;
- a capability Haxe syntax or existing `c.*` APIs cannot express as clearly;
- source-positioned, actionable diagnostics and negative fixtures;
- deterministic expansion plus inspectable generated-declaration, manifest,
  and C snapshots;
- explicit allocation, ownership, unsafe, runtime, and portability effects;
- no bypass around profile, runtime, environment, ABI, or raw-boundary policy;
- no runtime reflection when compile-time information is sufficient;
- a reviewed complexity/ergonomics benefit proportional to its maintenance and
  language-surface cost.

An implementation that lowers internally through raw C still owns an unsafe
boundary and must expose a typed API to application code.

## Consequences

- Metal authoring can remain close to C without giving up Haxe's compile-time
  feedback loop.
- Header generation, bindgen, ABI export, and application DSLs share one typed
  declaration and policy model instead of inventing incompatible annotations.
- Compile-time checks improve diagnostics, while C/C++ compiler and ABI-probe
  gates retain final native authority.
- Target-specific surfaces stay explicit, so ordinary domain code remains
  portable to sibling Haxe targets.
- Cleverness alone is not a reason to add a macro, metadata spelling, or DSL.

## Rejected alternatives

- Treating raw `__c__` as the metal authoring API: it defeats type checking,
  diagnostics, analysis, and deterministic structure.
- Recreating all C syntax as a string DSL: it is harder to validate than either
  Haxe or C and provides little product value.
- Asking macros to parse or infer C ABI facts: Clang and native probes are the
  authoritative tools for that job.
- Hiding ownership, allocation, or runtime use to make a DSL look concise:
  ergonomics must not erase the costs users selected `metal` to see.
