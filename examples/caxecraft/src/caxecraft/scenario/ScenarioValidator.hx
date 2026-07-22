package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;

/**
	Semantic gate between a parsed candidate and a playable Scenario.

	The call order below is also the diagnostic order promised by CAXEMAP 1.
	Each collaborator checks one part of the model, while the shared context owns
	identity indexes and source coordinates. Keeping the order here makes it hard
	for a readability refactor to silently reorder user-facing errors.
**/
final class ScenarioValidator {
	public static function validate(parsed:ParsedScenario, registry:ScenarioContentRegistry):ScenarioReadResult<Scenario> {
		final context = new ScenarioValidationContext(parsed, registry);
		final document = new ScenarioDocumentValidator(context);
		final world = new ScenarioWorldValidator(context);

		document.validateFeatures();
		new ScenarioMessageValidator(context).validate();
		world.validateWorld();
		context.indexIdentities();
		world.validateFluids();
		world.validateObjects();
		new ScenarioStoryValidator(context).validate();
		new CaxeFlowValidator(context).validate();
		document.validateExtensions();

		return context.diagnostics.length == 0 ? ReadOk(parsed.candidate) : ReadError(context.diagnostics);
	}
}
