package caxecraft.app;

#if c
import caxecraft.editor.EditorFlowAuthoring.EditorFlowCardAddress;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowActionChoice;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowContentChoices;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowEventChoice;
import caxecraft.editor.EditorFlowCardLibrary.EditorFlowPredicateChoice;
import caxecraft.editor.EditorFlowCardLibrary.actionCardChoices;
import caxecraft.editor.EditorFlowCardLibrary.documentFlowReferenceChoices;
import caxecraft.editor.EditorFlowCardLibrary.eventCardChoices;
import caxecraft.editor.EditorFlowCardLibrary.predicateCardChoices;
import caxecraft.editor.EditorFlowProjection.EditorFlowUiMessage;
import caxecraft.editor.EditorFlowProjection.actionFlowCardText;
import caxecraft.editor.EditorFlowProjection.eventFlowCardText;
import caxecraft.editor.EditorFlowProjection.predicateFlowCardText;
import caxecraft.editor.EditorFlowReferences.EditorFlowReferenceRole;
import caxecraft.localization.RuntimeUiCatalog;
import caxecraft.localization.UiTypes.LocaleCursor;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioId;
import raygui.GuiResult;
import raygui.Raygui;
import raylib.Color;
import raylib.Raylib;
import raylib.Rectangle;

/**
	Draws the registry-derived CaxeFlow modal without owning editor mutations.

	The stateful editor screen supplies one copy-owned draft and applies the typed
	result through `EditorSession`. Keeping drawing here makes the large palette an
	independent compiler unit and prevents Raygui state from becoming a second
	document model.
**/
/** The canonical card address currently shown by the library modal. */
enum CaxecraftFlowCardPanelTarget {
	NoFlowCardPanel;
	WhenFlowCardPanel(zone:ScenarioId, rule:ScenarioId);
	IfFlowCardPanel(zone:ScenarioId, rule:ScenarioId);
	DoFlowCardPanel(zone:ScenarioId, rule:ScenarioId, actionIndex:Int);
	InsertDoFlowCardPanel(zone:ScenarioId, rule:ScenarioId, actionIndex:Int);
	NestedIfFlowCardPanel(zone:ScenarioId, rule:ScenarioId, path:Array<Int>);
	NestedDoFlowCardPanel(zone:ScenarioId, rule:ScenarioId, parentActionIndex:Int, choiceIndex:Int, actionIndex:Int);
	SequenceDoFlowCardPanel(sequence:ScenarioId, actionIndex:Int);
	InsertSequenceDoFlowCardPanel(sequence:ScenarioId, actionIndex:Int);
}

/** One complete selection returned to the editor controller. */
enum CaxecraftFlowCardPanelAction {
	KeepFlowCardPanel;
	CloseFlowCardPanel;
	SelectFlowEvent(zone:ScenarioId, rule:ScenarioId, value:FlowEvent);
	SelectFlowPredicate(zone:ScenarioId, rule:ScenarioId, path:Null<Array<Int>>, value:FlowPredicate);
	SelectFlowAction(zone:ScenarioId, rule:ScenarioId, actionIndex:Int, choiceIndex:Int, nestedActionIndex:Int, insert:Bool, value:FlowAction);
	SelectFlowSequenceAction(sequence:ScenarioId, actionIndex:Int, insert:Bool, value:FlowAction);
}

/** Three non-text pictures that identify the WHEN, IF, and DO card families. */
private enum FlowCardFamilyIcon {
	WhenCardIcon;
	IfCardIcon;
	DoCardIcon;
}

/** One revision-bound document reference shown by the second modal. */
enum CaxecraftFlowDocumentPanelTarget {
	NoFlowDocumentPanel;
	FlowDocumentPanel(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCardAddress, referenceIndex:Int, role:EditorFlowReferenceRole, revision:Int);
}

/** One document-picker result returned without mutating screen state. */
enum CaxecraftFlowDocumentPanelAction {
	KeepFlowDocumentPanel;
	CloseFlowDocumentPanel;
	SelectFlowDocumentReference(zone:ScenarioId, rule:ScenarioId, card:EditorFlowCardAddress, referenceIndex:Int, role:EditorFlowReferenceRole, revision:Int,
		value:ScenarioId);
}

