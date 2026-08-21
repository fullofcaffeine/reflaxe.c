package caxecraft.qa;

import caxecraft.app.InteractionPrompt.InteractionPrompt;
import caxecraft.app.InteractionPrompt.InteractionTargetKind;
import caxecraft.app.InteractionPrompt.interactionPrompt;
import caxecraft.app.AtlasLayout.entityAtlasCellCount;
import caxecraft.app.AtlasLayout.entityAtlasCellIsValid;
import caxecraft.app.AtlasLayout.entityAtlasColumn;
import caxecraft.app.AtlasLayout.entityAtlasColumns;
import caxecraft.app.AtlasLayout.entityAtlasRow;
import caxecraft.app.AtlasLayout.entityAtlasRows;
import caxecraft.app.ConversationFlow.ConversationAdvance;
import caxecraft.app.ConversationFlow.advanceConversation;
import caxecraft.app.ConversationFlow.beginConversation;
import caxecraft.app.MotionInterpolation.advance;
import caxecraft.app.MotionInterpolation.reset;
import caxecraft.app.MotionInterpolation.sample;
import caxecraft.app.MotionInterpolation.start;
import caxecraft.app.HudLayout.HudRectangle;
import caxecraft.app.HudLayout.HudBreathGlyph;
import caxecraft.app.HudLayout.HudReplacementState;
import caxecraft.app.HudLayout.hudBreathGlyph;
import caxecraft.app.HudLayout.hudLayout;
import caxecraft.app.HudLayout.hudModalLayout;
import caxecraft.app.HudLayout.hudRectanglesOverlap;
import caxecraft.app.HudLayout.hudReplacementState;
import caxecraft.app.HudLayout.hudWrappedCharacterLimit;
import caxecraft.app.HudLayout.hudWrappedLineLimit;
import caxecraft.app.StatefulObjectVisual.StatefulObjectVisualKind;
import caxecraft.app.StatefulObjectVisual.statefulObjectVisual;
import caxecraft.domain.CharacterBody;
import caxecraft.gameplay.Inventory;

/**
	Cross-target specification for fixed-step presentation interpolation.

	The assertions run on Eval and generated native C. They test only the visual
	copy between committed positions; no renderer timing can mutate the bodies
	that physics, interactions, saves, and deterministic pilots observe.
**/
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck();
	#else
	Sys.println(selfCheck());
	#end
}

