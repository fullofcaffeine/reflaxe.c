package;

import haxe.io.Bytes;
import haxe.io.BytesOutput;
import sys.FileSystem;
import sys.io.File;

/**
 * Builds Caxecraft's small world props in MagicaVoxel format.
 *
 * The checked-in `.vox` file is the game asset. This small source file keeps
 * its shape and palette reviewable without adding a second modeling format.
 */
class CaxecraftVoxels {
	static inline final SIZE = 32;

	/** Build every reviewed prop in one requested asset directory. */
	static function main():Void {
		final arguments = Sys.args();
		final checkOnly = arguments.length == 2 && arguments[1] == "--check";
		if (arguments.length != 1 && !checkOnly) {
			Sys.println("usage: haxe --run CaxecraftVoxels <model-directory> [--check]");
			Sys.exit(2);
		}
		final modelDirectory = arguments[0];
		final relay:Array<Voxel> = [];
		addBase(relay);
		addCabinet(relay);
		addControlFace(relay);
		addCrystal(relay);
		writeOrCheck('$modelDirectory/forge-relay.vox', encode(relay, false), checkOnly);

		final winch:Array<Voxel> = [];
		addWinchBase(winch);
		addWinchSupports(winch);
		addWinchDrum(winch);
		addWinchChainAndCrank(winch);
		writeOrCheck('$modelDirectory/gate-winch.vox', encode(winch, false), checkOnly);

		final fieldNote:Array<Voxel> = [];
		addFieldNoteRoll(fieldNote);
		addFieldNoteBinding(fieldNote);
		writeOrCheck('$modelDirectory/field-note.vox', encode(fieldNote, true), checkOnly);

		final glyphs = [River, Leaf, Moon, Flame];
		final glyphNames = ["river", "leaf", "moon", "flame"];
		for (index in 0...glyphs.length) {
			final waitingGlyph:Array<Voxel> = [];
			addRuneStone(waitingGlyph, glyphs[index]);
			writeOrCheck('$modelDirectory/vault-glyph-${glyphNames[index]}.vox', encode(waitingGlyph, false), checkOnly);

			final enteredGlyph:Array<Voxel> = [];
			addRuneStone(enteredGlyph, glyphs[index]);
			addEnteredRuneLight(enteredGlyph);
			writeOrCheck('$modelDirectory/vault-glyph-${glyphNames[index]}-active.vox', encode(enteredGlyph, false), checkOnly);
			Sys.println('vault-glyph-${glyphNames[index]}.vox: ${waitingGlyph.length} waiting, ${enteredGlyph.length} active voxels');
		}
		Sys.println('forge-relay.vox: ${relay.length} voxels');
		Sys.println('gate-winch.vox: ${winch.length} voxels');
		Sys.println('field-note.vox: ${fieldNote.length} voxels');
	}

	/** Add one grounded carved stone whose raised mark stays readable from play height. */
	static function addRuneStone(voxels:Array<Voxel>, glyph:RuneGlyph):Void {
		for (z in 0...4)
			fillChamferedLayer(voxels, 4 - z, 27 + z, 5 - z, 26 + z, z, 5, z == 0 ? 2 : 3);
		for (z in 4...8)
			fillChamferedLayer(voxels, 6, 25, 7, 24, z, 4, z == 7 ? 4 : 3);
		for (z in 8...27)
			fillChamferedLayer(voxels, 7, 24, 9, 22, z, 4, z % 5 == 0 ? 4 : 2);
		for (z in 27...30)
			fillChamferedLayer(voxels, 6, 25, 8, 23, z, 5, z == 28 ? 4 : 3);

		// A recessed front panel prevents the mark from reading as a pasted card.
		fill(voxels, 9, 22, 7, 8, 11, 24, 1);
		for (x in 9...23) {
			put(voxels, x, 6, 10, x == 9 || x == 22 ? 5 : 6);
			put(voxels, x, 6, 25, x == 9 || x == 22 ? 5 : 7);
		}
		for (z in 11...25) {
			put(voxels, 9, 6, z, 6);
			put(voxels, 22, 6, z, 5);
		}

		switch glyph {
			case River:
				addRiverRune(voxels);
			case Leaf:
				addLeafRune(voxels);
			case Moon:
				addMoonRune(voxels);
			case Flame:
				addFlameRune(voxels);
		}
	}

