package caxecraft.app;

#if c
import caxecraft.editor.EditorFlowAuthoring.EditorFlowSequenceAuthoringResult;
import caxecraft.editor.EditorFlowAuthoring.EditorFlowSequenceEdit;
import caxecraft.editor.EditorFlowAuthoring.editFlowSequence;
import caxecraft.editor.EditorFlowDefinitions.EditorFlowVariableEdit;
import caxecraft.editor.EditorFlowDefinitions.editFlowVariable;
import caxecraft.editor.EditorFlowDefinitions.flowVariableIsReferenced;
import caxecraft.editor.EditorFlowDefinitions.nextFlowSequence;
import caxecraft.editor.EditorFlowDefinitions.nextFlowVariable;
import caxecraft.editor.EditorFlowProjection.EditorFlowUiMessage;
import caxecraft.editor.EditorFlowProjection.actionFlowCardText;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import raygui.GuiResult;
import raygui.Raygui;
import raylib.Color;
import raylib.Raylib;
import raylib.Rectangle;

/**
	Draws Advanced Flow declarations without owning a parallel document.

	Each visible row retains its typed variable, sequence, or action address.
	Buttons return one canonical `EditorCommand` or a request to open the shared
	action-card library. Referenced variables keep their kind and scope locked so
	a visual click cannot invalidate typed rules; their initial value remains
	editable. Advanced Text continues to expose exact IDs and parameters.
**/
/** One stable selectable row in the declaration modal. */
enum CaxecraftFlowDefinitionRow {
	AddFlagDefinitionRow;
	AddCounterDefinitionRow;
	AddStateDefinitionRow;
	AddSequenceDefinitionRow;
	VariableDefinitionRow(index:Int, variable:FlowVariable);
	SequenceDefinitionRow(index:Int, sequence:FlowSequence);
	SequenceActionDefinitionRow(sequenceIndex:Int, actionIndex:Int, sequence:FlowSequence, action:FlowAction);
}

/** One complete user decision returned to the editor controller. */
enum CaxecraftFlowDefinitionPanelAction {
	KeepFlowDefinitionPanel;
	CloseFlowDefinitionPanel;
	ApplyFlowDefinition(command:EditorCommand);
	OpenFlowSequenceAction(sequence:ScenarioId, actionIndex:Int, insert:Bool);
}

/** Project controls and declarations in one deterministic keyboard order. */
function flowDefinitionRows(draft:Scenario, states:Array<ContentId>):Array<CaxecraftFlowDefinitionRow> {
	final result:Array<CaxecraftFlowDefinitionRow> = [AddFlagDefinitionRow, AddCounterDefinitionRow];
	if (states.length > 0)
		result.push(AddStateDefinitionRow);
	result.push(AddSequenceDefinitionRow);
	for (index in 0...draft.flow.variables.length)
		result.push(VariableDefinitionRow(index, draft.flow.variables[index]));
	for (sequenceIndex in 0...draft.flow.sequences.length) {
		final sequence = draft.flow.sequences[sequenceIndex];
		result.push(SequenceDefinitionRow(sequenceIndex, sequence));
		for (actionIndex in 0...sequence.actions.length)
			result.push(SequenceActionDefinitionRow(sequenceIndex, actionIndex, sequence, sequence.actions[actionIndex]));
	}
	return result;
}

/** Convert semantic activation of one selected row into a typed edit. */
function flowDefinitionActionAt(draft:Scenario, states:Array<ContentId>, selection:Int):CaxecraftFlowDefinitionPanelAction {
	final rows = flowDefinitionRows(draft, states);
	if (selection < 0 || selection >= rows.length)
		return KeepFlowDefinitionPanel;
	return switch rows[selection] {
		case AddFlagDefinitionRow: ApplyFlowDefinition(PutFlowVariable(nextFlowVariable(Flag(false), draft.flow.variables)));
		case AddCounterDefinitionRow: ApplyFlowDefinition(PutFlowVariable(nextFlowVariable(Counter(0), draft.flow.variables)));
		case AddStateDefinitionRow: ApplyFlowDefinition(PutFlowVariable(nextFlowVariable(State(states[0]), draft.flow.variables)));
		case AddSequenceDefinitionRow: ApplyFlowDefinition(PutFlowSequence(nextFlowSequence(draft.flow.sequences)));
		case VariableDefinitionRow(_, variable):
			ApplyFlowDefinition(PutFlowVariable(editFlowVariable(variable, NextVariableValue, draft.flow.sequences, states)));
		case SequenceDefinitionRow(_, sequence): OpenFlowSequenceAction(sequence.id, sequence.actions.length, true);
		case SequenceActionDefinitionRow(_, actionIndex, sequence, _): OpenFlowSequenceAction(sequence.id, actionIndex, false);
	};
}

