package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.interop.CImportRegistry;
import reflaxe.c.interop.CImportRegistry.CImportContributionInventory;
import reflaxe.c.interop.CImportRegistry.CLoweredImports;
import reflaxe.c.interop.CImportRegistry.CPreparedImportConstant;
import reflaxe.c.interop.CImportRegistry.CPreparedImportFunction;
import reflaxe.c.interop.CImportRegistry.CPreparedImportType;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRegistry;
import reflaxe.c.lowering.CBodyArray.CPreparedBodyArray;
import reflaxe.c.lowering.CBodyBytes.CBodyBytesRegistry;
import reflaxe.c.lowering.CBodyBytes.CPreparedBodyBytes;
import reflaxe.c.lowering.CBodyClass.CBodyClassRegistry;
import reflaxe.c.lowering.CBodyClass.CBodyInterfaceImplementation;
import reflaxe.c.lowering.CBodyClass.CLoweredBodyClass;
import reflaxe.c.lowering.CBodyClass.CPreparedBodyClass;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRegistry;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyEnum.CLoweredBodyEnum;
import reflaxe.c.lowering.CBodyEnum.CPreparedBodyEnumInstance;
import reflaxe.c.lowering.CBodyInterface.CBodyInterfaceRegistry;
import reflaxe.c.lowering.CBodyInterface.CPreparedBodyInterface;
import reflaxe.c.lowering.CBodyIntMap.CBodyIntMapRegistry;
import reflaxe.c.lowering.CBodyIntMap.CPreparedBodyIntMap;
import reflaxe.c.lowering.CBodyOptional.CBodyOptionalRegistry;
import reflaxe.c.lowering.CBodyOptional.CLoweredBodyOptional;
import reflaxe.c.lowering.CBodyOptional.CPreparedBodyOptional;
import reflaxe.c.lowering.CBodyStringMap.CBodyStringMapRegistry;
import reflaxe.c.lowering.CBodyStringMap.CPreparedBodyStringMap;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes;

/** Module anchor for the closed anonymous-record lowering model. */
class CBodyAggregate {
	private function new() {}
}

/**
	Counts shared representations and enum-provenance ranges before HxcIR assembly.

	The compiler snapshots this request-local inventory around function
	construction. Layouts, imports, adapters, and collection representations must
	already be settled; a later change stops compilation rather than publishing an
	incomplete replay generation. Enum source ranges are the one explicit
	function-owned addition and are retained with that function's replay payload.
	The shallow inventory proves this boundary; it is not itself the cache key.
**/
typedef CBodyProgramContributionInventory = {
	final aggregates:Int;
	final enums:Int;
	final enumReasons:Int;
	final classes:Int;
	final interfaces:Int;
	final arrays:Int;
	final intMaps:Int;
	final stringMaps:Int;
	final bytes:Int;
	final optionals:Int;
	final importTypes:Int;
	final importFunctions:Int;
	final importConstants:Int;
	final importOwners:Int;
}

/** A closed body value category; aggregate values never enter primitive semantics. */
enum CBodyValueKind {
	CBVKPrimitive(mapping:CPrimitiveTypeMapping);

	/**
		An immutable Haxe String view whose bytes have program-long storage.

		This is deliberately not a numeric "primitive" and not a generally owned
		String. The separate category lets later lifetime work add owned strings
		without silently changing how today's allocation-free literal values copy.
	**/
	CBVKStaticString(sourceIdentity:String);

	/**
		An immutable Haxe String view that may share runtime-owned UTF-8 bytes.

		The carrier remains a direct value, but logical copies must retain its
		optional backing owner and destruction must release it.
	**/
	CBVKManagedString(sourceIdentity:String);

	CBVKFixedArray(element:CPrimitiveTypeMapping, length:Int, witnessId:String);
	CBVKSpan(element:CPrimitiveTypeMapping, mutable:Bool);
	CBVKCString;
	CBVKImport(value:CPreparedImportType);
	CBVKAggregate(aggregate:CPreparedBodyAggregate);
	CBVKEnum(value:CPreparedBodyEnumInstance);
	CBVKOwnedClass(value:CPreparedBodyClass);
	CBVKClass(value:CPreparedBodyClass, nullable:Bool);
	CBVKInterface(value:CPreparedBodyInterface);
	CBVKArray(value:CPreparedBodyArray);
	CBVKIntMap(value:CPreparedBodyIntMap);
	CBVKStringMap(value:CPreparedBodyStringMap);
	CBVKBytes(value:CPreparedBodyBytes);
	CBVKOptional(value:CPreparedBodyOptional);

	/** An allocation-free callable with one exact source-level signature. */
	CBVKFunction(parameters:Array<CBodyValueType>, result:CBodyValueType);

	/** A non-owning pointer used only inside a proven stack closure environment. */
	CBVKClosureCapturePointer(pointee:CBodyValueType);

	/**
		A mutable address borrowed only for one direct imported-C call.

		The source-level `c.Ref<T>` never becomes owned storage. Lowering creates
		this carrier only from an addressable place and the native-call validator
		requires the exact pointer type expected by the header-owned function.
	**/
	CBVKNativeRef(pointee:CBodyValueType);

	/**
		Mutable NUL-terminated bytes borrowed from one managed `haxe.io.Bytes`.

		Unlike `CBVKNativeRef`, this does not take the address of a scalar place.
		Lowering asks hxrt for the backing byte address, then HxcIR permits that
		address only as one direct imported `char *` argument.
	**/
	CBVKCStringBufferRef;

	/** The opaque `void *` passed from a closure carrier to its typed adapter. */
	CBVKClosureContext;

	/**
		A typed callable plus a caller-owned context pointer.

		The carrier is a direct C struct. Its semantic representation records the
		source-visible signature so HxcIR can distinguish it from an arbitrary
		record with similarly named fields.
	**/
	CBVKStackClosure(carrier:CPreparedBodyAggregate, parameters:Array<CBodyValueType>, result:CBodyValueType);
}

/** The exact admitted representation of one Haxe body value. */
class CBodyValueType {
	public final kind:CBodyValueKind;
	public final irType:HxcIRTypeRef;
	public final cSpelling:String;

