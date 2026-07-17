package reflaxe.c.runtime;

import haxe.crypto.Sha256;
import haxe.io.Bytes;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeAbiCompatibility;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeAbiContractRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeAbiStability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeApplicationAbiStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeGeneratedAbiCheck;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePublicExportTypePolicy;

/** One internal runtime compatibility contract shared by headers, manifests, and generated C. */
class RuntimeAbiContract {
	public static inline final MAJOR = 0;
	public static inline final MINOR = 4;
	public static inline final PATCH = 0;
	public static inline final MAJOR_MACRO = "HXC_RUNTIME_ABI_MAJOR";
	public static inline final SOURCE_HASH_ALGORITHM = "sha256";
	public static inline final RUNTIME_C_STANDARD = "c11";
	public static inline final PUBLIC_HEADER_CPP_STANDARD = "c++17";

	static final FORBIDDEN_APPLICATION_EXPORT_TYPES = [
		"hxc_allocation",
		"hxc_allocator",
		"hxc_borrowed_cstring",
		"hxc_byte_view",
		"hxc_owned_cstring",
		"hxc_owned_string",
		"hxc_string",
		"hxc_string_buffer"
	];

	public static function snapshot(definitions:Array<RuntimeFeatureDefinition>):RuntimeAbiContractRecord {
		return {
			stability: RuntimeAbiStability.InternalVersioned,
			version: {major: MAJOR, minor: MINOR, patch: PATCH},
			generatedCodeCompatibility: RuntimeAbiCompatibility.SameMajor,
			generatedCodeCheck: RuntimeGeneratedAbiCheck.C11StaticAssert,
			runtimeMajorMacro: MAJOR_MACRO,
			publicBoundary: {
				applicationAbiStatus: RuntimeApplicationAbiStatus.Unsupported,
				exportTypePolicy: RuntimePublicExportTypePolicy.RuntimeStructsForbidden,
				forbiddenRuntimeTypes: FORBIDDEN_APPLICATION_EXPORT_TYPES.copy()
			},
			releaseProvenance: {
				sourceHashAlgorithm: SOURCE_HASH_ALGORITHM,
				sourceSetSha256: sourceSetSha256(definitions),
				runtimeCStandard: RUNTIME_C_STANDARD,
				publicHeaderCppStandard: PUBLIC_HEADER_CPP_STANDARD
			}
		};
	}

	static function sourceSetSha256(definitions:Array<RuntimeFeatureDefinition>):String {
		final entries:Array<String> = [];
		for (definition in definitions) {
			for (artifact in definition.artifacts) {
				entries.push('${artifact.sourcePath}\x00${artifact.sourceSha256}\n');
			}
		}
		entries.sort(compareUtf8);
		return Sha256.make(Bytes.ofString(entries.join(""))).toHex();
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final shared = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...shared) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}
