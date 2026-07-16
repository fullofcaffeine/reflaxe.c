package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CEnvironment;
import reflaxe.c.CProfile;
import reflaxe.c.CRuntimeDiagnostics;
import reflaxe.c.CRuntimePolicy;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeArtifactRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeDependencyEdgeRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureAvailability;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverride;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureOverrideAction;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeOverrideRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningRequest;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeReasonRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementReason;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeSelectedFeatureRecord;

private typedef MutableDependencyEdge = {
	final featureId:String;
	final dependencyId:String;
	final reasonIds:Array<String>;
}

/** Resolves source-rooted requests into one policy-checked exact feature closure. */
class RuntimeFeaturePlanner {
	public static inline final PLAN_ALGORITHM = "hxc-runtime-plan-v1";
	public static inline final PLAN_SCHEMA_VERSION = 1;

	final registry:RuntimeFeatureRegistry;

	public function new(registry:RuntimeFeatureRegistry) {
		this.registry = registry;
	}

	public function plan(request:RuntimePlanningRequest):RuntimeFeaturePlanSnapshot {
		validateRequest(request);
		final reasons = canonicalReasons(request.rootReasons);
		if (reasons.length == 0) {
			if (request.noRuntimeProof == null || StringTools.trim(request.noRuntimeProof) == "") {
				internal("an empty runtime request requires a positive compiler-owned no-runtime proof");
			}
		} else {
			if (request.noRuntimeProof != null) {
				internal("a runtime-using request cannot also carry a no-runtime proof");
			}
			if (request.runtimePolicy == CRuntimePolicy.None) {
				policyViolation('runtime policy `none` forbids root feature `${reasons[0].featureId}`', [reasons[0].featureId.text()]);
			}
		}

		final selected:Map<String, Bool> = [];
		final rootFeatures:Map<String, Bool> = [];
		final reasonIdsByFeature:Map<String, Array<String>> = [];
		final edgesByKey:Map<String, MutableDependencyEdge> = [];
		for (reason in reasons) {
			final featureId = reason.featureId.text();
			rootFeatures.set(featureId, true);
			select(featureId, reason.id, request, selected, reasonIdsByFeature, edgesByKey);
		}

		final overrides = canonicalOverrides(request.manualOverrides);
		validateOverrides(overrides, selected, request);
		final orderedFeatures = dependencyOrder(selected);
		final dependencyEdges = dependencyRecords(edgesByKey);
		final featureRecords:Array<RuntimeSelectedFeatureRecord> = [];
		final artifactDetails:Array<RuntimeArtifactRecord> = [];
		final symbols:Array<String> = [];
		final libraries:Array<String> = [];
		final defines:Array<String> = [];
		for (featureId in orderedFeatures) {
			final definition = requireDefinition(featureId);
			final reasonIds = requiredReasonIds(reasonIdsByFeature, featureId);
			if (reasonIds.length == 0) {
				internal('selected runtime feature `$featureId` has an empty source-reason set', [featureId]);
			}
			reasonIds.sort(RuntimeFeatureRegistry.compareUtf8);
			final dependencies = definition.dependencies.map(dependency -> dependency.text());
			dependencies.sort(RuntimeFeatureRegistry.compareUtf8);
			final featureArtifacts = definition.artifacts.map(artifact -> artifact.outputPath);
			featureArtifacts.sort(RuntimeFeatureRegistry.compareUtf8);
			final featureSymbols = sortedUnique(definition.symbols);
			final featureLibraries = sortedUnique(definition.libraries);
			final featureDefines = definition.defines.map(define -> define.display());
			featureDefines.sort(RuntimeFeatureRegistry.compareUtf8);
			featureRecords.push({
				id: featureId,
				availability: definition.availability,
				root: rootFeatures.exists(featureId),
				reasonIds: reasonIds.copy(),
				dependencies: dependencies,
				artifacts: featureArtifacts,
				symbols: featureSymbols,
				libraries: featureLibraries,
				defines: featureDefines
			});
			for (artifact in definition.artifacts) {
				artifactDetails.push({
					featureId: featureId,
					sourcePath: artifact.sourcePath,
					outputPath: artifact.outputPath,
					kind: artifact.kind
				});
			}
			appendUnique(symbols, definition.symbols);
			appendUnique(libraries, definition.libraries);
			appendUnique(defines, featureDefines);
		}
		artifactDetails.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.outputPath, right.outputPath));
		symbols.sort(RuntimeFeatureRegistry.compareUtf8);
		libraries.sort(RuntimeFeatureRegistry.compareUtf8);
		defines.sort(RuntimeFeatureRegistry.compareUtf8);
		final artifacts = artifactDetails.map(artifact -> artifact.outputPath);
		final directDecisions = sortedUnique(request.directDecisions);
		return {
			schemaVersion: PLAN_SCHEMA_VERSION,
			algorithm: PLAN_ALGORITHM,
			status: orderedFeatures.length == 0 ? RuntimeFeaturePlanStatus.RuntimeFree : request.purpose == RuntimePlanningPurpose.CompilerProgram ? RuntimeFeaturePlanStatus.RuntimeFeatures : RuntimeFeaturePlanStatus.NativeSeedFeatures,
			planPurpose: request.purpose,
			profile: request.profile,
			requestedPolicy: request.runtimePolicy,
			resolvedPolicy: request.runtimePolicy,
			policyProvenance: request.runtimePolicyProvenance,
			diagnosticMode: request.runtimeDiagnostics,
			diagnosticProvenance: request.runtimeDiagnosticsProvenance,
			environment: request.environment,
			rootReasons: reasons.map(reasonRecord),
			manualOverrides: overrides.map(overrideRecord),
			directDecisions: directDecisions,
			dependencyEdges: dependencyEdges,
			selectedFeatures: featureRecords,
			features: orderedFeatures,
			artifactDetails: artifactDetails,
			artifacts: artifacts,
			symbols: symbols,
			libraries: libraries,
			defines: defines,
			noRuntimeProof: orderedFeatures.length == 0 ? request.noRuntimeProof : null
		};
	}

	function select(featureId:String, reasonId:String, request:RuntimePlanningRequest, selected:Map<String, Bool>,
			reasonIdsByFeature:Map<String, Array<String>>, edgesByKey:Map<String, MutableDependencyEdge>):Void {
		final definition = selectableDefinition(featureId, request);
		selected.set(featureId, true);
		appendUniqueValue(reasonIdsByFeature, featureId, reasonId);
		final dependencies = definition.dependencies.map(dependency -> dependency.text());
		dependencies.sort(RuntimeFeatureRegistry.compareUtf8);
		for (dependencyId in dependencies) {
			final key = edgeKey(featureId, dependencyId);
			var edge = edgesByKey.get(key);
			if (edge == null) {
				edge = {featureId: featureId, dependencyId: dependencyId, reasonIds: []};
				edgesByKey.set(key, edge);
			}
			if (edge.reasonIds.indexOf(reasonId) == -1) {
				edge.reasonIds.push(reasonId);
			}
			select(dependencyId, reasonId, request, selected, reasonIdsByFeature, edgesByKey);
		}
	}

	function selectableDefinition(featureId:String, request:RuntimePlanningRequest):RuntimeFeatureDefinition {
		final definition = registry.definitionByText(featureId);
		if (definition != null) {
			if (request.purpose == RuntimePlanningPurpose.CompilerProgram
				&& definition.availability != RuntimeFeatureAvailability.CompilerSelectable) {
				policyViolation('runtime feature `$featureId` is native-seed evidence and cannot be selected by generated Haxe', [featureId]);
			}
			if (definition.environments.indexOf(request.environment) == -1) {
				policyViolation('runtime feature `$featureId` is unavailable in environment `${request.environment}`', [featureId]);
			}
			if (request.runtimePolicy == CRuntimePolicy.Minimal && !definition.minimalAllowed) {
				policyViolation('runtime feature `$featureId` is outside the minimal allowlist', [featureId]);
			}
			return definition;
		}
		final reserved = registry.reservationByText(featureId);
		if (reserved != null) {
			policyViolation('runtime feature `$featureId` is reserved for ${reserved.ownerTask} and is not implemented', [featureId]);
		}
		return policyViolation('runtime feature `$featureId` is not registered', [featureId]);
	}

	function dependencyOrder(selected:Map<String, Bool>):Array<String> {
		final result:Array<String> = [];
		final visited:Map<String, Bool> = [];
		final ids = [for (id in selected.keys()) id];
		ids.sort(RuntimeFeatureRegistry.compareUtf8);
		for (id in ids) {
			appendDependencyOrder(id, selected, visited, result);
		}
		return result;
	}

	function appendDependencyOrder(featureId:String, selected:Map<String, Bool>, visited:Map<String, Bool>, result:Array<String>):Void {
		if (visited.exists(featureId)) {
			return;
		}
		visited.set(featureId, true);
		final definition = requireDefinition(featureId);
		final dependencies = definition.dependencies.map(dependency -> dependency.text());
		dependencies.sort(RuntimeFeatureRegistry.compareUtf8);
		for (dependencyId in dependencies) {
			if (!selected.exists(dependencyId)) {
				internal('selected feature `$featureId` lost dependency `$dependencyId`', [featureId, dependencyId]);
			}
			appendDependencyOrder(dependencyId, selected, visited, result);
		}
		result.push(featureId);
	}

	function dependencyRecords(edgesByKey:Map<String, MutableDependencyEdge>):Array<RuntimeDependencyEdgeRecord> {
		final keys = [for (key in edgesByKey.keys()) key];
		keys.sort(RuntimeFeatureRegistry.compareUtf8);
		final result:Array<RuntimeDependencyEdgeRecord> = [];
		for (key in keys) {
			final edge = requiredEdge(edgesByKey, key);
			edge.reasonIds.sort(RuntimeFeatureRegistry.compareUtf8);
			result.push({featureId: edge.featureId, dependencyId: edge.dependencyId, reasonIds: edge.reasonIds.copy()});
		}
		return result;
	}

	function canonicalReasons(input:Array<RuntimeRequirementReason>):Array<RuntimeRequirementReason> {
		final reasons = input.copy();
		reasons.sort((left, right) -> RuntimeFeatureRegistry.compareUtf8(left.id, right.id));
		var previous:Null<String> = null;
		for (reason in reasons) {
			if (!~/^[a-z][a-z0-9]*(?:[.-][a-z0-9]+)*$/.match(reason.id)) {
				internal('runtime reason ID must be stable lowercase dotted or kebab text: `${reason.id}`');
			}
			if (previous == reason.id) {
				internal('runtime reason ID `${reason.id}` is duplicated');
			}
			previous = reason.id;
			validateText(reason.kind, 'runtime reason `${reason.id}` kind');
			validateText(reason.surface, 'runtime reason `${reason.id}` typed surface');
			if (reason.alternative != null) {
				validateText(reason.alternative, 'runtime reason `${reason.id}` alternative');
			}
			validateSpan(reason.source, reason.id);
			if (registry.definitionByText(reason.featureId.text()) == null
				&& registry.reservationByText(reason.featureId.text()) == null) {
				policyViolation('runtime reason `${reason.id}` requests unknown feature `${reason.featureId}`', [reason.featureId.text()]);
			}
		}
		return reasons;
	}

	function canonicalOverrides(input:Array<RuntimeFeatureOverride>):Array<RuntimeFeatureOverride> {
		final overrides = input.copy();
		overrides.sort((left, right) -> {
			final featureComparison = RuntimeFeatureRegistry.compareUtf8(left.featureId.text(), right.featureId.text());
			return featureComparison != 0 ? featureComparison : RuntimeFeatureRegistry.compareUtf8(Std.string(left.action), Std.string(right.action));
		});
		var previousFeature:Null<String> = null;
		for (manual in overrides) {
			final featureId = manual.featureId.text();
			if (previousFeature == featureId) {
				policyViolation('runtime feature `$featureId` has duplicate or conflicting manual overrides', [featureId]);
			}
			previousFeature = featureId;
			validateText(manual.provenance, 'runtime feature `$featureId` override provenance');
			switch manual.action {
				case RuntimeFeatureOverrideAction.Require | RuntimeFeatureOverrideAction.Forbid:
				case _:
					policyViolation('runtime feature `$featureId` has unknown override action `${Std.string(manual.action)}`', [featureId]);
			}
			if (registry.definitionByText(featureId) == null) {
				policyViolation('manual override names unavailable runtime feature `$featureId`', [featureId]);
			}
		}
		return overrides;
	}

	function validateOverrides(overrides:Array<RuntimeFeatureOverride>, selected:Map<String, Bool>, request:RuntimePlanningRequest):Void {
		for (manual in overrides) {
			final featureId = manual.featureId.text();
			switch manual.action {
				case Require:
					if (!selected.exists(featureId)) {
						policyViolation('manual override cannot force unused runtime feature `$featureId`; no semantic root selected it', [featureId]);
					}
					if (request.runtimePolicy == CRuntimePolicy.None) {
						policyViolation('manual override cannot require `$featureId` under runtime policy `none`', [featureId]);
					}
				case Forbid:
					if (selected.exists(featureId)) {
						policyViolation('manual override forbids runtime feature `$featureId` required by reachable semantics', [featureId]);
					}
			}
		}
	}

	function validateRequest(request:RuntimePlanningRequest):Void {
		switch request.purpose {
			case RuntimePlanningPurpose.CompilerProgram | RuntimePlanningPurpose.NativeSeedFixture:
			case _:
				internal('unknown runtime planning purpose `${Std.string(request.purpose)}`');
		}
		switch request.profile {
			case CProfile.Portable | CProfile.Metal:
			case _:
				internal('unknown runtime planning profile `${Std.string(request.profile)}`');
		}
		switch request.environment {
			case CEnvironment.Hosted | CEnvironment.Freestanding | CEnvironment.Wasi | CEnvironment.Emscripten:
			case _:
				internal('unknown runtime planning environment `${Std.string(request.environment)}`');
		}
		switch request.runtimePolicy {
			case CRuntimePolicy.Auto | CRuntimePolicy.Minimal | CRuntimePolicy.None:
			case _:
				internal('unknown runtime policy `${Std.string(request.runtimePolicy)}`');
		}
		switch request.runtimeDiagnostics {
			case CRuntimeDiagnostics.Off | CRuntimeDiagnostics.Summary | CRuntimeDiagnostics.Warn:
			case _:
				internal('unknown runtime diagnostic mode `${Std.string(request.runtimeDiagnostics)}`');
		}
		validateText(request.runtimePolicyProvenance, "runtime policy provenance");
		validateText(request.runtimeDiagnosticsProvenance, "runtime diagnostic provenance");
		for (decision in request.directDecisions) {
			validateText(decision, "direct runtime-planning decision");
		}
	}

	function requireDefinition(featureId:String):RuntimeFeatureDefinition {
		final definition = registry.definitionByText(featureId);
		if (definition != null) {
			return definition;
		}
		return internal('runtime feature graph lost definition `$featureId`', [featureId]);
	}

	static function reasonRecord(reason:RuntimeRequirementReason):RuntimeReasonRecord {
		return {
			id: reason.id,
			featureId: reason.featureId.text(),
			kind: reason.kind,
			surface: reason.surface,
			source: {
				file: reason.source.file,
				start: {line: reason.source.startLine, column: reason.source.startColumn},
				end: {line: reason.source.endLine, column: reason.source.endColumn}
			},
			alternative: reason.alternative
		};
	}

	static function overrideRecord(manual:RuntimeFeatureOverride):RuntimeOverrideRecord
		return {featureId: manual.featureId.text(), action: manual.action, provenance: manual.provenance};

	static function validateSpan(source:HxcSourceSpan, reasonId:String):Void {
		if (!HxcSourceSpan.isNormalizedFile(source.file)
			|| source.startLine < 1
			|| source.startColumn < 1
			|| source.endLine < source.startLine
			|| source.endColumn < 1
			|| source.endLine == source.startLine
			&& source.endColumn < source.startColumn) {
			internal('runtime reason `$reasonId` has malformed source `${source.file}`');
		}
	}

	static function appendUniqueValue(valuesByKey:Map<String, Array<String>>, key:String, value:String):Void {
		var values = valuesByKey.get(key);
		if (values == null) {
			values = [];
			valuesByKey.set(key, values);
		}
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function requiredReasonIds(valuesByKey:Map<String, Array<String>>, featureId:String):Array<String> {
		for (key => values in valuesByKey) {
			if (key == featureId) {
				return values;
			}
		}
		return internal('selected runtime feature `$featureId` has no propagated source reason', [featureId]);
	}

	static function requiredEdge(edgesByKey:Map<String, MutableDependencyEdge>, expectedKey:String):MutableDependencyEdge {
		for (key => edge in edgesByKey) {
			if (key == expectedKey) {
				return edge;
			}
		}
		return internal('runtime dependency edge `$expectedKey` disappeared during canonicalization');
	}

	static function appendUnique(target:Array<String>, values:Array<String>):Void {
		for (value in values) {
			if (target.indexOf(value) == -1) {
				target.push(value);
			}
		}
	}

	static function sortedUnique(values:Array<String>):Array<String> {
		final result:Array<String> = [];
		appendUnique(result, values);
		result.sort(RuntimeFeatureRegistry.compareUtf8);
		return result;
	}

	static function edgeKey(featureId:String, dependencyId:String):String
		return '$featureId\x1f$dependencyId';

	static function validateText(value:String, label:String):Void {
		if (StringTools.trim(value) == "" || value.indexOf("\x00") != -1 || value.indexOf("\r") != -1 || value.indexOf("\n") != -1) {
			internal('$label must be non-empty single-line text');
		}
	}

	static function policyViolation<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.RuntimePolicyViolation, detail, sortedFeatureIds(featureIds));

	static function internal<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail, sortedFeatureIds(featureIds));

	static function sortedFeatureIds(values:Null<Array<String>>):Array<String> {
		final result = values == null ? [] : values.copy();
		result.sort(RuntimeFeatureRegistry.compareUtf8);
		return result;
	}
}
