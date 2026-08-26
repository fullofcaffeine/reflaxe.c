package caxecraft.editor;

import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.flowEventDescriptor;
import caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateDescriptor;
import caxecraft.scenario.CaxeFlowPredicateRegistry.flowPredicateDescriptor;
import caxecraft.scenario.CaxeFlowRuntime.FlowTick;
import caxecraft.scenario.CaxeFlowRuntime.FlowTraceEntry;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.editor.EditorFlowReferences.EditorFlowReference;
import caxecraft.editor.EditorFlowReferences.collectActionFlowReferences;
import caxecraft.editor.EditorFlowReferences.collectEventFlowReferences;
import caxecraft.editor.EditorFlowReferences.collectPredicateFlowReferences;

/**
	Projects zone events onto the trigger volumes that creators see in Plan view.

	The CaxeFlow rule keeps the stable reference. This module resolves that
	reference without changing the rule or copying logic into renderer state.
**/
enum EditorZoneRuleProjection {
	/** The rule points to one authored trigger with exact voxel bounds. */
	ResolvedZoneRule(ruleId:ScenarioId, zoneId:ScenarioId, bounds:VoxelBounds);

	/** The rule points to no trigger, so the editor can report the broken link. */
	UnresolvedZoneRule(ruleId:ScenarioId, zoneId:ScenarioId);
}

/** One pair of connected volumes whose interiors share at least one voxel. */
typedef EditorTriggerOverlap = {
	final first:ScenarioId;
	final second:ScenarioId;
}

/** Stable catalog keys for the fixed controls around flow cards. */
enum abstract EditorFlowUiMessage(String) {
	var CardLibraryMessage = "editor.flow.card-library";
	var ConnectObjectMessage = "editor.flow.connect-object";
	var DoMessage = "editor.flow.do";
	var DocumentPickerMessage = "editor.flow.document-picker";
	var DoneMessage = "editor.flow.done";
	var EditMessage = "editor.flow.edit";
	var IfMessage = "editor.flow.if";
	var PickObjectMessage = "editor.flow.pick-object";
	var OverlapMessage = "editor.flow.overlap";
	var WhenMessage = "editor.flow.when";
	var ToggleEnterLeaveMessage = "editor.flow.toggle-enter-leave";
	var ToggleSpawnDespawnMessage = "editor.flow.toggle-spawn-despawn";
	var UnavailableMessage = "editor.flow.unavailable";

	/** Narrow one closed editor key to the catalog boundary. */
	public inline function messageId():MessageId
		return new MessageId(this);
}

/** Return every fixed flow-card key required by the native editor. */
function allEditorFlowUiMessages():Array<EditorFlowUiMessage>
	return [
		CardLibraryMessage,
		ConnectObjectMessage,
		DoMessage,
		DocumentPickerMessage,
		DoneMessage,
		EditMessage,
		IfMessage,
		OverlapMessage,
		PickObjectMessage,
		ToggleEnterLeaveMessage,
		ToggleSpawnDespawnMessage,
		UnavailableMessage,
		WhenMessage
	];

/** Public card text for one complete event-library value. */
function eventFlowCardText(event:FlowEvent):EditorFlowCardText
	return eventSummary(event);

/** Public card text for one complete predicate-library value. */
function predicateFlowCardText(predicate:FlowPredicate):EditorFlowCardText
	return predicateSummary(predicate);

/** Public card text for one complete action-library value. */
function actionFlowCardText(action:FlowAction):EditorFlowCardText
	return actionSummary(action);

/** One data-owned sentence plus locale-independent replacement values. */
typedef EditorFlowCardText = {
	final message:MessageId;
	final arguments:Array<String>;
}

/**
	One child-readable sentence card backed by the same runtime descriptor.

	The card carries a stable message key and typed scalar arguments, not prose.
	`RuntimeUiCatalog` owns every translated sentence. Mutations still write the
	typed enum value and canonical CAXEMAP writer used by Advanced mode.
**/
enum EditorFlowCard {
	WhenFlowCard(descriptor:FlowEventDescriptor, text:EditorFlowCardText, references:Array<EditorFlowReference>);
	IfFlowCard(descriptor:FlowPredicateDescriptor, text:EditorFlowCardText, references:Array<EditorFlowReference>);
	DoFlowCard(index:Int, descriptor:FlowActionDescriptor, text:EditorFlowCardText, references:Array<EditorFlowReference>);
}