	public function new(kind:CBodyValueKind) {
		this.kind = kind;
		switch kind {
			case CBVKPrimitive(mapping):
				this.irType = mapping.irType;
				this.cSpelling = mapping.cSpelling;
			case CBVKStaticString(sourceIdentity):
				this.irType = IRTString;
				this.cSpelling = 'static-haxe-string-view:$sourceIdentity';
			case CBVKManagedString(sourceIdentity):
				this.irType = IRTManagedString;
				this.cSpelling = 'managed-haxe-string-view:$sourceIdentity';
			case CBVKFixedArray(element, length, witnessId):
				this.irType = IRTFixedArray(element.irType, length, witnessId);
				this.cSpelling = 'fixed-array:$length:$witnessId<${element.cSpelling}>';
			case CBVKSpan(element, mutable):
				this.irType = IRTSpan(element.irType, mutable);
				this.cSpelling = '${mutable ? "mutable" : "const"}-span:${element.cSpelling}';
			case CBVKCString:
				this.irType = IRTCString;
				this.cSpelling = "const-char-pointer:borrowed-literal";
			case CBVKImport(value):
				this.irType = value.semanticValueType();
				this.cSpelling = 'c-import:${value.haxePath}';
			case CBVKAggregate(aggregate):
				this.irType = IRTInstance(aggregate.instanceId);
				this.cSpelling = 'closed-record:${aggregate.digest}';
			case CBVKEnum(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-enum:${value.digest}';
			case CBVKOwnedClass(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'owned-haxe-class:${value.digest}';
			case CBVKClass(value, nullable):
				this.irType = IRTPointer(IRTInstance(value.instanceId), nullable);
				this.cSpelling = 'haxe-class-reference:${nullable ? "nullable" : "nonnull"}:${value.digest}';
			case CBVKInterface(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-interface-reference:${value.digest}';
			case CBVKArray(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-array-reference:${value.digest}<${value.element.cSpelling}>';
			case CBVKIntMap(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-int-map-reference:${value.digest}<Int,${value.value.cSpelling}>';
			case CBVKStringMap(value):
				this.irType = IRTInstance(value.instanceId);
				this.cSpelling = 'haxe-string-map-reference:${value.digest}<String,${value.value.cSpelling}>';
			case CBVKBytes(value):
				this.irType = IRTInstance(CPreparedBodyBytes.INSTANCE_ID);
				this.cSpelling = "haxe-bytes-reference";
			case CBVKOptional(value):
				this.irType = value.irType();
				this.cSpelling = 'direct-optional:${value.digest}<${value.payload.cSpelling}>';
			case CBVKFunction(parameters, result):
				this.irType = IRTFunction(parameters.map(parameter -> parameter.irType), result.irType);
				this.cSpelling = 'direct-function:(${parameters.map(parameter -> parameter.cSpelling).join(",")})->${result.cSpelling}';
			case CBVKClosureCapturePointer(pointee):
				this.irType = IRTPointer(pointee.irType, false);
				this.cSpelling = 'stack-closure-capture-pointer<${pointee.cSpelling}>';
			case CBVKNativeRef(pointee):
				this.irType = IRTPointer(pointee.irType, false);
				this.cSpelling = 'native-call-ref<${pointee.cSpelling}>';
			case CBVKCStringBufferRef:
				this.irType = IRTMutableCStringBuffer;
				this.cSpelling = "native-call-mutable-cstring-buffer";
			case CBVKClosureContext:
				this.irType = IRTPointer(IRTVoid, true);
				this.cSpelling = "stack-closure-context:void-pointer";
			case CBVKStackClosure(carrier, parameters, result):
				this.irType = IRTInstance(carrier.instanceId);
				this.cSpelling = 'stack-closure:${carrier.digest}:(${parameters.map(parameter -> parameter.cSpelling).join(",")})->${result.cSpelling}';
		}
	}

	public static function primitive(mapping:CPrimitiveTypeMapping):CBodyValueType
		return new CBodyValueType(CBVKPrimitive(mapping));

	/** Preserve the Haxe/abstract identity while selecting the shared C view. */
	public static function staticString(sourceIdentity:String):CBodyValueType
		return new CBodyValueType(CBVKStaticString(sourceIdentity));

	/** Preserve source identity while enabling the optional String owner. */
	public static function managedString(sourceIdentity:String):CBodyValueType
		return new CBodyValueType(CBVKManagedString(sourceIdentity));

	public static function fixedArray(element:CPrimitiveTypeMapping, length:Int, witnessId:String):CBodyValueType
		return new CBodyValueType(CBVKFixedArray(element, length, witnessId));

	public static function span(element:CPrimitiveTypeMapping, mutable:Bool):CBodyValueType
		return new CBodyValueType(CBVKSpan(element, mutable));

	public static function cString():CBodyValueType
		return new CBodyValueType(CBVKCString);

	public static function imported(value:CPreparedImportType):CBodyValueType
		return new CBodyValueType(CBVKImport(value));

	public static function aggregate(value:CPreparedBodyAggregate):CBodyValueType
		return new CBodyValueType(CBVKAggregate(value));

	public static function enumeration(value:CPreparedBodyEnumInstance):CBodyValueType
		return new CBodyValueType(CBVKEnum(value));

	public static function ownedClass(value:CPreparedBodyClass):CBodyValueType
		return new CBodyValueType(CBVKOwnedClass(value));

	public static function classReference(value:CPreparedBodyClass, nullable:Bool = true):CBodyValueType
		return new CBodyValueType(CBVKClass(value, nullable));

	public static function interfaceReference(value:CPreparedBodyInterface):CBodyValueType
		return new CBodyValueType(CBVKInterface(value));

	public static function arrayReference(value:CPreparedBodyArray):CBodyValueType
		return new CBodyValueType(CBVKArray(value));

	public static function intMapReference(value:CPreparedBodyIntMap):CBodyValueType
		return new CBodyValueType(CBVKIntMap(value));

	public static function stringMapReference(value:CPreparedBodyStringMap):CBodyValueType
		return new CBodyValueType(CBVKStringMap(value));

	public static function bytesReference(value:CPreparedBodyBytes):CBodyValueType
		return new CBodyValueType(CBVKBytes(value));

	public static function optional(value:CPreparedBodyOptional):CBodyValueType
		return new CBodyValueType(CBVKOptional(value));

	public static function directFunction(parameters:Array<CBodyValueType>, result:CBodyValueType):CBodyValueType
		return new CBodyValueType(CBVKFunction(parameters, result));

	public static function closureCapturePointer(pointee:CBodyValueType):CBodyValueType
		return new CBodyValueType(CBVKClosureCapturePointer(pointee));

	public static function nativeRef(pointee:CBodyValueType):CBodyValueType
		return new CBodyValueType(CBVKNativeRef(pointee));

	public static function cStringBufferRef():CBodyValueType
		return new CBodyValueType(CBVKCStringBufferRef);

	public static function closureContext():CBodyValueType
		return new CBodyValueType(CBVKClosureContext);

	public static function stackClosure(carrier:CPreparedBodyAggregate, parameters:Array<CBodyValueType>, result:CBodyValueType):CBodyValueType
		return new CBodyValueType(CBVKStackClosure(carrier, parameters, result));

	public function primitiveMapping():Null<CPrimitiveTypeMapping> {
		return switch kind {
			case CBVKPrimitive(mapping): mapping;
			case CBVKStaticString(_) | CBVKManagedString(_) | CBVKFixedArray(_, _, _) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) | CBVKAggregate(_) |
				CBVKEnum(_) | CBVKOwnedClass(_) | CBVKClass(_, _) | CBVKInterface(_) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) |
				CBVKOptional(_) | CBVKFunction(_, _) | CBVKClosureCapturePointer(_) | CBVKNativeRef(_) | CBVKCStringBufferRef | CBVKClosureContext |
				CBVKStackClosure(_, _, _): null;
		};
	}

	/** Return the nominal Haxe identity carried by either String lifetime plan. */
	public function staticStringIdentity():Null<String> {
		return switch kind {
			case CBVKStaticString(value) | CBVKManagedString(value): value;
			case _: null;
		};
	}

	public function fixedArrayShape():Null<{element:CPrimitiveTypeMapping, length:Int, witnessId:String}> {
		return switch kind {
			case CBVKFixedArray(element, length, witnessId): {element: element, length: length, witnessId: witnessId};
			case _: null;
		};
	}

	public function spanElement():Null<CPrimitiveTypeMapping> {
		return switch kind {
			case CBVKSpan(element, _): element;
			case _: null;
		};
	}

	public function spanMutable():Null<Bool> {
		return switch kind {
			case CBVKSpan(_, mutable): mutable;
			case _: null;
		};
	}

	public function isCString():Bool
		return kind == CBVKCString;

	public function isCStringBufferRef():Bool
		return kind == CBVKCStringBufferRef;

	public function importedValue():Null<CPreparedImportType> {
		return switch kind {
			case CBVKImport(value): value;
			case _: null;
		};
	}

	public function importedStructValue():Null<CPreparedImportType> {
		return switch kind {
			case CBVKImport(value): value.directStructTarget();
			case _: null;
		};
	}

	public function aggregateValue():Null<CPreparedBodyAggregate> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKStaticString(_) | CBVKManagedString(_) | CBVKFixedArray(_, _, _) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) |
				CBVKOwnedClass(_) | CBVKInterface(_) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) | CBVKOptional(_) | CBVKFunction(_, _) |
				CBVKClosureCapturePointer(_) | CBVKNativeRef(_) | CBVKCStringBufferRef | CBVKClosureContext | CBVKStackClosure(_, _, _): null;
			case CBVKAggregate(aggregate): aggregate;
			case CBVKEnum(_) | CBVKClass(_, _): null;
		};
	}

	public function enumValue():Null<CPreparedBodyEnumInstance> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKStaticString(_) | CBVKManagedString(_) | CBVKFixedArray(_, _, _) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) |
				CBVKAggregate(_) | CBVKOwnedClass(_) | CBVKClass(_, _) | CBVKInterface(_) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) |
				CBVKOptional(_) | CBVKFunction(_, _) | CBVKClosureCapturePointer(_) | CBVKNativeRef(_) | CBVKCStringBufferRef | CBVKClosureContext |
				CBVKStackClosure(_, _, _): null;
			case CBVKEnum(value): value;
		};
	}

	public function classValue():Null<CPreparedBodyClass> {
		return switch kind {
			case CBVKPrimitive(_) | CBVKStaticString(_) | CBVKManagedString(_) | CBVKFixedArray(_, _, _) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) |
				CBVKAggregate(_) | CBVKEnum(_) | CBVKInterface(_) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) | CBVKOptional(_) |
				CBVKFunction(_,
					_) | CBVKClosureCapturePointer(_) | CBVKNativeRef(_) | CBVKCStringBufferRef | CBVKClosureContext | CBVKStackClosure(_, _, _): null;
			case CBVKOwnedClass(value) | CBVKClass(value, _): value;
		};
	}

	public function ownedClassValue():Null<CPreparedBodyClass> {
		return switch kind {
			case CBVKOwnedClass(value): value;
			case _: null;
		};
	}

	public function interfaceValue():Null<CPreparedBodyInterface> {
		return switch kind {
			case CBVKInterface(value): value;
			case _: null;
		};
	}

	public function arrayValue():Null<CPreparedBodyArray> {
		return switch kind {
			case CBVKArray(value): value;
			case _: null;
		};
	}

	public function stringMapValue():Null<CPreparedBodyStringMap> {
		return switch kind {
			case CBVKStringMap(value): value;
			case _: null;
		};
	}

	public function intMapValue():Null<CPreparedBodyIntMap> {
		return switch kind {
			case CBVKIntMap(value): value;
			case _: null;
		};
	}

	public function bytesValue():Null<CPreparedBodyBytes> {
		return switch kind {
			case CBVKBytes(value): value;
			case _: null;
		};
	}

	public function optionalValue():Null<CPreparedBodyOptional> {
		return switch kind {
			case CBVKOptional(value): value;
			case _: null;
		};
	}

	public function functionValue():Null<{parameters:Array<CBodyValueType>, result:CBodyValueType}> {
		return switch kind {
			case CBVKFunction(parameters, result) | CBVKStackClosure(_, parameters, result): {parameters: parameters, result: result};
			case _: null;
		};
	}

	/** Return the direct struct carrier only for a proven stack closure value. */
	public function stackClosureValue():Null<{carrier:CPreparedBodyAggregate, parameters:Array<CBodyValueType>, result:CBodyValueType}> {
		return switch kind {
			case CBVKStackClosure(carrier, parameters, result): {carrier: carrier, parameters: parameters, result: result};
			case _: null;
		};
	}

	public function classNullable():Null<Bool> {
		return switch kind {
			case CBVKOwnedClass(_): false;
			case CBVKClass(_, nullable): nullable;
			case CBVKPrimitive(_) | CBVKStaticString(_) | CBVKManagedString(_) | CBVKFixedArray(_, _, _) | CBVKSpan(_, _) | CBVKCString | CBVKImport(_) |
				CBVKAggregate(_) | CBVKEnum(_) | CBVKInterface(_) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_) | CBVKBytes(_) | CBVKOptional(_) |
				CBVKFunction(_,
					_) | CBVKClosureCapturePointer(_) | CBVKNativeRef(_) | CBVKCStringBufferRef | CBVKClosureContext | CBVKStackClosure(_, _, _): null;
		};
	}

	/**
		Whether this value's selected C carrier already represents absence exactly.

		A nullable class reference, an ordinary Haxe `Array<T>`, and an ordinary
		`Map<String, V>` all lower to C pointers, so `NULL` is enough. Keeping that
		fact here prevents an explicit `Null` spelling from adding a second
		`{ has_value, value }` wrapper around a carrier that already represents
		absence exactly.
	**/
	public function hasExactNullCarrier():Bool
		return switch kind {
			case CBVKStaticString(_) | CBVKManagedString(_) | CBVKClass(_, true) | CBVKArray(_) | CBVKIntMap(_) | CBVKStringMap(_): true;
			case _: false;
		};
}

