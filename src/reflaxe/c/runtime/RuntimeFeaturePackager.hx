package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeArtifactRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;

/** Narrow typed adapter over the package/repository text source. */
interface RuntimeArtifactSource {
	public function read(sourcePath:String):Null<String>;
}

/** Materializes exactly the artifact records already selected by the planner. */
class RuntimeFeaturePackager {
	final registry:RuntimeFeatureRegistry;

	public function new(registry:RuntimeFeatureRegistry) {
		this.registry = registry;
	}

	public function packageFiles(plan:RuntimeFeaturePlanSnapshot, source:RuntimeArtifactSource):Array<GeneratedFile> {
		if (plan.schemaVersion != RuntimeFeaturePlanner.PLAN_SCHEMA_VERSION || plan.algorithm != RuntimeFeaturePlanner.PLAN_ALGORITHM) {
			throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError,
				'cannot package runtime plan schema `${plan.schemaVersion}` algorithm `${plan.algorithm}`');
		}
		if (plan.features.length == 0) {
			if (plan.status != RuntimeFeaturePlanStatus.RuntimeFree
				|| plan.noRuntimeProof == null
				|| StringTools.trim(plan.noRuntimeProof) == ""
				|| plan.artifactDetails.length != 0
				|| plan.artifacts.length != 0) {
				throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError,
					"runtime-free package plan has inconsistent status, proof, or artifact selections");
			}
			return [];
		}
		final expectedStatus = switch plan.planPurpose {
			case RuntimePlanningPurpose.CompilerProgram: RuntimeFeaturePlanStatus.RuntimeFeatures;
			case RuntimePlanningPurpose.NativeSeedFixture: RuntimeFeaturePlanStatus.NativeSeedFeatures;
			case _: invalid('runtime package plan has unknown purpose `${Std.string(plan.planPurpose)}`');
		};
		if (plan.status != expectedStatus || plan.noRuntimeProof != null) {
			invalid("runtime-using package plan has an inconsistent status or no-runtime proof");
		}
		validateArtifactSelection(plan);
		final files:Array<GeneratedFile> = [];
		for (artifact in plan.artifactDetails) {
			final contents = source.read(artifact.sourcePath);
			if (contents == null) {
				throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError,
					'selected runtime artifact `${artifact.sourcePath}` for `${artifact.featureId}` is unavailable', [artifact.featureId]);
			}
			files.push(new GeneratedFile(artifact.outputPath, contents, artifact.kind));
		}
		files.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.relativePath, right.relativePath));
		if (files.map(file -> file.relativePath).join("\n") != plan.artifacts.join("\n")) {
			throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, "runtime package artifact order differs from the resolved plan");
		}
		return files;
	}

	function validateArtifactSelection(plan:RuntimeFeaturePlanSnapshot):Void {
		final selected:Map<String, Bool> = [];
		final positions:Map<String, Int> = [];
		final expected:Array<RuntimeArtifactRecord> = [];
		for (index in 0...plan.features.length) {
			final featureId = plan.features[index];
			if (selected.exists(featureId)) {
				invalid('runtime package plan repeats selected feature `$featureId`', [featureId]);
			}
			final definition = requireDefinition(featureId);
			if (plan.planPurpose == RuntimePlanningPurpose.CompilerProgram
				&& definition.availability != RuntimeFeatureAvailability.CompilerSelectable) {
				invalid('runtime package plan exposes provisional feature `$featureId` to generated Haxe', [featureId]);
			}
			selected.set(featureId, true);
			positions.set(featureId, index);
			for (artifact in definition.artifacts) {
				expected.push({
					featureId: featureId,
					sourcePath: artifact.sourcePath,
					outputPath: artifact.outputPath,
					kind: artifact.kind
				});
			}
		}
		for (featureId in plan.features) {
			final definition = requireDefinition(featureId);
			final featurePosition = requiredPosition(positions, featureId);
			for (dependency in definition.dependencies) {
				final dependencyId = dependency.text();
				if (!selected.exists(dependencyId) || requiredPosition(positions, dependencyId) >= featurePosition) {
					invalid('runtime package feature `$featureId` is not dependency-closed before `$dependencyId`', [featureId, dependencyId]);
				}
			}
		}
		expected.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.outputPath, right.outputPath));
		if (expected.length != plan.artifactDetails.length) {
			invalid("runtime package artifact count differs from the selected feature registry");
		}
		if (expected.map(artifact -> artifact.outputPath).join("\n") != plan.artifacts.join("\n")) {
			invalid("runtime package artifact paths differ from the selected feature registry");
		}
		for (index in 0...expected.length) {
			final wanted = expected[index];
			final actual = plan.artifactDetails[index];
			if (actual.featureId != wanted.featureId
				|| actual.sourcePath != wanted.sourcePath
				|| actual.outputPath != wanted.outputPath
				|| actual.kind != wanted.kind) {
				invalid('runtime package artifact `${actual.sourcePath}` -> `${actual.outputPath}` is not the registered selection at index $index',
					[actual.featureId]);
			}
		}
	}

	function requireDefinition(featureId:String):RuntimeFeatureDefinition {
		final definition = registry.definitionByText(featureId);
		if (definition == null) {
			return invalid('runtime package plan names unavailable feature `$featureId`', [featureId]);
		}
		return definition;
	}

	static function requiredPosition(positions:Map<String, Int>, featureId:String):Int {
		final position = positions.get(featureId);
		if (position == null) {
			return invalid('runtime package plan lost selected feature position `$featureId`', [featureId]);
		}
		return position;
	}

	static function invalid<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail, featureIds);
}
