package caxecraft.qa;

import caxecraft.content.LevelContentResolver;
import caxecraft.content.ResolvedLevelPlan;
import caxecraft.content.ResolvedLevelPlan.ResolvedLevelPlanResult;
import caxecraft.domain.EntityId;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;
import caxecraft.qa.FocusedContentFixture.standardAquaticProfile;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import haxe.io.Bytes;

/**
 * Supplies one synthetic level to the resolver and generation ownership probes.
 *
 * The helper contains no expected terrain, actor, or generation result. It only
 * runs a small CAXEMAP input through the repository lexer, parser,
 * validator, and resolver so each focused probe exercises the same semantic
 * input without copying that ingress pipeline.
 */
/** Parse and validate the synthetic level through production code. */
function readResolutionScenario():Null<Scenario> {
	return switch ScenarioLexer.read(fixtureBytes()) {
		case ReadError(_):
			null;
		case ReadOk(records):
			switch ScenarioParser.parse(records) {
				case ReadError(_):
					null;
				case ReadOk(parsed):
					switch ScenarioValidator.validate(parsed, new FocusedContentRegistry()) {
						case ReadError(_): null;
						case ReadOk(value): value;
					}
			}
	}
}

/** Resolve the fixture with the shared local-player mechanics. */
function resolveResolutionScenario(scenario:Scenario, registry:LevelContentResolver):ResolvedLevelPlanResult
	return ResolvedLevelPlan.resolve(scenario, registry, {
		entityId: EntityId.fromValidatedStorageCode(1),
		initialHealth: MAX_HEALTH,
		aquaticProfile: standardAquaticProfile()
	});

/**
 * Keep the parser input independent of campaign edits and filesystem services.
 *
 * The input contains a terrain boundary and one editable block. It also includes
 * two overlapping fluid declarations and each actor kind needed by the assertions.
 */
private function fixtureBytes():Bytes
	return Bytes.ofString("CAXEMAP 1\nfeature required caxecraft:core\nmap qa.level-resolution\nasset-pack packs/caxecraft/base\ndefault-locale en\nlocale en\n  message title \"Level resolution fixture\"\n  message greeting \"Test dialogue\"\nend locale\ntitle message title\nmode adventure\nworld 32 16 32\npalette 0 caxecraft:air\npalette 1 caxecraft:bedrock\npalette 2 caxecraft:grass\nchunk world.base 0 0 0 32 16 32\n  run 1 32\n  run 0 8304\n  run 2 1\n  run 0 8047\nend chunk\nfluid water.pool caxecraft:water volume 2 4 2 4 1 4\nfluid water.spring caxecraft:water source 3 4 3\nobject enemy.mossling\n  tag enemy\n  placement entity caxecraft:mossling 15500 5000 13800 0\nend object\nobject guide.nia\n  tag friend\n  placement npc caxecraft:nia dialogue.fixture 17500 5000 13500 270\nend object\nobject item.tideweave\n  tag equipment\n  placement item caxecraft:tideweave-suit 1 4500 5000 4500 0\nend object\nobject player.start\n  tag player\n  placement player-spawn 16500 5000 16500 0\nend object\ndialogue dialogue.fixture\n  line speaker guide.nia message greeting\nend dialogue\nend-map\n");
