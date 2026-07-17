package reflaxe.c.runtime;

import reflaxe.c.CEnvironment;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureArtifact;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureReservation;

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
		final stringLiteral = RuntimeFeatureId.parse("string-literal");
		final string = RuntimeFeatureId.parse("string");
		final io = RuntimeFeatureId.parse("io");
		return [
			new RuntimeFeatureDefinition(runtimeBase, "Shared C types and visibility/alignment macros for selectively packaged runtime slices.",
				CompilerSelectable, true, environments, [], [header("base.h")], [], [], []),
			new RuntimeFeatureDefinition(runtimeAbi, "Provisional runtime ABI version query used only by native seed evidence.", NativeSeedOnly, true,
				environments, [runtimeBase], [header("abi.h"), source("abi.c")], ["hxc_runtime_abi_version"], [], []),
			new RuntimeFeatureDefinition(status, "Status value definitions used by selected runtime failure boundaries.", CompilerSelectable, true,
				environments, [runtimeBase], [header("status.h")], [], [], []),
			new RuntimeFeatureDefinition(statusName, "Symbolic status-name helper used only by native seed evidence.", NativeSeedOnly, true, environments,
				[status], [header("status_name.h"), source("status.c")], ["hxc_status_name"], [], []),
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
				[], []),
			new RuntimeFeatureDefinition(stringLiteral,
				"Immutable valid UTF-8 literal carrier with explicit byte length and no allocation or object dependency.", CompilerSelectable, true,
				environments, [runtimeBase], [header("string_literal.h")], [], [], []),
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
				[], []),
			new RuntimeFeatureDefinition(io, "Minimal hosted length-delimited literal output with explicit write and flush failure status.",
				CompilerSelectable, true, [CEnvironment.Hosted], [status, stringLiteral], [header("io.h"), source("io.c")], ["hxc_io_println"], [], [])
		];
	}

	public static function reservations():Array<RuntimeFeatureReservation> {
		return [
			reserved("array", "E4.T04", "Resizable typed Haxe Array runtime slice."),
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
		return new RuntimeFeatureArtifact('runtime/hxrt/include/hxrt/$name', 'runtime/include/hxrt/$name', GeneratedFileKind.RuntimeHeader);

	static function source(name:String):RuntimeFeatureArtifact
		return new RuntimeFeatureArtifact('runtime/hxrt/src/$name', 'runtime/src/$name', GeneratedFileKind.RuntimeSource);

	static function reserved(id:String, ownerTask:String, summary:String):RuntimeFeatureReservation
		return new RuntimeFeatureReservation(RuntimeFeatureId.parse(id), ownerTask, summary);
}
