#if macro
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CProfile;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypeMapper;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveStorage;
import reflaxe.c.semantics.CPrimitiveTypes.CTypedSourceType;

typedef PrimitiveTypeProbeRecord = {
	final field:String;
	final kind:String;
	final sourceType:Null<String>;
	final nullable:Bool;
	final storage:String;
	final cType:Null<String>;
	final identity:Null<String>;
	final profileInvariant:Bool;
}
#end

class PrimitiveTypeProbe {
	#if macro
	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("PrimitiveFixture") {
			case TInst(reference, _): reference.get();
			case _: Context.fatalError("PrimitiveFixture must be a class", Context.currentPos());
		}
		final fields = fixture.fields.get().copy();
		fields.sort((left, right) -> compareStrings(left.name, right.name));
		final records:Array<PrimitiveTypeProbeRecord> = [];
		for (field in fields) {
			final type = field.name == "returnsVoid" ? functionResult(field.type) : field.type;
			final portable = CPrimitiveTypeMapper.map(type, CProfile.Portable);
			final metal = CPrimitiveTypeMapper.map(type, CProfile.Metal);
			final portableRecord = record(field.name, portable);
			final metalRecord = record(field.name, metal);
			records.push({
				field: portableRecord.field,
				kind: portableRecord.kind,
				sourceType: portableRecord.sourceType,
				nullable: portableRecord.nullable,
				storage: portableRecord.storage,
				cType: portableRecord.cType,
				identity: portableRecord.identity,
				profileInvariant: Json.stringify(portableRecord) == Json.stringify(metalRecord)
			});
		}
		Sys.println("HXC_PRIMITIVE_TYPE_PROBE=" + Json.stringify(records));
	}

	static function functionResult(type:Type):Type {
		return switch type {
			case TFun(_, result): result;
			case _: Context.fatalError("returnsVoid must remain a function", Context.currentPos());
		}
	}

	static function record(field:String, source:CTypedSourceType):PrimitiveTypeProbeRecord {
		return switch source {
			case CTPrimitive(mapping):
				{
					field: field,
					kind: "primitive",
					sourceType: CPrimitiveSemantics.sourceTypeKey(mapping.sourceType),
					nullable: mapping.nullability == CPNullable,
					storage: storage(mapping.storage),
					cType: mapping.cSpelling,
					identity: null,
					profileInvariant: true
				};
			case CTReference(identity, nullable):
				{
					field: field,
					kind: "reference",
					sourceType: null,
					nullable: nullable,
					storage: "pointer",
					cType: null,
					identity: identity,
					profileInvariant: true
				};
			case CTNativePointer(pointeeIdentity, nullable):
				{
					field: field,
					kind: "native-pointer",
					sourceType: null,
					nullable: nullable,
					storage: "pointer",
					cType: null,
					identity: pointeeIdentity,
					profileInvariant: true
				};
			case CTUnsupported(reason):
				{
					field: field,
					kind: "unsupported",
					sourceType: null,
					nullable: false,
					storage: "none",
					cType: null,
					identity: reason,
					profileInvariant: true
				};
		}
	}

	static function storage(value:CPrimitiveStorage):String {
		return switch value {
			case CPStorageVoid: "no-value";
			case CPStorageScalar: "direct-scalar";
			case CPStorageTaggedOptional: "tagged-optional";
		}
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);
	#else
	public static function install():Void {}
	#end
}
