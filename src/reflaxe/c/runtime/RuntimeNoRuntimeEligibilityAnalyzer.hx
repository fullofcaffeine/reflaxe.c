package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeProofRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeScope;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePolicyBlockerRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeSourceSpanRecord;

/** Produces the positive whole-program proof or every source-rooted blocker. */
class RuntimeNoRuntimeEligibilityAnalyzer {
	public static inline final PROOF_SCHEMA_VERSION = 1;
	public static inline final PROOF_ALGORITHM = "hxc-no-runtime-eligibility-v1";

	final registry:RuntimeFeatureRegistry;

	public function new(registry:RuntimeFeatureRegistry) {
		this.registry = registry;
	}

	public function prove(purpose:RuntimePlanningPurpose, evidence:RuntimeNoRuntimeEvidence, directDecisions:Array<String>):RuntimeNoRuntimeProofRecord {
		final decisions = sortedUnique(directDecisions, "no-runtime direct decision");
		final helpers = sortedUnique(evidence.programLocalHelpers, "program-local helper ID");
		final proof:RuntimeNoRuntimeProofRecord = {
			schemaVersion: PROOF_SCHEMA_VERSION,
			algorithm: PROOF_ALGORITHM,
			status: RuntimeNoRuntimeStatus.Eligible,
			scope: evidence.scope,
			semanticProof: evidence.semanticProof,
			reachability: {
				modules: evidence.reachability.moduleCount,
				typeInstances: evidence.reachability.typeInstanceCount,
				functions: evidence.reachability.functionCount,
				blocks: evidence.reachability.blockCount,
				instructions: evidence.reachability.instructionCount,
				cleanupActions: evidence.reachability.cleanupActionCount,
				runtimeIntents: evidence.reachability.runtimeIntentCount
			},
			directDecisions: decisions,
			programLocalHelpers: helpers,
			runtimeAbsence: {
				features: [],
				includes: [],
				sources: [],
				defines: [],
				libraries: [],
				symbols: []
			}
		};
		validateProof(proof, purpose, decisions, helpers);
		return proof;
	}

