package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowExecutor;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickInput;
import caxecraft.scenario.CaxeFlowRuntime.FlowTickResult;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import haxe.Int64;

/**
	One disposable simulation created from a validated editor snapshot.

	Its mutable CaxeFlow state never points back into the draft. Leaving test
	play discards this object and every play-time change with it.
**/
final class EditorTestPlay {
	final scenario:Scenario;
	final executor:CaxeFlowExecutor;

	/** Only `EditorSession` may create a test from its validated deep copy. */
	@:allow(caxecraft.editor.EditorSession)
	function new(scenario:Scenario) {
		this.scenario = scenario;
		this.executor = new CaxeFlowExecutor(scenario);
	}

	public inline function runTick(input:FlowTickInput):FlowTickResult
		return executor.runTick(input);

	public inline function tick():Int64
		return executor.tick();

	public inline function variable(id:ScenarioId):Null<FlowValue>
		return executor.variable(id);

	public inline function inventoryQuantity(owner:ScenarioId, itemType:ContentId):Int
		return executor.inventoryQuantity(owner, itemType);

	public inline function objectActive(id:ScenarioId):Bool
		return executor.objectActive(id);

	public inline function objectState(id:ScenarioId):Null<ContentId>
		return executor.objectState(id);

	public inline function objectiveState(id:ScenarioId):Null<ObjectiveState>
		return executor.objectiveState(id);

	public inline function hasJournal(id:ScenarioId):Bool
		return executor.hasJournal(id);

	public inline function checkpoint():Null<ScenarioId>
		return executor.checkpoint();

	public function scenarioSnapshot():Scenario {
		return switch EditorScenarioSnapshot.capture(scenario) {
			case ImageReady(image): image.parsed.candidate;
			case ImageRejected(_): throw "validated editor test-play snapshot became unreadable";
		}
	}
}
