# ABI fixtures

Cases assert public headers, native layout and symbol facts, calling convention,
ownership/error contracts, and independent C/C++ consumers. See
[`docs/testing.md`](../../docs/testing.md).

The allocator-contract case hardens an internal native-seed owner/callback
boundary with custom freestanding allocation and C/C++ layout evidence. The
point-library and C++ shim remain independent interop seeds. None is generated
export evidence or a public ABI stability promise.