	public function policyFailure(reasons:Array<RuntimeRequirementReason>):RuntimeFeatureError {
		if (reasons.length == 0) {
			return internal("runtime-none blocker analysis received no root requirements");
		}
		final blockers = reasons.map(blockerRecord);
		blockers.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.id, right.id));
		final featureIds:Array<String> = [];
		for (blocker in blockers) {
			appendUnique(featureIds, blocker.featureId);
			for (chain in blocker.dependencyChains) {
				for (featureId in chain) {
					appendUnique(featureIds, featureId);
				}
			}
		}
		featureIds.sort(RuntimeFeatureRegistry.compareUtf8);
		return new RuntimeFeatureError(CDiagnosticId.RuntimePolicyViolation, blockerMessage(blockers), featureIds, blockers, reasons[0].source);
	}

	public static function validateProof(proof:RuntimeNoRuntimeProofRecord, purpose:RuntimePlanningPurpose, directDecisions:Array<String>,
			?programLocalHelpers:Array<String>):Void {
		if (proof.schemaVersion != PROOF_SCHEMA_VERSION
			|| proof.algorithm != PROOF_ALGORITHM
			|| proof.status != RuntimeNoRuntimeStatus.Eligible) {
			internal("no-runtime eligibility proof has an unsupported schema, algorithm, or status");
		}
		final expectedScope = switch purpose {
			case RuntimePlanningPurpose.CompilerProgram: RuntimeNoRuntimeScope.ReachableWholeProgram;
			case RuntimePlanningPurpose.NativeSeedFixture: RuntimeNoRuntimeScope.NativeSeedFixture;
			case _: return internal('no-runtime eligibility proof has unknown planning purpose `${Std.string(purpose)}`');
		};
		if (proof.scope != expectedScope) {
			internal('no-runtime eligibility scope `${proof.scope}` does not match planning purpose `${purpose}`');
		}
		validateText(proof.semanticProof, "no-runtime semantic proof");
		if (proof.reachability.modules < 1
			|| proof.reachability.functions < 1
			|| proof.reachability.blocks < 1
			|| proof.reachability.typeInstances < 0
			|| proof.reachability.instructions < 0
			|| proof.reachability.cleanupActions < 0
			|| proof.reachability.runtimeIntents != 0) {
			internal("no-runtime eligibility proof has invalid reachable-program counts or a surviving runtime intent");
		}
		final expectedDecisions = sortedUnique(directDecisions, "no-runtime direct decision");
		if (proof.directDecisions.join("\n") != expectedDecisions.join("\n")) {
			internal("no-runtime eligibility proof differs from the runtime plan's direct decisions");
		}
		final helpers = sortedUnique(proof.programLocalHelpers, "program-local helper ID");
		if (proof.programLocalHelpers.join("\n") != helpers.join("\n")) {
			internal("no-runtime eligibility proof has non-canonical program-local helpers");
		}
		if (programLocalHelpers != null) {
			final expectedHelpers = sortedUnique(programLocalHelpers, "program-local helper ID");
			if (proof.programLocalHelpers.join("\n") != expectedHelpers.join("\n")) {
				internal("no-runtime eligibility proof differs from emitted program-local helpers");
			}
		}
		if (proof.runtimeAbsence.features.length != 0
			|| proof.runtimeAbsence.includes.length != 0
			|| proof.runtimeAbsence.sources.length != 0
			|| proof.runtimeAbsence.defines.length != 0
			|| proof.runtimeAbsence.libraries.length != 0
			|| proof.runtimeAbsence.symbols.length != 0) {
			internal("an eligible no-runtime proof contains an hxrt feature, include, source, define, library, or symbol");
		}
	}

	function blockerRecord(reason:RuntimeRequirementReason):RuntimePolicyBlockerRecord {
		return {
			id: reason.id,
			featureId: reason.featureId.text(),
			operationId: reason.operationId,
			kind: reason.kind,
			surface: reason.surface,
			source: sourceRecord(reason),
			alternative: reason.alternative,
			dependencyChains: dependencyChains(reason.featureId.text())
		};
	}

	function dependencyChains(featureId:String):Array<Array<String>> {
		final definition = registry.definitionByText(featureId);
		if (definition == null || definition.dependencies.length == 0) {
			return [[featureId]];
		}
		final dependencies = definition.dependencies.map(dependency -> dependency.text());
		dependencies.sort(RuntimeFeatureRegistry.compareUtf8);
		final result:Array<Array<String>> = [];
		for (dependencyId in dependencies) {
			for (tail in dependencyChains(dependencyId)) {
				result.push([featureId].concat(tail));
			}
		}
		result.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.join("\x1f"), right.join("\x1f")));
		return result;
	}

	static function sourceRecord(reason:RuntimeRequirementReason):RuntimeSourceSpanRecord {
		return {
			file: reason.source.file,
			start: {line: reason.source.startLine, column: reason.source.startColumn},
			end: {line: reason.source.endLine, column: reason.source.endColumn}
		};
	}

	static function blockerMessage(blockers:Array<RuntimePolicyBlockerRecord>):String {
		final lines = [
			'runtime policy `none` found ${blockers.length} deduplicated runtime blocker(s):'
		];
		for (blocker in blockers) {
			final source = '${blocker.source.file}:${blocker.source.start.line}:${blocker.source.start.column}-${blocker.source.end.line}:${blocker.source.end.column}';
			final chains = blocker.dependencyChains.map(chain -> chain.join(" -> ")).join("; ");
			final alternative = blocker.alternative == null ? "none available" : blocker.alternative;
			lines.push('- ${blocker.id} kind=${blocker.kind} operation=${blocker.operationId} surface=`${blocker.surface}` source=$source feature=${blocker.featureId} dependency-chains=[$chains] alternative=`$alternative`');
		}
		return lines.join("\n");
	}

	static function sortedUnique(values:Array<String>, label:String):Array<String> {
		final result:Array<String> = [];
		for (value in values) {
			validateText(value, label);
			appendUnique(result, value);
		}
		result.sort(RuntimeFeatureRegistry.compareUtf8);
		return result;
	}

	static function appendUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function validateText(value:String, label:String):Void {
		if (StringTools.trim(value) == "" || value.indexOf("\x00") != -1 || value.indexOf("\r") != -1 || value.indexOf("\n") != -1) {
			internal('$label must be non-empty single-line text');
		}
	}

	static function internal<T>(detail:String):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail);
}