/** One lossless child row below a compound IF or weighted DO card. */
enum EditorFlowNestedCard {
	NestedIfFlowCard(path:Array<Int>, depth:Int, descriptor:FlowPredicateDescriptor, text:EditorFlowCardText, references:Array<EditorFlowReference>);
	NestedDoFlowCard(parentActionIndex:Int, choiceIndex:Int, actionIndex:Int, depth:Int, descriptor:FlowActionDescriptor, text:EditorFlowCardText,
		references:Array<EditorFlowReference>);
}

/** Complete WHEN / IF / ordered-DO projection for one canonical rule. */
typedef EditorFlowRuleProjection = {
	final ruleId:ScenarioId;
	final priority:Int;
	final cards:Array<EditorFlowCard>;
	final nestedCards:Array<EditorFlowNestedCard>;
}

/** Bounded rows suitable for the in-world event-flow overlay. */
enum EditorFlowTraceRow {
	SourceTrace(event:String, actor:Null<ScenarioId>);
	PredicateTrace(rule:ScenarioId, event:String, actor:Null<ScenarioId>, passed:Bool);
	ActionTrace(owner:ScenarioId, action:String);
	FollowUpTrace(owner:ScenarioId, event:String, readyTick:FlowTick);
	SequenceTrace(owner:ScenarioId, timer:ScenarioId, sequence:ScenarioId, readyTick:FlowTick);
}

/** Trace rows plus an explicit indication that the requested view was shorter. */
typedef EditorFlowTraceProjection = {
	final rows:Array<EditorFlowTraceRow>;
	final truncated:Bool;
}

/** Return zone-event links in canonical rule order and omit unrelated events. */
function projectZoneRules(rules:Array<FlowRule>, objects:Array<ScenarioObject>):Array<EditorZoneRuleProjection> {
	final result:Array<EditorZoneRuleProjection> = [];
	for (rule in rules) {
		final zone = switch rule.event {
			case EnterZone(id) | LeaveZone(id): id;
			case _: null;
		};
		if (zone == null)
			continue;
		final bounds = triggerBounds(objects, zone);
		if (bounds == null)
			result.push(UnresolvedZoneRule(rule.id, zone));
		else
			result.push(ResolvedZoneRule(rule.id, zone, {
				origin: {x: bounds.origin.x, y: bounds.origin.y, z: bounds.origin.z},
				size: {width: bounds.size.width, height: bounds.size.height, depth: bounds.size.depth}
			}));
	}
	return result;
}

/** Find deterministic non-blocking overlap warnings for connected trigger volumes. */
function projectTriggerOverlaps(rules:Array<FlowRule>, objects:Array<ScenarioObject>):Array<EditorTriggerOverlap> {
	final connected:Map<String, Bool> = [];
	for (rule in rules)
		switch rule.event {
			case EnterZone(id) | LeaveZone(id):
				connected.set(id.text(), true);
			case _:
		}
	final zones:Array<{final id:ScenarioId; final bounds:VoxelBounds;}> = [];
	for (object in objects)
		if (connected.exists(object.id.text()))
			switch object.placement {
				case TriggerZone(bounds):
					zones.push({id: object.id, bounds: bounds});
				case _:
			}
	final result:Array<EditorTriggerOverlap> = [];
	for (right in 0...zones.length)
		for (left in 0...right)
			if (boundsOverlap(zones[left].bounds, zones[right].bounds))
				result.push({first: zones[left].id, second: zones[right].id});
	return result;
}

/** Project every canonical rule into one readable WHEN / IF / DO card stack. */
function projectFlowRules(rules:Array<FlowRule>):Array<EditorFlowRuleProjection> {
	final result:Array<EditorFlowRuleProjection> = [];
	for (rule in rules) {
		final cards:Array<EditorFlowCard> = [
			WhenFlowCard(flowEventDescriptor(rule.event), eventSummary(rule.event), collectEventFlowReferences(rule.event)),
			IfFlowCard(flowPredicateDescriptor(rule.predicate), predicateSummary(rule.predicate), collectPredicateFlowReferences(rule.predicate))
		];
		for (index in 0...rule.actions.length) {
			final action = rule.actions[index];
			cards.push(DoFlowCard(index, flowActionDescriptor(action), actionSummary(action), collectActionFlowReferences(action)));
		}
		final nestedCards:Array<EditorFlowNestedCard> = [];
		projectNestedPredicateChildren(rule.predicate, [], 0, nestedCards);
		for (actionIndex in 0...rule.actions.length)
			projectNestedChoiceActions(rule.actions[actionIndex], actionIndex, nestedCards);
		result.push({
			ruleId: rule.id,
			priority: rule.priority,
			cards: cards,
			nestedCards: nestedCards
		});
	}
	return result;
}

