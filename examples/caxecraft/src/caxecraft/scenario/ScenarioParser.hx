package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;

/** Syntactic CAXEMAP entry point. Reference resolution belongs to the validator. */
final class ScenarioParser {
	public static function parse(records:Array<ScenarioLexRecord>):ScenarioReadResult<ParsedScenario>
		return new ScenarioDocumentReader(records).read();
}
