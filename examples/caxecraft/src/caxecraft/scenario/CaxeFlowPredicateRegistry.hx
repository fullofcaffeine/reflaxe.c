package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowPredicate;

/**
	Owns the inspectable catalog for every CaxeFlow `IF` condition.

	The predicate enum remains executable truth. This descriptor layer gives the
	parser, validator, visual cards, and advanced authoring tools one stable ID,
	field shape, and localization key for each closed constructor.
**/
enum abstract FlowPredicateId(String) {
	var AlwaysPredicate = "always";
	var AllPredicate = "all";
	var AnyPredicate = "any";
	var NotPredicate = "not";
	var FlagPredicate = "flag";
	var CounterPredicate = "counter";
	var StatePredicate = "state";
	var ObjectStatePredicate = "object-state";
	var InventoryPredicate = "inventory";
	var ObjectivePredicate = "objective";
	var NearPredicate = "near";
	var ModePredicate = "mode";
	var EventActorPredicate = "event-actor";
	var EventSweptPredicate = "event-swept";

	/** Return the canonical CAXEMAP spelling. */
	public inline function text():String
		return this;
}

/** Closed controls needed to edit one predicate without guessing field types. */
enum FlowPredicateSchema {
	NoPredicateArguments;
	PredicateChildren;
	OnePredicateChild;
	VariableAndFlag;
	VariableComparisonAndInteger;
	VariableAndContent;
	ObjectAndContent;
	OwnerItemComparisonAndQuantity;
	ObjectiveAndState;
	ActorObjectAndDistance;
	ScenarioModeValue;
	EventActorReference;
	EventSweptFlag;
}

/** Immutable metadata shared by simple cards and advanced nested forms. */
typedef FlowPredicateDescriptor = {
	final id:FlowPredicateId;
	final schema:FlowPredicateSchema;
	final editorLabel:MessageId;
	final editorHelp:MessageId;
	final traceName:String;
}

/** Exact descriptor inventory errors; unknown conditions never fall back. */
enum FlowPredicateRegistryError {
	DuplicatePredicateId(id:FlowPredicateId);
	MissingPredicateId(id:FlowPredicateId);
	WrongPredicateSchema(id:FlowPredicateId);
}

/** Canonical order used by cards, completion, tests, and generated output review. */
function allFlowPredicateIds():Array<FlowPredicateId>
	return [
		AlwaysPredicate,
		AllPredicate,
		AnyPredicate,
		NotPredicate,
		FlagPredicate,
		CounterPredicate,
		StatePredicate,
		ObjectStatePredicate,
		InventoryPredicate,
		ObjectivePredicate,
		NearPredicate,
		ModePredicate,
		EventActorPredicate,
		EventSweptPredicate
	];

/** Resolve one immutable condition descriptor. */
function flowPredicateDescriptorById(id:FlowPredicateId):FlowPredicateDescriptor {
	final schema = switch id {
		case AlwaysPredicate: NoPredicateArguments;
		case AllPredicate | AnyPredicate: PredicateChildren;
		case NotPredicate: OnePredicateChild;
		case FlagPredicate: VariableAndFlag;
		case CounterPredicate: VariableComparisonAndInteger;
		case StatePredicate: VariableAndContent;
		case ObjectStatePredicate: ObjectAndContent;
		case InventoryPredicate: OwnerItemComparisonAndQuantity;
		case ObjectivePredicate: ObjectiveAndState;
		case NearPredicate: ActorObjectAndDistance;
		case ModePredicate: ScenarioModeValue;
		case EventActorPredicate: EventActorReference;
		case EventSweptPredicate: EventSweptFlag;
	};
	final syntax = id.text();
	return {
		id: id,
		schema: schema,
		editorLabel: new MessageId('editor.predicate.$syntax.label'),
		editorHelp: new MessageId('editor.predicate.$syntax.help'),
		traceName: syntax
	};
}

/** Build the complete immutable condition palette. */
function allFlowPredicateDescriptors():Array<FlowPredicateDescriptor>
	return [for (id in allFlowPredicateIds()) flowPredicateDescriptorById(id)];

/** Link one typed condition to its stable descriptor ID. */
function flowPredicateId(predicate:FlowPredicate):FlowPredicateId
	return switch predicate {
		case Always: AlwaysPredicate;
		case All(_): AllPredicate;
		case AnyOf(_): AnyPredicate;
		case Not(_): NotPredicate;
		case FlagIs(_, _): FlagPredicate;
		case CounterCompare(_, _, _): CounterPredicate;
		case StateIs(_, _): StatePredicate;
		case ObjectStateIs(_, _): ObjectStatePredicate;
		case InventoryHas(_, _, _, _): InventoryPredicate;
		case ObjectiveIs(_, _): ObjectivePredicate;
		case NearObject(_, _, _): NearPredicate;
		case ModeIs(_): ModePredicate;
		case EventActorIs(_): EventActorPredicate;
		case EventSweptIs(_): EventSweptPredicate;
	};

/** Resolve metadata directly from a typed condition. */
function flowPredicateDescriptor(predicate:FlowPredicate):FlowPredicateDescriptor
	return flowPredicateDescriptorById(flowPredicateId(predicate));

/** Validate a supplied inventory for duplicate, missing, or drifted descriptors. */
function validateFlowPredicateDescriptors(values:Array<FlowPredicateDescriptor>):Array<FlowPredicateRegistryError> {
	final errors:Array<FlowPredicateRegistryError> = [];
	final expected = allFlowPredicateIds();
	for (index in 0...values.length) {
		for (earlier in 0...index)
			if (values[earlier].id == values[index].id)
				errors.push(DuplicatePredicateId(values[index].id));
		final canonical = flowPredicateDescriptorById(values[index].id);
		if (!sameSchema(values[index].schema, canonical.schema))
			errors.push(WrongPredicateSchema(values[index].id));
	}
	for (id in expected) {
		var found = false;
		for (value in values)
			if (value.id == id)
				found = true;
		if (!found)
			errors.push(MissingPredicateId(id));
	}
	return errors;
}

/** Compare closed schemas without relying on target enum ordinals. */
private function sameSchema(left:FlowPredicateSchema, right:FlowPredicateSchema):Bool
	return switch [left, right] {
		case [NoPredicateArguments, NoPredicateArguments] | [PredicateChildren, PredicateChildren] | [OnePredicateChild, OnePredicateChild] |
			[VariableAndFlag, VariableAndFlag] | [VariableComparisonAndInteger, VariableComparisonAndInteger] | [VariableAndContent, VariableAndContent] |
			[ObjectAndContent, ObjectAndContent] | [OwnerItemComparisonAndQuantity, OwnerItemComparisonAndQuantity] | [ObjectiveAndState, ObjectiveAndState] |
			[ActorObjectAndDistance, ActorObjectAndDistance] | [ScenarioModeValue, ScenarioModeValue] | [EventActorReference, EventActorReference] |
			[EventSweptFlag, EventSweptFlag]: true;
		case _: false;
	};