/** True while one card-library address owns the modal. */
function flowCardPanelOpen(target:CaxecraftFlowCardPanelTarget):Bool
	return switch target {
		case NoFlowCardPanel: false;
		case _: true;
	};

/** True while one document-reference address owns the modal. */
function flowDocumentPanelOpen(target:CaxecraftFlowDocumentPanelTarget):Bool
	return switch target {
		case NoFlowDocumentPanel: false;
		case _: true;
	};

/** Return the registry-derived row count for keyboard and controller traversal. */
function flowCardPanelChoiceCount(target:CaxecraftFlowCardPanelTarget, draft:Scenario, content:EditorFlowContentChoices):Int
	return switch target {
		case NoFlowCardPanel: 0;
		case WhenFlowCardPanel(_, _): eventCardChoices(draft, content).length;
		case IfFlowCardPanel(_, rule) | NestedIfFlowCardPanel(_, rule, _):
			final current = findRule(draft, rule);
			current == null ? 0 : predicateCardChoices(draft, current.event, content).length;
		case DoFlowCardPanel(_, rule, _) | InsertDoFlowCardPanel(_, rule, _): actionCardChoices(draft, rule, content, false).length;
		case NestedDoFlowCardPanel(_, rule, _, _, _): actionCardChoices(draft, rule, content, true).length;
		case SequenceDoFlowCardPanel(sequence,
			_) | InsertSequenceDoFlowCardPanel(sequence, _): actionCardChoices(draft, sequence, content, false, sequence).length;
	};

/** Convert one highlighted card row into the same typed action as a mouse click. */
function flowCardPanelActionAt(target:CaxecraftFlowCardPanelTarget, draft:Scenario, content:EditorFlowContentChoices,
		selection:Int):CaxecraftFlowCardPanelAction {
	if (selection < 0)
		return KeepFlowCardPanel;
	return switch target {
		case NoFlowCardPanel:
			CloseFlowCardPanel;
		case WhenFlowCardPanel(zone, rule):
			final choices = eventCardChoices(draft, content);
			if (selection >= choices.length) KeepFlowCardPanel; else switch choices[selection] {
				case ReadyEventChoice(_, value): SelectFlowEvent(zone, rule, value);
				case UnavailableEventChoice(_, _): KeepFlowCardPanel;
			}
		case IfFlowCardPanel(zone, rule):
			predicatePanelAction(zone, rule, null, draft, content, selection);
		case NestedIfFlowCardPanel(zone, rule, path):
			predicatePanelAction(zone, rule, path, draft, content, selection);
		case DoFlowCardPanel(zone, rule, actionIndex):
			actionPanelAction(zone, rule, actionIndex, -1, -1, false, false, draft, content, selection);
		case InsertDoFlowCardPanel(zone, rule, actionIndex):
			actionPanelAction(zone, rule, actionIndex, -1, -1, true, false, draft, content, selection);
		case NestedDoFlowCardPanel(zone, rule, parentActionIndex, choiceIndex, actionIndex):
			actionPanelAction(zone, rule, parentActionIndex, choiceIndex, actionIndex, false, true, draft, content, selection);
		case SequenceDoFlowCardPanel(sequence, actionIndex):
			sequenceActionPanelAction(sequence, actionIndex, false, draft, content, selection);
		case InsertSequenceDoFlowCardPanel(sequence, actionIndex):
			sequenceActionPanelAction(sequence, actionIndex, true, draft, content, selection);
	};
}

/** Resolve one reusable-sequence action from the same executable registry. */
private function sequenceActionPanelAction(sequence:ScenarioId, actionIndex:Int, insert:Bool, draft:Scenario, content:EditorFlowContentChoices,
		selection:Int):CaxecraftFlowCardPanelAction {
	final choices = actionCardChoices(draft, sequence, content, false, sequence);
	if (selection >= choices.length)
		return KeepFlowCardPanel;
	return switch choices[selection] {
		case ReadyActionChoice(_, value): SelectFlowSequenceAction(sequence, actionIndex, insert, value);
		case UnavailableActionChoice(_, _): KeepFlowCardPanel;
	};
}

