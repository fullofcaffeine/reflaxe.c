package reflaxe.c.runtime;

import haxe.io.Bytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CEnvironment;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFile.GeneratedFileKind;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureCatalogSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureCatalogStatus;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinition;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureDefinitionRecord;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureReservation;

/** Validated immutable-by-convention graph with deterministic dependency order. */
class RuntimeFeatureRegistry {
	public static inline final ALGORITHM = "hxc-runtime-feature-graph-v1";
	public static inline final SCHEMA_VERSION = 1;

	final definitions:Array<RuntimeFeatureDefinition>;
	final reservations:Array<RuntimeFeatureReservation>;
	final definitionsById:Map<String, RuntimeFeatureDefinition> = [];
	final reservationsById:Map<String, RuntimeFeatureReservation> = [];

	public function new(definitions:Array<RuntimeFeatureDefinition>, reservations:Array<RuntimeFeatureReservation>) {
		this.definitions = definitions.copy();
		this.reservations = reservations.copy();
		validateAndIndex();
		this.definitions.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		this.reservations.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
	}

	public function allDefinitions():Array<RuntimeFeatureDefinition>
		return definitions.copy();

	public function allReservations():Array<RuntimeFeatureReservation>
		return reservations.copy();

	public function definition(id:RuntimeFeatureId):Null<RuntimeFeatureDefinition>
		return definitionsById.get(id.text());

	public function definitionByText(id:String):Null<RuntimeFeatureDefinition>
		return definitionsById.get(id);

	public function reservation(id:RuntimeFeatureId):Null<RuntimeFeatureReservation>
		return reservationsById.get(id.text());

	public function reservationByText(id:String):Null<RuntimeFeatureReservation>
		return reservationsById.get(id);

	public function catalogSnapshot():RuntimeFeatureCatalogSnapshot {
		final compilerSelectableFeatures:Array<String> = [];
		final featureRecords:Array<RuntimeFeatureDefinitionRecord> = [];
		for (definition in definitions) {
			if (definition.availability == CompilerSelectable) {
				compilerSelectableFeatures.push(definition.id.text());
			}
			final environments = definition.environments.copy();
			environments.sort((left, right) -> compareUtf8(Std.string(left), Std.string(right)));
			final dependencies = definition.dependencies.map(id -> id.text());
			dependencies.sort(compareUtf8);
			final artifacts = definition.artifacts.map(artifact -> {
				sourcePath: artifact.sourcePath,
				outputPath: artifact.outputPath,
				kind: artifact.kind
			});
			artifacts.sort((left, right) -> compareUtf8(left.outputPath, right.outputPath));
			final symbols = sortedUniqueCopy(definition.symbols);
			final libraries = sortedUniqueCopy(definition.libraries);
			final defines = definition.defines.map(define -> define.display());
			defines.sort(compareUtf8);
			featureRecords.push({
				id: definition.id.text(),
				summary: definition.summary,
				availability: definition.availability,
				minimalAllowed: definition.minimalAllowed,
				environments: environments,
				dependencies: dependencies,
				artifacts: artifacts,
				symbols: symbols,
				libraries: libraries,
				defines: defines
			});
		}
		compilerSelectableFeatures.sort(compareUtf8);
		return {
			schemaVersion: SCHEMA_VERSION,
			algorithm: ALGORITHM,
			status: compilerSelectableFeatures.length == 0 ? RuntimeFeatureCatalogStatus.ProvisionalNativeSeedPackaging : RuntimeFeatureCatalogStatus.SelectiveCompilerPackaging,
			requirements: ["HXC-RT-001", "HXC-RT-002", "HXC-RT-004", "HXC-RT-008"],
			noUnconditionalCore: true,
			compilerSelectableFeatures: compilerSelectableFeatures,
			features: featureRecords,
			reservedFeatures: reservations.map(reservation -> {
				id: reservation.id.text(),
				ownerTask: reservation.ownerTask,
				summary: reservation.summary
			})
		};
	}

