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
}

/** One complete selection returned to the editor controller. */
enum CaxecraftFlowCardPanelAction {
	KeepFlowCardPanel;
	CloseFlowCardPanel;
	SelectFlowEvent(zone:ScenarioId, rule:ScenarioId, value:FlowEvent);
	SelectFlowPredicate(zone:ScenarioId, rule:ScenarioId, path:Null<Array<Int>>, value:FlowPredicate);
	SelectFlowAction(zone:ScenarioId, rule:ScenarioId, actionIndex:Int, choiceIndex:Int, nestedActionIndex:Int, insert:Bool, value:FlowAction);
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

/** Draw one complete card palette and return at most one typed user decision. */
function drawFlowCardPanel(catalog:RuntimeUiCatalog, locale:LocaleCursor, target:CaxecraftFlowCardPanelTarget, draft:Scenario,
		content:EditorFlowContentChoices, width:Int, height:Int):CaxecraftFlowCardPanelAction {
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
	return switch target {
		case NoFlowCardPanel: CloseFlowCardPanel;
		case WhenFlowCardPanel(zone, rule): drawEventRows(catalog, locale, zone, rule, draft, content, rowLeft, rowTop, rowWidth);
		case IfFlowCardPanel(zone, rule):
			final current = findRule(draft, rule);
			current == null ? CloseFlowCardPanel : drawPredicateRows(catalog, locale, zone, rule, null, current.event, draft, content, rowLeft, rowTop,
				rowWidth);
		case DoFlowCardPanel(zone, rule, actionIndex):
			drawActionRows(catalog, locale, zone, rule, actionIndex, -1, -1, false, draft, content, rowLeft, rowTop, rowWidth);
		case InsertDoFlowCardPanel(zone, rule, actionIndex):
			drawActionRows(catalog, locale, zone, rule, actionIndex, -1, -1, true, draft, content, rowLeft, rowTop, rowWidth);
		case NestedIfFlowCardPanel(zone, rule, path):
			final current = findRule(draft, rule);
			current == null ? CloseFlowCardPanel : drawPredicateRows(catalog, locale, zone, rule, path, current.event, draft, content, rowLeft, rowTop,
				rowWidth);
		case NestedDoFlowCardPanel(zone, rule, parentActionIndex, choiceIndex, actionIndex):
			drawActionRows(catalog, locale, zone, rule, parentActionIndex, choiceIndex, actionIndex, false, draft, content, rowLeft, rowTop, rowWidth);
	};
}

/** Draw the role-filtered identities already present in the map. */
function drawFlowDocumentPanel(catalog:RuntimeUiCatalog, locale:LocaleCursor, target:CaxecraftFlowDocumentPanelTarget, draft:Scenario, width:Int,
		height:Int):CaxecraftFlowDocumentPanelAction {
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
			for (index in 0...choices.length)
				if (Raygui.ButtonString(Rectangle.fromFloat(left + 28, top + 58 + index * 34, panelWidth - 56, 28), choices[index].text())
					.has(GuiResult.Pressed))
					selected = choices[index];
			selected == null ? KeepFlowDocumentPanel : SelectFlowDocumentReference(zone, rule, card, referenceIndex, role, revision, selected);
	};
}

/** Draw one complete value for each canonical event descriptor. */
private function drawEventRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, draft:Scenario,
		content:EditorFlowContentChoices, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final choices = eventCardChoices(draft, content);
	for (index in 0...choices.length)
		switch choices[index] {
			case ReadyEventChoice(descriptor, value):
				final text = eventFlowCardText(value);
				if (drawRow(catalog, locale, left, top + index * 29, width, catalog.format(locale, text.message, text.arguments), descriptor.editorHelp, true,
					Color.rgba(210, 105, 230)))
					return SelectFlowEvent(zone, rule, value);
			case UnavailableEventChoice(descriptor, _):
				drawRow(catalog, locale, left, top + index * 29, width, descriptor.traceName, descriptor.editorHelp, false, Color.rgba(210, 105, 230));
		}
	return KeepFlowCardPanel;
}

/** Draw one complete value for each canonical predicate descriptor. */
private function drawPredicateRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, path:Null<Array<Int>>, event:FlowEvent,
		draft:Scenario, content:EditorFlowContentChoices, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final choices = predicateCardChoices(draft, event, content);
	for (index in 0...choices.length)
		switch choices[index] {
			case ReadyPredicateChoice(descriptor, value):
				final text = predicateFlowCardText(value);
				if (drawRow(catalog, locale, left, top + index * 29, width, catalog.format(locale, text.message, text.arguments), descriptor.editorHelp, true,
					Color.rgba(84, 191, 205)))
					return SelectFlowPredicate(zone, rule, path, value);
			case UnavailablePredicateChoice(descriptor, _):
				drawRow(catalog, locale, left, top + index * 29, width, descriptor.traceName, descriptor.editorHelp, false, Color.rgba(84, 191, 205));
		}
	return KeepFlowCardPanel;
}

/** Draw top-level or weighted-branch actions from one registry. */
private function drawActionRows(catalog:RuntimeUiCatalog, locale:LocaleCursor, zone:ScenarioId, rule:ScenarioId, actionIndex:Int, choiceIndex:Int,
		nestedActionIndex:Int, insert:Bool, draft:Scenario, content:EditorFlowContentChoices, left:Int, top:Int, width:Int):CaxecraftFlowCardPanelAction {
	final nested = choiceIndex >= 0;
	final choices = actionCardChoices(draft, rule, content, nested);
	for (index in 0...choices.length)
		switch choices[index] {
			case ReadyActionChoice(descriptor, value):
				final text = actionFlowCardText(value);
				if (drawRow(catalog, locale, left, top + index * 29, width, catalog.format(locale, text.message, text.arguments), descriptor.editorHelp, true,
					Color.rgba(111, 174, 91)))
					return SelectFlowAction(zone, rule, actionIndex, choiceIndex, nestedActionIndex, insert, value);
			case UnavailableActionChoice(descriptor, _):
				drawRow(catalog, locale, left, top + index * 29, width, descriptor.traceName, descriptor.editorHelp, false, Color.rgba(111, 174, 91));
		}
	return KeepFlowCardPanel;
}

/** Draw one localized sentence, data-owned help, and a stable family stripe. */
private function drawRow(catalog:RuntimeUiCatalog, locale:LocaleCursor, left:Int, top:Int, width:Int, label:String, help:MessageId, ready:Bool,
		color:Color):Bool {
	final labelWidth = Std.int(width * 0.43);
	Raylib.DrawRectangle(left - 7, top + 2, 4, 21, color);
	var pressed = false;
	if (ready)
		pressed = Raygui.ButtonString(Rectangle.fromFloat(left, top, labelWidth, 25), label).has(GuiResult.Pressed);
	else {
		Raylib.DrawRectangle(left, top, labelWidth, 25, Color.rgba(29, 38, 42));
		Raylib.DrawTextString('$label  ×', left + 7, top + 6, 12, Color.rgba(160, 169, 170));
	}
	Raylib.DrawTextString(catalog.format(locale, help, []), left + labelWidth + 14, top + 6, 12,
		ready ? CaxecraftPalette.hudText() : Color.rgba(160, 169, 170));
	return pressed;
}

/** Find one rule without retaining another mutable index. */
private function findRule(draft:Scenario, expected:ScenarioId):Null<caxecraft.scenario.CaxeFlow.FlowRule> {
	for (rule in draft.flow.rules)
		if (rule.id.text() == expected.text())
			return rule;
	return null;
}
#end