/** Return the role-filtered row count for document-reference traversal. */
function flowDocumentPanelChoiceCount(target:CaxecraftFlowDocumentPanelTarget, draft:Scenario):Int
	return switch target {
		case NoFlowDocumentPanel: 0;
		case FlowDocumentPanel(_, _, _, _, role, _): documentFlowReferenceChoices(draft, role).length;
	};

/** Convert one highlighted document row into the same result as a mouse click. */
function flowDocumentPanelActionAt(target:CaxecraftFlowDocumentPanelTarget, draft:Scenario, selection:Int):CaxecraftFlowDocumentPanelAction {
	if (selection < 0)
		return KeepFlowDocumentPanel;
	return switch target {
		case NoFlowDocumentPanel:
			CloseFlowDocumentPanel;
		case FlowDocumentPanel(zone, rule, card, referenceIndex, role, revision):
			final choices = documentFlowReferenceChoices(draft, role);
			selection >= choices.length ? KeepFlowDocumentPanel : SelectFlowDocumentReference(zone, rule, card, referenceIndex, role, revision,
				choices[selection]);
	};
}

/** Resolve one root or nested predicate row without duplicating registry facts. */
private function predicatePanelAction(zone:ScenarioId, rule:ScenarioId, path:Null<Array<Int>>, draft:Scenario, content:EditorFlowContentChoices,
		selection:Int):CaxecraftFlowCardPanelAction {
	final current = findRule(draft, rule);
	if (current == null)
		return CloseFlowCardPanel;
	final choices = predicateCardChoices(draft, current.event, content);
	if (selection >= choices.length)
		return KeepFlowCardPanel;
	return switch choices[selection] {
		case ReadyPredicateChoice(_, value): SelectFlowPredicate(zone, rule, path, value);
		case UnavailablePredicateChoice(_, _): KeepFlowCardPanel;
	};
}

/** Resolve one top-level or weighted action row from the executable registry. */
private function actionPanelAction(zone:ScenarioId, rule:ScenarioId, actionIndex:Int, choiceIndex:Int, nestedActionIndex:Int, insert:Bool, nested:Bool,
		draft:Scenario, content:EditorFlowContentChoices, selection:Int):CaxecraftFlowCardPanelAction {
	final choices = actionCardChoices(draft, rule, content, nested);
	if (selection >= choices.length)
		return KeepFlowCardPanel;
	return switch choices[selection] {
		case ReadyActionChoice(_, value): SelectFlowAction(zone, rule, actionIndex, choiceIndex, nestedActionIndex, insert, value);
		case UnavailableActionChoice(_, _): KeepFlowCardPanel;
	};
}

/** Draw one complete card palette and return at most one typed user decision. */
function drawFlowCardPanel(catalog:RuntimeUiCatalog, locale:LocaleCursor, target:CaxecraftFlowCardPanelTarget, draft:Scenario,
		content:EditorFlowContentChoices, selection:Int, width:Int, height:Int):CaxecraftFlowCardPanelAction {
	Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 10, 14, 235));
	final panelWidth = width >= 1120 ? 1060 : width - 48;
	final panelHeight = height >= 680 ? 640 : height - 32;
	final left = Std.int((width - panelWidth) / 2);
	final top = Std.int((height - panelHeight) / 2);
	Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight), catalog.format(locale, EditorFlowUiMessage.CardLibraryMessage.messageId(), []));
	if (Raygui.ButtonString(Rectangle.fromFloat(left + panelWidth - 112, top + 12, 88, 28),
		catalog.format(locale, EditorFlowUiMessage.DoneMessage.messageId(), []))
		.has(GuiResult.Pressed))
		return CloseFlowCardPanel;
	final rowLeft = left + 22;
	final rowTop = top + 52;
	final rowWidth = panelWidth - 44;
	final visibleRows = visiblePaletteRows(panelHeight);
	return switch target {
		case NoFlowCardPanel: CloseFlowCardPanel;
		case WhenFlowCardPanel(zone, rule): drawEventRows(catalog, locale, zone, rule, draft, content, selection, visibleRows, rowLeft, rowTop, rowWidth);
		case IfFlowCardPanel(zone, rule):
			final current = findRule(draft, rule);
			current == null ? CloseFlowCardPanel : drawPredicateRows(catalog, locale, zone, rule, null, current.event, draft, content, selection, visibleRows,
				rowLeft, rowTop, rowWidth);
		case DoFlowCardPanel(zone, rule, actionIndex):
			drawActionRows(catalog, locale, zone, rule, actionIndex, -1, -1, false, draft, content, selection, visibleRows, rowLeft, rowTop, rowWidth);
		case InsertDoFlowCardPanel(zone, rule, actionIndex):
			drawActionRows(catalog, locale, zone, rule, actionIndex, -1, -1, true, draft, content, selection, visibleRows, rowLeft, rowTop, rowWidth);
		case NestedIfFlowCardPanel(zone, rule, path):
			final current = findRule(draft, rule);
			current == null ? CloseFlowCardPanel : drawPredicateRows(catalog, locale, zone, rule, path, current.event, draft, content, selection, visibleRows,
				rowLeft, rowTop, rowWidth);
		case NestedDoFlowCardPanel(zone, rule, parentActionIndex, choiceIndex, actionIndex):
			drawActionRows(catalog, locale, zone, rule, parentActionIndex, choiceIndex, actionIndex, false, draft, content, selection, visibleRows, rowLeft,
				rowTop, rowWidth);
		case SequenceDoFlowCardPanel(sequence, actionIndex):
			drawSequenceActionRows(catalog, locale, sequence, actionIndex, false, draft, content, selection, visibleRows, rowLeft, rowTop, rowWidth);
		case InsertSequenceDoFlowCardPanel(sequence, actionIndex):
			drawSequenceActionRows(catalog, locale, sequence, actionIndex, true, draft, content, selection, visibleRows, rowLeft, rowTop, rowWidth);
	};
}

