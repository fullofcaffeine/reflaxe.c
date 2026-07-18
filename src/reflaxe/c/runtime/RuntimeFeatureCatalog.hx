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
		final stringLiteral = RuntimeFeatureId.parse("string-literal");
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
			new RuntimeFeatureDefinition(alloc, "Hardened allocator ownership and failure contracts with hosted and custom native evidence.", NativeSeedOnly,
				true, environments, [status], [header("allocator.h"), source("allocator.c")], [
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
						nativeSeedRoot("Requested only by allocator, array, and string native seed fixtures; generated Haxe cannot select it yet.")
					],
					"Stack storage, fixed arrays, nonescaping spans, and bounded values remain direct C and never request allocation.",
					"A program-local allocator is preferred when whole-program escape and size facts permit a narrower specialized owner.",
					"Unknown runtime sizes and caller-supplied allocator identity need one shared ownership and failure contract, but compiler lowering for that case is still unsupported.",
					"docs/hxrt.md",
					[
						"scripts/ci/runtime_smoke.py",
						"runtime/hxrt/test/allocator_contract.c",
						"runtime/hxrt/test/allocator_abi.c"
					])),
			new RuntimeFeatureDefinition(array, "Resizable contiguous unboxed storage with checked growth and optional typed element lifecycle callbacks.",
				NativeSeedOnly, true, environments, [alloc], [header("array.h"), source("array.c")], [
					"hxc_array_element_ops_is_valid",
					"hxc_array_init",
					"hxc_array_is_valid",
					"hxc_array_reserve",
					"hxc_array_resize",
					"hxc_array_at",
					"hxc_array_at_const",
					"hxc_array_push_copy",
					"hxc_array_insert_copy",
					"hxc_array_set_copy",
					"hxc_array_remove_at",
					"hxc_array_move",
					"hxc_array_dispose"
				],
				[], [],
				documentation("Implements a move-only resizable unboxed buffer with deterministic checked growth, borrow invalidation, and optional typed element lifecycle callbacks.",
					[
						nativeSeedRoot("Requested only by the array differential and selective-package native fixtures; generated Haxe cannot select it yet.")
					],
					"Compiler-known fixed arrays and nonescaping spans use direct C storage and bounds checks.",
					"Closed element types and bounded capacities should use a program-local specialized helper when that is smaller and equally correct.",
					"Runtime-dependent capacity plus reusable allocator and element-lifecycle boundaries justify this shared seed only after future array lowering proves the need.",
					"docs/hxrt.md",
					[
						"test/differential/array-runtime/run.py",
						"test/runtime/runtime-feature-graph/run.py"
					])),
			new RuntimeFeatureDefinition(stringLiteral,
				"Immutable valid UTF-8 literal carrier with explicit byte length and no allocation or object dependency.", CompilerSelectable, true,
				environments, [runtimeBase], [header("string_literal.h")], [], [], [],
				documentation("Defines the allocation-free private hxc_string view used for compiler-owned valid UTF-8 literal storage, including embedded NUL bytes.",
					[
					dependencyRoot("Selected transitively when a runtime operation consumes the compiler's direct literal representation.")
				],
					"The literal bytes and length are emitted directly in generated C; no runtime source or constructor is used.",
					"The carrier itself is already the program-specific representation; cloning its ABI would make runtime consumers incompatible.",
					"The header only shares the three-field call-boundary layout needed by selected consumers such as io.", "docs/hxrt.md",
					["test/string_output/run.py", "test/runtime/runtime-feature-graph/run.py"])),
			new RuntimeFeatureDefinition(string,
				"Valid UTF-8 scalar primitives, owned construction, mutable building, and explicit CString conversion with native evidence.", NativeSeedOnly,
				true, environments, [alloc, stringLiteral], [header("string.h"), source("string.c")], [
					"hxc_byte_view_from_cstring",
					"hxc_utf8_validate",
					"hxc_string_is_valid",
					"hxc_string_from_utf8_checked",
					"hxc_string_from_utf8_lossy",
					"hxc_string_copy",
					"hxc_string_concat",
					"hxc_owned_string_dispose",
					"hxc_string_scalar_length",
					"hxc_string_scalar_at",
					"hxc_string_slice",
					"hxc_string_compare",
					"hxc_string_hash",
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
				documentation("Implements runtime-value-dependent scalar UTF-8 validation/indexing, owned strings, builders, slicing, hashing, and explicit borrowed or owned CString conversion.",
					[
						nativeSeedRoot("Requested only by the string differential and selective-package native fixtures; generated Haxe cannot select it yet.")
					],
					"Known literals, constant concatenation, and statically decidable string facts should remain direct compiler-owned C data.",
					"Closed call sites should receive specialized local operations when their representation, lifetime, and operation set are statically bounded.",
					"Arbitrary runtime bytes, scalar indexing, ownership transfer, builders, and CString lifetimes need shared validated machinery when future lowering admits them.",
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
			reserved("gc", "E4.T06", "Precise non-moving tracing collector and exact roots."),
			reserved("object", "E4.T05", "Reachable object and type descriptors."),
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
			case "allocator.h": "5f01b19f66588f5c778ea5035aaaba0e2d5a48ff027c5810e32d20bb601d696a";
			case "array.h": "65ec3d1a86705030e42890de9af2b4b96a30062f0f0a8d127280fec693a02c7d";
			case "base.h": "5727cd5798673e27f2215e7a3e1e2a1f92245510855903075caa71764181456d";
			case "io.h": "4670078a26fb991c5de1f32ba3ab2c20cdc5e1d1b578dfe2504efe2b7e2f7d2e";
			case "status.h": "6bf20f5d82594014ad0f2b79a25cb81417791bd9c07375d2fb89835e415be1c4";
			case "status_name.h": "64bf3917787ffcf924369c8e1c0a525cf10902d004d5bb4b898f2af46a7456cc";
			case "string.h": "16860609c4cdb6e8e81f3b02f212edb40be3da01f053743cc087b897df16ba63";
			case "string_literal.h": "0c9c2b70aa847b7e8a6f2a3fcca18e11bdafd03340a4955527446b3a47388e36";
			case _: throw 'runtime feature header `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function sourceSha256(name:String):String {
		return switch name {
			case "abi.c": "3300a4498a7ca20f771b1334d7be8f2c908d2bb067ea8f2fe3c059300e680b32";
			case "allocator.c": "1f93b7f5724bf77b05204e20d96f9179582e070210bad2ec9dd7b89794e8a77c";
			case "array.c": "85e958919867732af6371afb49983fe607335981f7cbaa893fdf6ca5b806c825";
			case "io.c": "c390615feea7f81c404941412909037ead8eb0ee1d3163d17f14154c20968e1c";
			case "status.c": "0695ab2528db6e29d5cf29d905ad736b7c1a3a79333082347ec18faea2d4e6d8";
			case "string.c": "abe937db71f7333d4a915d7b7e222384ceeba8815783e8fafffb1aeaeadfd5d4";
			case _: throw 'runtime feature source `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function reserved(id:String, ownerTask:String, summary:String):RuntimeFeatureReservation
		return new RuntimeFeatureReservation(RuntimeFeatureId.parse(id), ownerTask, summary);
}