/** Return zero, or the stable number of the first broken interpolation rule. */
function selfCheck():Int {
	final origin = body(0.0, 2.0, -4.0);
	var history = start(origin);
	final initial = sample(history, 0.025, 0.05);
	if (!near(initial.x, 0.0) || !near(initial.y, 2.0) || !near(initial.z, -4.0))
		return 1;

	history = advance(history, body(10.0, 6.0, 4.0));
	final startPoint = sample(history, 0.0, 0.05);
	if (!near(startPoint.x, 0.0) || !near(startPoint.y, 2.0) || !near(startPoint.z, -4.0))
		return 2;
	final midpoint = sample(history, 0.025, 0.05);
	if (!near(midpoint.x, 5.0) || !near(midpoint.y, 4.0) || !near(midpoint.z, 0.0))
		return 3;
	final endPoint = sample(history, 0.05, 0.05);
	if (!near(endPoint.x, 10.0) || !near(endPoint.y, 6.0) || !near(endPoint.z, 4.0))
		return 4;

	final early = sample(history, -1.0, 0.05);
	final late = sample(history, 1.0, 0.05);
	if (!near(early.x, 0.0) || !near(late.x, 10.0))
		return 5;

	history = advance(history, body(20.0, 8.0, 14.0));
	final adjacentMidpoint = sample(history, 0.025, 0.05);
	if (!near(adjacentMidpoint.x, 15.0) || !near(adjacentMidpoint.y, 7.0) || !near(adjacentMidpoint.z, 9.0))
		return 6;

	history = reset(body(-30.0, 40.0, 70.0));
	final teleported = sample(history, 0.04, 0.05);
	if (!near(teleported.x, -30.0) || !near(teleported.y, 40.0) || !near(teleported.z, 70.0))
		return 7;
	final invalidClock = sample(history, 0.04, 0.0);
	if (!near(invalidClock.x, -30.0) || !near(invalidClock.y, 40.0) || !near(invalidClock.z, 70.0))
		return 8;

	// The reviewed entity atlas is 1024x1280 with 4x5 square cells. Nia starts
	// the second row at index 4; the Fallskeeper occupies the admitted fifth row.
	if (entityAtlasColumns() != 4 || entityAtlasRows() != 5 || entityAtlasCellCount() != 20)
		return 9;
	if (entityAtlasColumn(4) != 0 || entityAtlasRow(4) != 1)
		return 10;
	if (entityAtlasColumn(19) != 3 || entityAtlasRow(19) != 4)
		return 11;
	if (!entityAtlasCellIsValid(0) || !entityAtlasCellIsValid(19) || entityAtlasCellIsValid(-1) || entityAtlasCellIsValid(20))
		return 12;
	if (Std.int(1024 / entityAtlasColumns()) != 256 || Std.int(1280 / entityAtlasRows()) != 256)
		return 13;
	if (interactionPrompt(InteractionTargetKind.NoInteractionTarget) != InteractionPrompt.NoInteractionPrompt)
		return 14;
	if (interactionPrompt(InteractionTargetKind.DialogueInteractionTarget) != InteractionPrompt.TalkInteractionPrompt)
		return 15;
	if (interactionPrompt(InteractionTargetKind.MechanismInteractionTarget) != InteractionPrompt.UseInteractionPrompt)
		return 16;
	final mechanism = statefulObjectVisual(1000, 1000, 1000, 270);
	if (mechanism.kind != StatefulObjectVisualKind.MechanismVisual || mechanism.widthMilli != 1000 || mechanism.depthMilli != 1000)
		return 17;
	final gateNorth = statefulObjectVisual(7000, 3000, 500, 0);
	if (gateNorth.kind != StatefulObjectVisualKind.StructureVisual
		|| gateNorth.widthMilli != 7000
		|| gateNorth.heightMilli != 3000
		|| gateNorth.depthMilli != 500)
		return 18;
	final gateEast = statefulObjectVisual(7000, 3000, 500, 90);
	if (gateEast.kind != StatefulObjectVisualKind.StructureVisual
		|| gateEast.widthMilli != 500
		|| gateEast.heightMilli != 3000
		|| gateEast.depthMilli != 7000)
		return 19;
	final gateWest = statefulObjectVisual(7000, 3000, 500, 270);
	if (gateWest.widthMilli != 500 || gateWest.depthMilli != 7000)
		return 20;
	final gateSouth = statefulObjectVisual(7000, 3000, 500, 180);
	if (gateSouth.widthMilli != 7000 || gateSouth.depthMilli != 500)
		return 21;
	var conversation = beginConversation();
	conversation = switch advanceConversation(conversation, 12, 2, 25, false, false) {
		case ConversationContinues(next): next;
		case ConversationCloses: return 22;
	};
	if (conversation.lineIndex != 0 || conversation.visibleCharacters != 1)
		return 23;
	conversation = switch advanceConversation(conversation, 12, 2, 0, true, false) {
		case ConversationContinues(next): next;
		case ConversationCloses: return 24;
	};
	if (conversation.visibleCharacters != 12)
		return 25;
	conversation = switch advanceConversation(conversation, 12, 2, 0, true, false) {
		case ConversationContinues(next): next;
		case ConversationCloses: return 26;
	};
	if (conversation.lineIndex != 1 || conversation.visibleCharacters != 0)
		return 27;
	conversation = switch advanceConversation(conversation, 8, 2, 300, false, true) {
		case ConversationContinues(next): next;
		case ConversationCloses: return 28;
	};
	switch advanceConversation(conversation, 8, 2, 300, false, true) {
		case ConversationContinues(_):
			return 29;
		case ConversationCloses:
	};
	var failure = hudViewportFailure(800, 450, true);
	if (failure != 0)
		return 30 + failure;
	failure = hudViewportFailure(899, 599, true);
	if (failure != 0)
		return 50 + failure;
	failure = hudViewportFailure(900, 600, false);
	if (failure != 0)
		return 70 + failure;
	failure = hudViewportFailure(999, 600, false);
	if (failure != 0)
		return 90 + failure;
	failure = hudViewportFailure(1000, 600, false);
	if (failure != 0)
		return 110 + failure;
	failure = hudViewportFailure(1155, 600, false);
	if (failure != 0)
		return 130 + failure;
	failure = hudViewportFailure(1156, 600, false);
	if (failure != 0)
		return 150 + failure;
	failure = hudViewportFailure(1280, 450, true);
	if (failure != 0)
		return 170 + failure;
	failure = hudViewportFailure(1280, 720, false);
	if (failure != 0)
		return 190 + failure;
	if (hudReplacementState(false, false, false) != HudReplacementState.NormalHud
		|| hudReplacementState(false, false, true) != HudReplacementState.ConversationHud
		|| hudReplacementState(false, true, true) != HudReplacementState.PausedHud
		|| hudReplacementState(true, true, true) != HudReplacementState.DefeatedHud)
		return 210;
	final compactConversation = hudModalLayout(800, 450, false).conversationText;
	final characterLimit = hudWrappedCharacterLimit(compactConversation, 18);
	final lineLimit = hudWrappedLineLimit(compactConversation, 25);
	if (characterLimit < 50 || lineLimit < 4)
		return 211;
	if (hudBreathGlyph(0, 1) != HudBreathGlyph.FilledBreath
		|| hudBreathGlyph(1, 1) != HudBreathGlyph.DepletedBreath
		|| hudBreathGlyph(-1, 1) != HudBreathGlyph.DepletedBreath)
		return 212;
	return 0;
}