	/**
	 * Add a bright inset frame that confirms a rune-stone interaction.
	 *
	 * Puzzle rules select this model through the generic object-state profile.
	 * They can therefore light one control or reset a group without renderer or
	 * campaign-specific code.
	 */
	static function addEnteredRuneLight(voxels:Array<Voxel>):Void {
		for (x in 9...23) {
			put(voxels, x, 5, 10, x % 4 == 0 ? 12 : 11);
			put(voxels, x, 5, 25, x % 4 == 0 ? 12 : 11);
		}
		for (z in 11...25) {
			put(voxels, 9, 5, z, z % 4 == 0 ? 12 : 11);
			put(voxels, 22, 5, z, z % 4 == 0 ? 12 : 11);
		}
		for (x in 12...20)
			put(voxels, x, 5, 28, x == 15 || x == 16 ? 12 : 11);
	}

	/** Raise three flowing bands, with highlights that distinguish the river rune. */
	static function addRiverRune(voxels:Array<Voxel>):Void {
		for (row in 0...3)
			for (step in 0...12) {
				final x = 10 + step;
				final z = 13 + row * 4 + (step % 4 < 2 ? 1 : 0);
				put(voxels, x, 5, z, step % 5 == 0 ? 11 : 9);
				put(voxels, x, 4, z, step % 3 == 0 ? 11 : 10);
			}
	}

	/** Raise a broad leaf and central stem instead of tracing a flat sprite edge. */
	static function addLeafRune(voxels:Array<Voxel>):Void {
		for (z in 12...24) {
			final distance = absolute(z - 18);
			final radius = 5 - Std.int(distance / 2);
			for (x in 16 - radius...17 + radius)
				if (absolute(x - 16) + distance <= 8)
					put(voxels, x, x == 16 ? 4 : 5, z, x <= 16 ? 8 : 10);
		}
		for (z in 11...20) {
			put(voxels, 16, 3, z, 11);
			if (z < 17)
				put(voxels, 17, 4, z, 9);
		}
	}

	/** Raise a thick crescent with a cut inner arc that remains visible at distance. */
	static function addMoonRune(voxels:Array<Voxel>):Void {
		for (x in 10...22)
			for (z in 12...24) {
				final dx = x - 16;
				final dz = z - 18;
				final outer = dx * dx + dz * dz;
				final innerDx = x - 18;
				final inner = innerDx * innerDx + dz * dz;
				if (outer <= 36 && (inner >= 20 || x <= 14))
					put(voxels, x, x <= 13 ? 5 : 4, z, x <= 14 ? 12 : 11);
			}
	}

	/** Raise a compact three-tongued flame with copper shade and cream core. */
	static function addFlameRune(voxels:Array<Voxel>):Void {
		for (z in 11...24) {
			final halfWidth = z < 16 ? 5 : (z < 20 ? 3 : 1);
			final center = z > 19 ? 15 : 16;
			for (x in center - halfWidth...center + halfWidth + 1) {
				final edge = x == center - halfWidth || x == center + halfWidth;
				put(voxels, x, edge ? 5 : 4, z, edge ? 5 : (z < 16 ? 7 : 6));
			}
		}
		for (z in 12...17)
			for (x in 15...18)
				put(voxels, x, 3, z, 12);
		for (z in 17...22)
			put(voxels, 20, 4, z, z == 21 ? 7 : 6);
	}

	/** Write one model, or prove that its checked-in bytes match this source. */
	static function writeOrCheck(path:String, expected:Bytes, checkOnly:Bool):Void {
		if (!checkOnly) {
			File.saveBytes(path, expected);
			return;
		}
		if (!FileSystem.exists(path)) {
			Sys.println('missing generated voxel model: $path');
			Sys.exit(1);
		}
		final actual = File.getBytes(path);
		if (actual.compare(expected) != 0) {
			Sys.println('stale generated voxel model: $path');
			Sys.exit(1);
		}
	}

