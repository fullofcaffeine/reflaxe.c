package caxecraft.scenario;

import caxecraft.scenario.CaxeFlowRuntime.FlowExecutionLimit;
import caxecraft.scenario.CaxeFlowRuntime.FlowRuntimeDiagnostic;

/**
	Maps CaxeFlow failures to locale-data keys and typed replacement arguments.

	This module owns no translated prose. `RuntimeUiCatalog` resolves the stable
	key through validated locale data and substitutes these bounded scalar values.
**/
typedef CaxeFlowDiagnosticMessage = {
	final message:MessageId;
	final arguments:Array<String>;
}

/** Convert one closed runtime failure to a catalog key and ordered arguments. */
function caxeFlowDiagnosticMessage(diagnostic:FlowRuntimeDiagnostic):CaxeFlowDiagnosticMessage
	return switch diagnostic {
		case LimitExceeded(kind, maximum, owner):
			owner == null ? message("caxeflow.runtime.limit",
				[limitId(kind), '$maximum']) : message("caxeflow.runtime.limit-owner", [owner.text(), limitId(kind), '$maximum']);
		case InvalidRuntimeReference(id):
			message("caxeflow.runtime.stale-reference", [id.text()]);
		case InvalidRuntimeAction(owner):
			message("caxeflow.runtime.unsupported-action", [owner.text()]);
		case InvalidRuntimeEvent(id):
			message("caxeflow.runtime.invalid-event-context", [id.text()]);
	};

/** Return every catalog key this diagnostic boundary can emit. */
function requiredCaxeFlowDiagnosticMessageIds():Array<MessageId>
	return [
		new MessageId("caxeflow.runtime.invalid-event-context"),
		new MessageId("caxeflow.runtime.limit"),
		new MessageId("caxeflow.runtime.limit-owner"),
		new MessageId("caxeflow.runtime.stale-reference"),
		new MessageId("caxeflow.runtime.unsupported-action")
	];

private inline function message(id:String, arguments:Array<String>):CaxeFlowDiagnosticMessage
	return {message: new MessageId(id), arguments: arguments};

/** Stable technical token interpolated inside localized limit prose. */
private function limitId(kind:FlowExecutionLimit):String
	return switch kind {
		case FixedTickEpochs: "fixed-tick-epochs";
		case TickEvents: "tick-events";
		case RuleExecutions: "rule-executions";
		case Actions: "actions";
		case SequenceCalls: "sequence-calls";
		case SequenceDepth: "sequence-depth";
		case SpawnedObjects: "spawned-objects";
		case ScheduledWork: "scheduled-work";
		case PredicateEvaluations: "predicate-evaluations";
		case DeferredWork: "deferred-work";
		case TraceEntries: "trace-entries";
	};
