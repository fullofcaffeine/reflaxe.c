package caxecraft.scenario;

import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;

/**
	Checks dialogue speakers and the objective links that make up story routes.

	This class is used only by `ScenarioValidator`. `@:noCompletion` keeps the
	implementation helper out of editor suggestions; it does not change Haxe
	visibility, runtime behavior, or type safety.
**/
@:noCompletion
final class ScenarioStoryValidator {
	final context:ScenarioValidationContext;

	public function new(context:ScenarioValidationContext)
		this.context = context;

	public function validate():Void {
		for (dialogue in context.scenario.story.dialogues) {
			final coordinate = context.coordinateForIdentity(dialogue.id, DialogueIdentity);
			if (dialogue.lines.length > ScenarioLimits.MAX_DIALOGUE_LINES)
				context.addAtCoordinate(LimitExceeded(DialogueLines, ScenarioLimits.MAX_DIALOGUE_LINES), coordinate);
			for (line in dialogue.lines)
				if (line.speaker != null && !context.hasObject(line.speaker))
					context.addAtCoordinate(UnresolvedReference(line.speaker), coordinate);
		}
		for (route in context.scenario.story.routes) {
			final coordinate = context.coordinateForIdentity(route.id, RouteIdentity);
			for (objective in route.objectives)
				if (!context.hasObjective(objective))
					context.addAtCoordinate(UnresolvedReference(objective), coordinate);
		}
	}
}