	/** Add a grounded parchment roll with shaded paper and visible end rings. */
	static function addFieldNoteRoll(voxels:Array<Voxel>):Void {
		for (x in 4...28)
			for (y in 8...24)
				for (z in 2...15) {
					final dy = y - 15;
					final dz = z - 8;
					if (dy * dy + dz * dz <= 42) {
						var color = z >= 11 ? 18 : 17;
						if (y <= 10 || z <= 3)
							color = 16;
						else if ((x + y + z) % 17 == 0)
							color = 19;
						put(voxels, x, y, z, color);
					}
				}

		for (x in [3, 4, 27, 28])
			for (y in 7...25)
				for (z in 1...16) {
					final dy = y - 15;
					final dz = z - 8;
					final radiusSquared = dy * dy + dz * dz;
					if (radiusSquared <= 56) {
						var color = radiusSquared >= 42 ? 16 : 17;
						if (radiusSquared <= 6 || (radiusSquared >= 18 && radiusSquared <= 25))
							color = 19;
						else if (z >= 10 && radiusSquared < 42)
							color = 18;
						put(voxels, x, y, z, color);
					}
				}
	}

	/** Add the teal travel binding, two loose tails, and a copper wax seal. */
	static function addFieldNoteBinding(voxels:Array<Voxel>):Void {
		for (x in 14...18)
			for (y in 8...24)
				for (z in 2...15) {
					final dy = y - 15;
					final dz = z - 8;
					if (dy * dy + dz * dz >= 33 && dy * dy + dz * dz <= 48)
						put(voxels, x, y, z, z >= 10 ? 10 : 9);
				}

		for (step in 0...8) {
			put(voxels, 14, 7 - step, 3, step % 3 == 0 ? 8 : 9);
			put(voxels, 15, 7 - step, 3, 9);
			put(voxels, 17, 7 - step, 3, step % 3 == 1 ? 8 : 10);
			put(voxels, 18, 7 - step, 3, 9);
		}

		for (x in 13...20)
			for (y in 5...9)
				for (z in 3...10) {
					final dx = x - 16;
					final dy = y - 6;
					final dz = z - 6;
					if (dx * dx + 2 * dy * dy + dz * dz <= 12)
						put(voxels, x, y, z, z >= 7 ? 7 : 6);
				}
		put(voxels, 16, 4, 6, 5);
		put(voxels, 16, 4, 7, 7);
	}

	/** Add the low timber platform and four iron-shod feet of the winch. */
	static function addWinchBase(voxels:Array<Voxel>):Void {
		fillChamferedLayer(voxels, 2, 29, 5, 27, 0, 3, 2);
		fillChamferedLayer(voxels, 3, 28, 6, 26, 1, 2, 12);
		fillChamferedLayer(voxels, 3, 28, 6, 26, 2, 2, 13);
		for (corner in [{x: 2, y: 5}, {x: 25, y: 5}, {x: 2, y: 23}, {x: 25, y: 23}]) {
			fill(voxels, corner.x, corner.x + 4, corner.y, corner.y + 4, 0, 4, 2);
			put(voxels, corner.x + 2, corner.y - 1, 2, 4);
		}
		for (x in 6...26)
			put(voxels, x, 5, 3, x % 4 == 0 ? 14 : 13);
	}

	/** Add two timber A-frame supports with dark iron caps and braces. */
	static function addWinchSupports(voxels:Array<Voxel>):Void {
		for (left in [true, false]) {
			final minimumX = left ? 4 : 23;
			final maximumX = left ? 8 : 27;
			for (z in 4...25) {
				final inset = z > 18 ? 1 : 0;
				fill(voxels, minimumX + inset, maximumX - inset, 10, 23, z, z, z % 5 == 0 ? 14 : 12);
			}
			fill(voxels, minimumX - 1, maximumX + 1, 9, 24, 22, 26, 2);
			fill(voxels, minimumX, maximumX, 8, 10, 7, 20, 3);
			put(voxels, left ? 5 : 26, 7, 14, 4);
		}
	}

