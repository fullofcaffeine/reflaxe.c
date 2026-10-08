package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueKind;
import reflaxe.c.lowering.CBodyAggregate.CBodyValueType;
import reflaxe.c.lowering.CBodyAggregate.CBodyAggregateRegistry;
import reflaxe.c.lowering.CBodyEmissionError;
import reflaxe.c.lowering.CBodyStringMap.CBodyStringMapRegistry;
import reflaxe.c.lowering.CBodyIterator.CPreparedBodyIterator;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;

/**
	Owns exact ObjectMap identity and EnumValueMap recursive value semantics.

	Both families share checked table mechanics, but never key policy. Object keys
	compare stable collector pointers. Enum keys compare their active constructor
	and admitted payloads recursively, with class payloads compared by identity.
	The map remains a typed collector object; keys and values are not boxed.
**/
class CBodyTypedMap {
	private function new() {}
}

/** The source-level key policy retained by one typed-map specialization. */
enum CBodyTypedMapFamily {
	CBTMObject;
	CBTMEnumValue;

	/** UTF-8 String equality with collector-bearing record values. */
	CBTMString;
}

/** Exact copy/destroy callbacks shared by map slots and their iterator snapshots. */
typedef CPreparedMapLifetime = {
	final value:CBodyValueType;
	final copy:CSymbolRequest;
	final destroy:CSymbolRequest;
}

/** Final identifiers retain the same exact value type as the prepared policy. */
typedef CLoweredMapLifetime = {
	final value:CBodyValueType;
	final copy:CIdentifier;
	final destroy:CIdentifier;
}

/** Maps a key or value through the ordinary exact body-value boundary. */
typedef CBodyTypedMapValueResolver = (Type, Position, String, String, (Position, String) -> Void, String) -> CBodyValueType;

/** One exact collector-owned map specialization before C names are finalized. */
class CPreparedBodyTypedMap {
	public final family:CBodyTypedMapFamily;
	public final semanticKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final key:CBodyValueType;
	public final value:CBodyValueType;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final position:Position;
	public final hashRequest:CSymbolRequest;
	public final equalRequest:CSymbolRequest;
	public final keyTraceRequest:Null<CSymbolRequest>;
	public final valueTraceRequest:Null<CSymbolRequest>;

	/** Registered before symbol finalization; absent for existing scalar map families. */
	public final lifetimes:Array<CPreparedMapLifetime> = [];

	/** Preserve the complete typed policy selected by the registry. */
	public function new(family:CBodyTypedMapFamily, semanticKey:String, digest:String, key:CBodyValueType, value:CBodyValueType, ownerModule:String,
			source:HxcSourceSpan, position:Position, hashRequest:CSymbolRequest, equalRequest:CSymbolRequest, keyTraceRequest:Null<CSymbolRequest>,
			valueTraceRequest:Null<CSymbolRequest>) {
		this.family = family;
		this.semanticKey = semanticKey;
		this.digest = digest;
		this.declarationId = 'type.haxe-${featureId()}.$digest';
		this.instanceId = 'instance.haxe-${featureId()}.$digest';
		this.key = key;
		this.value = value;
		this.ownerModule = ownerModule;
		this.source = source;
		this.position = position;
		this.hashRequest = hashRequest;
		this.equalRequest = equalRequest;
		this.keyTraceRequest = keyTraceRequest;
		this.valueTraceRequest = valueTraceRequest;
	}

	/** Stable runtime-feature identity for diagnostics and operation dispatch. */
	public function featureId():String
		return switch family {
			case CBTMObject: "object-map";
			case CBTMEnumValue: "enum-value-map";
			case CBTMString: "gc-string-map";
		};

	/** Describe one shared mutable Haxe map object. */
	public function declaration():HxcIRTypeDeclaration
		return {
			id: declarationId,
			displayName: 'Map<${key.cSpelling}, ${value.cSpelling}>',
			kind: IRTKReference,
			source: source
		};

	/** Make precise collector ownership and exact key/value types visible in IR. */
	public function instance():HxcIRTypeInstance
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [key.irType, value.irType],
			representation: IRRManaged("gc"),
			source: source
		};
}

/** Final callback names consumed by structural C emission. */
class CLoweredBodyTypedMap {
	public final prepared:CPreparedBodyTypedMap;
	public final hashName:CIdentifier;
	public final equalName:CIdentifier;
	public final keyTraceName:Null<CIdentifier>;
	public final valueTraceName:Null<CIdentifier>;
	public final lifetimes:Array<CLoweredMapLifetime>;

