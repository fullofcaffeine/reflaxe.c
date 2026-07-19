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
	CPCFloat32;
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
	CPUFloat32Narrow;
	CPUFloat32Widen;
	CPUNullableInject;
	CPUNullableUnwrap;
}

enum CPrimitiveConversionMeaning {
	CPExact;
	CPModuloBits(targetWidth:Int);
	CPTwosComplementBits(targetWidth:Int);
	CPSaturatingTruncate;
	CPCheckedRange;
	CPRoundToBinary32;
	CPWidenToBinary64;
	CPInjectPresent;
	CPUnwrapPresent;
}

/** Source-level unary operators whose primitive meaning is fixed by this target. */
enum CPrimitiveUnaryOperator {
	CPUONegate;
	CPUOBitwiseNot;
	CPUOLogicalNot;
}

/** Source-level binary operators whose primitive meaning is fixed by this target. */
enum CPrimitiveBinaryOperator {
	CPBOAdd;
	CPBOSubtract;
	CPBOMultiply;
	CPBODivide;
	CPBOModulo;
	CPBOShiftLeft;
	CPBOShiftRight;
	CPBOUnsignedShiftRight;
	CPBOBitAnd;
	CPBOBitOr;
	CPBOBitXor;
	CPBOEqual;
	CPBONotEqual;
	CPBOLess;
	CPBOLessEqual;
	CPBOGreater;
	CPBOGreaterEqual;
}

/** Closed identities for compiler-emitted helpers; none of these select `hxrt`. */
enum CPrimitiveHelperKind {
	CPHU32ToI32Bits;
	CPHI32Add;
	CPHI32Subtract;
	CPHI32Multiply;
	CPHI32Negate;
	CPHI32Modulo;
	CPHU32Modulo;
	CPHI32ShiftLeft;
	CPHI32ShiftRight;
	CPHI32UnsignedShiftRight;
	CPHI32BitAnd;
	CPHI32BitOr;
	CPHI32BitXor;
	CPHI32BitwiseNot;
	CPHF64Divide;
	CPHF64Modulo;
	CPHF64ToI32Saturating;
}

enum CPrimitiveUnaryOperationResult {
	CPUOperationAllowed(decision:CPrimitiveUnaryOperationDecision);
	CPUOperationRejected(reason:String);
}

enum CPrimitiveBinaryOperationResult {
	CPBOperationAllowed(decision:CPrimitiveBinaryOperationDecision);
	CPBOperationRejected(reason:String);
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

class CPrimitiveUnaryOperationDecision {
	public final operand:CPrimitiveTypeMapping;
	public final result:CPrimitiveTypeMapping;
	public final operationId:String;
	public final implementation:HxcIRImplementation;

	public function new(operand:CPrimitiveTypeMapping, result:CPrimitiveTypeMapping, operationId:String, implementation:HxcIRImplementation) {
		this.operand = operand;
		this.result = result;
		this.operationId = operationId;
		this.implementation = implementation;
	}
}

class CPrimitiveBinaryOperationDecision {
	public final leftOperand:CPrimitiveTypeMapping;
	public final rightOperand:CPrimitiveTypeMapping;
	public final result:CPrimitiveTypeMapping;
	public final operationId:String;
	public final implementation:HxcIRImplementation;

	public function new(leftOperand:CPrimitiveTypeMapping, rightOperand:CPrimitiveTypeMapping, result:CPrimitiveTypeMapping, operationId:String,
			implementation:HxcIRImplementation) {
		this.leftOperand = leftOperand;
		this.rightOperand = rightOperand;
		this.result = result;
		this.operationId = operationId;
		this.implementation = implementation;
	}
}
