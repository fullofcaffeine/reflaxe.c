package caxecraft.app;

#if (c && caxecraft_pilot)
import caxecraft.domain.PlayerState;
import caxecraft.domain.RaycastHit;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.gameplay.GuidePhase;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.PilotScript.PilotScriptName;
import raylib.Color;
import raylib.Raylib;

/**
 * Pilot-only bridge from native game state to the external test runner.
 *
 * The current compiler intentionally has no general file or socket runtime.
 * Instead of hiding that gap with raw C, a pilot build paints 32 fixed-width
 * words into a two-pixel-high strip at the bottom of the real framebuffer.
 * Each hexadecimal digit has one exact opaque color. The Python runner reads
 * the final flushed frame, validates the magic, version, and field count, and
 * writes the human-readable JSON report. `Main` presents that same frame after
 * the capture, avoiding desktop back-buffer ambiguity.
 *
 * This is test evidence, not a gameplay protocol. Ordinary builds do not
 * define `caxecraft_pilot`, so Haxe removes this complete module and its draw
 * call before haxe.c sees the program. The strip is also a temporary one-shot
 * carrier, not a pattern to extend into live control. Once haxe.c admits the
 * standard Haxe input/output stack, the generated Haxe program should exchange
 * bounded structured messages through inherited process pipes; screenshots
 * should then return to visual evidence only.
 */
/**
 * Paint one complete, versioned observation of the state just rendered.
 *
 * Values use integers so the record is stable across JSON libraries and C
 * floating-point printers. Positions and velocities are rounded toward zero
 * to thousandths of a world unit. Hashes preserve their complete 32-bit bit
 * patterns even when Haxe `Int` represents them as negative numbers.
 */
function drawPilotTelemetry(name:PilotScriptName, completedFrames:Int, completedTicks:Int, player:PlayerState, cells:WorldCells, selection:RaycastHit,
		removedBlocks:Int, placedBlocks:Int, rejectedEdits:Int, visibleBlocks:Int, terrainDrawCalls:Int, health:Int, hotbarSlot:Int, guidePhase:GuidePhase,
		mosslingAlive:Bool, onTitle:Bool, paused:Bool, captured:Bool):Void {
	var flags = 0;
	if (onTitle)
		flags |= 1;
	if (paused)
		flags |= 2;
	if (captured)
		flags |= 4;

	var word = 0;
	word = drawWord(word, 0x43585054); // "CXPT": Caxecraft pilot telemetry.
	word = drawWord(word, 1); // Protocol version.
	word = drawWord(word, 32); // Number of words in this closed version.
	word = drawWord(word, PilotScript.scriptCode(name));
	word = drawWord(word, PilotScript.inputHash(name));
	word = drawWord(word, completedFrames);
	word = drawWord(word, completedTicks);
	word = drawWord(word, Std.int(player.x * 1000.0));
	word = drawWord(word, Std.int(player.y * 1000.0));
	word = drawWord(word, Std.int(player.z * 1000.0));
	word = drawWord(word, Std.int(player.velocityX * 1000.0));
	word = drawWord(word, Std.int(player.velocityY * 1000.0));
	word = drawWord(word, Std.int(player.velocityZ * 1000.0));
	word = drawWord(word, player.grounded ? 1 : 0);
	word = drawWord(word, World.stateHash(cells));
	word = drawWord(word, selection.hit ? 1 : 0);
	word = drawWord(word, selection.cellX);
	word = drawWord(word, selection.cellY);
	word = drawWord(word, selection.cellZ);
	word = drawWord(word, selection.previousX);
	word = drawWord(word, selection.previousY);
	word = drawWord(word, selection.previousZ);
	word = drawWord(word, removedBlocks);
	word = drawWord(word, placedBlocks);
	word = drawWord(word, rejectedEdits);
	word = drawWord(word, visibleBlocks);
	word = drawWord(word, terrainDrawCalls);
	word = drawWord(word, health);
	word = drawWord(word, hotbarSlot);
	word = drawWord(word, guidePhase);
	word = drawWord(word, mosslingAlive ? 1 : 0);
	drawWord(word, flags);
}

/** Draw eight high-to-low hexadecimal digits and return the next word slot. */
private function drawWord(word:Int, value:Int):Int {
	var digit = 0;
	while (digit < 8) {
		final shift = (7 - digit) * 4;
		final nibble = (value >>> shift) & 15;
		Raylib.DrawRectangle((word * 8 + digit) * 2, Raylib.GetScreenHeight() - 2, 2, 2, nibbleColor(nibble));
		digit++;
	}
	return word + 1;
}

/**
 * Resolve the protocol's exact color for one four-bit digit.
 *
 * Every branch contains literal channels so haxe.c can prove the imported
 * `unsigned char` conversion; no dynamic narrowing or lookup allocation is
 * needed in the pilot executable.
 */
private function nibbleColor(nibble:Int):Color {
	return switch nibble {
		case 0: Color.rgba(8, 247, 170);
		case 1: Color.rgba(24, 231, 186);
		case 2: Color.rgba(40, 215, 138);
		case 3: Color.rgba(56, 199, 154);
		case 4: Color.rgba(72, 183, 234);
		case 5: Color.rgba(88, 167, 250);
		case 6: Color.rgba(104, 151, 202);
		case 7: Color.rgba(120, 135, 218);
		case 8: Color.rgba(136, 119, 42);
		case 9: Color.rgba(152, 103, 58);
		case 10: Color.rgba(168, 87, 10);
		case 11: Color.rgba(184, 71, 26);
		case 12: Color.rgba(200, 55, 106);
		case 13: Color.rgba(216, 39, 122);
		case 14: Color.rgba(232, 23, 74);
		case _: Color.rgba(248, 7, 90);
	};
}
#end
