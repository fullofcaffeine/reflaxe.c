package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeReachabilityEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementAnalysis;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementCandidate;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;

private class RuntimeIntentObservation {
	public final featureId:String;
	public final operationId:String;
	public final source:HxcSourceSpan;

	public function new(featureId:String, operationId:String, source:HxcSourceSpan) {
		this.featureId = featureId;
		this.operationId = operationId;
		this.source = source;
	}

	public function key():String
		return '$featureId\x00$operationId\x00${source.display()}';
}

/** Reconciles every reachable HxcIR runtime intent with one typed source root. */
class RuntimeRequirementAnalyzer {
	public static inline final ANALYSIS_SCHEMA_VERSION = 1;

	public function new() {}

	public function analyze(program:HxcIRProgram, input:Array<RuntimeRequirementCandidate>):RuntimeRequirementAnalysis {
		if (program.schemaVersion != 3) {
			internal('runtime requirement analysis needs validated schema-3 HxcIR; found `${program.schemaVersion}`');
		}
		final observations:Array<RuntimeIntentObservation> = [];
		var typeInstanceCount = 0;
		var functionCount = 0;
		var blockCount = 0;
		var instructionCount = 0;
		var cleanupActionCount = 0;
		for (module in program.modules) {
			typeInstanceCount += module.typeInstances.length;
			for (instance in module.typeInstances) {
				switch instance.representation {
					case IRRManaged(featureId):
						observations.push(new RuntimeIntentObservation(featureId, "managed-type-representation", instance.source));
					case _:
				}
			}
			functionCount += module.functions.length;
			for (fn in module.functions) {
				blockCount += fn.blocks.length;
				for (block in fn.blocks) {
					instructionCount += block.instructions.length;
					for (instruction in block.instructions) {
						collectInstruction(instruction, observations);
					}
				}
				for (region in fn.cleanupRegions) {
					cleanupActionCount += region.actions.length;
					for (action in region.actions) {
						collectCleanup(action, observations);
					}
				}
			}
		}

		final candidates = canonicalCandidates(input);
		final uniqueObservations = canonicalObservations(observations);
		final reasons:Array<RuntimeRequirementReason> = [];
		for (index in 0...uniqueObservations.length) {
			final observation = uniqueObservations[index];
			final candidate = candidateFor(candidates, observation);
			reasons.push(new RuntimeRequirementReason('runtime.${observation.featureId}.${observation.operationId}.$index',
				RuntimeFeatureId.parse(observation.featureId), observation.operationId, candidate.kind, candidate.surface, observation.source,
				candidate.alternative));
		}
		for (candidate in candidates) {
			if (!hasObservation(uniqueObservations, candidate)) {
				internal('runtime source reason for `${candidate.operationId}` at `${candidate.source.display()}` has no reachable HxcIR runtime intent',
					[candidate.featureId.text()]);
			}
		}
		return new RuntimeRequirementAnalysis(reasons,
			new RuntimeReachabilityEvidence(program.modules.length, typeInstanceCount, functionCount, blockCount, instructionCount, cleanupActionCount,
				observations.length));
	}

	static function collectInstruction(instruction:HxcIRInstruction, observations:Array<RuntimeIntentObservation>):Void {
		switch instruction.kind {
			case IRIOUnary(operationId, _, implementation) | IRIOBinary(operationId, _, _, implementation):
				collectImplementation(implementation, operationId, instruction.source, observations);
			case IRIOConvert(_, kind, _, implementation, _):
				collectImplementation(implementation, conversionOperation(kind), instruction.source, observations);
			case IRIOCall(call):
				switch call.dispatch {
					case IRCDRuntime(featureId, operationId):
						observations.push(new RuntimeIntentObservation(featureId, operationId, instruction.source));
					case _:
				}
			case IRIOAllocate(_, _, implementation, _):
				collectImplementation(implementation, "allocation", instruction.source, observations);
			case IRIODeallocate(_, implementation):
				collectImplementation(implementation, "deallocation", instruction.source, observations);
			case IRIORetain(_, implementation):
				collectImplementation(implementation, "retain", instruction.source, observations);
			case IRIOTrace(_, implementation):
				collectImplementation(implementation, "trace", instruction.source, observations);
			case _:
		}
	}