/** One canonical field before C names are finalized. */
class CPreparedBodyAggregateField {
	public final name:String;
	public final type:CBodyValueType;
	public final mutable:Bool;
	public final source:HxcSourceSpan;
	public final request:CSymbolRequest;

	public function new(name:String, type:CBodyValueType, mutable:Bool, source:HxcSourceSpan, request:CSymbolRequest) {
		this.name = name;
		this.type = type;
		this.mutable = mutable;
		this.source = source;
		this.request = request;
	}
}

private typedef CBodyAggregateTypedefOwner = {
	final modulePath:String;
	final displayName:String;
	final position:Position;
	final cacheKey:Null<String>;
}

/** One outer variable stored by address in a caller-owned closure environment. */
typedef CBodyStackClosureCapture = {
	final compilerId:Int;
	final sourceName:String;
	final identity:String;
	final type:CBodyValueType;
	final position:Position;
}

private typedef CBodySyntheticAggregateField = {
	final name:String;
	final readableName:String;
	final type:CBodyValueType;
	final mutable:Bool;
	final position:Position;
}

/** One shape-deduplicated anonymous record before symbol finalization. */
class CPreparedBodyAggregate {
	public final shapeKey:String;
	public final digest:String;
	public final declarationId:String;
	public final instanceId:String;
	public final displayName:Null<String>;
	public final ownerModule:String;
	public final source:HxcSourceSpan;
	public final typeRequest:CSymbolRequest;
	public final fields:Array<CPreparedBodyAggregateField>;
	public final representation:HxcIRRepresentation;
	public var managedLifetime:Bool = false;
	public var retainRequest:Null<CSymbolRequest> = null;
	public var destroyRequest:Null<CSymbolRequest> = null;
	public var retainParameterRequest:Null<CSymbolRequest> = null;
	public var destroyParameterRequest:Null<CSymbolRequest> = null;
	public var retainStatusRequest:Null<CSymbolRequest> = null;

	public function new(shapeKey:String, digest:String, displayName:Null<String>, ownerModule:String, source:HxcSourceSpan, typeRequest:CSymbolRequest,
			?representation:HxcIRRepresentation) {
		this.shapeKey = shapeKey;
		this.digest = digest;
		this.declarationId = 'type.closed-record.$digest';
		this.instanceId = 'instance.closed-record.$digest';
		this.displayName = displayName;
		this.ownerModule = ownerModule;
		this.source = source;
		this.typeRequest = typeRequest;
		this.fields = [];
		this.representation = representation == null ? IRRDirect : representation;
	}

	public function declaration():HxcIRTypeDeclaration {
		return {
			id: declarationId,
			displayName: displayName == null ? 'anonymous.record.${digest.substr(0, 16)}' : displayName,
			kind: IRTKAggregate(fields.map(field -> {
				name: field.name,
				type: field.type.irType,
				mutable: field.mutable,
				source: field.source
			})),
			source: source
		};
	}

	public function instance():HxcIRTypeInstance {
		return {
			id: instanceId,
			declarationId: declarationId,
			arguments: [],
			representation: representation,
			source: source
		};
	}

	public function retainImplementationId():Null<String>
		return managedLifetime ? 'aggregate-lifecycle:$instanceId:retain' : null;

	public function destroyImplementationId():Null<String>
		return managedLifetime ? 'aggregate-lifecycle:$instanceId:destroy' : null;
}

/** One finalized aggregate member used by structural C emission. */
class CLoweredBodyAggregateField {
	public final semanticName:String;
	public final type:CBodyValueType;
	public final mutable:Bool;
	public final cName:CIdentifier;

	public function new(semanticName:String, type:CBodyValueType, mutable:Bool, cName:CIdentifier) {
		this.semanticName = semanticName;
		this.type = type;
		this.mutable = mutable;
		this.cName = cName;
	}
}

/** One finalized private C struct and its matching direct HxcIR instance. */
class CLoweredBodyAggregate {
	public final prepared:CPreparedBodyAggregate;
	public final cTag:CIdentifier;
	public final fields:Array<CLoweredBodyAggregateField>;
	public final retainName:Null<CIdentifier>;
	public final destroyName:Null<CIdentifier>;
	public final retainParameterName:Null<CIdentifier>;
	public final destroyParameterName:Null<CIdentifier>;
	public final retainStatusName:Null<CIdentifier>;

	public function new(prepared:CPreparedBodyAggregate, cTag:CIdentifier, fields:Array<CLoweredBodyAggregateField>, retainName:Null<CIdentifier>,
			destroyName:Null<CIdentifier>, retainParameterName:Null<CIdentifier>, destroyParameterName:Null<CIdentifier>, retainStatusName:Null<CIdentifier>) {
		this.prepared = prepared;
		this.cTag = cTag;
		this.fields = fields.copy();
		this.retainName = retainName;
		this.destroyName = destroyName;
		this.retainParameterName = retainParameterName;
		this.destroyParameterName = destroyParameterName;
		this.retainStatusName = retainStatusName;
	}

	public function field(semanticName:String):Null<CLoweredBodyAggregateField> {
		for (field in fields) {
			if (field.semanticName == semanticName) {
				return field;
			}
		}
		return null;
	}
}