	function validateAndIndex():Void {
		final artifactSourceOwners:Map<String, String> = [];
		final artifactOutputOwners:Map<String, String> = [];
		final symbolOwners:Map<String, String> = [];
		for (definition in definitions) {
			final id = definition.id.text();
			if (definitionsById.exists(id) || reservationsById.exists(id)) {
				internal('runtime feature ID `$id` is duplicated', [id]);
			}
			validateText(definition.summary, 'runtime feature `$id` summary');
			validateAvailability(definition);
			validateEnvironments(definition);
			definitionsById.set(id, definition);
			for (artifact in definition.artifacts) {
				validateArtifact(id, artifact.sourcePath, artifact.outputPath, artifact.kind);
				final previousSourceOwner = artifactSourceOwners.get(artifact.sourcePath);
				if (previousSourceOwner != null) {
					internal('runtime artifact source `${artifact.sourcePath}` is owned by both `$previousSourceOwner` and `$id`', [previousSourceOwner, id]);
				}
				artifactSourceOwners.set(artifact.sourcePath, id);
				final previousOutputOwner = artifactOutputOwners.get(artifact.outputPath);
				if (previousOutputOwner != null) {
					internal('runtime artifact output `${artifact.outputPath}` is owned by both `$previousOutputOwner` and `$id`', [previousOutputOwner, id]);
				}
				artifactOutputOwners.set(artifact.outputPath, id);
			}
			for (symbol in definition.symbols) {
				validateRuntimeSymbol(symbol, id);
				final previous = symbolOwners.get(symbol);
				if (previous != null) {
					internal('runtime symbol `$symbol` is owned by both `$previous` and `$id`', [previous, id]);
				}
				symbolOwners.set(symbol, id);
			}
			validateUniqueText(definition.libraries, 'runtime feature `$id` library');
			final defineNames:Map<String, Bool> = [];
			for (define in definition.defines) {
				if (!~/^[A-Z][A-Z0-9_]*$/.match(define.name)) {
					internal('runtime feature `$id` has invalid define `${define.name}`', [id]);
				}
				validateText(define.value, 'runtime feature `$id` define `${define.name}` value');
				if (defineNames.exists(define.name)) {
					internal('runtime feature `$id` repeats define `${define.name}`', [id]);
				}
				defineNames.set(define.name, true);
			}
		}
		for (reservation in reservations) {
			final id = reservation.id.text();
			if (definitionsById.exists(id) || reservationsById.exists(id)) {
				internal('runtime feature ID `$id` is duplicated or both defined and reserved', [id]);
			}
			if (!~/^E[0-9]+\.T[0-9]+$/.match(reservation.ownerTask)) {
				internal('runtime feature reservation `$id` has invalid owner `${reservation.ownerTask}`', [id]);
			}
			validateText(reservation.summary, 'runtime feature reservation `$id` summary');
			reservationsById.set(id, reservation);
		}
		for (definition in definitions) {
			final seen:Map<String, Bool> = [];
			for (dependency in definition.dependencies) {
				final dependencyId = dependency.text();
				if (seen.exists(dependencyId)) {
					internal('runtime feature `${definition.id}` repeats dependency `$dependencyId`', [definition.id.text(), dependencyId]);
				}
				seen.set(dependencyId, true);
				if (!definitionsById.exists(dependencyId)) {
					internal('runtime feature `${definition.id}` depends on unavailable `$dependencyId`', [definition.id.text(), dependencyId]);
				}
			}
		}
		validateAcyclic();
	}

	function validateAvailability(definition:RuntimeFeatureDefinition):Void {
		switch definition.availability {
			case CompilerSelectable | NativeSeedOnly:
			case _:
				internal('runtime feature `${definition.id}` has unknown availability `${Std.string(definition.availability)}`', [definition.id.text()]);
		}
	}

	function validateEnvironments(definition:RuntimeFeatureDefinition):Void {
		if (definition.environments.length == 0) {
			internal('runtime feature `${definition.id}` supports no environment', [definition.id.text()]);
		}
		final seen:Map<String, Bool> = [];
		for (environment in definition.environments) {
			switch environment {
				case CEnvironment.Hosted | CEnvironment.Freestanding | CEnvironment.Wasi | CEnvironment.Emscripten:
				case _:
					internal('runtime feature `${definition.id}` has unknown environment `${Std.string(environment)}`', [definition.id.text()]);
			}
			final key = Std.string(environment);
			if (seen.exists(key)) {
				internal('runtime feature `${definition.id}` repeats environment `$key`', [definition.id.text()]);
			}
			seen.set(key, true);
		}
	}

