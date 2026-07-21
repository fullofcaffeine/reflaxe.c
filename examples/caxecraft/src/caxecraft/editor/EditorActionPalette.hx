package caxecraft.editor;

import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionConsumer;
import caxecraft.scenario.CaxeFlowActionRegistry.FlowActionDescriptor;
import caxecraft.scenario.CaxeFlowActionRegistry.flowActionPalette;

/**
	Actions the scenario editor may offer for a CaxeFlow `DO` step.

	This is renderer-independent data. A future visual editor can turn each
	descriptor's ordered argument roles into suitable controls without duplicating
	the parser grammar.
**/
function availableScenarioActions():Array<FlowActionDescriptor>
	return flowActionPalette(FlowActionConsumer.CaxeFlowDocument);
