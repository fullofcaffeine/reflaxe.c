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
			new RuntimeFeatureDefinition(runtimeBase, "Shared C types, internal ABI version, and visibility/alignment macros for selected runtime slices.",
				CompilerSelectable, true, environments, [], [header("base.h")], [], [], []),
			new RuntimeFeatureDefinition(runtimeAbi, "Version query for the internal same-major runtime ABI, used only by native seed evidence.",
				NativeSeedOnly, true, environments, [runtimeBase], [header("abi.h"), source("abi.c")], ["hxc_runtime_abi_version"], [], []),
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
		return new RuntimeFeatureArtifact('runtime/hxrt/include/hxrt/$name', 'runtime/include/hxrt/$name', GeneratedFileKind.RuntimeHeader, headerSha256(name));

	static function source(name:String):RuntimeFeatureArtifact
		return new RuntimeFeatureArtifact('runtime/hxrt/src/$name', 'runtime/src/$name', GeneratedFileKind.RuntimeSource, sourceSha256(name));

	static function headerSha256(name:String):String {
		return switch name {
			case "abi.h": "3f68191447abf3968d89d4b142a46cc08bc9a2caf288d7c0f3dbfc4698eee598";
			case "allocator.h": "798393dd7eb85916cd48a7652ef970b6ca4417f29f21f6f751dbefdd006185d7";
			case "base.h": "60d7a3332144b7913c1a663d39c4ceaf1fcd7da1e251a27bf37b0b0332aaddbf";
			case "io.h": "a80144f8bed89a7c9646b3cab2836417769f3c1eae213554c5c3268e96b2d5ca";
			case "status.h": "51311f4276de1652b86b2b68d92f2aa35a578063c089bd2ca2a0f92353110aa7";
			case "status_name.h": "601316d4420a87aedcf34459b39ad7e4cc44563b28047560013e011210f5f723";
			case "string.h": "4c1ccca0ff064a778594491b4e5d9946d592e51cd11876c8b2ef97cdb888e770";
			case "string_literal.h": "1f24f1358fcfef03193d79f181b0499a295a53544a021bcf1063f2a36c12f1a4";
			case _: throw 'runtime feature header `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function sourceSha256(name:String):String {
		return switch name {
			case "abi.c": "84d79962524123be2840436c6e6c1756d79027be438fc6cee2350d26aea7957c";
			case "allocator.c": "0a2d502da004f22db6e3412f3aebe6958230342d8e8f7b7e791f64f3be2bbcf5";
			case "io.c": "66647082818392b58030adfa85e434525c2034d91234debe3dcb1a975c789866";
			case "status.c": "cb5869a6a6a172560da82488b56a38c2cd8bec916661c1622c5e3aec70f3432b";
			case "string.c": "355bb142d05e2a520b687bbb834bf084aa5f73e203fe17931dd780d5f71e31dd";
			case _: throw 'runtime feature source `$name` has no reviewed SHA-256 provenance';
		};
	}

	static function reserved(id:String, ownerTask:String, summary:String):RuntimeFeatureReservation
		return new RuntimeFeatureReservation(RuntimeFeatureId.parse(id), ownerTask, summary);
}
