package reflaxe.c.semantics;

import reflaxe.c.ir.HxcIR.HxcIRConversionKind;
import reflaxe.c.ir.HxcIR.HxcIRImplementation;
import reflaxe.c.ir.HxcIR.HxcIRTypeRef;

/** A source-level scalar whose representation is owned by the C target. */
enum CPrimitiveSourceType {
	CPHaxeVoid;
	CPHaxeBool;
	CPHaxeInt;
	CPHaxeUInt;
	CPHaxeFloat;
	CPCExactInteger(width:Int, signed:Bool);
	CPCSize;
	CPCPtrDiff;
	CPCIntPtr;
	CPCUIntPtr;
}

enum CPrimitiveNullability {
	CPNonNullable;
	CPNullable;
}

enum CPrimitiveWidth {
	CPWidthNone;
	CPWidthExact(bits:Int);
	CPWidthTargetAbi;
	CPWidthAbiDefined;
}

enum CPrimitiveSignedness {
	CPSignNone;
	CPSignBoolean;
	CPSignSigned;
	CPSignUnsigned;
}

enum CPrimitiveStorage {
	CPStorageVoid;
	CPStorageScalar;
	CPStorageTaggedOptional;
}

enum CPrimitiveConversionUse {
	CPUImplicit;
	CPUStdInt;
	CPUWrapping;
	CPUChecked;
	CPUNullableInject;
	CPUNullableUnwrap;
}

enum CPrimitiveConversionMeaning {
	CPExact;
	CPModuloBits(targetWidth:Int);
	CPTwosComplementBits(targetWidth:Int);
	CPSaturatingTruncate;
	CPCheckedRange;
	CPInjectPresent;
	CPUnwrapPresent;
}

/** A closed result keeps unsupported conversions out of placeholder IR. */
enum CPrimitiveConversionResult {
	CPConversionElided;
	CPConversionAllowed(decision:CPrimitiveConversionDecision);
	CPConversionRejected(reason:String);
}

enum CPrimitiveMappingResult {
	CPMappingAvailable(mapping:CPrimitiveTypeMapping);
	CPMappingRejected(reason:String);
}

/** The typed compiler input classifications this bead can decide completely. */
enum CTypedSourceType {
	CTPrimitive(mapping:CPrimitiveTypeMapping);
	CTReference(identity:String, nullable:Bool);
	CTNativePointer(pointeeIdentity:String, nullable:Bool);
	CTUnsupported(reason:String);
}

class CPrimitiveTypeMapping {
	public final sourceType:CPrimitiveSourceType;
	public final nullability:CPrimitiveNullability;
	public final cSpelling:String;
	public final requiredHeader:Null<String>;
	public final width:CPrimitiveWidth;
	public final signedness:CPrimitiveSignedness;
	public final storage:CPrimitiveStorage;
	public final irType:HxcIRTypeRef;

	public function new(sourceType:CPrimitiveSourceType, nullability:CPrimitiveNullability, cSpelling:String, requiredHeader:Null<String>,
			width:CPrimitiveWidth, signedness:CPrimitiveSignedness, storage:CPrimitiveStorage, irType:HxcIRTypeRef) {
		this.sourceType = sourceType;
		this.nullability = nullability;
		this.cSpelling = cSpelling;
		this.requiredHeader = requiredHeader;
		this.width = width;
		this.signedness = signedness;
		this.storage = storage;
		this.irType = irType;
	}
}

class CPrimitiveConversionDecision {
	public final source:CPrimitiveTypeMapping;
	public final target:CPrimitiveTypeMapping;
	public final use:CPrimitiveConversionUse;
	public final meaning:CPrimitiveConversionMeaning;
	public final irKind:HxcIRConversionKind;
	public final implementation:HxcIRImplementation;
	public final failureRequired:Bool;

	public function new(source:CPrimitiveTypeMapping, target:CPrimitiveTypeMapping, use:CPrimitiveConversionUse, meaning:CPrimitiveConversionMeaning,
			irKind:HxcIRConversionKind, implementation:HxcIRImplementation, failureRequired:Bool) {
		this.source = source;
		this.target = target;
		this.use = use;
		this.meaning = meaning;
		this.irKind = irKind;
		this.implementation = implementation;
		this.failureRequired = failureRequired;
	}
}
