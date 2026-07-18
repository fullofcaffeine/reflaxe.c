package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Expr.Position;
#if macro
import haxe.macro.Context;
#end
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.CStaticFunctionProjectEmitter.CStaticFunctionDefinitionPlan;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyEnum.CBodyEnumRepresentation;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CLoweredBodyFunction;
import reflaxe.c.lowering.CGenericSpecialization.CGenericFunctionSpecialization;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeArgument;
import reflaxe.c.lowering.CGenericSpecialization.CGenericTypeCanonicalizer;
import reflaxe.c.lowering.CGenericSpecializationContract;
import reflaxe.c.lowering.CStaticFunctionGraph.CStaticFunctionGraph;
#end

typedef CGenericSourceSnapshot = {
	final file:String;
	final startLine:Int;
	final startColumn:Int;
	final endLine:Int;
	final endColumn:Int;
}

typedef CGenericArgumentSnapshot = {
	final parameter:String;
	final key:String;
	final displayName:String;
	final representation:String;
}

typedef CGenericReachabilitySnapshot = {
	final callerInstanceId:String;
	final source:CGenericSourceSnapshot;
}

typedef CGenericFunctionCostSnapshot = {
	final metric:String;
	final definitionBytes:Int;
	final definitionSha256:String;
	final irBlocks:Int;
	final irInstructions:Int;
}

typedef CGenericFunctionSpecializationSnapshot = {
	final baseFunctionId:String;
	final instanceId:String;
	final specializationKey:String;
	final semanticDigestSha256:String;
	final displayName:String;
	final cName:String;
	final arguments:Array<CGenericArgumentSnapshot>;
	final reasons:Array<CGenericReachabilitySnapshot>;
	final recursive:Bool;
	final codeSize:CGenericFunctionCostSnapshot;
}

typedef CGenericTypeCostSnapshot = {
	final metric:String;
	final structuralUnits:Int;
	final constructorCount:Int;
	final payloadFieldCount:Int;
}

typedef CGenericTypeSpecializationSnapshot = {
	final haxePath:String;
	final declarationId:String;
	final instanceId:String;
	final specializationKey:String;
	final semanticDigestSha256:String;
	final displayName:String;
	final cName:String;
	final representation:String;
	final arguments:Array<CGenericArgumentSnapshot>;
	final reasons:Array<CGenericSourceSnapshot>;
	final recursive:Bool;
	final codeSize:CGenericTypeCostSnapshot;
}

typedef CGenericSpecializationLimitsSnapshot = {
	final maxFunctionSpecializations:Int;
	final maxTypeSpecializations:Int;
	final maxEstimatedSpecializationCBytes:Int;
}

typedef CGenericSpecializationSummarySnapshot = {
	final functionSpecializations:Int;
	final typeSpecializations:Int;
	final mergedFunctionReasons:Int;
	final mergedTypeReasons:Int;
	final recursiveSpecializations:Int;
	final specializedFunctionDefinitionBytes:Int;
	final dependencyClosedEnumDefinitionBytes:Int;
	final estimatedSpecializationCBytes:Int;
	final generatedPayloadArtifacts:Int;
	final generatedPayloadBytes:Int;
}

/** Deterministic sidecar proving the exact bounded E3.T03 specialization work. */
typedef CGenericSpecializationReportSnapshot = {
	final schemaVersion:Int;
	final algorithm:String;
	final status:String;
	final keyEncoding:String;
	final compactNameDigest:String;
	final codeSizeAttribution:String;
	final limits:CGenericSpecializationLimitsSnapshot;
	final summary:CGenericSpecializationSummarySnapshot;
	final functionSpecializations:Array<CGenericFunctionSpecializationSnapshot>;
	final typeSpecializations:Array<CGenericTypeSpecializationSnapshot>;
}

