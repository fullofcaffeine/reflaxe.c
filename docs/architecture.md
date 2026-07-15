# Architecture overview

## Core invariant

One compiler pipeline serves both `portable` and `metal`. Profiles select representation policy, fallback permission, diagnostics, and runtime constraints; they do not fork the compiler.

## Components

```text
BuildDetection
CompilerBootstrap
CompilerInit
  -> BuildContextResolver
  -> boundary macros
  -> Reflaxe registration

CReflaxeCompiler
  -> collects Haxe module types
  -> resets CompilationContext
  -> invokes CCompiler
  -> writes GeneratedFile records through OutputManager

CCompiler
  -> validates semantic contract
  -> lowers typed AST to HxcIR
  -> runs analyses and HxcIR passes
  -> lowers to C AST
  -> runs C passes
  -> invokes ProjectEmitter

ProjectEmitter
  -> public/private headers
  -> source files
  -> boot/export/reflection units
  -> runtime slices
  -> CMake/Meson/tool-neutral manifest
  -> runtime, symbol, stdlib, ABI, and lowering reports
```

## Why `Manual` Reflaxe output

A compilation produces multiple file categories and sidecar reports. `Manual` lets the compiler control paths while retaining Reflaxe's changed-file and stale-file ownership behavior.

## IR boundary

`HxcIR` exists to normalize semantics that C syntax cannot safely express directly:

- evaluation sequence;
- expression-valued control flow;
- cleanup and exception edges;
- addressability;
- representation choices;
- allocation/ownership;
- dispatch classification.

`CAST` models C declarations and syntax precisely. It does not decide Haxe semantics.

## Fail-closed bootstrap

The scaffold's compiler classes establish lifecycle and interfaces, but broad language lowering is not marked complete. Until a construct has a tested lowering, the implementation must issue a stable diagnostic. Silent placeholder code is prohibited.

## Runtime feature graph

`RuntimeRequirementAnalyzer` records source reasons. `RuntimeFeatureRegistry` resolves dependencies. `ProjectEmitter` copies/emits only selected features. `hxc_runtime=none` asks `NoRuntimeEligibilityAnalyzer` for a proof; a failed proof returns all blocking features and source sites.

## ABI boundary

The internal object representation is not the public ABI. Export lowering maps Haxe values to a separate ABI model with:

- fixed scalars;
- frozen plain structs where explicitly selected;
- opaque handles;
- string/byte views;
- callbacks plus context;
- status and structured error outputs;
- explicit ownership.

## Build boundary

The compiler emits a tool-neutral `hxc.manifest.json`. CMake and Meson are adapters. Existing build systems can consume the manifest and generated source tree directly.