/**
	Request-local structural identity registry.

	Field expressions retain source evaluation order in HxcIR, while declaration
	fields are UTF-8 ordered here. Identical closed shapes therefore share one IR
	instance and one C tag even when typedef aliases or source field order differ.
 */
class CBodyAggregateRegistry {
	final context:CompilationContext;
	final runtimeCreatedStrings:Bool;
	final byShape:Map<String, CPreparedBodyAggregate> = [];
	final enumRegistry:CBodyEnumRegistry;
	final classRegistry:CBodyClassRegistry;
	final interfaceRegistry:CBodyInterfaceRegistry;
	final arrayRegistry:CBodyArrayRegistry;
	final intMapRegistry:CBodyIntMapRegistry;
	final stringMapRegistry:CBodyStringMapRegistry;
	final bytesRegistry:CBodyBytesRegistry;
	final optionalRegistry:CBodyOptionalRegistry;
	final importRegistry:Null<CImportRegistry>;
	final sourcePathsByModule:Map<String, String> = [];
	final namedRecordSources:Null<reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourcePlan>;
	final directPrimitiveValues:Map<String, CBodyValueType> = [];
	final namedRecordValues:Map<String, CBodyValueType> = [];
	final exactNominalValues:Map<String, CBodyValueType> = [];
	var namedRecordCacheHits = 0;
	var namedRecordCacheMisses = 0;
	var exactNominalCacheHits = 0;
	var exactNominalCacheMisses = 0;

	public function new(context:CompilationContext, ?program:TypedProgramInput, ?contract:TypedCContractSnapshot, runtimeCreatedStrings:Bool = false) {
		this.context = context;
		this.runtimeCreatedStrings = runtimeCreatedStrings;
		this.namedRecordSources = program == null ? null : program.namedRecordSources;
		if (program != null)
			for (module in program.modules)
				sourcePathsByModule.set(module.path, module.sourcePath);
		this.enumRegistry = new CBodyEnumRegistry(context, valueType);
		this.classRegistry = new CBodyClassRegistry(context, valueType);
		this.interfaceRegistry = new CBodyInterfaceRegistry(program);
		this.arrayRegistry = new CBodyArrayRegistry(context, valueType);
		this.intMapRegistry = new CBodyIntMapRegistry(context, valueType);
		this.stringMapRegistry = new CBodyStringMapRegistry(context, valueType);
		this.bytesRegistry = new CBodyBytesRegistry();
		this.optionalRegistry = new CBodyOptionalRegistry(context);
		this.importRegistry = program == null || contract == null ? null : new CImportRegistry(context, program, contract, valueType);
	}

	/**
		Snapshot shared representation counts without finalizing or sorting plans.

		This method is called only by opt-in profiling. It deliberately reports
		counts rather than compiler objects, so the diagnostic remains bounded
		and cannot become an accidental cross-request cache.
	**/
	@:noCompletion
	public function contributionInventory():CBodyProgramContributionInventory {
		final imports:CImportContributionInventory = importRegistry == null ? {
			types: 0,
			functions: 0,
			constants: 0,
			reachedOwners: 0
		} : importRegistry.contributionInventory();
		return {
			aggregates: countValues(byShape),
			enums: enumRegistry.preparedCount(),
			enumReasons: enumRegistry.totalReasonCount(),
			classes: classRegistry.preparedCount(),
			interfaces: interfaceRegistry.preparedCount(),
			arrays: arrayRegistry.preparedCount(),
			intMaps: intMapRegistry.preparedCount(),
			stringMaps: stringMapRegistry.preparedCount(),
			bytes: bytesRegistry.preparedCount(),
			optionals: optionalRegistry.preparedCount(),
			importTypes: imports.types,
			importFunctions: imports.functions,
			importConstants: imports.constants,
			importOwners: imports.reachedOwners
		};
	}

	static function countValues<T>(values:Map<String, T>):Int {
		var count = 0;
		for (_ in values)
			count++;
		return count;
	}

