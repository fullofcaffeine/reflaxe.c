package reflaxe.c.semantics;

import reflaxe.c.CProfile;
import reflaxe.c.ir.HxcIR.HxcIRAbiIntegerKind;
import reflaxe.c.ir.HxcIR.HxcIRConversionKind;
import reflaxe.c.ir.HxcIR.HxcIRImplementation;
import reflaxe.c.ir.HxcIR.HxcIRNullableRepresentation;
import reflaxe.c.ir.HxcIR.HxcIRTypeRef;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionDecision;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionMeaning;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveConversionUse;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveMappingResult;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveNullability;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSignedness;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveSourceType;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveStorage;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveTypeMapping;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveWidth;

typedef CPrimitiveWidthRecord = {
	final kind:String;
	final bits:Null<Int>;
}

typedef CPrimitiveRepresentationRecord = {
	final sourceType:String;
	final profiles:Array<String>;
	final profileDifference:String;
	final cType:String;
	final requiredHeaders:Array<String>;
	final width:CPrimitiveWidthRecord;
	final signedness:String;
	final storage:String;
	final hxcIrType:String;
	final runtimeFeatures:Array<String>;
}

typedef CPrimitiveConversionRecord = {
	final id:String;
	final sourceType:String;
	final targetType:String;
	final sourceNullability:String;
	final targetNullability:String;
	final use:String;
	final meaning:String;
	final hxcIrKind:String;
	final implementation:String;
	final failureEdgeRequired:Bool;
	final exceptionalInputs:Array<String>;
	final runtimeFeatures:Array<String>;
}

typedef CPrimitiveNullabilityRecord = {
	final sourcePattern:String;
	final representation:String;
	final invariant:String;
	final profiles:Array<String>;
	final runtimeFeatures:Array<String>;
}

typedef CPrimitiveFloatContract = {
	final cType:String;
	final format:String;
	final comparisons:String;
	final negativeZero:String;
	final fastMath:String;
	final stdIntExceptionalPolicy:Array<String>;
}

typedef CPrimitiveConstraintRecord = {
	final cDialect:String;
	final requiredCharacteristics:Array<String>;
	final forbiddenAssumptions:Array<String>;
	final runtimeFeatures:Array<String>;
}

typedef CPrimitiveContractSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:String;
	final requirements:Array<String>;
	final haxeBaseline:String;
	final profiles:Array<String>;
	final representations:Array<CPrimitiveRepresentationRecord>;
	final nullability:Array<CPrimitiveNullabilityRecord>;
	final conversions:Array<CPrimitiveConversionRecord>;
	final floatContract:CPrimitiveFloatContract;
	final constraints:CPrimitiveConstraintRecord;
}

