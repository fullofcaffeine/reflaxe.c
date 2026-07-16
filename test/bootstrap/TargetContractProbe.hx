#if macro
import haxe.Json;
import haxe.io.Path;
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.PlatformConfig;
import haxe.macro.Type;

private typedef TargetPlatformSnapshot = {
	final staticTypeSystem:Null<Bool>;
	final sys:Null<Bool>;
	final capturePolicy:Null<String>;
	final usesUtf16:Null<Bool>;
	final supportsUnicode:Null<Bool>;
	final supportsThreads:Null<Bool>;
	final supportsAtomics:Null<Bool>;
}

/** Emits a structural snapshot of target facts and selected upstream branches. */
class TargetContractProbe {
	public static inline final REPORT_PREFIX = "HXC_TARGET_CONTRACT=";
	static var reported = false;

	public static function install():Void {
		Context.onAfterTyping(_ -> {
			if (reported) {
				return;
			}
			reported = true;
			final configuration = Compiler.getConfiguration();
			final platform = configuration == null ? null : configuration.platformConfig;
			final report = {
				schemaVersion: 1,
				haxeVersion: Context.definedValue("haxe"),
				carrier: Context.definedValue("reflaxe_c_haxe_carrier"),
				environment: Context.definedValue("reflaxe_c_platform_environment"),
				platform: platformSnapshot(platform),
				defines: {
					bootstrapCount: Context.definedValue("reflaxe_c_bootstrap_count"),
					initCount: Context.definedValue("reflaxe_c_init_count"),
					cOutput: Context.defined("c_output"),
					c: Context.defined("c"),
					reflaxeC: Context.defined("reflaxe_c"),
					targetName: Context.definedValue("target.name"),
					customTarget: Context.defined("custom_target"),
					targetStatic: Context.defined("target.static"),
					targetUnicode: Context.defined("target.unicode"),
					targetUtf16: Context.defined("target.utf16"),
					utf16: Context.defined("utf16"),
					targetSys: Context.defined("target.sys"),
					targetThreaded: Context.defined("target.threaded"),
					targetAtomics: Context.defined("target.atomics")
				},
				stdlib: {
					stringSource: typeSource("String"),
					stringToolsSource: typeSource("StringTools"),
					unicodeStringSource: typeSource("UnicodeString"),
					stringToolsUtf16Helpers: classHasStatic("StringTools", "utf16CodePointAt"),
					unicodeStringUtf16Adapters: abstractImplHasStatic("UnicodeString", "get_length")
				}
			};
			Sys.println(REPORT_PREFIX + Json.stringify(report));
		});
	}

	static function platformSnapshot(platform:Null<PlatformConfig>):TargetPlatformSnapshot {
		if (platform == null) {
			return {
				staticTypeSystem: null,
				sys: null,
				capturePolicy: null,
				usesUtf16: null,
				supportsUnicode: null,
				supportsThreads: null,
				supportsAtomics: null
			};
		}
		return {
			staticTypeSystem: platform.staticTypeSystem,
			sys: platform.sys,
			capturePolicy: Std.string(platform.capturePolicy),
			usesUtf16: platform.usesUtf16,
			supportsUnicode: platform.supportsUnicode,
			supportsThreads: platform.supportsThreads,
			supportsAtomics: platform.supportsAtomics
		};
	}

	static function classHasStatic(typeName:String, fieldName:String):Bool {
		return switch (Context.getType(typeName)) {
			case TInst(reference, _):
				hasField(reference.get().statics.get(), fieldName);
			case _:
				false;
		};
	}

	static function abstractImplHasStatic(typeName:String, fieldName:String):Bool {
		return switch (Context.getType(typeName)) {
			case TAbstract(reference, _): final implementation = reference.get()
					.impl; implementation != null && hasField(implementation.get().statics.get(), fieldName);
			case _:
				false;
		};
	}

	static function hasField(fields:Array<ClassField>, fieldName:String):Bool {
		for (field in fields) {
			if (field.name == fieldName) {
				return true;
			}
		}
		return false;
	}

	static function typeSource(typeName:String):String {
		final position = switch (Context.getType(typeName)) {
			case TInst(reference, _):
				reference.get().pos;
			case TEnum(reference, _):
				reference.get().pos;
			case TType(reference, _):
				reference.get().pos;
			case TAbstract(reference, _):
				reference.get().pos;
			case _:
				Context.currentPos();
		};
		final file = Path.normalize(Context.getPosInfos(position).file);
		final configuration = Compiler.getConfiguration();
		if (configuration != null) {
			for (standardLibraryRoot in configuration.stdPath) {
				final root = Path.addTrailingSlash(Path.normalize(standardLibraryRoot));
				if (StringTools.startsWith(file, root)) {
					return "haxe-" + Context.definedValue("haxe") + "-std/" + file.substr(root.length);
				}
			}
		}
		return Path.withoutDirectory(file);
	}
}
#end
