package caxecraft.scenario;

import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceLocation;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioCoordinate;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;

/**
	Shared position and source-diagnostic state for the CAXEMAP record readers.

	A reader either advances this cursor after accepting a complete record or
	returns a typed error. Keeping that rule in one small object makes partial
	reads visible and prevents the document, world, story, and CaxeFlow readers
	from inventing subtly different source-coordinate behavior.
**/
// Used only by the CAXEMAP parser. Game and editor code should call
// ScenarioParser.parse(...) instead of constructing this class.
@:noCompletion
final class ScenarioRecordCursor {
	final records:Array<ScenarioLexRecord>;
	final locations:Array<ScenarioSourceLocation> = [];

	public var position(default, null):Int = 0;

	public function new(records:Array<ScenarioLexRecord>)
		this.records = records;

	public inline function hasRecord():Bool
		return position < records.length;

	public inline function current():ScenarioLexRecord
		return records[position];

	public inline function advance():Void
		position++;

	public function lastRecord():ScenarioLexRecord
		return records[records.length - 1];

	public function recordCoordinates():Array<ScenarioCoordinate>
		return [for (record in records) record.coordinate];

	public function sourceLocations():Array<ScenarioSourceLocation>
		return locations.copy();

	public function locate(subject:ScenarioSourceSubject, record:ScenarioLexRecord):Void
		locations.push({subject: subject, coordinate: record.coordinate});

	public function failAt<T>(record:Null<ScenarioLexRecord>, kind:ScenarioDiagnosticKind):ScenarioReadResult<T> {
		final coordinate:ScenarioCoordinate = record == null ? {line: 1, column: 1, record: 0} : record.coordinate;
		return ReadError([{coordinate: coordinate, kind: kind}]);
	}

	public function failToken<T>(token:ScenarioLexToken, kind:ScenarioDiagnosticKind):ScenarioReadResult<T>
		return ReadError([{coordinate: token.coordinate, kind: kind}]);
}