	/** Join one semantic plan with collision-safe C identifiers. */
	public function new(prepared:CPreparedBodyTypedMap, hashName:CIdentifier, equalName:CIdentifier, keyTraceName:Null<CIdentifier>,
			valueTraceName:Null<CIdentifier>, ?lifetimes:Array<CLoweredMapLifetime>) {
		this.prepared = prepared;
		this.hashName = hashName;
		this.equalName = equalName;
		this.keyTraceName = keyTraceName;
		this.valueTraceName = valueTraceName;
		this.lifetimes = lifetimes == null ? [] : lifetimes;
	}
}

/** Request-local recognizer and plan registry for the two typed map families. */
class CBodyTypedMapRegistry {
	final context:CompilationContext;
	final resolveValue:CBodyTypedMapValueResolver;
	final bySemanticKey:Map<String, CPreparedBodyTypedMap> = [];

	/** Create isolated specialization state for one compiler request. */
	public function new(context:CompilationContext, resolveValue:CBodyTypedMapValueResolver) {
		this.context = context;
		this.resolveValue = resolveValue;
	}

	/** Count prepared specializations without exposing mutable compiler state. */
	@:noCompletion
	public function preparedCount():Int {
		var count = 0;
		for (_ in bySemanticKey)
			count++;
		return count;
	}

	/**
		Recognize Map abstracts and their concrete standard-library implementations.

		A concrete ObjectMap or EnumValueMap cannot fall through to generic class
		lowering. Unsupported key or value shapes fail here while their exact source
		types are still available.
	**/
	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyTypedMap> {
		final parameters = mapParameters(type);
		if (parameters == null)
			return null;
		if (parameters.types.length != 2)
			return rejected(fail, position, '$node:typed-map-arity:${parameters.types.length}');
		final key = resolveValue(parameters.types[0], position, ownerModule, sourcePath, fail, '$node.typed-map-key');
		final family = parameters.family == null ? familyForKey(key) : parameters.family;
		if (family == null)
			return null;
		final keyRejection = keyRejection(family, key);
		if (keyRejection != null)
			return rejected(fail, position, '$node:${featureLabel(family)}-key-not-admitted:$keyRejection');
		final value = resolveValue(parameters.types[1], position, ownerModule, sourcePath, fail, '$node.typed-map-value');
		if (!valueIsAdmitted(value))
			return rejected(fail, position, '$node:${featureLabel(family)}-value-not-admitted:${value.cSpelling}');
		final semanticKey = 'haxe-${featureLabel(family)}-v1(${canonicalPart(key.cSpelling)},${canonicalPart(value.cSpelling)})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		final digest = Sha256.encode(semanticKey);
		final root = ["compiler", featureLabel(family), digest, "key-policy"];
		final hash = callbackRequest(root, "hash", 0);
		final equal = callbackRequest(root, "equal", 1);
		context.symbols.register(hash);
		context.symbols.register(equal);
		final keyTraced = containsCollectorReference(key);
		final valueTraced = containsCollectorReference(value);
		final keyTrace = keyTraced ? callbackRequest(root, "key-trace", 2) : null;
		final valueTrace = valueTraced ? callbackRequest(root, "value-trace", 3) : null;
		for (request in [keyTrace, valueTrace])
			if (request != null)
				context.symbols.register(request);
		final prepared = new CPreparedBodyTypedMap(family, semanticKey, digest, key, value, ownerModule, HaxeSourceSpan.fromPosition(position, sourcePath),
			position, hash, equal, keyTrace, valueTrace);
		bySemanticKey.set(semanticKey, prepared);
		return prepared;
	}

	/**
		Reuse the collector table for StringMap records with traced children.
		Ordinary StringMaps keep their existing smaller reference-counted carrier.
		Key comparison remains UTF-8 value equality, never object identity.
	**/
	public function collectorStringMapType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CPreparedBodyTypedMap> {
		final parameters = CBodyStringMapRegistry.mapParameters(type);
		if (parameters == null || parameters.length != 2 || CBodyAggregateRegistry.staticStringIdentity(parameters[0]) == null)
			return null;
		final value = resolveValue(parameters[1], position, ownerModule, sourcePath, fail, '$node.StringMap-value');
		if (value.aggregateValue() == null)
			return null;
		final key = resolveValue(parameters[0], position, ownerModule, sourcePath, fail, '$node.StringMap-key');
		final semanticKey = 'haxe-gc-string-map-v1(${canonicalPart(key.cSpelling)},${canonicalPart(value.cSpelling)})';
		final existing = bySemanticKey.get(semanticKey);
		if (existing != null)
			return existing;
		if (!CBodyStringMapRegistry.collectorRecord(value, parameters[1]))
			return null;
		final digest = Sha256.encode(semanticKey);
		final root = ["compiler", "gc-string-map", digest];
		final hash = callbackRequest(root, "hash", 0);
		final equal = callbackRequest(root, "equal", 1);
		final trace = callbackRequest(root, "value-trace", 2);
		for (request in [hash, equal, trace])
			context.symbols.register(request);
		final prepared = new CPreparedBodyTypedMap(CBTMString, semanticKey, digest, key, value, ownerModule,
			HaxeSourceSpan.fromPosition(position, sourcePath), position, hash, equal, null, trace);
		bySemanticKey.set(semanticKey, prepared);
		registerLifetime(prepared, key);
		registerLifetime(prepared, value);
		return prepared;
	}

