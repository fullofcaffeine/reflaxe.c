package caxecraft.scenario;

import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.allFlowActionDescriptors;
import caxecraft.scenario.CaxeFlowEventRegistry.FlowEventDescriptor;
import caxecraft.scenario.CaxeFlowEventRegistry.allFlowEventDescriptors;
import caxecraft.scenario.CaxeFlowPredicateRegistry.FlowPredicateDescriptor;
import caxecraft.scenario.CaxeFlowPredicateRegistry.allFlowPredicateDescriptors;

/**
	Publishes the complete registry-backed CaxeFlow authoring vocabulary.

	Events describe `WHEN` sources and runtime context, predicates describe `IF`
	forms, and actions describe ordered `DO` capabilities. Keeping this umbrella
	read-only prevents editor surfaces from growing separate partial catalogs.
**/
typedef CaxeFlowRegistry = {
	final events:Array<FlowEventDescriptor>;
	final predicates:Array<FlowPredicateDescriptor>;
	final actions:Array<FlowActionDescriptor>;
}

/** Return fresh arrays whose descriptors remain immutable semantic values. */
function caxeFlowRegistry():CaxeFlowRegistry
	return {
		events: allFlowEventDescriptors(),
		predicates: allFlowPredicateDescriptors(),
		actions: allFlowActionDescriptors()
	};