	public function valueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):CBodyValueType {
		final imported = importRegistry == null ? null : importRegistry.valueType(type, position, ownerModule, sourcePath, fail, node);
		if (imported != null)
			return imported;
		final nativeRef = nativeRefValueType(type, position, fail, node);
		if (nativeRef != null)
			return nativeRef;
		final cStringBufferRef = cStringBufferRefValueType(type);
		if (cStringBufferRef != null)
			return cStringBufferRef;
		final directPrimitive = directPrimitiveValueType(type);
		if (directPrimitive != null)
			return directPrimitive;
		final stringIdentity = staticStringIdentity(type);
		if (stringIdentity != null)
			return runtimeCreatedStrings ? CBodyValueType.managedString(stringIdentity) : CBodyValueType.staticString(stringIdentity);
		final directStringMap = stringMapRegistry.valueType(type, position, ownerModule, sourcePath, fail, node);
		if (directStringMap != null)
			return CBodyValueType.stringMapReference(directStringMap);
		final directIntMap = intMapRegistry.valueType(type, position, ownerModule, sourcePath, fail, node);
		if (directIntMap != null)
			return CBodyValueType.intMapReference(directIntMap);
		final exactNominal = exactNominalValueType(type, position, ownerModule, sourcePath, fail, node);
		if (exactNominal != null)
			return exactNominal;
		final aliasOwner = anonymousTypedefOwner(type);
		final resolved = unwrapAliases(type, position, fail, node);
		final resolvedImport = importRegistry == null ? null : importRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (resolvedImport != null)
			return resolvedImport;
		final functionValue = directFunctionValueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (functionValue != null)
			return functionValue;
		final array = arrayRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (array != null)
			return CBodyValueType.arrayReference(array);
		final stringMap = stringMapRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (stringMap != null)
			return CBodyValueType.stringMapReference(stringMap);
		final intMap = intMapRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (intMap != null)
			return CBodyValueType.intMapReference(intMap);
		final bytes = bytesRegistry.valueType(resolved, position, ownerModule, sourcePath);
		if (bytes != null)
			return CBodyValueType.bytesReference(bytes);
		final primitive = switch CPrimitiveTypeMapper.map(resolved, context.profile) {
			case CTPrimitive(mapping) if (mapping.nullability == CPNonNullable): mapping;
			case _: null;
		};
		if (primitive != null)
			return CBodyValueType.primitive(primitive);
		return switch resolved {
			case TAbstract(reference, parameters) if (isSpan(reference.get(), parameters)):
				final span = reference.get();
				final element = admittedSpanElement(parameters[0], position, fail, node);
				CBodyValueType.span(element, span.name == "Span");
			case TInst(reference, parameters) if (!reference.get().isExtern && reference.get().isInterface):
				CBodyValueType.interfaceReference(interfaceRegistry.require(reference, parameters, position, sourcePath, fail, node));
			case TInst(reference, parameters) if (!reference.get().isExtern):
				classRegistry.valueType(reference, parameters, position, ownerModule, sourcePath, fail, node);
			case TEnum(reference, parameters):
				enumRegistry.valueType(reference, parameters, position, ownerModule, sourcePath, fail, node);
			case TAbstract(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Null" && parameters.length == 1):
				final nullable = valueType(parameters[0], position, ownerModule, sourcePath, fail, '$node.nullable');
				if (nullable.hasExactNullCarrier()) {
					nullable;
				} else {
					CBodyValueType.optional(optionalRegistry.require(nullable, ownerModule, sourcePath, position, fail, node));
				}
			case TAbstract(reference, parameters) if (!reference.get().meta.has(":coreType")):
				final definition = reference.get();
				valueType(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), position, ownerModule, sourcePath, fail,
					'$node.abstract-representation');
			case TAnonymous(reference):
				anonymousValueType(reference, aliasOwner, position, ownerModule, sourcePath, fail, node);
			case _:
				CBodyValueType.primitive(admittedPrimitive(resolved, position, fail, node));
		};
	}

	/**
		Return a shared value plan for an exact primitive abstract.

		This is intentionally narrower than `valueType`: it does not follow
		typedefs, unwrap `Null<T>`, or classify user abstracts. Those forms may
		carry nominal, import, lifetime, or diagnostic meaning. Exact primitives
		have no use-site observation to replay, so one request-local immutable
		value plan is safe to share across every function that mentions them.
	**/
	public function directPrimitiveValueType(type:Type):Null<CBodyValueType> {
		final mapping = CPrimitiveTypeMapper.mapDirectNonNullable(type, context.profile);
		if (mapping == null)
			return null;
		final key = CPrimitiveSemantics.sourceTypeKey(mapping.sourceType);
		final existing = directPrimitiveValues.get(key);
		if (existing != null)
			return existing;
		final prepared = CBodyValueType.primitive(mapping);
		directPrimitiveValues.set(key, prepared);
		return prepared;
	}

	/** Number of repeated non-generic typedef-record classifications reused. */
	public inline function namedRecordHits():Int
		return namedRecordCacheHits;

	/** Number of distinct non-generic typedef records classified in this request. */
	public inline function namedRecordMisses():Int
		return namedRecordCacheMisses;

	/** Number of repeated exact class/interface value plans reused in this request. */
	public inline function exactNominalHits():Int
		return exactNominalCacheHits;

	/** Number of distinct exact class/interface value plans built in this request. */
	public inline function exactNominalMisses():Int
		return exactNominalCacheMisses;

	/**
		Reuses the value plan for an exact non-generic class or interface.

		A plain `TInst` already names one Haxe declaration. Once imports, native
		references, primitives, strings, and maps have declined that exact type,
		a non-extern declaration cannot change representation at another use
		site. Reusing its immutable wrapper avoids replaying alias, function,
		collection, and primitive classification for every field or expression.

		Core `Array`, imported/extern, generic, enum, typedef, abstract, `Null<T>`,
		and unresolved types deliberately return `null`; the complete classifier
		continues to own their representation and source-positioned failures.
	**/
	function exactNominalValueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CBodyValueType> {
		return switch type {
			case TInst(reference, parameters) if (parameters.length == 0 && !CBodyArrayRecognition.isCoreArray(reference)):
				final definition = reference.get();
				if (definition.isExtern) {
					null;
				} else {
					final path = definition.pack.concat([definition.name]).join(".");
					final cached = exactNominalValues.get(path);
					if (cached != null) {
						exactNominalCacheHits++;
						cached;
					} else {
						final prepared = definition.isInterface ? CBodyValueType.interfaceReference(interfaceRegistry.require(reference, parameters, position,
							sourcePath, fail, node)) : classRegistry.valueType(reference, parameters, position, ownerModule, sourcePath, fail, node);
						exactNominalValues.set(path, prepared);
						exactNominalCacheMisses++;
						prepared;
					}
				}
			case _: null;
		};
	}

	/**
		Classify one anonymous record, reusing a proven named typedef when safe.

		A non-generic typedef has one transparent Haxe definition for the whole
		request, so later mentions can share its immutable layout and value plan.
		This cache is deliberately request-local: Haxe's compilation server can
		attach a named anonymous field to either its typedef declaration or one
		rebuilt object-literal use in different requests. The typed frontend
		therefore supplies a separate content-keyed declaration-position plan;
		reusing this value/layout plan is not permission to reuse Haxe `Position`
		objects. Generic typedefs and object literals still take the complete
		structural path because their shape can depend on type arguments or local
		typing context.
	**/
	function anonymousValueType(reference:Ref<AnonType>, aliasOwner:Null<CBodyAggregateTypedefOwner>, position:Position, ownerModule:String,
			sourcePath:String, fail:(Position, String) -> Void, node:String):CBodyValueType {
		final cacheKey = aliasOwner == null ? null : aliasOwner.cacheKey;
		if (cacheKey != null) {
			final cached = namedRecordValues.get(cacheKey);
			if (cached != null) {
				namedRecordCacheHits++;
				return cached;
			}
			namedRecordCacheMisses++;
		}
		final fieldSources = aliasOwner == null ? null : namedRecordSources == null ? reflaxe.c.frontend.NamedRecordSourceProvenance.planRecord(aliasOwner.displayName,
			aliasOwner.position, reference) : namedRecordSources;
		final shape = anonymousShape(reference, [], position, ownerModule, sourcePath, fail, node, fieldSources,
			aliasOwner == null ? null : aliasOwner.displayName);
		var aggregate = byShape.get(shape);
		if (aggregate == null) {
			final aggregateOwner = aliasOwner == null ? ownerModule : aliasOwner.modulePath;
			final aggregateSource = sourcePathsByModule.exists(aggregateOwner) ? sourcePathsByModule.get(aggregateOwner) : sourcePath;
			if (aggregateSource == null)
				return rejected(fail, position, '$node:missing-source-for-aggregate-owner:$aggregateOwner');
			final aggregatePosition = aliasOwner == null ? position : aliasOwner.position;
			aggregate = prepareAggregate(reference, shape, aliasOwner == null ? null : aliasOwner.displayName, aggregatePosition, aggregateOwner,
				aggregateSource, fieldSources, fail, node);
			byShape.set(shape, aggregate);
		}
		final prepared = CBodyValueType.aggregate(aggregate);
		if (cacheKey != null)
			namedRecordValues.set(cacheKey, prepared);
		return prepared;
	}

	/**
		Map one exact non-capturing callable signature without erasing its values.

		Optional and rest-style indirect calls need their own argument-completion
		contract, so this first direct-function-pointer slice rejects them instead
		of silently giving C a different calling convention.
	**/
	function directFunctionValueType(type:Type, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):Null<CBodyValueType> {
		return switch type {
			case TFun(arguments, resultType):
				final parameters:Array<CBodyValueType> = [];
				for (index => argument in arguments) {
					if (argument.opt)
						return rejected(fail, position, '$node.function-argument-$index:optional-indirect-call-not-admitted');
					final parameter = valueType(argument.t, position, ownerModule, sourcePath, fail, '$node.function-argument-$index');
					if (parameter.irType == IRTVoid)
						return rejected(fail, position, '$node.function-argument-$index:Void');
					if (parameter.spanElement() != null)
						return rejected(fail, position, '$node.function-argument-$index:borrowed-span-indirect-call-not-admitted');
					if (parameter.functionValue() != null)
						return rejected(fail, position, '$node.function-argument-$index:nested-function-value-not-admitted');
					parameters.push(parameter);
				}
				final result = valueType(resultType, position, ownerModule, sourcePath, fail, '$node.function-result');
				if (result.spanElement() != null)
					return rejected(fail, position, '$node.function-result:borrowed-span-escape');
				if (result.functionValue() != null)
					return rejected(fail, position, '$node.function-result:nested-function-value-not-admitted');
				CBodyValueType.directFunction(parameters, result);
			case _: null;
		};
	}

	static function isSpan(value:AbstractType, parameters:Array<Type>):Bool
		return parameters.length == 1 && value.pack.join(".") == "c" && (value.name == "Span" || value.name == "ConstSpan");

	/**
		Classify the narrow mutable pointer accepted by direct imported-C calls.

		The first slice admits exact scalar carriers because their address has one
		unambiguous C type and no ownership lifecycle. Aggregates, managed values,
		nullable carriers, nested pointers, and `Void` remain fail-closed.
	**/
	function nativeRefValueType(type:Type, position:Position, fail:(Position, String) -> Void, node:String):Null<CBodyValueType> {
		return switch type {
			case TAbstract(reference, parameters) if (reference.get().pack.join(".") == "c" && reference.get().name == "Ref"):
				if (parameters.length != 1)
					rejected(fail, position, '$node:c.Ref-requires-one-pointee');
				final pointee = directPrimitiveValueType(parameters[0]);
				if (pointee == null || pointee.irType == IRTVoid)
					rejected(fail, position, '$node:c.Ref-pointee-requires-direct-non-Void-scalar');
				CBodyValueType.nativeRef(pointee);
			case _: null;
		};
	}

	/** Recognize the one call-scoped mutable `char *` carrier. */
	static function cStringBufferRefValueType(type:Type):Null<CBodyValueType> {
		return switch type {
			case TAbstract(reference, parameters)
				if (parameters.length == 0 && reference.get().pack.join(".") == "c" && reference.get().name == "CStringBufferRef"):
				CBodyValueType.cStringBufferRef();
			case _: null;
		};
	}

	function admittedSpanElement(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CPrimitiveTypeMapping {
		final mapping = admittedPrimitive(type, position, fail, '$node.element');
		return switch mapping.irType {
			case IRTBool | IRTInt(_, _) | IRTFloat(32) | IRTFloat(64): mapping;
			case _: rejected(fail, position, '$node:span-element:${mapping.cSpelling}');
		};
	}

	public function importFunction(callee:TypedExpr, position:Position, sourcePath:String):Null<CPreparedImportFunction>
		return importRegistry == null ? null : importRegistry.functionFor(callee, position, sourcePath);

	public function importStaticConstant(classReference:Ref<ClassType>, fieldReference:Ref<ClassField>, position:Position,
			sourcePath:String):Null<CPreparedImportConstant>
		return importRegistry == null ? null : importRegistry.staticConstantFor(classReference, fieldReference, position, sourcePath);

	public function importEnumConstant(reference:Ref<EnumType>, field:EnumField, position:Position, sourcePath:String):Null<CPreparedImportConstant>
		return importRegistry == null ? null : importRegistry.enumConstantFor(reference, field, position, sourcePath);

	public function canonicalImports():Array<CPreparedImportType>
		return importRegistry == null ? [] : importRegistry.canonicalTypes();

	public function finalizeImports(symbols:CSymbolRegistry):CLoweredImports
		return importRegistry == null ? CLoweredImports.empty() : importRegistry.finalize(symbols);

	public function canonicalEnums():Array<CPreparedBodyEnumInstance>
		return enumRegistry.canonicalEnums();

	/** Copy source provenance so one function can retain only its new ranges. */
	public function enumReasonSnapshot():Map<String, Array<HxcSourceSpan>>
		return enumRegistry.reasonSnapshot();

	/** Restore one function-owned generic-enum provenance range. */
	public function addEnumReason(instanceId:String, reason:HxcSourceSpan):Void
		enumRegistry.addReason(instanceId, reason);

	public function finalizeEnums(symbols:CSymbolRegistry):Array<CLoweredBodyEnum>
		return enumRegistry.finalize(symbols);

	public function canonicalClasses():Array<CPreparedBodyClass>
		return classRegistry.canonicalClasses();

	public function canonicalInterfaces():Array<CPreparedBodyInterface>
		return interfaceRegistry.canonicalInterfaces();

	public function canonicalArrays():Array<CPreparedBodyArray>
		return arrayRegistry.canonicalArrays();

	/** Register the typed callback adapter required by reachable `Array.sort`. */
	public function requireArraySort(value:CPreparedBodyArray):Void
		arrayRegistry.requireSortAdapter(value);

	public function canonicalStringMaps():Array<CPreparedBodyStringMap>
		return stringMapRegistry.canonicalMaps();

	public function canonicalIntMaps():Array<CPreparedBodyIntMap>
		return intMapRegistry.canonicalMaps();

	public function finalizeStringMaps(symbols:CSymbolRegistry):Array<reflaxe.c.lowering.CBodyStringMap.CLoweredBodyStringMap>
		return stringMapRegistry.finalize(symbols);

	public function finalizeArrays(symbols:CSymbolRegistry):Array<reflaxe.c.lowering.CBodyArray.CLoweredBodyArray>
		return arrayRegistry.finalize(symbols);

	public function canonicalBytes():Array<CPreparedBodyBytes>
		return bytesRegistry.canonicalBytes();

	public function canonicalOptionals():Array<CPreparedBodyOptional>
		return optionalRegistry.canonicalOptionals();

	public function finalizeOptionals(symbols:CSymbolRegistry):Array<CLoweredBodyOptional>
		return optionalRegistry.finalize(symbols);

	public function requireInterface(reference:Ref<ClassType>, parameters:Array<Type>, position:Position, sourcePath:String, fail:(Position, String) -> Void,
			node:String):CPreparedBodyInterface
		return interfaceRegistry.require(reference, parameters, position, sourcePath, fail, node);

	public function finalizeClasses(symbols:CSymbolRegistry):Array<CLoweredBodyClass>
		return classRegistry.finalize(symbols);

	public function requireVirtualHeader(root:CPreparedBodyClass, layoutId:String):Void
		classRegistry.requireVirtualHeader(root, layoutId);

	public function completeClassLayouts():Void
		classRegistry.completeLayouts();

	/**
	 * Select stable collector storage for class references returned to a caller.
	 *
	 * Function signatures are prepared before body lowering, so this pass can
	 * settle the representation once for every producer and consumer.
	 */
	public function requireEscapingReturnClasses(type:CBodyValueType):Void
		classRegistry.requireEscapingReturnClasses(type);

	/** Settle the selective GC graph before any function chooses stack or heap construction. */
	public function completeManagedRepresentations(interfaceImplementations:Array<CBodyInterfaceImplementation>):Void
		classRegistry.completeManagedRepresentations(arrayRegistry.canonicalArrays(), enumRegistry.canonicalEnums(), interfaceImplementations);

	public function canonicalAggregates():Array<CPreparedBodyAggregate> {
		final values = [for (aggregate in byShape) aggregate];
		values.sort((left, right) -> compareUtf8(left.digest, right.digest));
		final result:Array<CPreparedBodyAggregate> = [];
		final emitted:Map<String, Bool> = [];
		for (aggregate in values) {
			appendDependencies(aggregate, result, emitted);
		}
		return result;
	}

	/**
		Select one exact closure carrier for a synchronous callback parameter.

		The carrier is shared only inside the module that owns the receiving
		parameter. That keeps split-C ownership deterministic while callers use the
		callee's already-prepared parameter mapping. The `invoke` field includes one
		leading opaque context argument; `IRRStackClosure` separately records the
		source-visible signature used to validate indirect calls.
	**/
	public function requireStackClosureCarrier(direct:CBodyValueType, ownerModule:String, sourcePath:String, position:Position,
			fail:(Position, String) -> Void, node:String):CBodyValueType {
		final signature = switch direct.kind {
			case CBVKFunction(parameters, result): {parameters: parameters, result: result};
			case _: return rejected(fail, position, '$node:requires-direct-function-signature');
		};
		final signatureKey = '(${signature.parameters.map(value -> canonicalPart(value.cSpelling)).join("")})->${canonicalPart(signature.result.cSpelling)}';
		final shape = 'stack-closure-carrier-v1(${canonicalPart(ownerModule)}${canonicalPart(signatureKey)})';
		var carrier = byShape.get(shape);
		if (carrier == null) {
			final contextType = CBodyValueType.closureContext();
			final invokeType = CBodyValueType.directFunction([contextType].concat(signature.parameters), signature.result);
			carrier = prepareSyntheticAggregate(shape, '${ownerModule}.StackClosure', ownerModule, sourcePath, position,
				IRRStackClosure(signature.parameters.map(value -> value.irType), signature.result.irType), [
				{
					name: "invoke",
					readableName: "invoke",
					type: invokeType,
					mutable: false,
					position: position
				},
				{
					name: "context",
					readableName: "context",
					type: contextType,
					mutable: false,
					position: position
				}
			], "stack-closure");
			byShape.set(shape, carrier);
		}
		return CBodyValueType.stackClosure(carrier, signature.parameters, signature.result);
	}

	/**
		Build the private stack record that points at each captured Haxe variable.

		Fields contain pointers rather than value copies because Haxe closures
		observe later writes and may update the same variable. The environment owns
		nothing: its caller keeps both the record and every pointed-to value alive
		for the one proven synchronous call.
	**/
	public function requireStackClosureEnvironment(literalIdentity:String, displayName:String, captures:Array<CBodyStackClosureCapture>, ownerModule:String,
			sourcePath:String, position:Position):CPreparedBodyAggregate {
		final shape = 'stack-closure-environment-v1(${canonicalPart(ownerModule)}${canonicalPart(literalIdentity)}'
			+ captures.map(capture -> canonicalPart(capture.identity) + canonicalPart(capture.type.cSpelling)).join("")
			+ ")";
		final existing = byShape.get(shape);
		if (existing != null)
			return existing;
		final fields:Array<CBodySyntheticAggregateField> = [];
		for (index => capture in captures)
			fields.push({
				name: 'capture.$index',
				readableName: capture.sourceName,
				type: CBodyValueType.closureCapturePointer(capture.type),
				mutable: false,
				position: capture.position
			});
		final environment = prepareSyntheticAggregate(shape, displayName, ownerModule, sourcePath, position, IRRDirect, fields, "stack-closure-environment");
		byShape.set(shape, environment);
		return environment;
	}

	/** Prepare a compiler-owned struct through the same naming and layout path as records. */
	function prepareSyntheticAggregate(shape:String, displayName:String, ownerModule:String, sourcePath:String, position:Position,
			representation:HxcIRRepresentation, fields:Array<CBodySyntheticAggregateField>, role:String):CPreparedBodyAggregate {
		final digest = Sha256.encode(shape);
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		final typeRequest = new CSymbolRequest(CSKType, ["compiler", role, digest], CNSTag("translation-unit"), CSVInternal, null, [], [], null,
			displayName.split("."));
		context.symbols.register(typeRequest);
		final aggregate = new CPreparedBodyAggregate(shape, digest, displayName, ownerModule, source, typeRequest, representation);
		for (index => field in fields) {
			final request = new CSymbolRequest(CSKField, ["compiler", role, digest, field.name], CNSMember(aggregate.declarationId), CSVInternal, null, [],
				[], index, [field.readableName]);
			context.symbols.register(request);
			aggregate.fields.push(new CPreparedBodyAggregateField(field.name, field.type, field.mutable,
				HaxeSourceSpan.fromPosition(field.position, sourcePath), request));
		}
		return aggregate;
	}

	public function finalize(symbols:CSymbolRegistry):Array<CLoweredBodyAggregate> {
		final result:Array<CLoweredBodyAggregate> = [];
		for (aggregate in canonicalAggregates()) {
			result.push(new CLoweredBodyAggregate(aggregate, symbols.identifierFor(aggregate.typeRequest),
				aggregate.fields.map(field -> new CLoweredBodyAggregateField(field.name, field.type, field.mutable, symbols.identifierFor(field.request))),
				identifierOrNull(symbols, aggregate.retainRequest), identifierOrNull(symbols, aggregate.destroyRequest),
				identifierOrNull(symbols, aggregate.retainParameterRequest), identifierOrNull(symbols, aggregate.destroyParameterRequest),
				identifierOrNull(symbols, aggregate.retainStatusRequest)));
		}
		return result;
	}

	static function identifierOrNull(symbols:CSymbolRegistry, request:Null<CSymbolRequest>):Null<CIdentifier>
		return request == null ? null : symbols.identifierFor(request);

	function prepareAggregate(reference:Ref<AnonType>, shape:String, displayName:Null<String>, position:Position, ownerModule:String, sourcePath:String,
			fieldSources:Null<reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourcePlan>, fail:(Position, String) -> Void,
			node:String):CPreparedBodyAggregate {
		final digest = Sha256.encode(shape);
		final source = HaxeSourceSpan.fromPosition(position, sourcePath);
		final readableName = displayName == null ? ["anonymous", "record", digest.substr(0, 12)] : displayName.split(".");
		final typeRequest = new CSymbolRequest(CSKType, ["compiler", "closed-record", digest], CNSTag("translation-unit"), CSVInternal, null, [], [], null,
			readableName);
		context.symbols.register(typeRequest);
		final aggregate = new CPreparedBodyAggregate(shape, digest, displayName, ownerModule, source, typeRequest);
		final fields = reference.get().fields.copy();
		fields.sort((left, right) -> compareUtf8(left.name, right.name));
		for (index in 0...fields.length) {
			final field = fields[index];
			final fieldPosition = displayName == null ? field.pos : requireNamedRecordFieldPosition(fieldSources, displayName, field.name, field.pos);
			final fieldType = valueType(field.type, fieldPosition, ownerModule, sourcePath, fail, '$node.field:${field.name}');
			if (fieldType.spanElement() != null) {
				return rejected(fail, fieldPosition, '$node.field:${field.name}:borrowed-span-field-escape');
			}
			if (fieldType.irType == IRTVoid) {
				return rejected(fail, fieldPosition, '$node.field:${field.name}:Void-not-an-object-type');
			}
			final mutable = switch field.kind {
				case FVar(_, write): isWritable(write);
				case FMethod(_): rejected(fail, fieldPosition, '$node.field:${field.name}:method');
			};
			final request = new CSymbolRequest(CSKField, ["compiler", "closed-record", digest, field.name], CNSMember(aggregate.declarationId), CSVInternal,
				null, [], [], index, [field.name]);
			context.symbols.register(request);
			aggregate.fields.push(new CPreparedBodyAggregateField(field.name, fieldType, mutable, HaxeSourceSpan.fromPosition(fieldPosition, sourcePath),
				request));
		}
		for (field in aggregate.fields) {
			if (valueHasManagedLifetime(field.type)) {
				aggregate.managedLifetime = true;
				break;
			}
		}
		if (aggregate.managedLifetime)
			registerAggregateLifecycle(aggregate);
		return aggregate;
	}

	function requireNamedRecordFieldPosition(plan:Null<reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourcePlan>, declarationPath:String,
			fieldName:String, fallback:Position):Position {
		if (plan == null) {
			throw new reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourceProvenanceError('Named record `$declarationPath` reached lowering without a typed-program source plan.',
				fallback);
		}
		return plan.requireFieldPosition(declarationPath, fieldName, fallback);
	}

	static function valueHasManagedLifetime(value:CBodyValueType):Bool
		return switch value.kind {
			case CBVKManagedString(_) | CBVKArray(_) | CBVKBytes(_): true;
			case CBVKEnum(enumValue): enumValue.managedLifetime;
			case CBVKAggregate(aggregate): aggregate.managedLifetime;
			case CBVKOptional(optional): optional.managedLifetime;
			case _: false;
		};

	function registerAggregateLifecycle(value:CPreparedBodyAggregate):Void {
		final root = ["compiler", "closed-record", value.digest, "lifecycle"];
		value.retainRequest = new CSymbolRequest(CSKMethod, root.concat(["retain"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 0,
			["record", value.digest.substr(0, 8), "retain"]);
		value.destroyRequest = new CSymbolRequest(CSKMethod, root.concat(["destroy"]), CNSOrdinary("translation-unit"), CSVInternal, null, [], [], 1,
			["record", value.digest.substr(0, 8), "destroy"]);
		context.symbols.register(value.retainRequest);
		context.symbols.register(value.destroyRequest);
		value.retainParameterRequest = aggregateLifecycleLocal(value.retainRequest, "value", 0);
		value.destroyParameterRequest = aggregateLifecycleLocal(value.destroyRequest, "value", 0);
		value.retainStatusRequest = aggregateLifecycleLocal(value.retainRequest, "operation_status", 1);
	}

	function aggregateLifecycleLocal(owner:CSymbolRequest, role:String, ordinal:Int):CSymbolRequest {
		final request = new CSymbolRequest(CSKLocal, owner.qualifiedName.concat([role]), CNSOrdinary(owner.stableKey()), CSVInternal, null, [], [], ordinal,
			[role]);
		context.symbols.register(request);
		return request;
	}

	function anonymousShape(reference:Ref<AnonType>, stack:Array<Ref<AnonType>>, position:Position, ownerModule:String, sourcePath:String,
			fail:(Position, String) -> Void, node:String, ?fieldSources:reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourcePlan,
			?declarationPath:String):String {
		for (active in stack) {
			if (active == reference) {
				return rejected(fail, position, '$node:recursive-by-value-shape');
			}
		}
		final value = reference.get();
		switch value.status {
			case AClosed | AConst:
			case AOpened:
				return rejected(fail, position, '$node:open-anonymous-shape');
			case AExtend(_):
				return rejected(fail, position, '$node:extended-anonymous-shape');
			case AClassStatics(_) | AEnumStatics(_) | AAbstractStatics(_):
				return rejected(fail, position, '$node:static-container-not-record');
		}
		if (value.fields.length == 0) {
			return rejected(fail, position, '$node:empty-anonymous-shape-not-strict-c11');
		}
		final fields = value.fields.copy();
		fields.sort((left, right) -> compareUtf8(left.name, right.name));
		final nextStack = stack.copy();
		nextStack.push(reference);
		final parts:Array<String> = [];
		for (field in fields) {
			final fieldPosition = declarationPath == null ? field.pos : requireNamedRecordFieldPosition(fieldSources, declarationPath, field.name, field.pos);
			final access = switch field.kind {
				case FVar(read, write): 'var:${accessKey(read)}:${accessKey(write)}:${field.isFinal ? "final" : "nonfinal"}';
				case FMethod(_): return rejected(fail, fieldPosition, '$node.field:${field.name}:method');
			};
			parts.push(canonicalPart(field.name)
				+ canonicalPart(access)
				+ canonicalPart(typeShape(field.type, nextStack, fieldPosition, ownerModule, sourcePath, fail, '$node.field:${field.name}')));
		}
		return 'closed-record-v1(${parts.join("")})';
	}

	function typeShape(type:Type, stack:Array<Ref<AnonType>>, position:Position, ownerModule:String, sourcePath:String, fail:(Position, String) -> Void,
			node:String):String {
		final imported = importRegistry == null ? null : importRegistry.valueType(type, position, ownerModule, sourcePath, fail, node);
		if (imported != null)
			return importedTypeShape(imported);
		final stringIdentity = staticStringIdentity(type);
		if (stringIdentity != null)
			return 'static-haxe-string-view(${canonicalPart(stringIdentity)})';
		final resolved = unwrapAliases(type, position, fail, node);
		final resolvedImport = importRegistry == null ? null : importRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (resolvedImport != null)
			return importedTypeShape(resolvedImport);
		final array = arrayRegistry.valueType(resolved, position, ownerModule, sourcePath, fail, node);
		if (array != null)
			return 'haxe-array-reference(${canonicalPart(array.semanticKey)})';
		if (bytesRegistry.valueType(resolved, position, ownerModule, sourcePath) != null)
			return "haxe-bytes-reference-v1";
		return switch resolved {
			case TAnonymous(reference): anonymousShape(reference, stack, position, ownerModule, sourcePath, fail, node);
			case TAbstract(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Null" && parameters.length == 1):
				final payload = valueType(parameters[0], position, ownerModule, sourcePath, fail, '$node.nullable');
				if (payload.hasExactNullCarrier()) {
					typeShape(parameters[0], stack, position, ownerModule, sourcePath, fail, '$node.nullable-carrier');
				} else {
					final optional = optionalRegistry.require(payload, ownerModule, sourcePath, position, fail, node);
					'direct-optional(${canonicalPart(optional.semanticKey)})';
				}
			case TEnum(reference, parameters):
				final value = enumRegistry.require(reference, parameters, position, ownerModule, sourcePath, fail, node);
				// Haxe enums are nominal: two declarations with the same constructors are
				// still different types. Keep the registry's complete semantic key so
				// structural record deduplication cannot merge records whose by-value C
				// fields use incompatible native-enum or tagged-union definitions.
				'haxe-enum(${canonicalPart(value.shapeKey)})';
			case TAbstract(reference, parameters) if (!reference.get().meta.has(":coreType")):
				// Haxe abstracts are compile-time types over another representation.
				// Use that representation in the record's structural identity just as
				// `valueType` does while preparing the field. In particular, an
				// `enum abstract ... (Int)` remains a useful closed Haxe domain while
				// occupying one ordinary Int field in generated C.
				final definition = reference.get();
				typeShape(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), stack, position, ownerModule, sourcePath,
					fail, '$node.abstract-representation');
			case _:
				final mapping = admittedPrimitive(resolved, position, fail, node);
				mapping.irType == IRTVoid ? rejected(fail, position, '$node:Void-not-an-object-type') : primitiveTypeKey(mapping.irType);
		};
	}

	/** Use the validated imported semantic identity in the enclosing record hash. */
	static function importedTypeShape(type:CBodyValueType):String {
		return switch type.irType {
			case IRTInstance(instanceId): 'c-import(${canonicalPart(instanceId)})';
			case _: throw new CBodyEmissionError("C import registry returned a non-value record field type");
		};
	}

	/**
		Recognize `String`, its documentary `Null` spelling, and nominal String abstracts.

		The returned name is a source identity, not a C type name. Keeping it in
		plans means diagnostics can still say `LogicalPath` even though C stores the
		same immutable four-field view as ordinary `String`: data, byte length,
		trailing-NUL knowledge, and an optional backing owner.

		Haxe's default legacy null safety makes reference types nullable already,
		so `Null<String>` does not introduce a second tagged representation. The
		shared String carrier represents `null` directly through a null data
		pointer and keeps a real empty String distinct through a non-null
		zero-length pointer.
	**/
	public static function staticStringIdentity(type:Type, depth:Int = 0, ?outerIdentity:String):Null<String> {
		if (depth > 32)
			return null;
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : staticStringIdentity(resolved, depth + 1, outerIdentity);
			case TLazy(resolve):
				staticStringIdentity(resolve(), depth + 1, outerIdentity);
			case TType(reference, parameters):
				final definition = reference.get();
				staticStringIdentity(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), depth + 1, outerIdentity);
			case TInst(reference, parameters): final definition = reference.get(); parameters.length == 0 && definition.pack.length == 0 && definition.name == "String" ? (outerIdentity == null ? "String" : outerIdentity) : null;
			case TAbstract(reference, parameters) if (reference.get().pack.length == 0 && reference.get().name == "Null" && parameters.length == 1):
				staticStringIdentity(parameters[0], depth + 1, outerIdentity);
			case TAbstract(reference, parameters) if (!reference.get().meta.has(":coreType")):
				final definition = reference.get();
				final identity = outerIdentity == null ? definition.pack.concat([definition.name]).join(".") : outerIdentity;
				staticStringIdentity(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), depth + 1, identity);
			case _:
				null;
		};
	}

	function admittedPrimitive(type:Type, position:Position, fail:(Position, String) -> Void, node:String):CPrimitiveTypeMapping {
		return switch CPrimitiveTypeMapper.map(type, context.profile) {
			case CTPrimitive(mapping):
				final admitted = mapping.nullability == CPNonNullable;
				if (!admitted) {
					return rejected(fail, position, '$node:${mapping.cSpelling}');
				}
				mapping;
			case CTReference(identity, nullable):
				rejected(fail, position, '$node:reference-$identity-${nullable ? "nullable" : "non-null"}');
			case CTNativePointer(identity, nullable):
				rejected(fail, position, '$node:native-pointer-$identity-${nullable ? "nullable" : "non-null"}');
			case CTUnsupported(reason):
				rejected(fail, position, '$node:$reason');
		};
	}

	static function unwrapAliases(type:Type, position:Position, fail:(Position, String) -> Void, node:String):Type {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? rejected(fail, position, '$node:unresolved-monomorph') : unwrapAliases(resolved, position, fail, node);
			case TLazy(resolve): unwrapAliases(resolve(), position, fail, node);
			case TType(reference, parameters):
				final definition = reference.get();
				unwrapAliases(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), position, fail, node);
			case _: type;
		};
	}

	/**
		Returns the innermost typedef module that actually declares an anonymous
		record. Consumers may mention an alias of that typedef, but the generated
		complete C definition should remain with the module that owns the fields.
	**/
	static function anonymousTypedefOwner(type:Type, ?candidate:CBodyAggregateTypedefOwner):Null<CBodyAggregateTypedefOwner> {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? null : anonymousTypedefOwner(resolved, candidate);
			case TLazy(resolve): anonymousTypedefOwner(resolve(), candidate);
			case TType(reference, parameters):
				final definition = reference.get();
				anonymousTypedefOwner(haxe.macro.TypeTools.applyTypeParameters(definition.type, definition.params, parameters), {
					modulePath: definition.module,
					displayName: definition.pack.concat([definition.name]).join("."),
					position: definition.pos,
					cacheKey: definition.params.length == 0
					&& parameters.length == 0 ? definition.pack.concat([definition.name]).join(".") : null});
			case TAnonymous(_): candidate;
			case _: null;
		};
	}

	static function isWritable(access:VarAccess):Bool {
		return switch access {
			case AccNo | AccNever: false;
			case AccNormal | AccResolve | AccCall | AccPrivateCall | AccInline | AccRequire(_, _) | AccCtor: true;
		};
	}

	static function accessKey(access:VarAccess):String {
		return switch access {
			case AccNormal: "normal";
			case AccNo: "none";
			case AccNever: "never";
			case AccResolve: "resolve";
			case AccCall: "call";
			case AccPrivateCall: "private-call";
			case AccInline: "inline";
			case AccRequire(requirement, message): 'require:${canonicalPart(requirement)}:${canonicalPart(message == null ? "" : message)}';
			case AccCtor: "constructor";
		};
	}

	function appendDependencies(aggregate:CPreparedBodyAggregate, result:Array<CPreparedBodyAggregate>, emitted:Map<String, Bool>):Void {
		if (emitted.exists(aggregate.declarationId)) {
			return;
		}
		for (field in aggregate.fields) {
			final dependency = field.type.aggregateValue();
			if (dependency != null) {
				appendDependencies(dependency, result, emitted);
			}
		}
		emitted.set(aggregate.declarationId, true);
		result.push(aggregate);
	}

	static function primitiveTypeKey(type:HxcIRTypeRef):String {
		return switch type {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTFloat(width): 'f$width';
			case IRTVoid: "void";
			case _: throw new CBodyEmissionError("anonymous record field resolved to a non-direct primitive type");
		};
	}

	static function canonicalPart(value:String):String
		return '${Bytes.ofString(value).length}:$value';

	static function compareUtf8(left:String, right:String):Int {
		return reflaxe.c.CUtf8Order.compare(left, right);
	}

	static function rejected<T>(fail:(Position, String) -> Void, position:Position, node:String):T {
		fail(position, node);
		throw new CBodyEmissionError("anonymous record rejection callback returned unexpectedly");
	}
}
#else
class CBodyAggregate {
	public function new() {}
}
#end