	/** Add the large faceted timber drum and its two iron retaining bands. */
	static function addWinchDrum(voxels:Array<Voxel>):Void {
		for (x in 8...24)
			for (y in 7...24)
				for (z in 8...25) {
					final dy = y - 15;
					final dz = z - 16;
					if (dy * dy + dz * dz <= 64) {
						var color = (y + z) % 4 == 0 ? 14 : 13;
						if (x == 10 || x == 11 || x == 20 || x == 21)
							color = 3;
						put(voxels, x, y, z, color);
					}
				}
		fill(voxels, 5, 27, 14, 17, 15, 17, 2);
	}

	/** Add a hanging copper chain and a side crank with a wooden grip. */
	static function addWinchChainAndCrank(voxels:Array<Voxel>):Void {
		for (z in 3...23) {
			final sway = (z % 6 < 3) ? 0 : 1;
			for (x in [14 + sway, 17 - sway]) {
				put(voxels, x, 5, z, z % 4 < 2 ? 6 : 7);
				if (z % 4 == 0)
					put(voxels, x, 4, z, 7);
			}
		}

		fill(voxels, 28, 31, 13, 18, 14, 19, 5);
		for (step in 0...8) {
			put(voxels, 30, 13 - step, 14 - step, step % 3 == 0 ? 7 : 6);
			put(voxels, 31, 13 - step, 14 - step, 6);
		}
		fill(voxels, 28, 31, 3, 8, 5, 8, 13);
		fill(voxels, 29, 31, 2, 9, 6, 7, 14);
	}

	/** Add the broad, beveled foot that makes the relay read as one object. */
	static function addBase(voxels:Array<Voxel>):Void {
		for (corner in [{x: 3, y: 4}, {x: 23, y: 4}, {x: 3, y: 22}, {x: 23, y: 22}]) {
			fill(voxels, corner.x, corner.x + 5, corner.y, corner.y + 5, 0, 4, 1);
			fill(voxels, corner.x + 1, corner.x + 4, corner.y - 1, corner.y - 1, 1, 3, 3);
			put(voxels, corner.x + 2, corner.y - 2, 2, 4);
			put(voxels, corner.x + 3, corner.y - 2, 2, 4);
		}
		for (z in 3...7)
			fillChamferedLayer(voxels, 4, 27, 5, 26, z, z == 3 ? 3 : 2, z == 6 ? 3 : 2);
		outlineChamferedLayer(voxels, 4, 27, 5, 26, 6, 2, 4);
		for (x in [8, 12, 15, 19, 23])
			put(voxels, x, 4, 5, x == 15 ? 7 : 5);
	}

	/** Add the solid dark cabinet and its raised side rails. */
	static function addCabinet(voxels:Array<Voxel>):Void {
		fillChamferedLayer(voxels, 6, 25, 7, 24, 7, 2, 2);
		for (z in 8...19)
			fillChamferedLayer(voxels, 7, 24, 8, 23, z, 2, z % 3 == 0 ? 3 : 2);
		for (z in 19...22)
			fillChamferedLayer(voxels, 6, 25, 7, 24, z, 2, z == 20 ? 3 : 2);

		for (z in 8...20)
			for (x in [7, 8, 23, 24]) {
				put(voxels, x, 7, z, x < 10 ? 4 : 1);
				if (z == 10 || z == 17)
					put(voxels, x, 6, z, 5);
			}
		for (z in [10, 13, 16]) {
			put(voxels, 6, 15, z, 4);
			put(voxels, 25, 15, z, 1);
		}

		fill(voxels, 5, 10, 7, 23, 20, 22, 3);
		fill(voxels, 21, 26, 7, 23, 20, 22, 2);
		fill(voxels, 6, 10, 6, 11, 21, 23, 8);
		fill(voxels, 21, 25, 6, 11, 21, 23, 8);
		for (x in [7, 9, 22, 24])
			put(voxels, x, 5, 22, 9);
	}