/** Draw the role-filtered identities already present in the map. */
function drawFlowDocumentPanel(catalog:RuntimeUiCatalog, locale:LocaleCursor, target:CaxecraftFlowDocumentPanelTarget, draft:Scenario, selection:Int,
		width:Int, height:Int):CaxecraftFlowDocumentPanelAction {
	Raylib.DrawRectangle(0, 0, width, height, Color.rgba(4, 10, 14, 235));
	final panelWidth = width >= 760 ? 680 : width - 48;
	final panelHeight = height >= 520 ? 440 : height - 48;
	final left = Std.int((width - panelWidth) / 2);
	final top = Std.int((height - panelHeight) / 2);
	Raygui.PanelString(Rectangle.fromFloat(left, top, panelWidth, panelHeight),
		catalog.format(locale, EditorFlowUiMessage.DocumentPickerMessage.messageId(), []));
	if (Raygui.ButtonString(Rectangle.fromFloat(left + panelWidth - 112, top + 12, 88, 28),
		catalog.format(locale, EditorFlowUiMessage.DoneMessage.messageId(), []))
		.has(GuiResult.Pressed))
		return CloseFlowDocumentPanel;
	return switch target {
		case NoFlowDocumentPanel: CloseFlowDocumentPanel;
		case FlowDocumentPanel(zone, rule, card, referenceIndex, role, revision):
			final choices = documentFlowReferenceChoices(draft, role);
			if (choices.length == 0)
				Raylib.DrawTextString(catalog.format(locale, EditorFlowUiMessage.UnavailableMessage.messageId(), []), left + 28, top + 64, 18,
					Color.rgba(255, 154, 112));
			var selected:Null<ScenarioId> = null;
			final visibleRows = visibleDocumentRows(panelHeight);
			final start = paletteWindowStart(selection, choices.length, visibleRows);
			final end = minimum(choices.length, start + visibleRows);
			for (index in start...end) {
				final rowTop = top + 58 + (index - start) * 34;
				if (index == selection)
					Raylib.DrawRectangleLines(left + 25, rowTop - 3, panelWidth - 50, 34, CaxecraftPalette.selection());
				if (Raygui.ButtonString(Rectangle.fromFloat(left + 28, rowTop, panelWidth - 56, 28), choices[index].text()).has(GuiResult.Pressed))
					selected = choices[index];
			}
			selected == null ? KeepFlowDocumentPanel : SelectFlowDocumentReference(zone, rule, card, referenceIndex, role, revision, selected);
	};
}