	/** Snapshot callbacks own their values after the source map is cleared or collected. */
	public function completeStringLifetimes(iterators:Array<CPreparedBodyIterator>):Void {
		for (map in canonicalMaps())
			if (map.family == CBTMString) {
				for (iterator in iterators) {
					final element = iterator.element;
					if (element.cSpelling == map.key.cSpelling || element.cSpelling == map.value.cSpelling) {
						registerLifetime(map, element);
						continue;
					}
					final pair = element.aggregateValue();
					if (pair == null || pair.fields.length != 2)
						continue;
					var key = false;
					var value = false;
					for (field in pair.fields) {
						if (field.name == "key" && field.type.cSpelling == map.key.cSpelling)
							key = true;
						if (field.name == "value" && field.type.cSpelling == map.value.cSpelling)
							value = true;
					}
					if (key && value)
						registerLifetime(map, element);
				}
			}
	}

	/** Register each exact carrier once; no callback retains a collector pointer. */
	function registerLifetime(map:CPreparedBodyTypedMap, value:CBodyValueType):Void {
		for (entry in map.lifetimes)
			if (entry.value.cSpelling == value.cSpelling)
				return;
		final root = [
			"compiler",
			"gc-string-map",
			map.digest,
			"lifetime",
			Sha256.encode(value.cSpelling)
		];
		final copy = callbackRequest(root, "copy", 0);
		final destroy = callbackRequest(root, "destroy", 1);
		context.symbols.register(copy);
		context.symbols.register(destroy);
		map.lifetimes.push({value: value, copy: copy, destroy: destroy});
	}

	/** Return stable specialization order for HxcIR and C planning. */
	public function canonicalMaps():Array<CPreparedBodyTypedMap> {
		final values = [for (value in bySemanticKey) value];
		values.sort((left, right) -> reflaxe.c.CUtf8Order.compare(left.semanticKey, right.semanticKey));
		return values;
	}

	/** Finalize every program-local callback name after global collision checks. */
	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyTypedMap>
		return canonicalMaps().map(value -> new CLoweredBodyTypedMap(value, symbols.identifierFor(value.hashRequest),
			symbols.identifierFor(value.equalRequest), identifierOrNull(symbols, value.keyTraceRequest), identifierOrNull(symbols, value.valueTraceRequest),
			value.lifetimes.map(entry -> {
				value: entry.value,
				copy: symbols.identifierFor(entry.copy),
				destroy: symbols.identifierFor(entry.destroy)
			})));

	static function keyRejection(family:CBodyTypedMapFamily, key:CBodyValueType):Null<String>
		return switch family {
			case CBTMObject: key.classValue() != null && key.ownedClassValue() == null ? null : key.cSpelling;
			case CBTMString: key.staticStringIdentity() != null ? null : key.cSpelling;
			case CBTMEnumValue:
				final value = key.enumValue();
				value == null ? key.cSpelling : enumKeyRejection(value, []);
		};

	/** Keep the first slice exact and unboxed: scalars and collector references. */
	static function valueIsAdmitted(value:CBodyValueType):Bool
		return switch value.kind {
			case CBVKPrimitive(mapping):
				switch mapping.irType {
					case IRTBool | IRTInt(32, true): true;
					case _: false;
				}
			case CBVKClass(_, _): true;
			case _: false;
		};

	/** Prove every active enum payload has one implemented equality rule. */
	static function enumKeyRejection(value:reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance, visited:Map<String, Bool>):Null<String> {
		if (value.recursive)
			return 'recursive-enum:${value.haxePath}';
		if (visited.exists(value.instanceId))
			return 'recursive-enum-graph:${value.haxePath}';
		visited.set(value.instanceId, true);
		for (tagCase in value.cases)
			for (payload in tagCase.payload) {
				if (payload.indirect)
					return '${value.haxePath}.${tagCase.name}.${payload.name}:indirect-payload';
				final rejection = switch payload.valueType.kind {
					case CBVKPrimitive(mapping):
						switch mapping.irType {
							case IRTBool | IRTInt(32, true): null;
							case _: payload.valueType.cSpelling;
						}
					case CBVKClass(_, _): null;
					case CBVKEnum(nested): enumKeyRejection(nested, copyVisited(visited));
					case _: payload.valueType.cSpelling;
				};
				if (rejection != null)
					return '${value.haxePath}.${tagCase.name}.${payload.name}:$rejection';
			}
		return null;
	}

