#if macro
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.lowering.CBodyTypedMap.CBodyTypedMapRecognition;
import reflaxe.c.lowering.CBodyTypedMap.CBodyTypedMapFamily;
#end

/** A generic alias must preserve the map receiver's key policy. */
typedef RecognitionMapAlias<K, V> = haxe.Constraints.IMap<K, V>;

/** Independent object, interface, and enum keys distinguish map families. */
class RecognitionMapKey {
	public function new() {}
}

interface RecognitionMapInterface {}

enum RecognitionMapEnum {
	Key;
}

/**
	Test map recognition with compiler-owned declarations and a counted reference.
	The wrapper returns the genuine class unchanged; it measures redundant reads
	without inventing class metadata or bypassing Haxe's type bridge for key types.
**/
class MapRecognitionProbe {
	#if macro
	static function require(value:Bool, message:String):Void {
		if (!value)
			throw new haxe.Exception(message);
	}

	/** Check family, arity, alias and lazy contracts before ordinary lowering. */
	public static function check():Void {
		function reference(name:String):Ref<ClassType>
			return switch Context.getType(name) {
				case TInst(value, _): value;
				case _: throw new haxe.Exception('class required: $name');
			};
		final objectRef = reference("haxe.ds.ObjectMap");
		final enumRef = reference("haxe.ds.EnumValueMap");
		final interfaceRef = reference("haxe.Constraints.IMap");
		final ordinaryRef = reference("MapRecognitionProbe.RecognitionMapKey");
		final objectKey = Context.getType("MapRecognitionProbe.RecognitionMapKey");
		final enumKey = Context.getType("MapRecognitionProbe.RecognitionMapEnum");
		final intType = Context.getType("Int");
		final mapRef = switch Context.getType("haxe.ds.Map") {
			case TAbstract(value, _): value;
			case _: throw new haxe.Exception("Map abstract required");
		};
		function expect(name:String, type:Type, expected:Null<CBodyTypedMapFamily>):Void {
			require(CBodyTypedMapRecognition.familyForMapType(type) == expected, name);
		}
		function counted(name:String, source:Ref<ClassType>, parameters:Array<Type>, expected:Null<CBodyTypedMapFamily>):Void {
			var reads = 0;
			final counted:Ref<ClassType> = {
				get: () -> {
					reads++;
					return source.get();
				},
				toString: () -> source.toString()
			};
			expect(name, TInst(counted, parameters), expected);
			require(reads == 1, '$name expected one declaration read, got $reads');
		}
		counted("ordinary", ordinaryRef, [], null);
		counted("ObjectMap", objectRef, [objectKey, intType], CBTMObject);
		counted("EnumValueMap", enumRef, [enumKey, intType], CBTMEnumValue);
		counted("ObjectMap identity without arguments", objectRef, [], CBTMObject);
		counted("IMap object", interfaceRef, [objectKey, intType], CBTMObject);
		counted("IMap enum", interfaceRef, [enumKey, intType], CBTMEnumValue);
		counted("IMap wrong arity", interfaceRef, [], null);
		expect("String key", TInst(interfaceRef, [Context.getType("String"), intType]), null);
		expect("interface key", TInst(interfaceRef, [Context.getType("MapRecognitionProbe.RecognitionMapInterface"), intType]), null);
		expect("Map object", TAbstract(mapRef, [objectKey, intType]), CBTMObject);
		expect("Map enum", TAbstract(mapRef, [enumKey, intType]), CBTMEnumValue);
		expect("Map wrong arity", TAbstract(mapRef, []), null);
		var failedRead = false;
		final failing:Ref<ClassType> = {
			get: () -> {
				throw new haxe.Exception("map-ref-sentinel");
			},
			toString: () -> "failing"
		};
		try
			CBodyTypedMapRecognition.familyForMapType(TInst(failing, []))
		catch (error:haxe.Exception)
			failedRead = error.message == "map-ref-sentinel";
		require(failedRead, "first reference failure preserved");
		final alias = switch Context.getType("MapRecognitionProbe.RecognitionMapAlias") {
			case TType(value, _): value;
			case _: throw new haxe.Exception("alias required");
		};
		expect("generic alias", TType(alias, [objectKey, intType]), CBTMObject);
		final mono = Context.makeMonomorph();
		expect("unresolved", mono, null);
		require(Context.unify(mono, TInst(objectRef, [objectKey, intType])), "bind monomorph");
		expect("resolved", mono, CBTMObject);
		var lazyReads = 0;
		expect("lazy", TLazy(() -> {
			lazyReads++;
			return TInst(objectRef, []);
		}), CBTMObject);
		require(lazyReads == 1, "lazy resolution count");
		var sentinel = false;
		try
			CBodyTypedMapRecognition.familyForMapType(TLazy(() -> {
				throw new haxe.Exception("map-lazy-sentinel");
			}))
		catch (error:haxe.Exception)
			sentinel = error.message == "map-lazy-sentinel";
		require(sentinel, "lazy diagnostic preserved");
	}
	#end
}