	/** Add a thin copper-framed panel with the relay's teal key glyph. */
	static function addControlFace(voxels:Array<Voxel>):Void {
		fill(voxels, 8, 23, 6, 6, 9, 18, 1);
		fill(voxels, 9, 22, 5, 5, 10, 17, 2);
		for (x in 8...24) {
			put(voxels, x, 4, 9, x == 8 || x == 23 ? 5 : 6);
			put(voxels, x, 4, 18, x == 8 || x == 23 ? 5 : 7);
		}
		for (z in 10...18) {
			put(voxels, 8, 4, z, 6);
			put(voxels, 23, 4, z, 5);
		}
		for (point in [{x: 9, z: 10}, {x: 22, z: 10}, {x: 9, z: 17}, {x: 22, z: 17}])
			put(voxels, point.x, 3, point.z, 7);

		fill(voxels, 12, 15, 3, 3, 12, 15, 10);
		fill(voxels, 16, 20, 3, 3, 13, 14, 9);
		fill(voxels, 20, 22, 3, 3, 11, 16, 6);
		fill(voxels, 21, 22, 2, 2, 12, 15, 7);
		put(voxels, 13, 2, 13, 11);
		put(voxels, 14, 2, 14, 11);
	}

	/** Add the octagonal socket, four prongs, and a finely faceted crystal. */
	static function addCrystal(voxels:Array<Voxel>):Void {
		for (z in 21...24)
			fillChamferedLayer(voxels, 10, 21, 10, 21, z, 3, z == 22 ? 7 : 6);
		for (point in [{x: 10, y: 10}, {x: 21, y: 10}, {x: 10, y: 21}, {x: 21, y: 21}]) {
			put(voxels, point.x, point.y, 24, 6);
			put(voxels, point.x + (point.x < 15 ? 1 : -1), point.y + (point.y < 15 ? 1 : -1), 25, 7);
		}

		addCrystalLayer(voxels, 23, 3);
		addCrystalLayer(voxels, 24, 5);
		addCrystalLayer(voxels, 25, 7);
		addCrystalLayer(voxels, 26, 8);
		addCrystalLayer(voxels, 27, 8);
		addCrystalLayer(voxels, 28, 7);
		addCrystalLayer(voxels, 29, 5);
		addCrystalLayer(voxels, 30, 3);
		addCrystalLayer(voxels, 31, 1);
	}

	/** Fill one rectangular layer while cutting its four square corners. */
	static function fillChamferedLayer(voxels:Array<Voxel>, minimumX:Int, maximumX:Int, minimumY:Int, maximumY:Int, z:Int, cornerCut:Int, color:Int):Void {
		for (x in minimumX...maximumX + 1)
			for (y in minimumY...maximumY + 1) {
				final fromX = x - minimumX < maximumX - x ? x - minimumX : maximumX - x;
				final fromY = y - minimumY < maximumY - y ? y - minimumY : maximumY - y;
				if (fromX + fromY >= cornerCut)
					put(voxels, x, y, z, color);
			}
	}

	/** Recolor the exposed border of one chamfered layer. */
	static function outlineChamferedLayer(voxels:Array<Voxel>, minimumX:Int, maximumX:Int, minimumY:Int, maximumY:Int, z:Int, cornerCut:Int, color:Int):Void {
		for (x in minimumX...maximumX + 1)
			for (y in minimumY...maximumY + 1) {
				final fromX = x - minimumX < maximumX - x ? x - minimumX : maximumX - x;
				final fromY = y - minimumY < maximumY - y ? y - minimumY : maximumY - y;
				if (fromX + fromY >= cornerCut && (fromX == 0 || fromY == 0 || fromX + fromY == cornerCut))
					put(voxels, x, y, z, color);
			}
	}

	/** Add one centered crystal layer with baked side and highlight colors. */
	static function addCrystalLayer(voxels:Array<Voxel>, z:Int, radius:Int):Void {
		for (x in 7...25)
			for (y in 7...25)
				if (absolute(2 * x - 31) + absolute(2 * y - 31) <= radius * 2) {
					var layerColor = z >= 29 ? 10 : 9;
					if (x <= 13)
						layerColor = 8;
					else if (y <= 12 || (x + y + z) % 11 == 0)
						layerColor = 11;
					put(voxels, x, y, z, layerColor);
				}
	}

	/** Return a non-negative integer without depending on the target Math API. */
	static inline function absolute(value:Int):Int {
		return value < 0 ? -value : value;
	}

