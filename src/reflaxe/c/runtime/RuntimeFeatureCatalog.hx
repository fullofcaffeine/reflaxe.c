package reflaxe.c.runtime;

import reflaxe.c.CEnvironment;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureArtifact;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureReservation;

/** Checked-in feature vocabulary; provisional slices are never compiler-selectable. */
class RuntimeFeatureCatalog {
	public static function registry():RuntimeFeatureRegistry
		return new RuntimeFeatureRegistry(definitions(), reservations());

	public static function definitions():Array<RuntimeFeatureDefinition> {
		final environments = [CEnvironment.Hosted, CEnvironment.Freestanding];
		final runtimeAbi = RuntimeFeatureId.parse("runtime-abi");
		final status = RuntimeFeatureId.parse("status");
		final alloc = RuntimeFeatureId.parse("alloc");
		final string = RuntimeFeatureId.parse("string");
		return [
			new RuntimeFeatureDefinition(runtimeAbi, "Provisional runtime ABI version query used only by native seed evidence.", NativeSeedOnly, true,
				environments, [], [header("base.h"), header("abi.h"), source("abi.c")], ["hxc_runtime_abi_version"], [], []),
			new RuntimeFeatureDefinition(status, "Provisional status values and symbolic names for native seed evidence.", NativeSeedOnly, true, environments,
				[runtimeAbi], [header("status.h"), source("status.c")], ["hxc_status_name"], [], []),
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
			new RuntimeFeatureDefinition(string, "Provisional byte-view and owned-copy string seed; full Haxe String semantics remain E4.T03.",
				NativeSeedOnly, true, environments, [alloc], [header("string.h"), source("string.c")],
				["hxc_string_view_from_cstr", "hxc_string_copy", "hxc_owned_string_dispose"], [], [])
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
			reserved("io", "E5.T04", "Byte streams and hosted or custom I/O adapters."),
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