/** Return zero, or the first broken fit or separation rule for one viewport. */
function hudViewportFailure(width:Int, height:Int, compact:Bool):Int {
	final layout = hudLayout(width, height, Inventory.SLOT_COUNT);
	if (layout.compact != compact)
		return 1;
	final expectedHotbarWidth = layout.compact ? 456 : 608;
	if (layout.hotbar.width != expectedHotbarWidth || layout.slotSize != (layout.compact ? 48 : 64))
		return 2;
	if (!rectangleFits(layout.objective, width, height)
		|| !rectangleFits(layout.diagnostics, width, height)
		|| !rectangleFits(layout.health, width, height)
		|| !rectangleFits(layout.equipment, width, height)
		|| !rectangleFits(layout.alert, width, height)
		|| !rectangleFits(layout.action, width, height)
		|| !rectangleFits(layout.breath, width, height)
		|| !rectangleFits(layout.crosshair, width, height)
		|| !rectangleFits(layout.hotbar, width, height))
		return 3;
	if (hudRectanglesOverlap(layout.objective, layout.health)
		|| hudRectanglesOverlap(layout.objective, layout.equipment)
		|| hudRectanglesOverlap(layout.objective, layout.alert)
		|| hudRectanglesOverlap(layout.alert, layout.health)
		|| hudRectanglesOverlap(layout.alert, layout.equipment))
		return 4;
	if (hudRectanglesOverlap(layout.diagnostics, layout.objective)
		|| hudRectanglesOverlap(layout.diagnostics, layout.alert)
		|| hudRectanglesOverlap(layout.diagnostics, layout.health)
		|| hudRectanglesOverlap(layout.diagnostics, layout.equipment))
		return 5;
	if (hudRectanglesOverlap(layout.action, layout.hotbar)
		|| hudRectanglesOverlap(layout.action, layout.breath)
		|| hudRectanglesOverlap(layout.action, layout.crosshair)
		|| hudRectanglesOverlap(layout.breath, layout.hotbar)
		|| hudRectanglesOverlap(layout.breath, layout.crosshair)
		|| hudRectanglesOverlap(layout.crosshair, layout.hotbar))
		return 6;
	final simpleModal = hudModalLayout(width, height, false);
	final journalModal = hudModalLayout(width, height, true);
	if (!rectangleFits(simpleModal.conversation, width, height)
		|| !rectangleFits(simpleModal.pause, width, height)
		|| !rectangleFits(simpleModal.defeat, width, height)
		|| !rectangleFits(journalModal.pause, width, height))
		return 7;
	if (!rectangleContains(simpleModal.conversation, simpleModal.conversationPortrait)
		|| !rectangleContains(simpleModal.conversation, simpleModal.conversationText)
		|| !rectangleContains(simpleModal.conversation, simpleModal.conversationHelp))
		return 8;
	if (hudRectanglesOverlap(simpleModal.conversationText, simpleModal.conversationHelp))
		return 9;
	return 0;
}

/** True when one positive HUD region stays inside its declared viewport. */
function rectangleFits(rectangle:HudRectangle, width:Int, height:Int):Bool
	return rectangle.x >= 0
		&& rectangle.y >= 0
		&& rectangle.width > 0
		&& rectangle.height > 0
		&& rectangle.x + rectangle.width <= width
		&& rectangle.y + rectangle.height <= height;

/** True when one positive child rectangle stays inside its parent. */
function rectangleContains(parent:HudRectangle, child:HudRectangle):Bool
	return child.x >= parent.x
		&& child.y >= parent.y
		&& child.width > 0
		&& child.height > 0
		&& child.x + child.width <= parent.x + parent.width
		&& child.y + child.height <= parent.y + parent.height;

/** Build one committed-body-shaped fixture; non-position fields stay irrelevant. */
function body(x:Float, y:Float, z:Float):CharacterBody
	return {
		x: x,
		y: y,
		z: z,
		velocityX: 0.0,
		velocityY: 0.0,
		velocityZ: 0.0,
		grounded: true
	};

/** Small deterministic tolerance for arithmetic shared by Eval and native C. */
inline function near(actual:Float, expected:Float):Bool {
	final difference = actual - expected;
	return difference >= -0.000001 && difference <= 0.000001;
}
