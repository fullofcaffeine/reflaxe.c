package reflaxe.c.semantics;

#if (macro || reflaxe_runtime)
import haxe.macro.Type;
import haxe.macro.TypeTools;
import reflaxe.c.CProfile;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveMappingResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveNullability;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSourceType;
import reflaxe.c.semantics.CPrimitiveTypes.CTypedSourceType;

/** Recognizes primitive contracts from real Haxe compiler `Type` values. */
class CPrimitiveTypeMapper {
	public static function map(type:Type, profile:CProfile):CTypedSourceType {
		return mapType(type, profile);
	}

	static function mapType(type:Type, profile:CProfile):CTypedSourceType {
		return switch type {
			case TMono(reference):
				final resolved = reference.get();
				resolved == null ? CTUnsupported("unresolved monomorph at primitive lowering") : mapType(resolved, profile);
			case TLazy(resolve):
				mapType(resolve(), profile);
			case TType(reference, parameters):
				final definition = reference.get();
				mapType(TypeTools.applyTypeParameters(definition.type, definition.params, parameters), profile);
			case TAbstract(reference, parameters):
				mapAbstract(reference.get(), parameters, profile);
			case TInst(reference, _):
				final classType = reference.get();
				switch classType.kind {
					case KTypeParameter(_): CTUnsupported('unresolved type parameter `${qualifiedName(classType)}`');
					case _: CTReference(qualifiedName(classType), false);
				}
			case TEnum(reference, _):
				CTUnsupported('enum `${qualifiedName(reference.get())}` awaits its representation-selection pass');
			case TFun(_, _):
				CTUnsupported("function values await closure/function representation lowering");
			case TAnonymous(_):
				CTUnsupported("anonymous structures await aggregate representation lowering");
			case TDynamic(_):
				CTUnsupported("the dynamic source semantic type cannot stand in for a primitive");
		};
	}

	static function mapAbstract(abstractType:AbstractType, parameters:Array<Type>, profile:CProfile):CTypedSourceType {
		final identity = qualifiedName(abstractType);
		if (identity == "Null") {
			if (parameters.length != 1) {
				return CTUnsupported("Null must have exactly one type argument");
			}
			return makeNullable(mapType(parameters[0], profile), profile);
		}

		final sourceType = primitiveSource(identity);
		if (sourceType != null) {
			return mappingResult(CPrimitiveSemantics.mapping(profile, sourceType, CPNonNullable));
		}
		if (abstractType.pack.length == 1 && abstractType.pack[0] == "c" && isNativePointer(abstractType.name)) {
			if (parameters.length != 1) {
				return CTUnsupported('$identity must have exactly one pointee type');
			}
			return CTNativePointer(TypeTools.toString(parameters[0]), abstractType.name == "NullablePtr");
		}
		return CTUnsupported('abstract `$identity` is not an admitted primitive or native pointer contract');
	}

	static function makeNullable(source:CTypedSourceType, profile:CProfile):CTypedSourceType {
		return switch source {
			case CTPrimitive(mapping):
				mappingResult(CPrimitiveSemantics.mapping(profile, mapping.sourceType, CPNullable));
			case CTReference(identity, _):
				CTReference(identity, true);
			case CTNativePointer(pointeeIdentity, _):
				CTNativePointer(pointeeIdentity, true);
			case CTUnsupported(reason):
				CTUnsupported(reason);
		}
	}

	static function mappingResult(result:CPrimitiveMappingResult):CTypedSourceType {
		return switch result {
			case CPMappingAvailable(mapping): CTPrimitive(mapping);
			case CPMappingRejected(reason): CTUnsupported(reason);
		}
	}

	static function primitiveSource(identity:String):Null<CPrimitiveSourceType> {
		return switch identity {
			case "Void": CPHaxeVoid;
			case "Bool": CPHaxeBool;
			case "Int": CPHaxeInt;
			case "UInt": CPHaxeUInt;
			case "Float": CPHaxeFloat;
			case "c.Float32": CPCFloat32;
			case "c.Int8": CPCExactInteger(8, true);
			case "c.Int16": CPCExactInteger(16, true);
			case "c.Int32": CPCExactInteger(32, true);
			case "c.Int64": CPCExactInteger(64, true);
			case "c.UInt8": CPCExactInteger(8, false);
			case "c.UInt16": CPCExactInteger(16, false);
			case "c.UInt32": CPCExactInteger(32, false);
			case "c.UInt64": CPCExactInteger(64, false);
			case "c.Size": CPCSize;
			case "c.PtrDiff": CPCPtrDiff;
			case "c.IntPtr": CPCIntPtr;
			case "c.UIntPtr": CPCUIntPtr;
			case _: null;
		}
	}

	static function isNativePointer(name:String):Bool {
		return name == "Ptr" || name == "ConstPtr" || name == "Ref" || name == "ConstRef" || name == "NullablePtr";
	}

	static function qualifiedName(base:BaseType):String
		return base.pack.concat([base.name]).join(".");
}
#else
class CPrimitiveTypeMapper {}
#end
