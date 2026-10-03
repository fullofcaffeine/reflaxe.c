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
		final dynamicFeature = RuntimeFeatureId.parse("dynamic");
		final exceptionFeature = RuntimeFeatureId.parse("exception");
		final alloc = RuntimeFeatureId.parse("alloc");
		final array = RuntimeFeatureId.parse("array");
		final iterator = RuntimeFeatureId.parse("iterator");
		final intMap = RuntimeFeatureId.parse("int-map");
		final stringMap = RuntimeFeatureId.parse("string-map");
		final typedMap = RuntimeFeatureId.parse("typed-map");
		final gcStringMap = RuntimeFeatureId.parse("gc-string-map");
		final objectMap = RuntimeFeatureId.parse("object-map");
		final enumValueMap = RuntimeFeatureId.parse("enum-value-map");
		final bytes = RuntimeFeatureId.parse("bytes");
		final bytesString = RuntimeFeatureId.parse("bytes-string");
		final object = RuntimeFeatureId.parse("object");
		final gc = RuntimeFeatureId.parse("gc");
		final stringLiteral = RuntimeFeatureId.parse("string-literal");
		final stringScalar = RuntimeFeatureId.parse("string-scalar");
		final string = RuntimeFeatureId.parse("string");
		final stringLowerCase = RuntimeFeatureId.parse("string-lower-case");
		final stringFloat = RuntimeFeatureId.parse("string-float");
		final stringSplit = RuntimeFeatureId.parse("string-split");
		final arrayJoin = RuntimeFeatureId.parse("array-join");
		final io = RuntimeFeatureId.parse("io");
		final dateTime = RuntimeFeatureId.parse("date-time");
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
			new RuntimeFeatureDefinition(dynamicFeature, "Private tagged carrier for source-required closed-world Dynamic values.", CompilerSelectable, true,
				environments, [status], [header("dynamic.h"), source("dynamic.c")], [
					"hxc_dynamic_type_is_valid",
					"hxc_value_init_bool",
					"hxc_value_init_float64",
					"hxc_value_init_int32",
					"hxc_value_init_managed_reference",
					"hxc_value_init_managed_wrapper",
					"hxc_value_init_null",
					"hxc_value_init_static_token",
					"hxc_value_is_null",
					"hxc_value_is_valid",
					"hxc_value_managed_payload",
					"hxc_value_read_bool",
					"hxc_value_read_float64",
					"hxc_value_read_int32",
					"hxc_value_read_managed_reference",
					"hxc_value_read_managed_wrapper",
					"hxc_value_read_static_token"
				], [],
				[],
				documentation("Carries one validated Dynamic type identity and active payload without routing ordinary typed values through a universal box.",
					[
						new RuntimeFeatureSelectionRoot("dynamic-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable validated HxcIR Dynamic box, cast, field, call, equality, or managed-payload operation.")
					],
					"A statically typed value or operation keeps its direct specialized C representation and omits this feature.",
					"A closed expression may use direct or program-local specialization only when no Dynamic carrier is observable across that boundary.",
					"Observable Dynamic values need one shared tag, type identity, checked scalar access, and managed-root projection across generated C files. The carrier itself remains allocation-free; managed adapters select object and collector support separately.",
					"docs/hxrt.md",
					[
						"runtime/hxrt/test/dynamic_contract.c",
						"runtime/hxrt/test/dynamic_header_cpp.cpp",
						"test/runtime/dynamic/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(exceptionFeature, "Contained same-thread exception frames with rooted payload transport and reverse cleanup.",
				CompilerSelectable, false, environments, [dynamicFeature], [header("exception.h"), source("exception.c")], [
					"hxc_exception_cleanup_push",
					"hxc_exception_cleanup_run",
					"hxc_exception_cleanup_discard",
					"hxc_exception_frame_payload",
					"hxc_exception_frame_take_payload",
					"hxc_exception_frame_pop",
					"hxc_exception_frame_push",
					"hxc_exception_root_slot_update",
					"hxc_exception_raise"
				],
				[], [],
				documentation("Maintains a thread-local stack of active lexical handlers, rooted Dynamic payloads, and exactly-once cleanup callbacks around compiler-owned setjmp sites.",
					[
						new RuntimeFeatureSelectionRoot("general-exception-region", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable throw/catch region cannot be proven equivalent to ordinary result/status control flow.")
					],
					"Closed exact-type regions use explicit HxcIR failure edges and ordinary C labels with no exception runtime.",
					"A complete closed call graph may use generated status propagation when payload matching, cleanup, and observable behavior remain equivalent.",
					"Arbitrary cross-call throw and catch need one same-thread target chain. The frame keeps setjmp in generated code, roots managed payloads through a supplied slot, runs registered cleanups in reverse, and rejects missing or stale targets.",
					"docs/hxrt.md",
					[
						"runtime/hxrt/test/exception_contract.c",
						"runtime/hxrt/test/exception_header_cpp.cpp",
						"test/exception_lowering/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
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
					"hxc_array_ref_copy",
					"hxc_array_ref_copy_in_place",
					"hxc_array_ref_dispose_in_place",
					"hxc_array_ref_get_copy",
					"hxc_array_ref_init_in_place",
					"hxc_array_ref_is_valid",
					"hxc_array_ref_length",
					"hxc_array_ref_pop_move",
					"hxc_array_ref_shift_move",
					"hxc_array_ref_splice_one_discard",
					"hxc_array_ref_splice_one_copy",
					"hxc_array_ref_splice_discard",
					"hxc_array_ref_splice_copy",
					"hxc_array_ref_insert_copy",
					"hxc_array_ref_push_copy",
					"hxc_array_ref_resize_default",
					"hxc_array_ref_release",
					"hxc_array_ref_release_slot",
					"hxc_array_ref_retain",
					"hxc_array_ref_set_copy",
					"hxc_array_ref_sort",
					"hxc_array_insert_copy",
					"hxc_array_set_copy",
					"hxc_array_remove_at",
					"hxc_array_pop_move",
					"hxc_array_shift_move",
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
							"A reachable ordinary Haxe Array length, checked indexing, mutation, copy, arbitrary-range splice, resize, or sort operation."),
						new RuntimeFeatureSelectionRoot("splice-one-discard", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array.splice(pos, 1) whose removed Array result is discarded."),
						new RuntimeFeatureSelectionRoot("splice-one-copy", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array.splice(pos, 1) that returns the removed typed Array."),
						new RuntimeFeatureSelectionRoot("sort", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Array.sort call with an admitted exact typed comparator.")
					],
					"Compiler-known fixed arrays and nonescaping spans use direct C storage and bounds checks.",
					"Closed element types and bounded capacities should use a program-local specialized helper when that is smaller and equally correct.",
					"Ordinary Haxe Array values combine run-time growth with shared mutable identity. A reference-counted container preserves that identity for the admitted acyclic element slice while the compiler keeps fixed arrays and spans direct and runtime-free.",
					"docs/hxrt.md",
					[
						"test/differential/array-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(iterator, "Shared-cursor typed snapshots and live Array cursors for standard Haxe Iterator values.",
				CompilerSelectable, true, environments, [alloc, array], [header("iterator.h"), source("iterator.c")], [
					"hxc_iterator_element_ops_is_valid",
					"hxc_iterator_ref_create_snapshot",
					"hxc_iterator_ref_create_traced_snapshot",
					"hxc_iterator_ref_create_array_values",
					"hxc_iterator_ref_create_array_pairs",
					"hxc_iterator_ref_retain",
					"hxc_iterator_ref_release",
					"hxc_iterator_ref_release_slot",
					"hxc_iterator_ref_has_next",
					"hxc_iterator_ref_next_move"
				],
				[], [],
				documentation("Preserves one shared cursor across Iterator aliases while keeping each element exact and unboxed; maps snapshot their producer, while Array cursors retain live identity and observe later length changes.",
					[
						new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable standard Haxe Iterator<T> whose shared cursor crosses ordinary expressions or calls."),
						new RuntimeFeatureSelectionRoot("iterator-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable standard Iterator.hasNext or Iterator.next operation."),
						new RuntimeFeatureSelectionRoot("array-cursor", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ArrayIterator or ArrayKeyValueIterator whose shared live cursor crosses a call or return."),
						new RuntimeFeatureSelectionRoot("retain", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Iterator alias must retain the same shared cursor."),
						new RuntimeFeatureSelectionRoot("cleanup-release", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Iterator owner must release unconsumed snapshot elements when its Haxe lifetime ends.")
					],
					"Compile-time-known iteration can remain direct control flow when no Iterator value or shared cursor is observable.",
					"A closed producer may use a program-local cursor only when aliases, element lifetime, and exhaustion behavior remain identical.",
					"General Iterator values need run-time shared cursor identity. Map snapshots and live Array cursors share the carrier, while their exact element layout and lifecycle remain compiler-selected.",
					"docs/hxrt.md",
					[
						"test/differential/array-runtime/run.py",
						"test/differential/string-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringMap, "String-keyed shared Haxe Map identity with copied UTF-8 keys and exact unboxed value storage.",
				CompilerSelectable, true, environments, [alloc, iterator, string, stringLiteral], [header("string_map.h"), source("string_map.c")], [
					"hxc_string_map_ref_create",
					"hxc_string_map_ref_create_with_ops",
					"hxc_string_map_ref_retain",
					"hxc_string_map_ref_release",
					"hxc_string_map_ref_release_slot",
					"hxc_string_map_ref_copy",
					"hxc_string_map_ref_set_copy",
					"hxc_string_map_ref_exists",
					"hxc_string_map_ref_get_copy",
					"hxc_string_map_ref_remove",
					"hxc_string_map_ref_clear",
					"hxc_string_map_ref_value_iterator",
					"hxc_string_map_ref_key_iterator",
					"hxc_string_map_ref_pair_iterator",
					"hxc_string_map_ref_to_string",
					"hxc_string_map_value_ops_is_valid"
				],
				[], [],
				documentation("Preserves ordinary Map<String, V> alias identity while copying canonical UTF-8 keys and keeping each admitted V specialization exact and unboxed; trivial values copy as bytes, while managed direct records use a complete compiler-generated copy/assign/destroy policy.",
					[
						new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe Map<String, V> whose keys, contents, and shared identity change at run time."),
						new RuntimeFeatureSelectionRoot("string-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable admitted construction, copy, lookup, membership, insertion, removal, or clear operation.")
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
				environments, [alloc, iterator, string], [header("int_map.h"), source("int_map.c")], [
					"hxc_int_bool_map_ref_create",
					"hxc_int_bool_map_ref_retain",
					"hxc_int_bool_map_ref_release",
					"hxc_int_bool_map_ref_release_slot",
					"hxc_int_bool_map_ref_copy",
					"hxc_int_bool_map_ref_set",
					"hxc_int_bool_map_ref_exists",
					"hxc_int_bool_map_ref_get",
					"hxc_int_bool_map_ref_remove",
					"hxc_int_bool_map_ref_clear",
					"hxc_int_bool_map_ref_value_iterator",
					"hxc_int_bool_map_ref_key_iterator",
					"hxc_int_bool_map_ref_pair_iterator",
					"hxc_int_bool_map_ref_to_string"
				],
				[], [],
				documentation("Preserves ordinary Map<Int, Bool> alias identity and key presence while storing both key and value in their exact C scalar forms.",
					[
					new RuntimeFeatureSelectionRoot("managed-type-representation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe Map<Int, Bool> whose contents and shared identity change at run time."),
					new RuntimeFeatureSelectionRoot("int-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable admitted construction, copy, insertion, lookup, membership, removal, or clear operation.")
				],
					"A compiler-known immutable integer lookup table can remain direct const C data when mutation and alias identity are unobservable.",
					"A closed bounded key range can use a program-local bitset or table when the compiler can prove that range and preserve Map identity.",
					"General run-time keys need mutable shared storage. This Bool specialization keeps values unboxed and represents missing lookup results with the compiler's typed optional carrier.",
					"docs/hxrt.md", ["test/differential/int-map/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(typedMap, "Checked exact-layout hash-table mechanics shared by compiler-specialized identity and enum-value maps.",
				CompilerSelectable, true, environments, [gc, iterator], [header("typed_map.h"), source("typed_map.c")], [
					"hxc_typed_map_key_ops_is_valid",
					"hxc_typed_map_value_ops_is_valid",
					"hxc_typed_map_identity_hash",
					"hxc_typed_map_hash_mix",
					"hxc_typed_map_type_descriptor",
					"hxc_typed_map_ref_create",
					"hxc_typed_map_init_collector_owned",
					"hxc_typed_map_dispose_in_place",
					"hxc_typed_map_ref_retain",
					"hxc_typed_map_ref_release",
					"hxc_typed_map_ref_copy",
					"hxc_typed_map_copy_in_place",
					"hxc_typed_map_ref_set_copy",
					"hxc_typed_map_ref_exists",
					"hxc_typed_map_ref_get_copy",
					"hxc_typed_map_ref_remove",
					"hxc_typed_map_ref_clear",
					"hxc_typed_map_ref_value_iterator",
					"hxc_typed_map_ref_key_iterator",
					"hxc_typed_map_ref_pair_iterator"
				],
				[], [],
				documentation("Owns collision handling, checked allocation, failure-atomic mutation, and rooted iterator snapshots for exact compiler-provided key and value layouts.",
					[
						dependencyRoot("Selected by ObjectMap or EnumValueMap after the compiler has chosen an exact key policy and unboxed value layout.")
					],
					"A closed immutable lookup can remain direct generated data when mutable map identity is unobservable.",
					"A program-local bounded table is valid only when it preserves the same equality, aliasing, mutation, and failure contracts.",
					"The shared runtime owns table mechanics only. Generated typed callbacks retain Haxe key meaning and exact tracing, so the runtime does not box keys or values.",
					"docs/hxrt.md",
					[
						"test/differential/object-enum-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(gcStringMap, "String-keyed maps with precisely traced record values.", CompilerSelectable, true, environments,
				[typedMap, string], [], [], [], [],
				documentation("Keeps collector-managed children alive through map slots and iterator snapshots while preserving UTF-8 key equality.", [
					new RuntimeFeatureSelectionRoot("gc-string-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable StringMap operation whose record value contains collector-managed children.")
				],
					"Maps whose values need no collector retain the ordinary reference-counted StringMap representation.",
					"A bounded table specialization must preserve key equality, shared map identity, exact roots, and failure-atomic mutation.",
					"The existing typed-map runtime owns storage and collection. Generated callbacks own String equality and each exact value's tracing and cleanup.",
					"docs/hxrt.md",
					[
						"test/differential/string-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(objectMap, "Object-identity Map keys over the shared exact typed-map runtime.", CompilerSelectable, true,
				environments, [typedMap], [], [], [], [],
				documentation("Preserves stable Haxe object identity as map-key equality while tracing occupied object keys and exact values strongly.", [
					new RuntimeFeatureSelectionRoot("object-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe ObjectMap construction, copy, lookup, insertion, removal, clear, or iterator operation.")
				],
					"A compiler-known immutable identity lookup can remain direct when no shared mutable map value is observable.",
					"A bounded program-local specialization is allowed only when distinct objects with equal fields remain distinct keys.",
					"Mutable ObjectMap values need shared identity and stable pointer-key equality. Exact generated callbacks keep every key and value unboxed and precisely traced.",
					"docs/hxrt.md",
					[
						"test/differential/object-enum-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(enumValueMap, "Recursive enum-value Map keys over the shared exact typed-map runtime.", CompilerSelectable, true,
				environments, [typedMap], [], [], [], [],
				documentation("Compares enum constructors and payloads recursively, using identity for class payloads, while tracing occupied keys and exact values strongly.",
					[
						new RuntimeFeatureSelectionRoot("enum-value-map-operation", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe EnumValueMap construction, copy, lookup, insertion, removal, clear, or iterator operation.")
					],
					"A compiler-known immutable enum lookup can remain direct when no shared mutable map value is observable.",
					"A bounded program-local specialization is allowed only when recursive payload equality and class-payload identity remain exact.",
					"Mutable EnumValueMap values need shared identity and equality that follows the active constructor. Generated callbacks keep that policy typed and unboxed.",
					"docs/hxrt.md",
					[
						"test/differential/object-enum-map/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(bytes, "Fixed-length mutable binary storage with checked ranges and shared Haxe identity.", CompilerSelectable, true,
				environments, [alloc, stringLiteral], [header("bytes.h"), source("bytes.c")], [
					"hxc_bytes_ref_create_zeroed",
					"hxc_bytes_ref_create_copy",
					"hxc_bytes_ref_create_utf8_copy",
					"hxc_bytes_ref_is_valid",
					"hxc_bytes_ref_retain",
					"hxc_bytes_ref_release",
					"hxc_bytes_ref_release_slot",
					"hxc_bytes_ref_length",
					"hxc_bytes_ref_get",
					"hxc_bytes_ref_set",
					"hxc_bytes_ref_sub",
					"hxc_bytes_ref_blit",
					"hxc_bytes_ref_fill",
					"hxc_bytes_ref_compare",
					"hxc_bytes_ref_borrow_mutable_cstring"
				],
				[], [],
				documentation("Implements exact-length arbitrary byte buffers, alias-visible mutation, checked copying, explicit UTF-8 String-to-bytes copying, and a validated one-call mutable C-string borrow.",
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
			new RuntimeFeatureDefinition(bytesString, "Checked UTF-8 decoding from mutable Bytes into an independently owned Haxe String.",
				CompilerSelectable, true, environments, [bytes, string], [header("bytes_string.h"), source("bytes_string.c")],
				["hxc_bytes_ref_get_string_utf8"], [], [],
				documentation("Copies one checked Bytes range into a fresh immutable String only after the range is valid UTF-8.", [
					new RuntimeFeatureSelectionRoot("get-string-utf8", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Bytes.getString or Bytes.toString interprets run-time binary storage as UTF-8.")
				],
					"Compiler-known immutable valid UTF-8 bytes may become a direct literal-backed String when mutation and identity are unobservable.",
					"A closed fixed-capacity buffer may use a program-local checked decoder when that preserves bounds, malformed-input failure, and String ownership.",
					"General Bytes and String values have separate owners. This composition validates the selected bytes, copies them once, and publishes a String whose lifetime no longer depends on later buffer mutation.",
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
					"hxc_gc_root_frame_pop_cleanup",
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
						new RuntimeFeatureSelectionRoot("type-carrier", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable stored Haxe String type whose generated C declaration needs the allocation-free carrier."),
						dependencyRoot("Selected transitively when a runtime operation consumes the compiler's direct literal representation.")
					],
					"The literal bytes and length are emitted directly in generated C; Haxe null uses a null data pointer, while every real String including the empty String has a non-null address.",
					"The carrier itself is already the program-specific representation; cloning its ABI would make runtime consumers incompatible.",
					"The header shares the value fields plus an opaque optional owner pointer. Literal-only programs keep that pointer null and still allocate nothing.",
					"docs/hxrt.md", ["test/string_output/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(stringScalar,
				"Allocation-free valid-UTF-8 validation, Unicode-scalar indexing, slicing, search, comparison, and hashing.", CompilerSelectable, true,
				environments, [status, stringLiteral], [header("string_scalar.h"), header("string_decode.h"), source("string_scalar.c")], [
					"hxc_utf8_validate",
					"hxc_string_is_valid",
					"hxc_string_scalar_length",
					"hxc_string_haxe_length",
					"hxc_string_scalar_at",
					"hxc_string_slice",
					"hxc_string_char_at",
					"hxc_string_char_code_at",
					"hxc_string_index_of",
					"hxc_string_last_index_of",
					"hxc_string_substr",
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
					new RuntimeFeatureSelectionRoot("index-of", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.indexOf whose receiver, needle, or start position is known only at run time."),
					new RuntimeFeatureSelectionRoot("last-index-of", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.lastIndexOf whose receiver, needle, or start position is known only at run time."),
					new RuntimeFeatureSelectionRoot("substr", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.substr whose bounds are known only at run time."),
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
					"hxc_string_release_slot",
					"hxc_string_from_scalar",
					"hxc_string_from_int32",
					"hxc_string_concat_ref",
					"hxc_byte_view_from_cstring",
					"hxc_string_from_utf8_checked",
					"hxc_string_from_utf8_lossy",
					"hxc_string_copy",
					"hxc_string_copy_ref",
					"hxc_string_concat",
					"hxc_owned_string_dispose",
					"hxc_string_buffer_init",
					"hxc_string_buffer_view",
					"hxc_string_buffer_append_utf8_checked",
					"hxc_string_buffer_append_scalar",
					"hxc_string_buffer_finish",
					"hxc_string_buffer_finish_ref",
					"hxc_string_buffer_dispose",
					"hxc_string_borrow_cstring",
					"hxc_string_prepare_call_cstring",
					"hxc_call_cstring_dispose",
					"hxc_string_to_cstring_owned",
					"hxc_owned_cstring_dispose"
				],
				[], [],
				documentation("Adds reference-counted ordinary Haxe String values, explicit owned strings, builders, lossy decoding, concatenation, and CString conversion above the shared scalar slice.",
					[
						new RuntimeFeatureSelectionRoot("from-scalar", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe String.fromCharCode whose scalar is known only at run time."),
						new RuntimeFeatureSelectionRoot("from-int", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Std.string(Int) whose decimal spelling depends on a run-time value."),
						new RuntimeFeatureSelectionRoot("concat", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable ordinary Haxe String concatenation whose result bytes are not compile-time constants."),
						new RuntimeFeatureSelectionRoot("borrow-cstring", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable direct C import borrows one validated Haxe String as immutable NUL-terminated text only until that call returns."),
						new RuntimeFeatureSelectionRoot("prepare-cstring", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable direct C import borrows already terminated text or creates one bounded temporary copy for an interior String view."),
						new RuntimeFeatureSelectionRoot("dispose-cstring", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A prepared C-string argument releases its optional temporary storage immediately after the direct native call returns."),
						new RuntimeFeatureSelectionRoot("retain", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable managed String copy must keep its optional backing owner alive."),
						new RuntimeFeatureSelectionRoot("cleanup-release", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable managed String owner must be released when its Haxe lifetime ends."),
						new RuntimeFeatureSelectionRoot("type-carrier", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable stored managed String type whose generated C declaration needs the owner-capable carrier.")
					],
					"Known literals, constant concatenation, and statically decidable string facts should remain direct compiler-owned C data.",
					"Closed call sites should receive specialized local operations when their representation, lifetime, and operation set are statically bounded.",
					"Runtime-created UTF-8 bytes can cross ordinary calls and container boundaries, so their allocator identity and final release need one shared owner contract.",
					"docs/hxrt.md",
					[
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringLowerCase, "Locale-independent Haxe Eval simple lowercase conversion into a fresh managed String.",
				CompilerSelectable, true, environments, [string], [
					header("string_lower_case.h"),
					header("string_lower_case_data.h"),
					source("string_lower_case.c")
				],
				["hxc_string_to_lower_case"], [], [],
				documentation("Maps each valid UTF-8 scalar through the pinned Haxe Eval lowercase table and publishes one fresh managed String owner.", [
					new RuntimeFeatureSelectionRoot("to-lower-case", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.toLowerCase depends on run-time source bytes."),
					dependencyRoot("Selected only by a broader feature that requires Eval-compatible lowercase conversion.")
				],
					"A compiler-known literal may fold only when compile-time mapping uses the same generated table and preserves the same fresh-value semantics where observable.",
					"A closed ASCII-only protocol may use a smaller program-local normalizer when its admitted alphabet is statically proven and is not exposed as ordinary Haxe String.toLowerCase.",
					"General String values need one locale-independent mapping table, checked UTF-8 decoding, size-changing encoding, failure-atomic allocation, and exact result ownership. A separate feature keeps that table out of unrelated String programs.",
					"docs/string-runtime.md",
					[
						"test/differential/string-runtime/GenerateLowercaseData.hx",
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringFloat, "Hosted Haxe-compatible Float formatting into an owned String.", CompilerSelectable, true,
				[CEnvironment.Hosted], [string], [header("string_float.h"), source("string_float.c")], ["hxc_string_from_float64"], [], [],
				documentation("Formats one binary64 Haxe Float with the first 12-, 15-, or 18-digit decimal spelling that round-trips exactly, including Haxe's non-finite spellings.",
					[
						new RuntimeFeatureSelectionRoot("from-float", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Std.string(Float) whose spelling depends on a run-time value.")
					],
					"A compiler-known Float can become a direct compiler-owned String only when constant folding uses the same reviewed spelling oracle.",
					"Duplicating host formatting and locale normalization at each call site would multiply policy and failure handling.",
					"Correct shortest-enough round-trip text needs hosted decimal formatting and parsing. A separate feature keeps ordinary String programs freestanding-safe.",
					"docs/string-runtime.md",
					[
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringSplit, "Unicode-scalar String splitting into an element-specialized managed Array<String>.",
				CompilerSelectable, true, environments, [array, string], [header("string_split.h"), source("string_split.c")], ["hxc_string_split"], [], [],
				documentation("Composes immutable String slices with the compiler-generated Array<String> lifecycle callbacks.", [
					new RuntimeFeatureSelectionRoot("split", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe String.split has runtime-dependent input or delimiter bytes.")
				],
					"A compiler-known source and delimiter may fold to a direct literal Array when bounded compile-time work is cheaper.",
					"A closed program may specialize a local splitter when its delimiter and storage limits prove a smaller implementation.",
					"Runtime-sized results need checked Array growth, partial-failure rollback, Unicode-scalar boundaries, and retained String owners. Keeping this composition separate prevents unrelated String programs from selecting Array support.",
					"docs/string-runtime.md",
					[
						"test/differential/string-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(arrayJoin, "Linear managed Array<String> joining through one failure-atomic UTF-8 builder.", CompilerSelectable,
				true, environments, [array, string], [header("array_join.h"), source("array_join.c")], ["hxc_array_string_join"], [], [],
				documentation("Composes one exact managed Array<String> and immutable separator into a fresh managed String without repeated concatenation.", [
					new RuntimeFeatureSelectionRoot("join", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"A reachable ordinary Haxe Array<String>.join has runtime-dependent elements or separator bytes.")
				],
					"A compiler-known bounded literal Array and separator may fold directly when compile-time work and output size remain reasonable.",
					"A closed fixed-capacity program may use a smaller program-local joiner when whole-program bounds prove it sufficient.",
					"Runtime-sized Array contents need checked growth, ordered separator insertion, one final String owner, and partial-allocation cleanup. A separate composition feature keeps ordinary Array and String programs independent.",
					"docs/hxrt.md",
					[
						"test/differential/array-runtime/array_join_runtime.c",
						"test/differential/array-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(dateTime, "Hosted wall-clock, monotonic-clock, local-calendar, and timezone adapters.", CompilerSelectable, true,
				[CEnvironment.Hosted], [status], [header("date_time.h"), source("date_time.c")], [
					"hxc_date_time_wall_milliseconds",
					"hxc_date_time_monotonic_seconds",
					"hxc_date_time_local_to_milliseconds",
					"hxc_date_time_timezone_offset"
				],
				[], [],
				documentation("Reads wall and monotonic clocks separately, converts local civil fields through host timezone rules, and reports Haxe-sign timezone offsets with checked time_t range conversion.",
					[
						new RuntimeFeatureSelectionRoot("wall-clock", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Date.now call needs the host's adjustable Unix-epoch clock."),
						new RuntimeFeatureSelectionRoot("monotonic-clock", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Timer.stamp call needs elapsed time that cannot move backwards with wall-clock adjustments."),
						new RuntimeFeatureSelectionRoot("local-calendar", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable Date constructor needs host timezone and daylight-saving normalization."),
						new RuntimeFeatureSelectionRoot("timezone-offset", RuntimeFeatureSelectionRootKind.HxcIrOperation,
							"A reachable local Date projection needs the host offset at that exact timestamp.")
					],
					"UTC Date projection remains deterministic program-local Gregorian arithmetic and Date.fromTime needs only ordinary object allocation.",
					"A platform with a closed embedded calendar can replace these calls with a program-local adapter that preserves the same status and clock-kind contracts.",
					"Timezone databases, daylight-saving normalization, and clocks are host services. One narrow status/out boundary keeps those effects separate from portable Date arithmetic.",
					"docs/date-time.md", ["test/date_time/run.py", "runtime/hxrt/test/date_time_contract.c"])),
			new RuntimeFeatureDefinition(io, "Minimal hosted length-delimited String output with explicit write and flush failure status.",
				CompilerSelectable, true, [CEnvironment.Hosted], [status, stringLiteral], [header("io.h"), source("io.c")], ["hxc_io_println"], [], [],
				documentation("Writes one valid length-delimited Haxe String value plus a newline to hosted stdout and reports write or flush failure explicitly.",
					[
					new RuntimeFeatureSelectionRoot("sys-println-literal", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"Reachable Sys.println with a compiler-owned String literal."),
					new RuntimeFeatureSelectionRoot("sys-println-string", RuntimeFeatureSelectionRootKind.HxcIrOperation,
						"Reachable Sys.println with a statically typed runtime String value."),
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
			case "array.h": "b647fc5be70b1ddca8c0da723732cacf5d52ac43f061671ab4483ab8a677d96b";
			case "array_join.h": "5829a159dab0bd3446b5bc418c2ee32ad2902c0fec6bcc04f82efeb66c294fea";
			case "base.h": "7d4f67124bf94b76bfc24d5db973426f48f3f9f37daeae975fd4948f5b1dea25";
			case "bytes.h": "dc9f59ab163486e2fc06f988cd931065eda3f480dfadae6917ee08ab60e9a4f5";
			case "bytes_string.h": "9d944e38a748696628076b0c5fd56339668e48953a220d51c8da1630fbdf9c40";
			case "date_time.h": "07086c9185ea03a13dc6bf39d02f00f99b7cbd8151ba0bdf90d7e457c07880d4";
			case "dynamic.h": "6acbca9069ce4670988e682c5c214a32968fadee892ea4490d0844674c2e24b2";
			case "exception.h": "7672ed148ac81df19bd4461ecbf94901f176e474a92100dbda7a6e33af6b5713";
			case "gc.h": "d99575a5bad765d45822a1d6221f7bc1b620d59dd6111e0c8ec8a2d45db36159";
			case "io.h": "4b92f03451dc4d04ea74c857ca3ce54d52fbe80d31f155b93781ee2fab946589";
			case "int_map.h": "11213ebbb4fccb5620a4e949ec4a0852a512c7be8d1f5750d987f56aca71cd7f";
			case "iterator.h": "10ec767355e93a45e214b4774435a46496dd2c601f285412bc072af7904a3e51";
			case "object.h": "779b452097e4c58c7971b90743ace19a2dc6c91e381557abc84fbd5f9b30f1e5";
			case "status.h": "6bf20f5d82594014ad0f2b79a25cb81417791bd9c07375d2fb89835e415be1c4";
			case "status_name.h": "64bf3917787ffcf924369c8e1c0a525cf10902d004d5bb4b898f2af46a7456cc";
			case "string.h": "fe4b3130433bf6b64d27da5b1acf0bb477763acce0644a7882c15bb67226d77f";
			case "string_lower_case.h": "c2fb77f0f59ba1b8804e308ca769c75fac2fde81a6faf52056424c8f6c7e490a";
			case "string_lower_case_data.h": "b069c988dec0cd7f7cfc5b116ec0c534136f022d80c71684efd9294290ea9961";
			case "string_decode.h": "aa93ea7f132aff625adfdcc7498532b139f621196deab4c0e9ecb5de2934fd48";
			case "string_float.h": "8747a86c3cabae9bf54a4125305f043d6c70d7c97bc9f6f90174ba6185e3ecc1";
			case "string_literal.h": "ac6b5ad9fa13004c62e3b33b9b28a935bfb8a22287cd4595ce6e6eb81490e283";
			case "string_map.h": "a12868bdfbc4b5420b2930bc920fc45e6e69ead5b72be130c54919c54ba0c042";
			case "string_scalar.h": "b400d7ef9af853410334b30627ea98a5af87d5c3a863f6aa4c770d7cc4b3d90b";
			case "string_split.h": "a17c9cd6c31cfdb8da2cf4955b980090c144e68ee1ae4f1d0f0b543f4b6eb3eb";
			case "typed_map.h": "8e0838bbf09921bf4167fc85e55d99a762cd208b069a41008e645b5e07949f11";
			case _: throw 'runtime feature header `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function sourceSha256(name:String):String {
		return switch name {
			case "abi.c": "3300a4498a7ca20f771b1334d7be8f2c908d2bb067ea8f2fe3c059300e680b32";
			case "allocator.c": "13385273c7c3d4a15785caa3095dd82d97bda8a026ebd9b6d54e2f531eb3b10e";
			case "array.c": "c5fec3dcf78ed27dcd38219efec8289dd38a23eeef352785396bde64fa99c98a";
			case "array_join.c": "b158708b62c7e407f9da21c24a1b3306d4b41baa6b63f2d8019f631a98008fde";
			case "bytes.c": "10a4c6c17d1cedc31562fef6708fd54351e094ec4be631848d6348bb82ced46c";
			case "bytes_string.c": "0ee9604f1b4ae78baeeaf7cac8b2a35b5634f115c958a7575230c790e8aa6ca6";
			case "date_time.c": "546e3f244d3187993254aca85dd4acfd64bd3c259b1531d736c950ce9de51b24";
			case "dynamic.c": "804371b7eb2bfa6dbcb6598ff729754b312a2ba7b24a94a615915c30dee68503";
			case "exception.c": "e6660d0b55b56be3cd436af8f0c16a7668b70f7e82031687a5a149e029c12741";
			case "gc.c": "a79c93c94db215b3bc303ea4c761de627637d0eb881faeeaf10c07f9bed4c502";
			case "io.c": "898b3f351b60a91f25fd1ffdfe8d832e95a5a6a738ffe226ac33581f1fcb5b0f";
			case "int_map.c": "1b9a0cf4a376e2c2afe7cb79457f50557d50274e8187239e9e4ae80a5a8108cf";
			case "iterator.c": "a4f3da3f7e3a3fb5ad2f24497b00d77eae1b11c600167b8f5553b656623dba6b";
			case "object.c": "0e7fc6a55b562eaaf03fe63eca743dd73248f0bee1c09e21b79464917e8c89c0";
			case "status.c": "0695ab2528db6e29d5cf29d905ad736b7c1a3a79333082347ec18faea2d4e6d8";
			case "string.c": "07fb06813ca533677bf00f8580f874bd3c05a41a5bd1b647f45598b0d3e3c8c1";
			case "string_lower_case.c": "55a692cfd855f71f1a1fa4f90f311f1653ec0638797ecfb024764e23a66680c8";
			case "string_float.c": "60e5189e7f7304ccbc1f69136b7393e4eea35760cde590853ebced414bf39267";
			case "string_map.c": "c637ffdce4e990fe7436f88e7445376722ee0b28dec10cf57db860a5120706e9";
			case "string_scalar.c": "2c44eebc655dd34ed374b58402de9dfe731425fb4e0b54997a7c16c12e1309fb";
			case "string_split.c": "799fc917a450169e4babd86748e879fe7222b4abfef293880c47891e671f9d1b";
			case "typed_map.c": "2a890d9f7a66a2faaffff43eee084fcf704138442f5915e8e728acbcad6fbcff";
			case _: throw 'runtime feature source `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function reserved(id:String, ownerTask:String, summary:String):RuntimeFeatureReservation
		return new RuntimeFeatureReservation(RuntimeFeatureId.parse(id), ownerTask, summary);
}