/** Draw one complete value for each canonical event descriptor. */
private function drawEventRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, draft:Scenario,
		content:EditorFlowContentChoices, selection:Int, visibleRows:Int, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final choices = eventCardChoices(draft, content);
	final start = paletteWindowStart(selection, choices.length, visibleRows);
	final end = minimum(choices.length, start + visibleRows);
	for (index in start...end)
		switch choices[index] {
			case ReadyEventChoice(descriptor, value):
				final text = eventFlowCardText(value);
				if (drawRow(catalog, locale, left, top + (index - start) * 29, width, catalog.format(locale, text.message, text.arguments),
					descriptor.editorHelp, WhenCardIcon, true, index == selection, Color.rgba(210, 105, 230)))
					return SelectFlowEvent(zone, rule, value);
			case UnavailableEventChoice(descriptor, _):
				drawRow(catalog, locale, left, top + (index - start) * 29, width, descriptor.traceName, descriptor.editorHelp, WhenCardIcon, false,
					index == selection, Color.rgba(210, 105, 230));
		}
	return KeepFlowCardPanel;
}

/** Draw one complete value for each canonical predicate descriptor. */
private function drawPredicateRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, path:Null<Array<Int>>, event:FlowEvent,
		draft:Scenario, content:EditorFlowContentChoices, selection:Int, visibleRows:Int, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final choices = predicateCardChoices(draft, event, content);
	final start = paletteWindowStart(selection, choices.length, visibleRows);
	final end = minimum(choices.length, start + visibleRows);
	for (index in start...end)
		switch choices[index] {
			case ReadyPredicateChoice(descriptor, value):
				final text = predicateFlowCardText(value);
				if (drawRow(catalog, locale, left, top + (index - start) * 29, width, catalog.format(locale, text.message, text.arguments),
					descriptor.editorHelp, IfCardIcon, true, index == selection, Color.rgba(84, 191, 205)))
					return SelectFlowPredicate(zone, rule, path, value);
			case UnavailablePredicateChoice(descriptor, _):
				drawRow(catalog, locale, left, top + (index - start) * 29, width, descriptor.traceName, descriptor.editorHelp, IfCardIcon, false,
					index == selection, Color.rgba(84, 191, 205));
		}
	return KeepFlowCardPanel;
}

/** Draw top-level or weighted-branch actions from one registry. */
private function drawActionRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, actionIndex:Int, choiceIndex:Int,
		nestedActionIndex:Int, insert:Bool, draft:Scenario, content:EditorFlowContentChoices, selection:Int, visibleRows:Int, left:Int, top:Int,
		width:Int):CaxecraftFlowCardPanelAction {
	final nested = choiceIndex >= 0;
	final choices = actionCardChoices(draft, rule, content, nested);
	final start = paletteWindowStart(selection, choices.length, visibleRows);
	final end = minimum(choices.length, start + visibleRows);
	for (index in start...end)
		switch choices[index] {
			case ReadyActionChoice(descriptor, value):
				final text = actionFlowCardText(value);
				if (drawRow(catalog, locale, left, top + (index - start) * 29, width, catalog.format(locale, text.message, text.arguments),
					descriptor.editorHelp, DoCardIcon, true, index == selection, Color.rgba(111, 174, 91)))
					return SelectFlowAction(zone, rule, actionIndex, choiceIndex, nestedActionIndex, insert, value);
			case UnavailableActionChoice(descriptor, _):
				drawRow(catalog, locale, left, top + (index - start) * 29, width, descriptor.traceName, descriptor.editorHelp, DoCardIcon, false,
					index == selection, Color.rgba(111, 174, 91));
		}
	return KeepFlowCardPanel;
}

/** Draw sequence actions from the same registry without pretending a sequence is a rule. */
private function drawSequenceActionRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, sequence:ScenarioId, actionIndex:Int, insert:Bool, draft:Scenario,
		content:EditorFlowContentChoices, selection:Int, visibleRows:Int, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final choices = actionCardChoices(draft, sequence, content, false, sequence);
	final start = paletteWindowStart(selection, choices.length, visibleRows);
	final end = minimum(choices.length, start + visibleRows);
	for (index in start...end)
		switch choices[index] {
			case ReadyActionChoice(descriptor, value):
				final text = actionFlowCardText(value);
				if (drawRow(catalog, locale, left, top + (index - start) * 29, width, catalog.format(locale, text.message, text.arguments),
					descriptor.editorHelp, DoCardIcon, true, index == selection, Color.rgba(111, 174, 91)))
					return SelectFlowSequenceAction(sequence, actionIndex, insert, value);
			case UnavailableActionChoice(descriptor, _):
				drawRow(catalog, locale, left, top + (index - start) * 29, width, descriptor.traceName, descriptor.editorHelp, DoCardIcon, false,
					index == selection, Color.rgba(111, 174, 91));
		}
	return KeepFlowCardPanel;
}

