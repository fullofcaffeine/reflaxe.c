package caxecraft.scenario;

import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;

/**
	Checks document-wide feature and extension rules around the semantic model.

	This class is used only by `ScenarioValidator`. `@:noCompletion` keeps the
	implementation helper out of editor suggestions; it does not make the class
	private at runtime or change the checks it performs.
**/
@:noCompletion
final class ScenarioDocumentValidator {
	final context:ScenarioValidationContext;

	public function new(context:ScenarioValidationContext)
		this.context = context;

	public function validateFeatures():Void {
		final seen:Map<String, Bool> = [];
		var core = false;
		for (feature in context.scenario.requiredFeatures) {
			final key = feature.text();
			if (seen.exists(key))
				context.addAtCoordinate(DuplicateContentId(feature), context.coordinateForFeature(feature));
			seen.set(key, true);
			if (key == "caxecraft:core")
				core = true;
			if (!context.registry.supportsFeature(feature))
				context.add(UnknownRequiredFeature(feature));
		}
		for (feature in context.scenario.optionalFeatures) {
			final key = feature.text();
			if (seen.exists(key))
				context.addAtCoordinate(DuplicateContentId(feature), context.coordinateForFeature(feature));
			seen.set(key, true);
		}
		if (!core)
			context.add(MissingRecord(CoreFeatureRecord));
	}

	public function validateExtensions():Void {
		final optional:Map<String, Bool> = [];
		for (feature in context.scenario.optionalFeatures)
			optional.set(feature.text(), true);
		final identities:Map<String, Bool> = [];
		for (extension in context.scenario.extensions) {
			final key = extension.feature.text() + ":" + extension.id.text();
			if (!optional.exists(extension.feature.text()) || identities.exists(key))
				context.addAtCoordinate(InvalidExtension(extension.id), context.coordinateForExtension(extension.feature, extension.id));
			identities.set(key, true);
		}
	}
}
