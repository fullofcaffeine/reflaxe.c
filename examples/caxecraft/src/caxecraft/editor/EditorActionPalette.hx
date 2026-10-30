package caxecraft.editor;

import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionConsumer;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionPalette;

/**
	Actions the scenario editor may offer for a CaxeFlow `DO` step.

	This is renderer-independent data. The visual editor uses each ordered role
	to select a world picker or a typed field. It does not copy parser rules.
**/
function availableScenarioActions():Array<FlowActionDescriptor>
	return flowActionPalette(FlowActionConsumer.CaxeFlowDocument);