	/** Add one solid box of voxels with inclusive coordinates. */
	static function fill(voxels:Array<Voxel>, minimumX:Int, maximumX:Int, minimumY:Int, maximumY:Int, minimumZ:Int, maximumZ:Int, color:Int):Void {
		for (x in minimumX...maximumX + 1)
			for (y in minimumY...maximumY + 1)
				for (z in minimumZ...maximumZ + 1)
					put(voxels, x, y, z, color);
	}

	/** Add or recolor one voxel so later detail layers remain deterministic. */
	static function put(voxels:Array<Voxel>, x:Int, y:Int, z:Int, color:Int):Void {
		for (voxel in voxels)
			if (voxel.x == x && voxel.y == y && voxel.z == z) {
				voxel.color = color;
				return;
			}
		voxels.push({
			x: x,
			y: y,
			z: z,
			color: color
		});
	}

	/** Encode one MagicaVoxel 150 scene with a custom Caxecraft palette. */
	static function encode(voxels:Array<Voxel>, includeParchmentColors:Bool):Bytes {
		final children = new BytesOutput();
		writeChunk(children, "SIZE", sizeChunk(), Bytes.alloc(0));
		writeChunk(children, "XYZI", voxelChunk(voxels), Bytes.alloc(0));
		writeChunk(children, "RGBA", paletteChunk(includeParchmentColors), Bytes.alloc(0));
		final output = new BytesOutput();
		output.writeString("VOX ");
		output.writeInt32(150);
		writeChunk(output, "MAIN", Bytes.alloc(0), children.getBytes());
		return output.getBytes();
	}

	/** Encode the fixed 32-voxel asset volume. */
	static function sizeChunk():Bytes {
		final output = new BytesOutput();
		output.writeInt32(SIZE);
		output.writeInt32(SIZE);
		output.writeInt32(SIZE);
		return output.getBytes();
	}

	/** Encode each colored voxel as four one-byte values. */
	static function voxelChunk(voxels:Array<Voxel>):Bytes {
		final output = new BytesOutput();
		output.writeInt32(voxels.length);
		for (voxel in voxels) {
			output.writeByte(voxel.x);
			output.writeByte(voxel.y);
			output.writeByte(voxel.z);
			output.writeByte(voxel.color);
		}
		return output.getBytes();
	}

	/** Encode the shared prop colors, then fill unused palette slots. */
	static function paletteChunk(includeParchmentColors:Bool):Bytes {
		final colors:Array<Int> = [
			0x11171bff, 0x202a2fff, 0x303b40ff, 0x4a585dff, 0x59331fff, 0x8d512fff, 0xc17a43ff, 0x064d53ff, 0x08777aff, 0x16aaa7ff, 0x82e6deff, 0xc99d57ff,
			0x3b261aff, 0x65442cff, 0x8b633dff
		];
		if (includeParchmentColors) {
			colors.push(0x9b7849ff);
			colors.push(0xd8b878ff);
			colors.push(0xffe4a3ff);
			colors.push(0x6c5033ff);
		}
		final output = new BytesOutput();
		for (index in 0...255)
			writeColor(output, index < colors.length ? colors[index] : 0x00000000);
		writeColor(output, 0x00000000);
		return output.getBytes();
	}

	/** Write one RGBA value from a review-friendly hexadecimal integer. */
	static function writeColor(output:BytesOutput, rgba:Int):Void {
		output.writeByte((rgba >>> 24) & 0xff);
		output.writeByte((rgba >>> 16) & 0xff);
		output.writeByte((rgba >>> 8) & 0xff);
		output.writeByte(rgba & 0xff);
	}

	/** Write one standard VOX chunk and its exact byte counts. */
	static function writeChunk(output:BytesOutput, id:String, content:Bytes, children:Bytes):Void {
		output.writeString(id);
		output.writeInt32(content.length);
		output.writeInt32(children.length);
		output.write(content);
		output.write(children);
	}
}

/** One colored cell in the model volume. */
private typedef Voxel = {
	final x:Int;
	final y:Int;
	final z:Int;
	var color:Int;
}

/** Closed set of marks used by the shared rune-stone shape. */
private enum RuneGlyph {
	River;
	Leaf;
	Moon;
	Flame;
}
