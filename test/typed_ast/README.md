# Typed-AST input adapter fixtures

This suite compiles real Haxe through the custom C target and inspects the
implementation-only `reflaxe_c_typed_ast_report` inventory immediately before
body lowering. The rich and isolation fixtures must compile successfully and
emit C. Their reports prove classification and deterministic input
normalization. The separate `test/body_lowering` suite checks how typed Haxe
expressions become the compiler's intermediate representation and C.

The rich fixture covers primary and secondary module ownership, classes,
interfaces, externs, enums, typedefs, abstracts, metadata, entry-point facts,
fields, and expression-node inventory. The isolation fixture follows a richer
request through the same Haxe compiler server and must match its own cold-build
report byte for byte.

Run `npm run test:typed-ast`. Refresh its reviewed JSON only through
`npm run snapshots:update -- --suite typed-ast`.

Run `python3 test/typed_ast/run.py --source-identity-only` for the focused
source-file identity check. It verifies reuse within one normalization request,
fresh identities after edits, retry after a failed read, and exact recovered
positions. This macro-only check does not load the complete compiler.

Function source tracking hashes each readable file once per normalization
request. A new request creates a new tracking instance, so server reuse cannot
hide file edits. The separate persistent position cache still requires matching
source content and typed expressions. The full suite retains compiler-server
and generated-output checks; this focused check does not replace them.