	/** True when one exact value graph contains a collector pointer. */
	static function containsCollectorReference(value:CBodyValueType):Bool
		return switch value.kind {
			case CBVKClass(_, _): true;
			case CBVKEnum(enumeration): enumContainsClass(enumeration, []);
			case _: false;
		};

	static function enumContainsClass(value:reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance, visited:Map<String, Bool>):Bool {
		if (visited.exists(value.instanceId))
			return false;
		visited.set(value.instanceId, true);
		for (tagCase in value.cases)
			for (payload in tagCase.payload)
				switch payload.valueType.kind {
					case CBVKClass(_, _):
						return true;
					case CBVKEnum(nested):
						if (enumContainsClass(nested, visited))
							return true;
					case _:
				}
		return false;
	}

	static function familyForKey(key:CBodyValueType):Null<CBodyTypedMapFamily>
		return switch key.kind {
			case CBVKClass(_, _): CBTMObject;
			case CBVKEnum(_): CBTMEnumValue;
			case _: null;
		};

	static function mapParameters(type:Type):Null<{family:Null<CBodyTypedMapFamily>, types:Array<Type>}>
		return switch type {
			case TAbstract(reference, parameters) if (isMapAbstract(reference.get())):
				{family: null, types: parameters};
			case TInst(reference, parameters) if (CBodyTypedMapRecognition.family(reference) != null):
				{family: CBodyTypedMapRecognition.family(reference), types: parameters};
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : mapParameters(resolved);
			case TLazy(resolve): mapParameters(resolve());
			case TType(reference, parameters):
				final definition = reference.get();
				mapParameters(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case _: null;
		};

	static function isMapAbstract(value:AbstractType):Bool
		return value.name == "Map" && (value.pack.length == 0 || value.pack.join(".") == "haxe.ds");

	static function callbackRequest(root:Array<String>, role:String, rank:Int):CSymbolRequest
		return new CSymbolRequest(CSKMethod, root.concat([role]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], rank, ["typed_map", role]);

	static function featureLabel(family:CBodyTypedMapFamily):String
		return switch family {
			case CBTMObject: "object-map";
			case CBTMEnumValue: "enum-value-map";
			case CBTMString: "gc-string-map";
		};

	static function canonicalPart(value:String):String {
		final bytes = Bytes.ofString(value);
		return '${bytes.length}:$value';
	}

	static function copyVisited(source:Map<String, Bool>):Map<String, Bool> {
		final copy:Map<String, Bool> = [];
		for (key in source.keys())
			copy.set(key, true);
		return copy;
	}

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("typed map rejection callback returned unexpectedly");
	}
}

/** Exact concrete owner recognition shared by reachability and body lowering. */
class CBodyTypedMapRecognition {
	/** Return the standard map family, or null for an ordinary class. */
	public static function family(reference:Ref<ClassType>):Null<CBodyTypedMapFamily> {
		final value = reference.get();
		if (value.pack.join(".") != "haxe.ds")
			return null;
		return switch value.name {
			case "ObjectMap": CBTMObject;
			case "EnumValueMap": CBTMEnumValue;
			case _: null;
		};
	}

	/** Recognize the generic interface view inserted around a specialized Map. */
	public static function isIMapType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, _): final value = reference.get(); value.pack.join(".") == "haxe" && value.name == "IMap";
			case _: false;
		};

	/** True only for Haxe's generic map interface declaration. */
	public static function isIMapOwner(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.join(".") == "haxe" && value.name == "IMap";
	}

	/** Recover object/enum key policy from a typed Map or IMap receiver view. */
	public static function familyForMapType(type:Type):Null<CBodyTypedMapFamily>
		return switch type {
			case TAbstract(reference, parameters) if (reference.get().name == "Map" && parameters.length == 2):
				familyForKeyType(parameters[0]);
			case TInst(reference, parameters) if (family(reference) != null): family(reference);
			case TInst(reference, parameters) if (reference.get().pack.join(".") == "haxe"
				&& reference.get().name == "IMap"
				&& parameters.length == 2):
				familyForKeyType(parameters[0]);
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : familyForMapType(resolved);
			case TLazy(resolve): familyForMapType(resolve());
			case TType(reference, parameters):
				final definition = reference.get();
				familyForMapType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters));
			case _: null;
		};

	static function familyForKeyType(type:Type):Null<CBodyTypedMapFamily>
		return switch TypeTools.follow(type) {
			// Core String has a class type in Haxe, but map keys compare its value.
			// Its inlined IMap view belongs to StringMap, not object identity maps.
			case TInst(reference, _) if (reference.get().pack.length == 0 && reference.get().name == "String"): null;
			case TInst(reference, _) if (!reference.get().isInterface): CBTMObject;
			case TEnum(_, _): CBTMEnumValue;
			case _: null;
		};
}
#else
class CBodyTypedMap {
	private function new() {}
}
#end
