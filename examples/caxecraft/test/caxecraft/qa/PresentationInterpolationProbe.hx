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
import caxecraft.app.StatefulObjectVisual.StatefulObjectVisualKind;
import caxecraft.app.StatefulObjectVisual.statefulObjectVisual;
import caxecraft.app.VoxelModelMotion.voxelModelLiftMilli;
import caxecraft.content.RuntimeContentPack.RuntimeModelMotion;
import caxecraft.domain.CharacterBody;

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
	if (voxelModelLiftMilli(StaticModel, 5) != 0
		|| voxelModelLiftMilli(PulseModel, 0) != 0
		|| voxelModelLiftMilli(PulseModel, 5) != 60
		|| voxelModelLiftMilli(PulseModel, 10) != 120
		|| voxelModelLiftMilli(PulseModel, 15) != 60
		|| voxelModelLiftMilli(PulseModel, 20) != 0)
		return 30;

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
	return 0;
}

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