/** Add every predicate below the root IF in deterministic depth-first order. */
private function projectNestedPredicateChildren(predicate:FlowPredicate, path:Array<Int>, depth:Int, result:Array<EditorFlowNestedCard>):Void {
	switch predicate {
		case All(children) | AnyOf(children):
			for (index in 0...children.length) {
				final childPath = path.copy();
				childPath.push(index);
				projectNestedPredicate(children[index], childPath, depth + 1, result);
			}
		case Not(child):
			final childPath = path.copy();
			childPath.push(0);
			projectNestedPredicate(child, childPath, depth + 1, result);
		case _:
	}
}

/** Add one nested predicate row, then retain all descendants. */
private function projectNestedPredicate(predicate:FlowPredicate, path:Array<Int>, depth:Int, result:Array<EditorFlowNestedCard>):Void {
	result.push(NestedIfFlowCard(path.copy(), depth, flowPredicateDescriptor(predicate), predicateSummary(predicate),
		collectPredicateFlowReferences(predicate)));
	projectNestedPredicateChildren(predicate, path, depth, result);
}

/** Add every action inside one weighted choice without flattening branch order. */
private function projectNestedChoiceActions(action:FlowAction, parentActionIndex:Int, result:Array<EditorFlowNestedCard>):Void {
	switch action {
		case ChooseSeeded(_, choices):
			for (choiceIndex in 0...choices.length)
				for (actionIndex in 0...choices[choiceIndex].actions.length) {
					final nested = choices[choiceIndex].actions[actionIndex];
					result.push(NestedDoFlowCard(parentActionIndex, choiceIndex, actionIndex, 1, flowActionDescriptor(nested), actionSummary(nested),
						collectActionFlowReferences(nested)));
				}
		case _:
	}
}

/** Keep at most `maximum` already-bounded runtime trace entries for an overlay. */
function projectFlowTrace(trace:Array<FlowTraceEntry>, maximum:Int):EditorFlowTraceProjection {
	final limit = maximum < 0 ? 0 : maximum;
	final rows:Array<EditorFlowTraceRow> = [];
	for (entry in trace) {
		if (rows.length >= limit)
			break;
		rows.push(switch entry {
			case EventObserved(event, actor): SourceTrace(event.text(), actor);
			case PredicateEvaluated(rule, event, actor, passed): PredicateTrace(rule, event.text(), actor, passed);
			case ActionExecuted(owner, action): ActionTrace(owner, action.text());
			case FollowUpDeferred(owner, event, readyTick): FollowUpTrace(owner, event.text(), readyTick);
			case SequenceDeferred(owner, timer, sequence, readyTick): SequenceTrace(owner, timer, sequence, readyTick);
		});
	}
	return {rows: rows, truncated: rows.length < trace.length};
}

/** Keep the last meaningful test-play trace when later ticks are quiet. */
function retainLatestFlowTrace(current:EditorFlowTraceProjection, observed:Array<FlowTraceEntry>, maximum:Int):EditorFlowTraceProjection
	return observed.length == 0 ? current : projectFlowTrace(observed, maximum);

/** Find only the trigger role. Another object with the same ID is not a zone. */
private function triggerBounds(objects:Array<ScenarioObject>, expected:ScenarioId):Null<VoxelBounds> {
	for (object in objects)
		if (object.id.text() == expected.text())
			return switch object.placement {
				case TriggerZone(bounds): bounds;
				case _: null;
			};
	return null;
}

/** True only when two half-open voxel boxes share positive volume. */
private function boundsOverlap(left:VoxelBounds, right:VoxelBounds):Bool
	return left.origin.x < right.origin.x + right.size.width
		&& right.origin.x < left.origin.x + left.size.width
		&& left.origin.y < right.origin.y + right.size.height
		&& right.origin.y < left.origin.y + left.size.height
		&& left.origin.z < right.origin.z + right.size.depth
		&& right.origin.z < left.origin.z + left.size.depth;

