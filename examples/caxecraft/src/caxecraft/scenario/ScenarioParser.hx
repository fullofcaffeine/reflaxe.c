package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ParsedScenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;

/**
	Parses one complete CAXEMAP document from the records produced by
	`ScenarioLexer.read(...)`. Game and editor code call this method directly;
	the reader classes it uses are internal implementation details.

	This step checks the document's syntax. Call `ScenarioValidator.validate(...)`
	afterward to resolve references and check that the document makes sense as a
	whole.
**/
final class ScenarioParser {
	public static function parse(records:Array<ScenarioLexRecord>):ScenarioReadResult<ParsedScenario>
		return new ScenarioDocumentReader(records).read();
}