/** Draw one bounded modal and return at most one typed command. */
function drawFlowDefinitionPanel(catalog:RuntimeUiCatalog, locale:LocaleCursor, draft:Scenario, states:Array<ContentId>, selection:Int, width:Int,
		height:Int):CaxecraftFlowDefinitionPanelAction {
	Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 10, 14, 235));
	final panelWidth = width >= 980 ? 920 : width - 32;
	final panelHeight = height >= 660 ? 620 : height - 32;
	final left = Std.int((width - panelWidth) / 2);
	final top = Std.int((height - panelHeight) / 2);
	Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight), catalog.format(locale, EditorFlowUiMessage.DefinitionsMessage.messageId(), []));
	if (Raygui.ButtonString(Rectangle.fromFloat(left + panelWidth - 112, top + 12, 88, 28),
		catalog.format(locale, EditorFlowUiMessage.DoneMessage.messageId(), []))
		.has(GuiResult.Pressed))
		return CloseFlowDefinitionPanel;
	final rows = flowDefinitionRows(draft, states);
	final visible = maximum(1, Std.int((panelHeight - 78) / 36));
	final start = windowStart(selection, rows.length, visible);
	final end = minimum(rows.length, start + visible);
	for (index in start...end) {
		final rowTop = top + 54 + (index - start) * 36;
		if (index == selection)
			Raylib.DrawRectangleLines(left + 18, rowTop - 2, panelWidth - 36, 34, CaxecraftPalette.selection());
		final action = drawDefinitionRow(catalog, locale, draft, states, rows[index], left + 22, rowTop, panelWidth - 44);
		switch action {
			case KeepFlowDefinitionPanel:
			case _:
				return action;
		}
	}
	return KeepFlowDefinitionPanel;
}

private function drawDefinitionRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, draft:Scenario, states:Array<ContentId>, row:CaxecraftFlowDefinitionRow,
		left:Int, top:Int, width:Int):CaxecraftFlowDefinitionPanelAction {
	return switch row {
		case AddFlagDefinitionRow:
			addButton(catalog, locale, left, top, width, EditorFlowUiMessage.AddFlagMessage,
				PutFlowVariable(nextFlowVariable(Flag(false), draft.flow.variables)));
		case AddCounterDefinitionRow:
			addButton(catalog, locale, left, top, width, EditorFlowUiMessage.AddCounterMessage,
				PutFlowVariable(nextFlowVariable(Counter(0), draft.flow.variables)));
		case AddStateDefinitionRow:
			addButton(catalog, locale, left, top, width, EditorFlowUiMessage.AddStateMessage,
				PutFlowVariable(nextFlowVariable(State(states[0]), draft.flow.variables)));
		case AddSequenceDefinitionRow:
			addButton(catalog, locale, left, top, width, EditorFlowUiMessage.AddSequenceMessage, PutFlowSequence(nextFlowSequence(draft.flow.sequences)));
		case VariableDefinitionRow(_, variable):
			drawVariableRow(catalog, locale, draft, states, variable, left, top, width);
		case SequenceDefinitionRow(_, sequence):
			drawSequenceRow(catalog, locale, sequence, left, top, width);
		case SequenceActionDefinitionRow(_, actionIndex, sequence, action):
			drawSequenceActionRow(catalog, locale, sequence, actionIndex, action, left, top, width);
	};
}

private function addButton(catalog:RuntimeUiCatalog, locale:LocaleCursor, left:Int, top:Int, width:Int, message:EditorFlowUiMessage,
		command:EditorCommand):CaxecraftFlowDefinitionPanelAction
	return Raygui.ButtonString(Rectangle.fromFloat(left, top, width, 28), catalog.format(locale, message.messageId(), []))
		.has(GuiResult.Pressed) ? ApplyFlowDefinition(command) : KeepFlowDefinitionPanel;

private function drawVariableRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, draft:Scenario, states:Array<ContentId>, variable:FlowVariable, left:Int,
		top:Int, width:Int):CaxecraftFlowDefinitionPanelAction {
	final locked = flowVariableIsReferenced(draft, variable.id);
	Raylib.DrawTextString('${variable.id.text()}  ·  ${scopeText(catalog, locale, variable.scope)}  ·  ${valueText(variable.initial)}', left + 4, top + 8, 13,
		CaxecraftPalette.hudText());
	final controls = left + width - 178;
	if (!locked && Raygui.ButtonString(Rectangle.fromFloat(controls, top + 2, 30, 25), "◇").has(GuiResult.Pressed))
		return ApplyFlowDefinition(PutFlowVariable(editFlowVariable(variable, NextVariableKind, draft.flow.sequences, states)));
	if (!locked && Raygui.ButtonString(Rectangle.fromFloat(controls + 34, top + 2, 30, 25), "◎").has(GuiResult.Pressed))
		return ApplyFlowDefinition(PutFlowVariable(editFlowVariable(variable, NextVariableScope, draft.flow.sequences, states)));
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 68, top + 2, 30, 25), "-").has(GuiResult.Pressed))
		return ApplyFlowDefinition(PutFlowVariable(editFlowVariable(variable, PreviousVariableValue, draft.flow.sequences, states)));
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 102, top + 2, 30, 25), "+").has(GuiResult.Pressed))
		return ApplyFlowDefinition(PutFlowVariable(editFlowVariable(variable, NextVariableValue, draft.flow.sequences, states)));
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 136, top + 2, 42, 25), "X").has(GuiResult.Pressed))
		return ApplyFlowDefinition(RemoveFlowVariable(variable.id));
	return KeepFlowDefinitionPanel;
}