/** Builds the report only when a closed generic function or enum is reachable. */
#if (macro || reflaxe_runtime)
class CGenericSpecializationReportBuilder {
	public static inline final TEST_CODE_SIZE_LIMIT_DEFINE = "reflaxe_c_test_generic_code_size_limit";

	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function build(graph:CStaticFunctionGraph, lowered:CBodyLoweringResult, functionDefinitions:Array<CStaticFunctionDefinitionPlan>,
			units:Array<GeneratedFile>, fallbackPosition:Position, fallbackSourcePath:String):Null<CGenericSpecializationReportSnapshot> {
		final genericEnums = lowered.enums.filter(value -> value.prepared.typeParameterNames.length > 0);
		if (graph.specializations.length == 0 && genericEnums.length == 0)
			return null;

		final functionsById:Map<String, CLoweredBodyFunction> = [];
		for (fn in lowered.functions)
			functionsById.set(fn.ir.id, fn);
		final definitionsById:Map<String, CStaticFunctionDefinitionPlan> = [];
		for (definition in functionDefinitions) {
			if (definitionsById.exists(definition.functionId))
				throw new CBodyEmissionError('generic specialization report received duplicate function definition `${definition.functionId}`');
			definitionsById.set(definition.functionId, definition);
		}
		final callTargets = directCallTargets(lowered.functions);
		final bodyEmitter = new CBodyEmitter(lowered.aggregates, lowered.enums, lowered.classes, lowered.dispatch);
		final printer = new CASTPrinter();
		final functionRecords:Array<CGenericFunctionSpecializationSnapshot> = [];
		var specializedFunctionBytes = 0;
		var mergedFunctionReasons = 0;
		var recursiveCount = 0;
		for (specialization in graph.specializations) {
			final fn = functionsById.get(specialization.instanceId);
			if (fn == null)
				throw new CBodyEmissionError('generic specialization `${specialization.instanceId}` was not lowered');
			final definitionPlan = definitionsById.get(specialization.instanceId);
			if (definitionPlan == null)
				throw new CBodyEmissionError('generic specialization `${specialization.instanceId}` has no finalized C definition');
			final definition = printer.printDecl(definitionPlan.declaration);
			final definitionBytes = Bytes.ofString(definition).length;
			specializedFunctionBytes += definitionBytes;
			mergedFunctionReasons += specialization.reasons.length;
			final recursive = reaches(specialization.instanceId, specialization.instanceId, callTargets, []);
			if (recursive)
				recursiveCount++;
			var instructionCount = 0;
			for (block in fn.ir.blocks)
				instructionCount += block.instructions.length;
			functionRecords.push({
				baseFunctionId: specialization.baseFunctionId,
				instanceId: specialization.instanceId,
				specializationKey: specialization.key,
				semanticDigestSha256: specialization.digest,
				displayName: specialization.displayName,
				cName: fn.cName.value,
				arguments: argumentSnapshots(specialization.typeParameters.map(parameter -> parameter.name), specialization.arguments),
				reasons: specialization.reasons.map(reason -> {
					callerInstanceId: reason.callerInstanceId,
					source: sourceSnapshot(reason.source)
				}),
				recursive: recursive,
				codeSize: {
					metric: "strict-c11-utf8-function-definition-bytes",
					definitionBytes: definitionBytes,
					definitionSha256: Sha256.encode(definition),
					irBlocks: fn.ir.blocks.length,
					irInstructions: instructionCount
				}
			});
		}

		final typeRecords:Array<CGenericTypeSpecializationSnapshot> = [];
		var mergedTypeReasons = 0;
		for (value in genericEnums) {
			var payloadCount = 0;
			for (tagCase in value.cases)
				payloadCount += tagCase.payload.length;
			mergedTypeReasons += value.prepared.reasons.length;
			if (value.prepared.recursive)
				recursiveCount++;
			typeRecords.push({
				haxePath: value.prepared.haxePath,
				declarationId: value.prepared.declarationId,
				instanceId: value.prepared.instanceId,
				specializationKey: value.prepared.shapeKey,
				semanticDigestSha256: value.prepared.digest,
				displayName: value.prepared.displayName,
				cName: value.valueTag.value,
				representation: switch value.prepared.representation {
					case CBERNativeEnum: "native-enum";
					case CBERTaggedUnion: "tagged-union";
				},
				arguments: argumentSnapshots(value.prepared.typeParameterNames, value.prepared.typeArguments),
				reasons: value.prepared.reasons.map(sourceSnapshot),
				recursive: value.prepared.recursive,
				codeSize: {
					metric: "typed-enum-layout-structural-units",
					structuralUnits: 1 + value.cases.length + payloadCount,
					constructorCount: value.cases.length,
					payloadFieldCount: payloadCount
				}
			});
		}

		functionRecords.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.specializationKey, right.specializationKey));
		typeRecords.sort((left, right) -> CGenericTypeCanonicalizer.compareUtf8(left.specializationKey, right.specializationKey));
		final enumText = genericEnums.length == 0 ? "" : enumDefinitionText(bodyEmitter, printer);
		final enumBytes = Bytes.ofString(enumText).length;
		final estimatedBytes = specializedFunctionBytes + enumBytes;
		final effectiveCodeSizeLimit = codeSizeLimit();
		if (estimatedBytes > effectiveCodeSizeLimit) {
			final source = HaxeSourceSpan.fromPosition(fallbackPosition, fallbackSourcePath);
			throw new CBodyLoweringError(HxcIRDiagnostic.unsupportedTypedAstNode(Std.string(context.profile),
				'generic-specialization-code-size-budget:$estimatedBytes-over-$effectiveCodeSizeLimit', "generic specialization code-size planning", source),
				fallbackPosition);
		}
		if (effectiveCodeSizeLimit != CGenericSpecializationContract.MAX_ESTIMATED_SPECIALIZATION_C_BYTES) {
			throw new CBodyEmissionError('test generic code-size limit `$effectiveCodeSizeLimit` did not reject estimated size `$estimatedBytes`');
		}
		var payloadArtifacts = 0;
		var payloadBytes = 0;
		for (unit in units) {
			if (GeneratedFile.isPayloadKind(unit.kind)) {
				payloadArtifacts++;
				payloadBytes += Bytes.ofString(unit.contents).length;
			}
		}
		return {
			schemaVersion: CGenericSpecializationContract.REPORT_SCHEMA_VERSION,
			algorithm: CGenericSpecializationContract.REPORT_ALGORITHM,
			status: CGenericSpecializationContract.REPORT_STATUS,
			keyEncoding: CGenericSpecializationContract.KEY_ENCODING,
			compactNameDigest: CGenericSpecializationContract.COMPACT_NAME_DIGEST,
			codeSizeAttribution: CGenericSpecializationContract.CODE_SIZE_ATTRIBUTION,
			limits: {
				maxFunctionSpecializations: CGenericSpecializationContract.MAX_FUNCTION_SPECIALIZATIONS,
				maxTypeSpecializations: CGenericSpecializationContract.MAX_TYPE_SPECIALIZATIONS,
				maxEstimatedSpecializationCBytes: CGenericSpecializationContract.MAX_ESTIMATED_SPECIALIZATION_C_BYTES
			},
			summary: {
				functionSpecializations: functionRecords.length,
				typeSpecializations: typeRecords.length,
				mergedFunctionReasons: mergedFunctionReasons,
				mergedTypeReasons: mergedTypeReasons,
				recursiveSpecializations: recursiveCount,
				specializedFunctionDefinitionBytes: specializedFunctionBytes,
				dependencyClosedEnumDefinitionBytes: enumBytes,
				estimatedSpecializationCBytes: estimatedBytes,
				generatedPayloadArtifacts: payloadArtifacts,
				generatedPayloadBytes: payloadBytes
			},
			functionSpecializations: functionRecords,
			typeSpecializations: typeRecords
		};
	}

	static function argumentSnapshots(parameterNames:Array<String>, arguments:Array<CGenericTypeArgument>):Array<CGenericArgumentSnapshot> {
		if (parameterNames.length != arguments.length)
			throw new CBodyEmissionError("generic specialization report received mismatched type parameters and arguments");
		return [
			for (index in 0...arguments.length)
				{
					parameter: parameterNames[index],
					key: arguments[index].key,
					displayName: arguments[index].displayName,
					representation: Std.string(arguments[index].representation)
				}
		];
	}

	static function sourceSnapshot(source:HxcSourceSpan):CGenericSourceSnapshot
		return {
			file: source.file,
			startLine: source.startLine,
			startColumn: source.startColumn,
			endLine: source.endLine,
			endColumn: source.endColumn
		};

	static function enumDefinitionText(emitter:CBodyEmitter, printer:CASTPrinter):String {
		final declarations = emitter.enumDefinitions().concat(emitter.enumLayoutAssertions());
		return declarations.map(printer.printDecl).join("\n\n") + "\n";
	}

	static function directCallTargets(functions:Array<CLoweredBodyFunction>):Map<String, Array<String>> {
		final result:Map<String, Array<String>> = [];
		for (fn in functions) {
			final targets:Array<String> = [];
			for (block in fn.ir.blocks) {
				for (instruction in block.instructions) {
					switch instruction.kind {
						case IRIOCall({dispatch: IRCDDirect(target)}):
							if (targets.indexOf(target) == -1)
								targets.push(target);
						case _:
					}
				}
			}
			targets.sort(CGenericTypeCanonicalizer.compareUtf8);
			result.set(fn.ir.id, targets);
		}
		return result;
	}

	static function reaches(current:String, target:String, calls:Map<String, Array<String>>, visited:Map<String, Bool>):Bool {
		if (visited.exists(current))
			return false;
		visited.set(current, true);
		final targets = calls.get(current);
		if (targets == null)
			return false;
		for (next in targets) {
			if (next == target || reaches(next, target, calls, visited))
				return true;
		}
		return false;
	}

	static function codeSizeLimit():Int {
		#if macro
		final raw = Context.definedValue(TEST_CODE_SIZE_LIMIT_DEFINE);
		if (raw != null) {
			if (!~/^[1-9][0-9]*$/.match(raw))
				throw new CBodyEmissionError('test generic code-size limit `$raw` is not a positive decimal integer');
			final parsed = Std.parseInt(raw);
			if (parsed == null || parsed > CGenericSpecializationContract.MAX_ESTIMATED_SPECIALIZATION_C_BYTES)
				throw new CBodyEmissionError('test generic code-size limit `$raw` exceeds the production bound');
			return parsed;
		}
		#end
		return CGenericSpecializationContract.MAX_ESTIMATED_SPECIALIZATION_C_BYTES;
	}
}
#else
class CGenericSpecializationReportBuilder {
	public function new() {}
}
#end