/** Draw one localized sentence, data-owned help, and a stable family picture. */
private function drawRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, left:Int, top:Int, width:Int, label:String, help:MessageId, icon:FlowCardFamilyIcon,
		ready:Bool, selected:Bool, color:Color):Bool {
	final labelWidth = Std.int(width * 0.43);
	if (selected)
		Raylib.DrawRectangleLines(left - 10, top - 2, width + 12, 29, CaxecraftPalette.selection());
	Raylib.DrawRectangle(left - 7, top + 2, 4, 21, color);
	var pressed = false;
	if (ready)
		pressed = Raygui.ButtonString(Rectangle.fromFloat(left, top, labelWidth, 25), label).has(GuiResult.Pressed);
	else {
		Raylib.DrawRectangle(left, top, labelWidth, 25, Color.rgba(29, 38, 42));
		Raylib.DrawTextString('$label  ×', left + 29, top + 6, 12, Color.rgba(160, 169, 170));
	}
	drawFlowCardFamilyIcon(icon, left + 5, top + 3, ready ? color : Color.rgba(160, 169, 170));
	Raylib.DrawTextString(catalog.format(locale, help, []), left + labelWidth + 14, top + 6, 12,
		ready ? CaxecraftPalette.hudText() : Color.rgba(160, 169, 170));
	return pressed;
}

/** Maximum complete card rows that fit without clipping the modal. */
private inline function visiblePaletteRows(panelHeight:Int):Int
	return maximum(1, Std.int((panelHeight - 70) / 29));

/** Maximum complete document rows that fit without clipping the modal. */
private inline function visibleDocumentRows(panelHeight:Int):Int
	return maximum(1, Std.int((panelHeight - 78) / 34));

/** Keep the selected row visible while retaining stable registry order. */
private function paletteWindowStart(selection:Int, count:Int, visible:Int):Int {
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

/** Draw one compact family picture from primitives supported by every backend. */
private function drawFlowCardFamilyIcon(icon:FlowCardFamilyIcon, left:Int, top:Int, color:Color):Void {
	final centerX = left + 8;
	final centerY = top + 9;
	switch icon {
		case WhenCardIcon:
			Raylib.DrawCircle(centerX, centerY, c.Float32.fromFloat(3.0), color);
			Raylib.DrawLine(centerX, top, centerX, top + 4, color);
			Raylib.DrawLine(centerX, top + 14, centerX, top + 18, color);
			Raylib.DrawLine(left - 1, centerY, left + 3, centerY, color);
			Raylib.DrawLine(left + 13, centerY, left + 17, centerY, color);
		case IfCardIcon:
			Raylib.DrawLine(centerX, top, left + 16, centerY, color);
			Raylib.DrawLine(left + 16, centerY, centerX, top + 18, color);
			Raylib.DrawLine(centerX, top + 18, left, centerY, color);
			Raylib.DrawLine(left, centerY, centerX, top, color);
			Raylib.DrawCircle(centerX, centerY, c.Float32.fromFloat(2.0), color);
		case DoCardIcon:
			Raylib.DrawLine(left, centerY, left + 15, centerY, color);
			Raylib.DrawLine(left + 10, top + 4, left + 15, centerY, color);
			Raylib.DrawLine(left + 15, centerY, left + 10, top + 14, color);
	}
}

/** Find one rule without retaining another mutable index. */
private function findRule(draft:Scenario, expected:ScenarioId):Null<caxecraft.scenario.CaxeFlow.FlowRule> {
	for (rule in draft.flow.rules)
		if (rule.id.text() == expected.text())
			return rule;
	return null;
}
#end
