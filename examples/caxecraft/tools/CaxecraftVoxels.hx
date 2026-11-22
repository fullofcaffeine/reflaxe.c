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
		final relayNames = ["forge-relay", "forge-relay-switching", "forge-relay-active"];
		for (pose in 0...relayNames.length) {
			final relay:Array<Voxel> = [];
			addBase(relay);
			addCabinet(relay);
			addControlFace(relay);
			addRelayLever(relay, pose);
			addCrystal(relay, pose);
			writeOrCheck('$modelDirectory/${relayNames[pose]}.vox', encode(relay, false), checkOnly);
			Sys.println('${relayNames[pose]}.vox: ${relay.length} voxels');
		}

		final winchNames = ["gate-winch", "gate-winch-turning", "gate-winch-active"];
		for (pose in 0...winchNames.length) {
			final winch:Array<Voxel> = [];
			addWinchBase(winch);
			addWinchSupports(winch);
			addWinchDrum(winch, pose);
			addWinchChainAndCrank(winch, pose);
			writeOrCheck('$modelDirectory/${winchNames[pose]}.vox', encode(winch, false), checkOnly);
			Sys.println('${winchNames[pose]}.vox: ${winch.length} voxels');
		}

		final noteNames = ["field-note", "field-note-opening", "field-note-open"];
		for (pose in 0...noteNames.length) {
			final fieldNote:Array<Voxel> = [];
			addFieldNotePose(fieldNote, pose);
			writeOrCheck('$modelDirectory/${noteNames[pose]}.vox', encode(fieldNote, true), checkOnly);
			Sys.println('${noteNames[pose]}.vox: ${fieldNote.length} voxels');
		}

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

			final lightingGlyph:Array<Voxel> = [];
			addRuneStone(lightingGlyph, glyphs[index]);
			addEnteringRuneLight(lightingGlyph);
			writeOrCheck('$modelDirectory/vault-glyph-${glyphNames[index]}-lighting.vox', encode(lightingGlyph, false), checkOnly);
			Sys.println('vault-glyph-${glyphNames[index]}.vox: ${waitingGlyph.length} waiting, ${lightingGlyph.length} lighting, ${enteredGlyph.length} active voxels');
		}

		final boundaryThicket:Array<Voxel> = [];
		addBoundaryThicket(boundaryThicket);
		writeOrCheck('$modelDirectory/boundary-thicket.vox', encode(boundaryThicket, false), checkOnly);
		Sys.println('boundary-thicket.vox: ${boundaryThicket.length} voxels');

		final boundaryRoot:Array<Voxel> = [];
		addBoundaryRoot(boundaryRoot);
		writeOrCheck('$modelDirectory/boundary-root.vox', encode(boundaryRoot, false), checkOnly);
		Sys.println('boundary-root.vox: ${boundaryRoot.length} voxels');

		final boundaryBoulder:Array<Voxel> = [];
		addBoundaryBoulder(boundaryBoulder);
		writeOrCheck('$modelDirectory/boundary-boulder.vox', encode(boundaryBoulder, false), checkOnly);
		Sys.println('boundary-boulder.vox: ${boundaryBoulder.length} voxels');
	}

	/**
	 * Build a deep, tangled thicket that reads as vegetation from every side.
	 *
	 * Overlapping leaf masses hide the finite-map edge, while visible trunks,
	 * roots, and small gaps keep the solid collision volume understandable.
	 */
	static function addBoundaryThicket(voxels:Array<Voxel>):Void {
		for (trunk in [
			{
				x: 7,
				y: 11,
				leanX: 1,
				leanY: 0,
				height: 23
			},
			{
				x: 18,
				y: 8,
				leanX: -1,
				leanY: 1,
				height: 28
			},
			{
				x: 24,
				y: 20,
				leanX: 0,
				leanY: -1,
				height: 25
			}
		])
			for (z in 0...trunk.height) {
				final x = trunk.x + Std.int(z / 9) * trunk.leanX;
				final y = trunk.y + Std.int(z / 10) * trunk.leanY;
				fill(voxels, x - 1, x + 1, y - 1, y + 1, z, z, z % 5 == 0 ? 15 : 14);
			}

		for (step in 0...12) {
			fill(voxels, 3 + step, 5 + step, 3 + step, 5 + step, Std.int(step / 3), Std.int(step / 3) + 1, step % 4 == 0 ? 13 : 14);
			fill(voxels, 27 - step, 29 - step, 4 + step, 6 + step, Std.int(step / 4), Std.int(step / 4) + 1, step % 3 == 0 ? 15 : 13);
		}

		addLeafMass(voxels, 8, 10, 18, 8, 8, 10);
		addLeafMass(voxels, 18, 9, 22, 10, 8, 9);
		addLeafMass(voxels, 23, 20, 19, 8, 10, 9);
		addLeafMass(voxels, 12, 23, 22, 9, 8, 8);
		addLeafMass(voxels, 18, 17, 27, 11, 10, 5);
	}

	/** Add one irregular ellipsoid of layered teal leaves with bounded openings. */
	static function addLeafMass(voxels:Array<Voxel>, centerX:Int, centerY:Int, centerZ:Int, radiusX:Int, radiusY:Int, radiusZ:Int):Void {
		for (x in centerX - radiusX...centerX + radiusX + 1)
			for (y in centerY - radiusY...centerY + radiusY + 1)
				for (z in centerZ - radiusZ...centerZ + radiusZ + 1) {
					if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE)
						continue;
					final dx = x - centerX;
					final dy = y - centerY;
					final dz = z - centerZ;
					final inside = dx * dx * radiusY * radiusY * radiusZ * radiusZ
						+ dy * dy * radiusX * radiusX * radiusZ * radiusZ
						+ dz * dz * radiusX * radiusX * radiusY * radiusY <= radiusX * radiusX * radiusY * radiusY * radiusZ * radiusZ;
					if (inside && ((x * 3 + y * 5 + z * 7) % 29 != 0 || z < centerZ)) {
						final shade = (x + 2 * y + z) % 11;
						put(voxels, x, y, z, shade < 3 ? 8 : shade < 8 ? 9 : shade < 10 ? 10 : 11);
					}
				}
	}

	/**
	 * Build a grounded root fan with crossing arms and an asymmetric stump.
	 *
	 * The broad roots explain the solid footprint from play height. Copper and
	 * teal surface patches distinguish bark, cut wood, and moss without a sprite.
	 */
	static function addBoundaryRoot(voxels:Array<Voxel>):Void {
		for (z in 0...22) {
			final radius = z < 7 ? 7 : z < 15 ? 5 : 3;
			for (x in 16 - radius...17 + radius)
				for (y in 15 - radius...16 + radius)
					if (absolute(x - 16) + absolute(y - 15) <= radius + 2)
						put(voxels, x, y, z, (x + y + z) % 7 == 0 ? 15 : (x < 16 ? 13 : 14));
		}
		for (step in 0...13) {
			final height = 6 - Std.int(step / 3);
			fill(voxels, 13 - step, 18 - step, 12 - Std.int(step / 3), 17 - Std.int(step / 3), 0, height, step % 4 == 0 ? 15 : 13);
			fill(voxels, 14 + step, 19 + step, 14 + Std.int(step / 4), 19 + Std.int(step / 4), 0, height, step % 5 == 0 ? 15 : 14);
		}
		for (step in 0...12) {
			final height = 5 - Std.int(step / 3);
			fill(voxels, 13 - Std.int(step / 3), 18 - Std.int(step / 3), 12 + step, 17 + step, 0, height, step % 3 == 0 ? 15 : 14);
			fill(voxels, 16 + Std.int(step / 4), 21 + Std.int(step / 4), 12 - step, 17 - step, 0, height, step % 4 == 0 ? 15 : 13);
		}
		for (point in [
			 {x: 5, y: 8, z: 4},  {x: 25, y: 20, z: 3}, {x: 12, y: 25, z: 4},
			{x: 20, y: 8, z: 5}, {x: 13, y: 14, z: 18}, {x: 18, y: 17, z: 20}
		]) {
			fill(voxels, point.x, point.x + 2, point.y, point.y + 2, point.z, point.z + 1, 8);
			put(voxels, point.x + 1, point.y + 1, point.z + 2, 9);
		}
	}

	/**
	 * Build an irregular two-part boulder with chamfered faces and moss patches.
	 *
	 * Its layered outline gives the renderer a different silhouette from every
	 * quarter turn, while the broad base keeps collision visually predictable.
	 */
	static function addBoundaryBoulder(voxels:Array<Voxel>):Void {
		for (z in 0...20) {
			final inset = z < 3 ? 2 : z < 12 ? 0 : Std.int((z - 10) / 2);
			fillChamferedLayer(voxels, 2 + inset, 29 - inset, 4 + inset, 27 - inset, z, 4, z % 6 == 0 ? 4 : z % 3 == 0 ? 3 : 2);
		}
		for (z in 15...30) {
			final distance = absolute(z - 22);
			final inset = Std.int(distance / 2);
			fillChamferedLayer(voxels, 12 + inset, 29 - inset, 7 + inset, 24 - inset, z, 3, (z + inset) % 4 == 0 ? 4 : 3);
		}
		for (point in [
			{x: 5, y: 7, z: 15},
			{x: 8, y: 22, z: 17},
			{x: 17, y: 7, z: 25},
			{x: 23, y: 12, z: 27},
			{x: 25, y: 22, z: 18}
		]) {
			fill(voxels, point.x, point.x + 3, point.y, point.y + 2, point.z, point.z + 1, 8);
			fill(voxels, point.x + 1, point.x + 4, point.y + 1, point.y + 3, point.z + 2, point.z + 2, 9);
		}
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

	/** Add a partial light sweep used between the carved and fully lit poses. */
	static function addEnteringRuneLight(voxels:Array<Voxel>):Void {
		for (x in 9...17) {
			put(voxels, x, 5, 10, x % 4 == 0 ? 12 : 11);
			put(voxels, x, 5, 25, x % 4 == 0 ? 12 : 11);
		}
		for (z in 11...19)
			put(voxels, 9, 5, z, z % 4 == 0 ? 12 : 11);
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

	/** Build closed, opening, or fully open poses for the readable field note. */
	static function addFieldNotePose(voxels:Array<Voxel>, pose:Int):Void {
		if (pose == 0) {
			addFieldNoteRoll(voxels);
			addFieldNoteBinding(voxels);
			return;
		}

		final maximumY = pose == 1 ? 18 : 26;
		for (x in 5...28)
			for (y in 5...maximumY)
				for (z in 2...5) {
					var color = z == 4 ? 17 : 16;
					if ((x + y) % 13 == 0)
						color = 18;
					put(voxels, x, y, z, color);
				}
		for (x in 4...29)
			for (z in 2...7) {
				put(voxels, x, 4, z, z >= 5 ? 18 : 16);
				put(voxels, x, maximumY, z, z >= 5 ? 18 : 16);
			}
		for (x in 14...18)
			for (y in 5...maximumY)
				put(voxels, x, y, 5, pose == 1 ? 9 : 10);
		for (point in [{x: 8, y: 10}, {x: 23, y: 10}, {x: 8, y: maximumY - 5}, {x: 23, y: maximumY - 5}])
			put(voxels, point.x, point.y, 5, 19);
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
	static function addWinchDrum(voxels:Array<Voxel>, pose:Int):Void {
		for (x in 8...24)
			for (y in 7...24)
				for (z in 8...25) {
					final dy = y - 15;
					final dz = z - 16;
					if (dy * dy + dz * dz <= 64) {
						var color = (y + z + pose * 2) % 4 == 0 ? 14 : 13;
						if (x == 10 || x == 11 || x == 20 || x == 21)
							color = 3;
						put(voxels, x, y, z, color);
					}
				}
		fill(voxels, 5, 27, 14, 17, 15, 17, 2);
	}

	/** Add a hanging copper chain and a side crank with a wooden grip. */
	static function addWinchChainAndCrank(voxels:Array<Voxel>, pose:Int):Void {
		for (z in 3...23) {
			final sway = ((z + pose * 2) % 6 < 3) ? 0 : 1;
			for (x in [14 + sway, 17 - sway]) {
				put(voxels, x, 5, z, z % 4 < 2 ? 6 : 7);
				if (z % 4 == 0)
					put(voxels, x, 4, z, 7);
			}
		}

		fill(voxels, 28, 31, 13, 18, 14, 19, 5);
		for (step in 0...8) {
			final handleY = pose == 0 ? 13 - step : pose == 1 ? 10 : 7 + step;
			final handleZ = pose == 1 ? 10 - step : 14 - step;
			put(voxels, 30, handleY, handleZ, step % 3 == 0 ? 7 : 6);
			put(voxels, 31, handleY, handleZ, 6);
		}
		final gripY = pose == 0 ? 3 : pose == 1 ? 8 : 14;
		final gripZ = pose == 1 ? 2 : 5;
		fill(voxels, 28, 31, gripY, gripY + 5, gripZ, gripZ + 3, 13);
		fill(voxels, 29, 31, gripY - 1, gripY + 6, gripZ + 1, gripZ + 2, 14);
		if (pose == 2)
			for (point in [{x: 6, y: 8, z: 25}, {x: 25, y: 8, z: 25}, {x: 15, y: 6, z: 24}])
				put(voxels, point.x, point.y, point.z, 11);
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

	/** Add a three-pose copper lever whose silhouette clearly shows activation. */
	static function addRelayLever(voxels:Array<Voxel>, pose:Int):Void {
		fill(voxels, 13, 18, 1, 3, 11, 15, 3);
		for (step in 0...9) {
			final x = pose == 1 ? 12 + step : pose == 0 ? 16 - Std.int(step / 3) : 16 + Std.int(step / 3);
			final z = pose == 1 ? 15 + Std.int(step / 2) : pose == 0 ? 15 + step : 15 - step;
			put(voxels, x, 0, z, step < 6 ? 6 : 7);
			put(voxels, x, 1, z, step < 6 ? 6 : 7);
		}
		final tipZ = pose == 1 ? 19 : pose == 0 ? 23 : 7;
		final tipX = pose == 1 ? 20 : pose == 0 ? 14 : 18;
		fill(voxels, tipX - 1, tipX + 1, 0, 2, tipZ - 1, tipZ + 1, pose == 2 ? 11 : 7);
	}

	/** Add the octagonal socket, four prongs, and a finely faceted crystal. */
	static function addCrystal(voxels:Array<Voxel>, pose:Int):Void {
		for (z in 21...24)
			fillChamferedLayer(voxels, 10, 21, 10, 21, z, 3, z == 22 ? 7 : 6);
		for (point in [{x: 10, y: 10}, {x: 21, y: 10}, {x: 10, y: 21}, {x: 21, y: 21}]) {
			put(voxels, point.x, point.y, 24, 6);
			put(voxels, point.x + (point.x < 15 ? 1 : -1), point.y + (point.y < 15 ? 1 : -1), 25, 7);
		}

		addCrystalLayer(voxels, 23, 3, pose);
		addCrystalLayer(voxels, 24, 5, pose);
		addCrystalLayer(voxels, 25, 7, pose);
		addCrystalLayer(voxels, 26, 8, pose);
		addCrystalLayer(voxels, 27, 8, pose);
		addCrystalLayer(voxels, 28, 7, pose);
		addCrystalLayer(voxels, 29, 5, pose);
		addCrystalLayer(voxels, 30, 3, pose);
		addCrystalLayer(voxels, 31, 1, pose);
		if (pose == 2)
			for (point in [
				{x: 6, y: 15, z: 27},
				{x: 25, y: 15, z: 27},
				{x: 15, y: 6, z: 27},
				{x: 15, y: 25, z: 27}
			])
				put(voxels, point.x, point.y, point.z, 11);
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
	static function addCrystalLayer(voxels:Array<Voxel>, z:Int, radius:Int, pose:Int):Void {
		for (x in 7...25)
			for (y in 7...25)
				if (absolute(2 * x - 31) + absolute(2 * y - 31) <= radius * 2) {
					var layerColor = pose == 0 ? 8 : pose == 1 ? 9 : 10;
					if (x <= 13 && pose < 2)
						layerColor = pose == 0 ? 8 : 9;
					else if (y <= 12 || (x + y + z) % 11 == 0)
						layerColor = pose == 2 ? 11 : 10;
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