private function drawSequenceRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, sequence:FlowSequence, left:Int, top:Int,
		width:Int):CaxecraftFlowDefinitionPanelAction {
	Raylib.DrawTextString('${sequence.id.text()}  ·  ${sequence.parameters.length} ${catalog.format(locale, EditorFlowUiMessage.ParametersMessage.messageId(), [])}  ·  ${sequence.actions.length} ${catalog.format(locale, EditorFlowUiMessage.ActionsMessage.messageId(), [])}',
		left
		+ 4, top
		+ 8, 13, CaxecraftPalette.hudText());
	if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 150, top + 2, 104, 25),
		catalog.format(locale, EditorFlowUiMessage.AddActionMessage.messageId(), []))
		.has(GuiResult.Pressed))
		return OpenFlowSequenceAction(sequence.id, sequence.actions.length, true);
	if (Raygui.ButtonString(Rectangle.fromFloat(left + width - 42, top + 2, 42, 25), "X").has(GuiResult.Pressed))
		return ApplyFlowDefinition(RemoveFlowSequence(sequence.id));
	return KeepFlowDefinitionPanel;
}

private function drawSequenceActionRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, sequence:FlowSequence, actionIndex:Int, action:FlowAction, left:Int,
		top:Int, width:Int):CaxecraftFlowDefinitionPanelAction {
	final text = actionFlowCardText(action);
	Raylib.DrawTextString('  ${actionIndex + 1}. ${catalog.format(locale, text.message, text.arguments)}', left + 4, top + 8, 13, CaxecraftPalette.hudText());
	final controls = left + width - 142;
	if (Raygui.ButtonString(Rectangle.fromFloat(controls, top + 2, 42, 25), catalog.format(locale, EditorFlowUiMessage.EditMessage.messageId(), []))
		.has(GuiResult.Pressed))
		return OpenFlowSequenceAction(sequence.id, actionIndex, false);
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 46, top + 2, 26, 25), "^").has(GuiResult.Pressed))
		return sequenceEdit(sequence, MoveSequenceDo(actionIndex, actionIndex - 1));
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 76, top + 2, 26, 25), "v").has(GuiResult.Pressed))
		return sequenceEdit(sequence, MoveSequenceDo(actionIndex, actionIndex + 1));
	if (Raygui.ButtonString(Rectangle.fromFloat(controls + 106, top + 2, 36, 25), "X").has(GuiResult.Pressed))
		return sequenceEdit(sequence, RemoveSequenceDo(actionIndex));
	return KeepFlowDefinitionPanel;
}

private function sequenceEdit(sequence:FlowSequence, edit:EditorFlowSequenceEdit):CaxecraftFlowDefinitionPanelAction
	return switch editFlowSequence(sequence, edit) {
		case FlowSequenceAuthored(value): ApplyFlowDefinition(PutFlowSequence(value));
		case FlowSequenceUnchanged | FlowSequenceAuthoringRejected: KeepFlowDefinitionPanel;
	};

private function scopeText(catalog:RuntimeUiCatalog, locale:LocaleCursor, scope:FlowScope):String
	return switch scope {
		case Map: catalog.format(locale, EditorFlowUiMessage.MapScopeMessage.messageId(), []);
		case Player: catalog.format(locale, EditorFlowUiMessage.PlayerScopeMessage.messageId(), []);
		case Quest: catalog.format(locale, EditorFlowUiMessage.QuestScopeMessage.messageId(), []);
		case Local(sequence): catalog.format(locale, EditorFlowUiMessage.LocalScopeMessage.messageId(), [sequence.text()]);
	};

private function valueText(value:FlowValue):String
	return switch value {
		case Flag(current): current ? "✓" : "×";
		case Counter(current): Std.string(current);
		case State(current): current.text();
	};

private function windowStart(selection:Int, count:Int, visible:Int):Int {
	if (count <= visible || selection < visible)
		return 0;
	final latest = selection - visible + 1;
	final maximumStart = count - visible;
	return latest < maximumStart ? latest : maximumStart;
}

private inline function minimum(left:Int, right:Int):Int
	return left < right ? left : right;

private inline function maximum(left:Int, right:Int):Int
	return left > right ? left : right;
#end
