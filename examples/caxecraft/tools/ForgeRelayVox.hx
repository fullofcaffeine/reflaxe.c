package;

import haxe.io.Bytes;
import haxe.io.BytesOutput;
import sys.io.File;

/**
 * Builds Caxecraft's first two castle mechanisms in MagicaVoxel format.
 *
 * The checked-in `.vox` file is the game asset. This small source file keeps
 * its shape and palette reviewable without adding a second modeling format.
 */
class ForgeRelayVox {
	static inline final SIZE = 32;

	/** Build the relay and winch at the two requested output paths. */
	static function main():Void {
		final arguments = Sys.args();
		if (arguments.length != 2) {
			Sys.println("usage: haxe --run ForgeRelayVox <relay.vox> <winch.vox>");
			Sys.exit(2);
		}
		final relay:Array<Voxel> = [];
		addBase(relay);
		addCabinet(relay);
		addControlFace(relay);
		addCrystal(relay);
		File.saveBytes(arguments[0], encode(relay));

		final winch:Array<Voxel> = [];
		addWinchBase(winch);
		addWinchSupports(winch);
		addWinchDrum(winch);
		addWinchChainAndCrank(winch);
		File.saveBytes(arguments[1], encode(winch));
		Sys.println('forge-relay.vox: ${relay.length} voxels');
		Sys.println('gate-winch.vox: ${winch.length} voxels');
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
	static function encode(voxels:Array<Voxel>):Bytes {
		final children = new BytesOutput();
		writeChunk(children, "SIZE", sizeChunk(), Bytes.alloc(0));
		writeChunk(children, "XYZI", voxelChunk(voxels), Bytes.alloc(0));
		writeChunk(children, "RGBA", paletteChunk(), Bytes.alloc(0));
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

	/** Encode the shared mechanism colors, then fill unused palette slots. */
	static function paletteChunk():Bytes {
		final colors = [
			0x11171bff, 0x202a2fff, 0x303b40ff, 0x4a585dff, 0x59331fff, 0x8d512fff, 0xc17a43ff, 0x064d53ff, 0x08777aff, 0x16aaa7ff, 0x82e6deff, 0xc99d57ff,
			0x3b261aff, 0x65442cff, 0x8b633dff
		];
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
