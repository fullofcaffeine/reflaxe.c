package;

import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioId;

/** Must fail because a registry content key is not a scenario object identity. */
final class ScenarioIdentityMixup {
	static function needsScenarioId(id:ScenarioId):Void {}

	static function main():Void {
		needsScenarioId(new ContentId("caxecraft:stone"));
	}
}