	function validateArtifact(featureId:String, sourcePath:String, outputPath:String, kind:GeneratedFileKind):Void {
		if (!GeneratedFile.isNormalizedRelativePath(sourcePath) || !GeneratedFile.isNormalizedRelativePath(outputPath)) {
			internal('runtime feature `$featureId` has a non-normalized artifact path `$sourcePath` -> `$outputPath`', [featureId]);
		}
		final valid = switch kind {
			case RuntimeHeader:
				StringTools.startsWith(sourcePath, "runtime/hxrt/include/")
				&& StringTools.startsWith(outputPath, "runtime/include/")
				&& StringTools.endsWith(sourcePath, ".h")
				&& StringTools.endsWith(outputPath, ".h");
			case RuntimeSource:
				StringTools.startsWith(sourcePath, "runtime/hxrt/src/")
				&& StringTools.startsWith(outputPath, "runtime/src/")
				&& StringTools.endsWith(sourcePath, ".c")
				&& StringTools.endsWith(outputPath, ".c");
			case _:
				false;
		};
		if (!valid) {
			internal('runtime feature `$featureId` artifact `$sourcePath` -> `$outputPath` has invalid kind `${Std.string(kind)}`', [featureId]);
		}
	}

	function validateRuntimeSymbol(symbol:String, featureId:String):Void {
		if (!~/^hxc_[A-Za-z0-9_]+$/.match(symbol)) {
			internal('runtime feature `$featureId` has invalid symbol `$symbol`', [featureId]);
		}
	}

	function validateAcyclic():Void {
		final state:Map<String, Int> = [];
		final stack:Array<String> = [];
		final ids = definitions.map(definition -> definition.id.text());
		ids.sort(compareUtf8);
		for (id in ids) {
			visit(id, state, stack);
		}
	}

	function visit(id:String, state:Map<String, Int>, stack:Array<String>):Void {
		final current = state.get(id);
		if (current == 2) {
			return;
		}
		if (current == 1) {
			final start = stack.indexOf(id);
			final cycle = (start < 0 ? stack.copy() : stack.slice(start));
			cycle.push(id);
			internal('runtime feature dependency cycle: ${cycle.join(" -> ")}', cycle);
		}
		state.set(id, 1);
		stack.push(id);
		final definition = requireDefinition(id);
		final dependencies = definition.dependencies.map(dependency -> dependency.text());
		dependencies.sort(compareUtf8);
		for (dependencyId in dependencies) {
			visit(dependencyId, state, stack);
		}
		stack.pop();
		state.set(id, 2);
	}

	static function validateUniqueText(values:Array<String>, label:String):Void {
		final seen:Map<String, Bool> = [];
		for (value in values) {
			validateText(value, label);
			if (seen.exists(value)) {
				internal('$label `$value` is duplicated');
			}
			seen.set(value, true);
		}
	}

	static function validateText(value:String, label:String):Void {
		if (StringTools.trim(value) == "" || value.indexOf("\x00") != -1 || value.indexOf("\r") != -1 || value.indexOf("\n") != -1) {
			internal('$label must be non-empty single-line text');
		}
	}

	static function sortedUniqueCopy(values:Array<String>):Array<String> {
		final result:Array<String> = [];
		for (value in values) {
			if (result.indexOf(value) == -1) {
				result.push(value);
			}
		}
		result.sort(compareUtf8);
		return result;
	}

	function requireDefinition(id:String):RuntimeFeatureDefinition {
		for (definition in definitions) {
			if (definition.id.text() == id) {
				return definition;
			}
		}
		return internal('runtime feature graph lost definition `$id`', [id]);
	}

	public static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function internal<T>(detail:String, ?featureIds:Array<String>):T
		throw new RuntimeFeatureError(CDiagnosticId.InternalCompilerError, detail, featureIds);
}
