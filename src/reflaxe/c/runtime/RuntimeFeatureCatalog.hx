package reflaxe.c.runtime;

import reflaxe.c.CEnvironment;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureArtifact;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDocumentation;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureReservation;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureSelectionRoot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureSelectionRootKind;

/** Checked-in feature vocabulary with evidence-bounded compiler selection. */
class RuntimeFeatureCatalog {
	public static function registry():RuntimeFeatureRegistry
		return new RuntimeFeatureRegistry(definitions(), reservations());

	public static function definitions():Array<RuntimeFeatureDefinition> {
		final environments = [CEnvironment.Hosted, CEnvironment.Freestanding];
		final runtimeBase = RuntimeFeatureId.parse("runtime-base");
		final runtimeAbi = RuntimeFeatureId.parse("runtime-abi");
		final status = RuntimeFeatureId.parse("status");
		final statusName = RuntimeFeatureId.parse("status-name");
		final alloc = RuntimeFeatureId.parse("alloc");
		final array = RuntimeFeatureId.parse("array");
		final intMap = RuntimeFeatureId.parse("int-map");
		final stringMap = RuntimeFeatureId.parse("string-map");
		final bytes = RuntimeFeatureId.parse("bytes");
		final object = RuntimeFeatureId.parse("object");
		final gc = RuntimeFeatureId.parse("gc");
		final stringLiteral = RuntimeFeatureId.parse("string-literal");
		final stringScalar = RuntimeFeatureId.parse("string-scalar");
		final string = RuntimeFeatureId.parse("string");
		final io = RuntimeFeatureId.parse("io");
		return [
			new RuntimeFeatureDefinition(runtimeBase, "Shared C types, internal ABI version, and visibility/alignment macros for selected runtime slices.",
				CompilerSelectable, true, environments, [], [header("base.h")], [], [], [],
				documentation("Defines the narrow C11/C++ foundation and internal ABI macros shared by every packaged hxrt slice; it owns no allocation, failure, or thread state.",
					[
						dependencyRoot("Selected only when another registered feature needs the shared types or ABI macros.")
					],
					"Runtime-free generated C includes the standard headers and direct C types it actually needs, so this feature is omitted.",
					"A program-local copy would duplicate the internal ABI and visibility contract in every selected slice.",
					"A single header is the smallest dependency that keeps independently packaged runtime slices ABI-consistent.", "docs/hxrt.md",
					["test/runtime/runtime-feature-graph/run.py", "test/string_output/run.py"])),
			new RuntimeFeatureDefinition(runtimeAbi, "Version query for the internal same-major runtime ABI, used only by native seed evidence.",
				NativeSeedOnly, true, environments, [runtimeBase], [header("abi.h"), source("abi.c")], ["hxc_runtime_abi_version"], [], [],
				documentation("Returns the packed internal hxrt version for independent native compatibility probes; generated Haxe cannot select this query.",
					[
					nativeSeedRoot("Requested only by the independent native runtime ABI smoke fixture.")
				],
					"Generated C checks HXC_RUNTIME_ABI_MAJOR directly with a structural _Static_assert and needs no query call.",
					"A program-local query would report the compiler's assumption rather than the linked runtime's version.",
					"The linked runtime must answer for its own build, but this evidence helper is not a generated-program semantic dependency.",
					"docs/hxrt.md",
					[
						"scripts/ci/runtime_smoke.py",
						"runtime/hxrt/test/runtime_smoke.c",
						"runtime/hxrt/test/public_header_cpp.cpp"
					])),
			new RuntimeFeatureDefinition(status, "Status value definitions used by selected runtime failure boundaries.", CompilerSelectable, true,
				environments, [runtimeBase], [header("status.h")], [], [], [],
				documentation("Defines the closed non-throwing status vocabulary returned by current runtime operations; it allocates nothing and stores no error state.",
					[
						dependencyRoot("Selected transitively by a slice whose C boundary can fail, currently hosted io or native seeds.")
					],
					"Direct generated C uses ordinary control flow when the complete operation and failure semantics can stay local.",
					"Duplicating numeric status values in each program-local helper would make independently packaged feature boundaries disagree.",
					"The header is the smallest shared failure vocabulary for separately compiled runtime sources.", "docs/hxrt.md",
					["test/runtime/runtime-feature-graph/run.py", "test/string_output/run.py"])),
			new RuntimeFeatureDefinition(statusName, "Symbolic status-name helper used only by native seed evidence.", NativeSeedOnly, true, environments,
				[status], [header("status_name.h"), source("status.c")], ["hxc_status_name"], [], [],
				documentation("Maps every known hxc_status value to a stable diagnostic name without allocating or retaining state.", [
					nativeSeedRoot("Requested only by independent native seed diagnostics and smoke fixtures.")
				],
					"Generated code branches on typed status values directly and does not need a name lookup.",
					"A fixture could duplicate the switch, but that would stop testing the runtime's own status vocabulary.",
					"The helper is shared native evidence, not a fallback selected for generated Haxe.", "docs/hxrt.md",
					["scripts/ci/runtime_smoke.py", "runtime/hxrt/test/runtime_smoke.c"])),
			new RuntimeFeatureDefinition(alloc, "Hardened allocator ownership and failure contracts with hosted and custom native evidence.",
				CompilerSelectable, true, environments, [status], [header("allocator.h"), source("allocator.c")], [
					"hxc_default_allocator",
					"hxc_allocator_is_valid",
					"hxc_allocator_same_identity",
					"hxc_size_add",
					"hxc_size_mul",
					"hxc_alloc",
					"hxc_realloc",
					"hxc_free",
					"hxc_allocation_is_valid",
					"hxc_allocation_allocate",
					"hxc_allocation_resize",
					"hxc_allocation_move",
					"hxc_allocation_dispose"
				],
				[], [],
				documentation("Implements checked size arithmetic, explicit allocator identity, aligned allocation, failure-atomic resize, and move-only allocation owners.",
					[
						new RuntimeFeatureSelectionRoot("allocation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable compiler-owned allocation whose lifetime is explicit in HxcIR."),
						new RuntimeFeatureSelectionRoot("runtime-sized-storage", RuntimeFeatureSelectionRootKind.TransitiveDependency,
							"Selected transitively when a compiler-admitted runtime feature needs runtime-sized owned storage.")
					],
					"Stack storage, fixed arrays, nonescaping spans, and bounded values remain direct C and never request allocation.",
					"A program-local allocator is preferred when whole-program escape and size facts permit a narrower specialized owner.",
					"Unknown runtime sizes and allocator identity need one shared ownership and failure contract. The compiler still keeps fixed, bounded, or nonescaping storage direct when it can prove that representation is sufficient.",
					"docs/hxrt.md",
					[
						"scripts/ci/runtime_smoke.py",
						"runtime/hxrt/test/allocator_contract.c",
						"runtime/hxrt/test/allocator_abi.c"
					])),
			new RuntimeFeatureDefinition(array, "Resizable contiguous unboxed storage with checked growth and optional typed element lifecycle callbacks.",
				CompilerSelectable, true, environments, [alloc], [header("array.h"), source("array.c")], [
					"hxc_array_element_ops_is_valid",
					"hxc_array_init",
					"hxc_array_is_valid",
					"hxc_array_reserve",
					"hxc_array_resize",
					"hxc_array_at",
					"hxc_array_at_const",
					"hxc_array_push_copy",
					"hxc_array_ref_create",
					"hxc_array_ref_create_trivial",
					"hxc_array_ref_dispose_in_place",
					"hxc_array_ref_get_copy",
					"hxc_array_ref_init_in_place",
					"hxc_array_ref_is_valid",
					"hxc_array_ref_length",
					"hxc_array_ref_push_copy",
					"hxc_array_ref_release",
					"hxc_array_ref_retain",
					"hxc_array_ref_set_copy",
					"hxc_array_insert_copy",
					"hxc_array_set_copy",
					"hxc_array_remove_at",
					"hxc_array_move",
					"hxc_array_dispose"
				],
				[], [],
				documentation("Implements a move-only resizable unboxed buffer with deterministic checked growth, borrow invalidation, and optional typed element lifecycle callbacks.",
					[
						new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array<T> whose length and shared identity are decided at run time."),
						new RuntimeFeatureSelectionRoot("create-literal", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array literal for an admitted unboxed element representation."),
						new RuntimeFeatureSelectionRoot("collection-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array length, checked indexing, or push operation.")
					],
					"Compiler-known fixed arrays and nonescaping spans use direct C storage and bounds checks.",
					"Closed element types and bounded capacities should use a program-local specialized helper when that is smaller and equally correct.",
					"Ordinary Haxe Array values combine run-time growth with shared mutable identity. A reference-counted container preserves that identity for the admitted acyclic element slice while the compiler keeps fixed arrays and spans direct and runtime-free.",
					"docs/hxrt.md",
					[
						"test/differential/array-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringMap, "String-keyed shared Haxe Map identity with copied UTF-8 keys and exact unboxed value storage.",
				CompilerSelectable, true, environments, [alloc, stringLiteral], [header("string_map.h"), source("string_map.c")], [
					"hxc_string_map_ref_create",
					"hxc_string_map_ref_create_with_ops",
					"hxc_string_map_ref_retain",
					"hxc_string_map_ref_release",
					"hxc_string_map_ref_set_copy",
					"hxc_string_map_ref_exists",
					"hxc_string_map_ref_get_copy",
					"hxc_string_map_ref_remove",
					"hxc_string_map_ref_clear",
					"hxc_string_map_value_ops_is_valid"
				],
				[], [],
				documentation("Preserves ordinary Map<String, V> alias identity while copying canonical UTF-8 keys and keeping each admitted V specialization exact and unboxed; trivial values copy as bytes, while managed direct records use a complete compiler-generated copy/assign/destroy policy.",
					[
						new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Map<String, V> whose keys, contents, and shared identity change at run time."),
						new RuntimeFeatureSelectionRoot("string-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable admitted construction, lookup, membership, insertion, removal, or clear operation.")
					],
					"A compiler-known immutable lookup table can remain direct const C data when Haxe mutation and alias identity are unobservable.",
					"A closed, bounded map can use a program-local specialization when it preserves String equality, mutation, missing values, and alias identity.",
					"General mutable maps need run-time key ownership and shared identity. The compiler still chooses one exact value layout per specialization, so the runtime does not introduce Dynamic values or universal boxing.",
					"docs/hxrt.md",
					[
						"test/differential/string-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(intMap, "Integer-keyed shared Map<Int, Bool> identity with exact unboxed storage.", CompilerSelectable, true,
				environments, [alloc], [header("int_map.h"), source("int_map.c")], [
					"hxc_int_bool_map_ref_create",
					"hxc_int_bool_map_ref_retain",
					"hxc_int_bool_map_ref_release",
					"hxc_int_bool_map_ref_set",
					"hxc_int_bool_map_ref_exists"
				],
				[], [],
				documentation("Preserves ordinary Map<Int, Bool> alias identity and key presence while storing both key and value in their exact C scalar forms.",
					[
					new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe Map<Int, Bool> whose contents and shared identity change at run time."),
					new RuntimeFeatureSelectionRoot("int-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable admitted construction, insertion, or membership operation.")
				],
					"A compiler-known immutable integer lookup table can remain direct const C data when mutation and alias identity are unobservable.",
					"A closed bounded key range can use a program-local bitset or table when the compiler can prove that range and preserve Map identity.",
					"General run-time keys need mutable shared storage. This first Bool specialization avoids Dynamic values and boxing while leaving unproved IntMap methods unsupported.",
					"docs/hxrt.md", ["test/differential/int-map/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(bytes, "Fixed-length mutable binary storage with checked ranges and shared Haxe identity.", CompilerSelectable, true,
				environments, [alloc, stringLiteral], [header("bytes.h"), source("bytes.c")], [
					"hxc_bytes_ref_create_zeroed",
					"hxc_bytes_ref_create_copy",
					"hxc_bytes_ref_create_utf8_copy",
					"hxc_bytes_ref_is_valid",
					"hxc_bytes_ref_retain",
					"hxc_bytes_ref_release",
					"hxc_bytes_ref_length",
					"hxc_bytes_ref_get",
					"hxc_bytes_ref_set",
					"hxc_bytes_ref_sub",
					"hxc_bytes_ref_blit",
					"hxc_bytes_ref_fill",
					"hxc_bytes_ref_compare"
				],
				[], [],
				documentation("Implements exact-length arbitrary byte buffers, alias-visible mutation, checked copying, and explicit UTF-8 String-to-bytes copying.",
					[
					new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable haxe.io.Bytes value whose contents or identity live at run time."),
					new RuntimeFeatureSelectionRoot("binary-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable admitted Bytes allocation, range, copy, comparison, or mutation operation.")
				],
					"Compiler-known immutable byte tables can remain direct const C data when Haxe identity and mutation are unobservable.",
					"A closed bounded buffer can use a program-local specialization when it preserves the same alias and bounds contract.",
					"General Bytes values have run-time size and shared mutable identity. The selected slice owns that storage without treating arbitrary bytes as text or boxed integers.",
					"docs/hxrt.md",
					[
						"test/differential/bytes-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(object, "Selective immutable object/type descriptors with exact trace and optional finalization dispatch.",
				CompilerSelectable, true, environments, [runtimeBase], [header("object.h"), source("object.c")], [
					"hxc_type_descriptor_is_valid",
					"hxc_object_header_init",
					"hxc_object_header_is_valid",
					"hxc_type_descriptor_trace",
					"hxc_type_descriptor_finalize"
				], [], [],
				documentation("Defines the collector-neutral internal descriptor and header contract for reachable managed payload types.", [
					new RuntimeFeatureSelectionRoot("managed-object-descriptor", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable escaping object representation needs exact size, alignment, tracing, or cleanup facts."),
					dependencyRoot("Selected transitively by the precise collector for managed object allocation and tracing.")
				],
					"Nonescaping classes and values with proven direct lifetimes keep ordinary private C storage and no descriptor.",
					"A closed bounded region may use a program-local ownership plan when identity, lifetime, and cleanup are fully proven.",
					"Escaping object graphs need one immutable, versioned description of payload layout and exact outgoing references. The descriptor remains separate from reflection names and collector-private mark state.",
					"docs/object-descriptors.md", ["test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(gc,
				"Precise non-moving mark-and-sweep collection with explicit exact roots, pins, pressure thresholds, and reports.", CompilerSelectable, true,
				environments, [alloc, object], [header("gc.h"), source("gc.c")], [
					"hxc_gc_init",
					"hxc_gc_dispose",
					"hxc_gc_allocate",
					"hxc_gc_collect",
					"hxc_gc_safepoint",
					"hxc_gc_owns_exact",
					"hxc_gc_get_stats",
					"hxc_gc_thread_register",
					"hxc_gc_thread_unregister",
					"hxc_gc_root_frame_push",
					"hxc_gc_root_frame_pop",
					"hxc_gc_root_table_register",
					"hxc_gc_root_table_unregister",
					"hxc_gc_pin_object",
					"hxc_gc_unpin_object"
				], [], [],
				documentation("Implements the selected precise collector backend over immutable type descriptors and the reviewed allocator ABI.", [
					new RuntimeFeatureSelectionRoot("managed-object-graph", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable escaping identity-bearing graph cannot use a proven stack, region, or manual lifetime."),
					new RuntimeFeatureSelectionRoot("managed-cycle", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable managed representation may contain a reference cycle that local retain/release ownership cannot reclaim.")
				],
					"Nonescaping values, static literals, bounded regions, and explicitly manual ownership remain direct and collector-free.",
					"A closed program-local region is preferred when the compiler can prove all identities, escapes, and destruction points.",
					"General escaping Haxe graphs need stable identity, cyclic reclamation, and exact roots coordinated across functions. One selected backend owns those shared lifetimes without conservatively scanning arbitrary C memory.",
					"docs/gc-runtime.md", ["test/runtime/gc/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(stringLiteral,
				"Nullable immutable UTF-8 String carrier with explicit byte length and no allocation or object dependency.", CompilerSelectable, true,
				environments, [runtimeBase], [header("string_literal.h")], [], [], [],
				documentation("Defines the allocation-free private hxc_string view used for Haxe null and compiler-owned valid UTF-8 literal storage, including embedded NUL bytes.",
					[
						new RuntimeFeatureSelectionRoot("direct-string-value", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable immutable Haxe String value backed by compiler-owned literal storage."),
						dependencyRoot("Selected transitively when a runtime operation consumes the compiler's direct literal representation.")
					],
					"The literal bytes and length are emitted directly in generated C; Haxe null uses a null data pointer, while every real String including the empty String has a non-null address.",
					"The carrier itself is already the program-specific representation; cloning its ABI would make runtime consumers incompatible.",
					"The header shares the value fields plus an opaque optional owner pointer. Literal-only programs keep that pointer null and still allocate nothing.",
					"docs/hxrt.md", ["test/string_output/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(stringScalar, "Allocation-free valid-UTF-8 validation, Unicode-scalar indexing, slicing, comparison, and hashing.",
				CompilerSelectable, true, environments, [status, stringLiteral],
				[header("string_scalar.h"), header("string_decode.h"), source("string_scalar.c")], [
					"hxc_utf8_validate",
					"hxc_string_is_valid",
					"hxc_string_scalar_length",
					"hxc_string_haxe_length",
					"hxc_string_scalar_at",
					"hxc_string_slice",
					"hxc_string_char_at",
					"hxc_string_char_code_at",
					"hxc_string_substring",
					"hxc_string_compare",
					"hxc_string_hash"
				], [], [],
				documentation("Inspects immutable valid-UTF-8 String views with Unicode-scalar indices and no allocation or retained state.", [
					new RuntimeFeatureSelectionRoot("char-at", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.charAt whose receiver or index is known only at run time."),
					new RuntimeFeatureSelectionRoot("char-code-at", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.charCodeAt whose receiver or index is known only at run time."),
					new RuntimeFeatureSelectionRoot("length", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.length whose scalar count is not known at compile time."),
					new RuntimeFeatureSelectionRoot("substring", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.substring whose bounds are known only at run time."),
					dependencyRoot("Selected transitively by a broader String feature that reuses the same validated scalar rules.")
				],
					"A compiler-known literal and index may fold directly to another literal-backed String view.",
					"One closed program can use a generated local scalar helper, but duplicating the UTF-8 decoder across programs risks semantic drift.",
					"Runtime-dependent scalar positions need one shared implementation of ADR 0004 indexing, invalid-input checks, and borrowed slice construction without selecting owned strings or allocation.",
					"docs/hxrt.md",
					[
						"test/differential/string-char-at/run.py",
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(string,
				"Owned UTF-8 construction, reference-counted Haxe values, mutable building, and explicit CString conversion.", CompilerSelectable, true,
				environments, [alloc, stringScalar], [header("string.h"), source("string.c")], [
					"hxc_string_retain",
					"hxc_string_release",
					"hxc_string_from_scalar",
					"hxc_string_concat_ref",
					"hxc_byte_view_from_cstring",
					"hxc_string_from_utf8_checked",
					"hxc_string_from_utf8_lossy",
					"hxc_string_copy",
					"hxc_string_concat",
					"hxc_owned_string_dispose",
					"hxc_string_buffer_init",
					"hxc_string_buffer_view",
					"hxc_string_buffer_append_utf8_checked",
					"hxc_string_buffer_append_scalar",
					"hxc_string_buffer_finish",
					"hxc_string_buffer_dispose",
					"hxc_string_borrow_cstring",
					"hxc_string_to_cstring_owned",
					"hxc_owned_cstring_dispose"
				],
				[], [],
				documentation("Adds reference-counted ordinary Haxe String values, explicit owned strings, builders, lossy decoding, concatenation, and CString conversion above the shared scalar slice.",
					[
						new RuntimeFeatureSelectionRoot("from-scalar", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe String.fromCharCode whose scalar is known only at run time."),
						new RuntimeFeatureSelectionRoot("concat", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe String concatenation whose result bytes are not compile-time constants."),
						new RuntimeFeatureSelectionRoot("retain", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable managed String copy must keep its optional backing owner alive."),
						new RuntimeFeatureSelectionRoot("cleanup-release", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable managed String owner must be released when its Haxe lifetime ends.")
					],
					"Known literals, constant concatenation, and statically decidable string facts should remain direct compiler-owned C data.",
					"Closed call sites should receive specialized local operations when their representation, lifetime, and operation set are statically bounded.",
					"Runtime-created UTF-8 bytes can cross ordinary calls and container boundaries, so their allocator identity and final release need one shared owner contract.",
					"docs/hxrt.md",
					[
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(io, "Minimal hosted length-delimited literal output with explicit write and flush failure status.",
				CompilerSelectable, true, [CEnvironment.Hosted], [status, stringLiteral], [header("io.h"), source("io.c")], ["hxc_io_println"], [], [],
				documentation("Writes one valid length-delimited Haxe String value plus a newline to hosted stdout and reports write or flush failure explicitly.",
					[
					new RuntimeFeatureSelectionRoot("sys-println-literal", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"Reachable Sys.println with a compiler-owned String literal."),
					new RuntimeFeatureSelectionRoot("trace-literal", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"Reachable default trace with compiler-owned literal text and source prefix.")
				],
					"The literal representation stays direct C, but ISO C has no expression that performs the required hosted write and flush side effect.",
					"Inlining fwrite/fputc/fflush at each call site would duplicate platform and failure policy rather than specialize semantics.",
					"One narrow hosted service centralizes exact-length output, embedded-NUL handling, flushing, and status mapping without pulling in general strings or allocation.",
					"docs/hxrt.md", [
						"test/string_output/run.py",
						"test/runtime/runtime-feature-graph/run.py",
						"examples/hello/run.py"
					]))
		];
	}

	public static function reservations():Array<RuntimeFeatureReservation> {
		return [
			reserved("closure", "E3.T08", "Escaping closure environment support after escape analysis."),
			reserved("date-time", "E5.T08", "Date, timezone, wall-clock, and monotonic-time adapters."),
			reserved("dynamic", "E4.T07", "Source-required tagged Haxe dynamic values and operations."),
			reserved("exception", "E4.T09", "Contained general exception frames after result lowering is ineligible."),
			reserved("export-error", "E7.T04", "Thread-safe exported status and error-detail boundary."),
			reserved("filesystem", "E5.T09", "Hosted filesystem and file-resource adapters."),
			reserved("process", "E5.T09", "Hosted environment and process adapters."),
			reserved("reflection", "E4.T08", "Reachability-selected type and member reflection metadata."),
			reserved("regex", "E5.T07", "Selected regular-expression backend and adapter."),
			reserved("socket", "E5.T10", "Network address, socket, and error adapters."),
			reserved("thread", "E5.T11", "Threads, synchronization, thread roots, TLS, and atomics."),
			reserved("unicode", "E5.T02", "Unicode scalar algorithms not feasible as direct or local specialized C.")
		];
	}

	static function header(name:String):RuntimeFeatureArtifact
		return new RuntimeFeatureArtifact('runtime/hxrt/include/hxrt/$name', 'runtime/include/hxrt/$name', GeneratedFileKind.RuntimeHeader, headerSha256(name));

	static function source(name:String):RuntimeFeatureArtifact
		return new RuntimeFeatureArtifact('runtime/hxrt/src/$name', 'runtime/src/$name', GeneratedFileKind.RuntimeSource, sourceSha256(name));

	static function documentation(contract:String, selectionRoots:Array<RuntimeFeatureSelectionRoot>, directAlternative:String,
			programLocalAlternative:String, runtimeRationale:String, referencePath:String, evidence:Array<String>):RuntimeFeatureDocumentation
		return new RuntimeFeatureDocumentation(contract, selectionRoots, directAlternative, programLocalAlternative, runtimeRationale, referencePath, evidence);

	static function dependencyRoot(description:String):RuntimeFeatureSelectionRoot
		return new RuntimeFeatureSelectionRoot("dependency-only", RuntimeFeatureSelectionRootKind.TransitiveDependency, description);

	static function nativeSeedRoot(description:String):RuntimeFeatureSelectionRoot
		return new RuntimeFeatureSelectionRoot("native-seed-fixture", RuntimeFeatureSelectionRootKind.NativeSeedFixture, description);

	static function headerSha256(name:String):String {
		return switch name {
			case "abi.h": "787d82dc867999ba8e8e6987cc6933ad6f6ab5d087b415e97042934c454ccf62";
			case "allocator.h": "6e21c0bc498eb40bcec901914a04dd1bee33b6b21e5a27f1ac5f169a8a1cc448";
			case "array.h": "5fa277cf34f4b0e01c1a5d3b7152857cf6570d3a9d537cb2a18c41f444db3512";
			case "base.h": "72e09e81ff4186d2850957001152e4323af77aa495664d3a5e4394aee3372139";
			case "bytes.h": "428c7879c1556fb3313c8135f7adf1ca4109dc5fe035efd5dabcf1eb653b1693";
			case "gc.h": "2ca9523f1c74c62877c3f006bab9bd8a3a2a1eced93d67ad59d015a7c6ecb9de";
			case "io.h": "4670078a26fb991c5de1f32ba3ab2c20cdc5e1d1b578dfe2504efe2b7e2f7d2e";
			case "int_map.h": "dd54b016db1d391dc7778b13e6cff856c886543ca87119b37141c5ad150f8080";
			case "object.h": "779b452097e4c58c7971b90743ace19a2dc6c91e381557abc84fbd5f9b30f1e5";
			case "status.h": "6bf20f5d82594014ad0f2b79a25cb81417791bd9c07375d2fb89835e415be1c4";
			case "status_name.h": "64bf3917787ffcf924369c8e1c0a525cf10902d004d5bb4b898f2af46a7456cc";
			case "string.h": "fcd9a99bb22df6a2173d4570961437baf177f0cbccef26505a0e70266235e121";
			case "string_decode.h": "aa93ea7f132aff625adfdcc7498532b139f621196deab4c0e9ecb5de2934fd48";
			case "string_literal.h": "ac6b5ad9fa13004c62e3b33b9b28a935bfb8a22287cd4595ce6e6eb81490e283";
			case "string_map.h": "26d94aa3cdfca1ae6edb678c575ed466bf32b7d6ccc635e55a706ec393c5db54";
			case "string_scalar.h": "87359eaff01a93c45cd2bee6664020aa1a0835920371cf01d8350f526ce1f2bf";
			case _: throw 'runtime feature header `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function sourceSha256(name:String):String {
		return switch name {
			case "abi.c": "3300a4498a7ca20f771b1334d7be8f2c908d2bb067ea8f2fe3c059300e680b32";
			case "allocator.c": "13385273c7c3d4a15785caa3095dd82d97bda8a026ebd9b6d54e2f531eb3b10e";
			case "array.c": "79e25b048ee656e4c98675a2c9e5bf30b5687764519d1c06883b524f07f05a2d";
			case "bytes.c": "4db5d3ddcaf32684e900abe7d81ffe3a008edc53806573aaebe84089c0c6a787";
			case "gc.c": "96cf942d6752070aaa5005eae3bc45c7d00aca37c360dfecaeb76d8db767b4cc";
			case "io.c": "c390615feea7f81c404941412909037ead8eb0ee1d3163d17f14154c20968e1c";
			case "int_map.c": "68a649d20d244f6fa73709da7d6a1d412a4ecb6e350048f0ed09fec6b044933e";
			case "object.c": "0e7fc6a55b562eaaf03fe63eca743dd73248f0bee1c09e21b79464917e8c89c0";
			case "status.c": "0695ab2528db6e29d5cf29d905ad736b7c1a3a79333082347ec18faea2d4e6d8";
			case "string.c": "432062278957800ffed153e6b237b4bd22c27906a17e7299c330c4bc9621e0e7";
			case "string_map.c": "6db2d30dd800c52131e18d74449995f15c170cc2c99be2596fd22b40506a0b04";
			case "string_scalar.c": "944de09c0a63531f725b61967f8bb91d19c5ff6fd428c45ed6cde169995f7f8d";
			case _: throw 'runtime feature source `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function reserved(id:String, ownerTask:String, summary:String):RuntimeFeatureReservation
		return new RuntimeFeatureReservation(RuntimeFeatureId.parse(id), ownerTask, summary);
}
