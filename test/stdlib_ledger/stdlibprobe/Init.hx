package stdlibprobe;

#if macro
import haxe.macro.Compiler;
import haxe.macro.PlatformConfig;
#end

/** Test-only target that exposes upstream capability declarations without a generator. */
@:noCompletion
class Init {
	#if macro
	public static function init():Void {
		final dynamicType = {pack: ["StdTypes"], name: "Dynamic"};
		final configuration:PlatformConfig = {
			staticTypeSystem: true,
			sys: true,
			capturePolicy: None,
			padNulls: false,
			addFinalReturn: false,
			overloadFunctions: false,
			reservedTypePaths: [],
			supportsFunctionEquality: true,
			usesUtf16: false,
			thisBeforeSuper: true,
			supportsThreads: true,
			supportsUnicode: true,
			supportsRestArgs: false,
			exceptions: {
				nativeThrows: [],
				nativeCatches: [],
				avoidWrapping: true,
				wildcardCatch: dynamicType,
				baseThrow: dynamicType
			},
			scoping: {
				scope: BlockScope,
				flags: []
			},
			supportsAtomics: true
		};
		Compiler.setPlatformConfiguration(configuration);
	}
	#else
	public static function init():Void {}
	#end
}