/** Pair one event label key with its locale-independent authored values. */
private function eventSummary(event:FlowEvent):EditorFlowCardText {
	final descriptor = flowEventDescriptor(event);
	final arguments = switch event {
		case EnterZone(zone) | LeaveZone(zone): [zone.text()];
		case Interact(objectId): [objectId.text()];
		case BlockChanged(zone, block): [zone.text(), block.text()];
		case UseItem(item) | ItemCollected(item): [item.text()];
		case EntityDefeated(entity): [entity.text()];
		case SignalReceived(signal): [signal.text()];
		case TimerExpired(timer): [timer.text()];
		case ObjectiveChanged(objective): [objective.text()];
		case StateChanged(variable): [variable.text()];
		case LevelEntered(level): [level.text()];
		case CampaignExitRequested(exit): [exit.text()];
	};
	return {message: descriptor.editorLabel, arguments: arguments};
}

/** Pair one condition label key with bounded technical values. */
private function predicateSummary(predicate:FlowPredicate):EditorFlowCardText {
	final descriptor = flowPredicateDescriptor(predicate);
	final arguments = switch predicate {
		case Always: [];
		case All(children) | AnyOf(children): ['${children.length}'];
		case Not(child): [flowPredicateDescriptor(child).id.text()];
		case FlagIs(variable, expected): [variable.text(), '$expected'];
		case CounterCompare(variable, comparison, value): [variable.text(), comparisonText(comparison), '$value'];
		case StateIs(variable, expected): [variable.text(), expected.text()];
		case ObjectStateIs(objectId, expected): [objectId.text(), expected.text()];
		case InventoryHas(owner, item, comparison, quantity): [owner.text(), comparisonText(comparison), '$quantity', item.text()];
		case ObjectiveIs(objective, expected): [objective.text(), objectiveStateText(expected)];
		case NearObject(actor, objectId, maximum): [actor.text(), '$maximum', objectId.text()];
		case ModeIs(mode): [mode == Creative ? "creative" : "adventure"];
		case EventActorIs(actor): [actor.text()];
		case EventSweptIs(expected): ['$expected'];
	};
	return {message: descriptor.editorLabel, arguments: arguments};
}

/** Pair one action label key with bounded technical values. */
private function actionSummary(action:FlowAction):EditorFlowCardText {
	final descriptor = flowActionDescriptor(action);
	final arguments = switch action {
		case ShowDialogue(id) | AddJournal(id) | Spawn(id) | Despawn(id) | SetCheckpoint(id) | RequestCampaignExit(id): [id.text()];
		case SetFlag(id, value): [id.text(), '$value'];
		case SetCounter(id, value) | AddCounter(id, value): [id.text(), '$value'];
		case SetState(id, value) | SetObjectState(id, value): [id.text(), value.text()];
		case GiveItem(owner, item, quantity) | TakeItem(owner, item, quantity): [owner.text(), item.text(), '$quantity'];
		case SetObjective(id, value): [id.text(), objectiveStateText(value)];
		case PlayEffect(effect, objectId): [effect.text(), objectId == null ? "none" : objectId.text()];
		case EmitSignal(signal): [signal.text()];
		case Schedule(timer, ticks, sequence, values): [timer.text(), '$ticks', sequence.text(), '${values.length}'];
		case CallSequence(sequence, values): [sequence.text(), '${values.length}'];
		case ChooseSeeded(seed, choices): [seed.text(), '${choices.length}'];
	};
	return {message: descriptor.editorLabel, arguments: arguments};
}

/** Canonical comparison symbols are concise and language-independent. */
private function comparisonText(value:FlowComparison):String
	return switch value {
		case Equal: "=";
		case NotEqual: "!=";
		case Less: "<";
		case LessOrEqual: "<=";
		case Greater: ">";
		case GreaterOrEqual: ">=";
	};

/** Stable objective-state token interpolated by the data-owned sentence. */
private function objectiveStateText(value:caxecraft.scenario.ScenarioStory.ObjectiveState):String
	return switch value {
		case Hidden: "hidden";
		case Active: "active";
		case Complete: "complete";
		case Failed: "failed";
	};