/** Renders the machine contract from the same typed decisions used by lowering. */
class CPrimitiveContract {
	public static function snapshot():CPrimitiveContractSnapshot {
		final representations = [];
		for (sourceType in CPrimitiveSemantics.sourceTypes()) {
			final portable = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, sourceType));
			final metal = requireMapping(CPrimitiveSemantics.mapping(CProfile.Metal, sourceType));
			if (mappingKey(portable) != mappingKey(metal)) {
				throw 'Primitive mapping changed by profile without a source contract: ${CPrimitiveSemantics.sourceTypeKey(sourceType)}';
			}
			representations.push(representationRecord(portable));
		}

		return {
			schemaVersion: 1,
			algorithm: "hxc-primitive-semantics-v1",
			status: "semantic-contract-no-production-c-lowering",
			requirements: ["HXC-SEM-001", "HXC-SEM-003", "HXC-SEM-005"],
			haxeBaseline: "5.0.0-preview.1",
			profiles: ["portable", "metal"],
			representations: representations,
			nullability: [
				{
					sourcePattern: "Null<non-null scalar>",
					representation: "tagged optional { bool has_value; T value; }",
					invariant: "value is read only when has_value is true; a proven non-null value may erase the wrapper",
					profiles: ["portable", "metal"],
					runtimeFeatures: []
				},
				{
					sourcePattern: "Null<reference-like T>",
					representation: "nullable pointer to target-owned T representation",
					invariant: "null is the null pointer; non-null unwrap requires an explicit checked edge",
					profiles: ["portable", "metal"],
					runtimeFeatures: []
				},
				{
					sourcePattern: "c.NullablePtr<T>",
					representation: "C pointer with explicit nullable native contract",
					invariant: "ordinary c.Ptr/c.Ref variants remain non-null; pointer ownership and lifetime stay separate",
					profiles: ["portable", "metal"],
					runtimeFeatures: []
				}
			],
			conversions: conversionRecords(),
			floatContract: {
				cType: "double",
				format: "IEC 60559 / IEEE 754 binary64 required by the resolved target tuple",
				comparisons: "ordered C comparisons; NaN is unordered and unequal to itself; infinities retain IEEE ordering",
				negativeZero: "preserved by storage and floating operations; compares equal to positive zero",
				fastMath: "forbidden for Haxe semantic lowering because it can erase NaN, infinity, and signed-zero behavior",
				stdIntExceptionalPolicy: [
					"finite in-range values truncate toward zero",
					"negative zero converts to 0",
					"NaN converts to 0",
					"positive infinity and finite positive overflow convert to 2147483647",
					"negative infinity and finite negative overflow convert to -2147483648"
				]
			},
			constraints: {
				cDialect: "strict ISO C11",
				requiredCharacteristics: [
					"CHAR_BIT == 8",
					"exact int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/int64_t/uint64_t typedefs",
					"double is 64-bit binary64",
					"intptr_t and uintptr_t exist for pointer round trips"
				],
				forbiddenAssumptions: [
					"C int has Haxe Int width",
					"C long has any fixed width",
					"out-of-range unsigned-to-signed C casts preserve bits",
					"out-of-range or non-finite floating-to-integer C casts are valid"
				],
				runtimeFeatures: []
			}
		};
	}

	static function conversionRecords():Array<CPrimitiveConversionRecord> {
		return [
			conversionRecord("haxe-int-to-float", CPHaxeInt, CPNonNullable, CPHaxeFloat, CPNonNullable, CPUImplicit, []),
			conversionRecord("haxe-uint-to-float", CPHaxeUInt, CPNonNullable, CPHaxeFloat, CPNonNullable, CPUImplicit, []),
			conversionRecord("haxe-int-to-uint-bits", CPHaxeInt, CPNonNullable, CPHaxeUInt, CPNonNullable, CPUImplicit, []),
			conversionRecord("haxe-uint-to-int-bits", CPHaxeUInt, CPNonNullable, CPHaxeInt, CPNonNullable, CPUImplicit, []),
			conversionRecord("haxe-std-int", CPHaxeFloat, CPNonNullable, CPHaxeInt, CPNonNullable, CPUStdInt,
				[
					"NaN->0",
					"+infinity->2147483647",
					"-infinity->-2147483648",
					"positive-overflow->2147483647",
					"negative-overflow->-2147483648"
				]),
			conversionRecord("exact-signed-widen", CPCExactInteger(8, true), CPNonNullable, CPCExactInteger(32, true), CPNonNullable, CPUWrapping, []),
			conversionRecord("exact-unsigned-to-signed-widen", CPCExactInteger(8, false), CPNonNullable, CPCExactInteger(16, true), CPNonNullable,
				CPUWrapping, []),
			conversionRecord("exact-signed-narrow-wrapping", CPCExactInteger(32, true), CPNonNullable, CPCExactInteger(8, true), CPNonNullable, CPUWrapping,
				[]),
			conversionRecord("exact-unsigned-narrow-wrapping", CPCExactInteger(32, false), CPNonNullable, CPCExactInteger(8, false), CPNonNullable,
				CPUWrapping, []),
			conversionRecord("exact-unsigned-to-signed-bits", CPCExactInteger(32, false), CPNonNullable, CPCExactInteger(32, true), CPNonNullable,
				CPUWrapping, []),
			conversionRecord("exact-checked-narrow", CPCExactInteger(32, true), CPNonNullable, CPCExactInteger(8, true), CPNonNullable, CPUChecked,
				["out-of-range->failure-edge"]),
			conversionRecord("nullable-scalar-inject", CPHaxeInt, CPNonNullable, CPHaxeInt, CPNullable, CPUNullableInject, []),
			conversionRecord("nullable-scalar-unwrap", CPHaxeInt, CPNullable, CPHaxeInt, CPNonNullable, CPUNullableUnwrap, ["absent->failure-edge"])
		];
	}

	static function conversionRecord(id:String, sourceType:CPrimitiveSourceType, sourceNullability:CPrimitiveNullability, targetType:CPrimitiveSourceType,
			targetNullability:CPrimitiveNullability, use:CPrimitiveConversionUse, exceptionalInputs:Array<String>):CPrimitiveConversionRecord {
		final source = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, sourceType, sourceNullability));
		final target = requireMapping(CPrimitiveSemantics.mapping(CProfile.Portable, targetType, targetNullability));
		final decision = requireDecision(CPrimitiveSemantics.conversion(source, target, use));
		return {
			id: id,
			sourceType: CPrimitiveSemantics.sourceTypeKey(source.sourceType),
			targetType: CPrimitiveSemantics.sourceTypeKey(target.sourceType),
			sourceNullability: nullability(source.nullability),
			targetNullability: nullability(target.nullability),
			use: conversionUse(decision.use),
			meaning: conversionMeaning(decision.meaning),
			hxcIrKind: conversionKind(decision.irKind),
			implementation: implementation(decision.implementation),
			failureEdgeRequired: decision.failureRequired,
			exceptionalInputs: exceptionalInputs,
			runtimeFeatures: []
		};
	}

	static function representationRecord(mapping:CPrimitiveTypeMapping):CPrimitiveRepresentationRecord {
		return {
			sourceType: CPrimitiveSemantics.sourceTypeKey(mapping.sourceType),
			profiles: ["portable", "metal"],
			profileDifference: "none",
			cType: mapping.cSpelling,
			requiredHeaders: mapping.requiredHeader == null ? [] : [mapping.requiredHeader],
			width: width(mapping.width),
			signedness: signedness(mapping.signedness),
			storage: storage(mapping.storage),
			hxcIrType: typeRef(mapping.irType),
			runtimeFeatures: []
		};
	}

	static function requireMapping(result:CPrimitiveMappingResult):CPrimitiveTypeMapping {
		return switch result {
			case CPMappingAvailable(mapping): mapping;
			case CPMappingRejected(reason): throw 'Required primitive mapping was rejected: $reason';
		}
	}

	static function requireDecision(result:CPrimitiveConversionResult):CPrimitiveConversionDecision {
		return switch result {
			case CPConversionAllowed(decision): decision;
			case CPConversionElided: throw "Required primitive conversion was unexpectedly elided";
			case CPConversionRejected(reason): throw 'Required primitive conversion was rejected: $reason';
		}
	}

	static function mappingKey(mapping:CPrimitiveTypeMapping):String {
		return
			'${mapping.cSpelling}|${mapping.requiredHeader}|${widthKey(mapping.width)}|${signedness(mapping.signedness)}|${storage(mapping.storage)}|${typeRef(mapping.irType)}';
	}

	static function width(value:CPrimitiveWidth):CPrimitiveWidthRecord {
		return switch value {
			case CPWidthNone: {kind: "none", bits: null};
			case CPWidthExact(bits): {kind: "exact", bits: bits};
			case CPWidthTargetAbi: {kind: "target-abi", bits: null};
			case CPWidthAbiDefined: {kind: "c-abi-defined", bits: null};
		}
	}

	static function widthKey(value:CPrimitiveWidth):String {
		final record = width(value);
		return '${record.kind}:${record.bits}';
	}

	static function signedness(value:CPrimitiveSignedness):String {
		return switch value {
			case CPSignNone: "not-applicable";
			case CPSignBoolean: "boolean";
			case CPSignSigned: "signed";
			case CPSignUnsigned: "unsigned";
		}
	}

	static function storage(value:CPrimitiveStorage):String {
		return switch value {
			case CPStorageVoid: "no-value";
			case CPStorageScalar: "direct-scalar";
			case CPStorageTaggedOptional: "tagged-optional";
		}
	}

	static function nullability(value:CPrimitiveNullability):String
		return value == CPNullable ? "nullable" : "non-null";

	static function conversionUse(value:CPrimitiveConversionUse):String {
		return switch value {
			case CPUImplicit: "implicit";
			case CPUStdInt: "Std.int-or-typed-numeric-cast";
			case CPUWrapping: "explicit-wrapping";
			case CPUChecked: "explicit-checked";
			case CPUNullableInject: "nullable-inject";
			case CPUNullableUnwrap: "nullable-unwrap";
		}
	}

	static function conversionMeaning(value:CPrimitiveConversionMeaning):String {
		return switch value {
			case CPExact: "exact";
			case CPModuloBits(targetWidth): 'modulo-2^$targetWidth';
			case CPTwosComplementBits(targetWidth): 'low-$targetWidth-bits-as-twos-complement';
			case CPSaturatingTruncate: "truncate-toward-zero-with-defined-saturation";
			case CPCheckedRange: "range-check-then-exact";
			case CPInjectPresent: "construct-present-nullable";
			case CPUnwrapPresent: "check-present-then-extract";
		}
	}

	static function conversionKind(value:HxcIRConversionKind):String {
		return switch value {
			case IRCNumericExact: "numeric-exact";
			case IRCNumericWrapping: "numeric-wrapping";
			case IRCNumericSaturating: "numeric-saturating";
			case IRCNumericChecked: "numeric-checked";
			case IRCNullableInject: "nullable-inject";
			case IRCNullableUnwrap: "nullable-unwrap";
			case IRCPointer: "pointer";
			case IRCBox: "box";
			case IRCUnbox: "unbox";
			case IRCRepresentation: "representation";
		}
	}

	static function implementation(value:HxcIRImplementation):String {
		return switch value {
			case IRIStatic: "direct-c";
			case IRIProgramLocal(helperId): 'program-local:$helperId';
			case IRIRuntime(featureId): 'runtime:$featureId';
		}
	}

	static function typeRef(value:HxcIRTypeRef):String {
		return switch value {
			case IRTBool: "bool";
			case IRTInt(width, signed): '${signed ? "i" : "u"}$width';
			case IRTAbiInteger(kind): 'abi-int:${abiInteger(kind)}';
			case IRTFloat(width): 'f$width';
			case IRTVoid: "void";
			case IRTInstance(instanceId): 'instance:$instanceId';
			case IRTPointer(pointee, nullable): 'pointer:${nullable ? "nullable" : "nonnull"}<${typeRef(pointee)}>';
			case IRTNullable(inner, representation): 'nullable:${nullableRepresentation(representation)}<${typeRef(inner)}>';
			case IRTFunction(parameters, result): 'function(${parameters.map(typeRef).join(",")})->${typeRef(result)}';
			case IRTDynamic: "dynamic";
		}
	}

	static function abiInteger(value:HxcIRAbiIntegerKind):String {
		return switch value {
			case IRAKSize: "size";
			case IRAKPtrDiff: "ptrdiff";
			case IRAKIntPtr: "intptr";
			case IRAKUIntPtr: "uintptr";
		}
	}

	static function nullableRepresentation(value:HxcIRNullableRepresentation):String {
		return switch value {
			case IRNTagged: "tagged";
			case IRNPointer: "pointer";
		}
	}
}
