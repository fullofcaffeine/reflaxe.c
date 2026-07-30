package caxecraft.qa;

import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.ResolvedLevelPlan;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;

/**
 * Supplies one shared validated first-playable fixture to content QA probes.
 *
 * The helper contains no expected terrain, actor, or generation result. It only
 * runs the real embedded CAXEMAP bytes through the repository lexer, parser,
 * validator, and resolver so each focused probe exercises the same semantic
 * input without copying that ingress pipeline.
 */
/** Parse and validate the embedded first-playable bytes through production code. */
function readFirstPlayableScenario():Null<Scenario> {
	return switch ScenarioLexer.read(ScenarioNativeCodecProbe.firstPlayableBytes()) {
		case ReadError(_):
			null;
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(_):
					null;
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, new BaseContentRegistry()) {
						case ReadError(_): null;
						case ReadOk(value): value;
					}
			}
	}
}

/** Resolve the fixture with the ordinary first-playable local-player mechanics. */
function resolveFirstPlayable(scenario:Scenario, registry:LevelContentResolver):ResolvedLevelPlanResult
	return ResolvedLevelPlan.resolve(scenario, registry, {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH,
		aquaticProfile: BaseContentPack.aquaticProfile(BaseContentPack.defaultAquaticProfile())
	});