	static function collectCleanup(action:HxcIRCleanupAction, observations:Array<RuntimeIntentObservation>):Void {
		switch action.kind {
			case IRCARelease(_, implementation):
				collectImplementation(implementation, "cleanup-release", action.source, observations);
			case IRCADeallocate(_, implementation):
				collectImplementation(implementation, "cleanup-deallocate", action.source, observations);
			case _:
		}
	}

	static function collectImplementation(implementation:HxcIRImplementation, operationId:String, source:HxcSourceSpan,
			observations:Array<RuntimeIntentObservation>):Void {
		switch implementation {
			case IRIRuntime(featureId):
				observations.push(new RuntimeIntentObservation(featureId, operationId, source));
			case _:
		}
	}

	static function conversionOperation(kind:HxcIRConversionKind):String {
		return switch kind {
			case IRCNumericExact: "conversion-numeric-exact";
			case IRCNumericWrapping: "conversion-numeric-wrapping";
			case IRCNumericSaturating: "conversion-numeric-saturating";
			case IRCNumericChecked: "conversion-numeric-checked";
			case IRCNullableInject: "conversion-nullable-inject";
			case IRCNullableUnwrap: "conversion-nullable-unwrap";
			case IRCPointer: "conversion-pointer";
			case IRCBox: "conversion-box";
			case IRCUnbox: "conversion-unbox";
			case IRCRepresentation: "conversion-representation";
		};
	}

	static function canonicalCandidates(input:Array<RuntimeRequirementCandidate>):Array<RuntimeRequirementCandidate> {
		final candidates = input.copy();
		candidates.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(candidateKey(left), candidateKey(right)));
		final result:Array<RuntimeRequirementCandidate> = [];
		for (candidate in candidates) {
			if (result.length == 0 || candidateKey(result[result.length - 1]) != candidateKey(candidate)) {
				result.push(candidate);
				continue;
			}
			final previous = result[result.length - 1];
			if (previous.kind != candidate.kind
				|| previous.surface != candidate.surface
				|| previous.alternative != candidate.alternative) {
				internal('conflicting runtime source reasons describe `${candidate.operationId}` at `${candidate.source.display()}`',
					[candidate.featureId.text()]);
			}
		}
		return result;
	}

	static function canonicalObservations(input:Array<RuntimeIntentObservation>):Array<RuntimeIntentObservation> {
		final observations = input.copy();
		observations.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.key(), right.key()));
		final result:Array<RuntimeIntentObservation> = [];
		for (observation in observations) {
			if (result.length == 0 || result[result.length - 1].key() != observation.key()) {
				result.push(observation);
			}
		}
		return result;
	}

	static function candidateFor(candidates:Array<RuntimeRequirementCandidate>, observation:RuntimeIntentObservation):RuntimeRequirementCandidate {
		for (candidate in candidates) {
			if (candidateKey(candidate) == observation.key()) {
				return candidate;
			}
		}
		return
			internal('reachable HxcIR runtime intent `${observation.featureId}/${observation.operationId}` at `${observation.source.display()}` has no typed source reason',
			[observation.featureId]);
	}

	static function hasObservation(observations:Array<RuntimeIntentObservation>, candidate:RuntimeRequirementCandidate):Bool {
		final key = candidateKey(candidate);
		for (observation in observations) {
			if (observation.key() == key) {
				return true;
			}
		}
		return false;
	}

	static function candidateKey(candidate:RuntimeRequirementCandidate):String
		return '${candidate.featureId.text()}\x00${candidate.operationId}\x00${candidate.source.display()}';

	static function internal<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail, featureIds);
}
