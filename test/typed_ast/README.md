# Typed-AST input adapter fixtures

This suite compiles real Haxe through the custom C target and inspects the
implementation-only `reflaxe_c_typed_ast_report` inventory before the current
`HXC1000` lowering boundary. It proves classification and deterministic input
normalization; it does not claim that Haxe-to-C lowering exists.

The rich fixture covers primary and secondary module ownership, classes,
interfaces, externs, enums, typedefs, abstracts, metadata, entry-point facts,
fields, and expression-node inventory. The isolation fixture follows a richer
request through the same Haxe compiler server and must match its own cold-build
report byte for byte.

Run `npm run test:typed-ast`. Refresh its reviewed JSON only through
`npm run snapshots:update -- --suite typed-ast`.
