package caxecraft.tool;

import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.domain.World;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import caxecraft.scenario.ScenarioWorld.ScenarioFluidPlacement;
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import sys.io.File;

/** One compact terrain run after validated chunks are placed in world order. */
private typedef GeneratedRun = {
	final code:Int;
	final count:Int;
}

/** One source or finite initial volume in deterministic runtime-load order. */
private typedef GeneratedFluid = {
	final id:String;
	final kind:Int;
	final x:Int;
	final y:Int;
	final z:Int;
	final width:Int;
	final height:Int;
	final depth:Int;
	final presentationCell:Int;
}

/** One validated map item resolved to the pack-local code used by native C. */
private typedef GeneratedItem = {
	final id:String;
	final storageCode:Int;
	final quantity:Int;
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
	final yawDegrees:Int;
}

/**
	Generate the native first-playable adapter from the real validated CAXEMAP.

	This Eval-only build tool is the semantic owner of the generated data. It
	uses the same lexer, parser, validator, and content registry as the editor,
	then prints ordinary typed Haxe. The Python wrapper only compares or writes
	those bytes; it does not parse terrain, fluids, or object placement.
**/
function main():Void {
	final source = File.getBytes("scenarios/first-playable/map.caxemap");
	final scenario = readScenario(source);
	if (scenario.id.text() != "adventure.first-playable")
		fail("expected the adventure.first-playable map");
	if (scenario.world.size.width != World.WIDTH || scenario.world.size.height != World.HEIGHT || scenario.world.size.depth != World.DEPTH)
		fail("first-playable world dimensions must match the current fixed native world");
	final registry = new BaseContentRegistry();
	final runs = terrainRuns(scenario, registry);
	final fluids = generatedFluids(scenario, registry);
	final items = generatedItems(scenario, registry);
	final spawn = playerSpawn(scenario);
	Sys.print(render(Sha256.make(source).toHex(), runs, fluids, items, spawn.xMilli, spawn.yMilli, spawn.zMilli, spawn.yawDegrees));
}

/** Parse and validate one complete candidate before reading any generated fact. */
private function readScenario(source:Bytes):Scenario {
	final records = switch ScenarioLexer.read(source) {
		case ReadOk(value): value;
		case ReadError(_): fail("first-playable CAXEMAP did not lex");
	};
	final parsed = switch ScenarioParser.parse(records) {
		case ReadOk(value): value;
		case ReadError(_): fail("first-playable CAXEMAP did not parse");
	};
	return switch ScenarioValidator.validate(parsed, new BaseContentRegistry()) {
		case ReadOk(value): value;
		case ReadError(_): fail("first-playable CAXEMAP did not validate against the built-in content pack");
	};
}

/** Resolve chunk-local RLE into one complete ascending-index terrain stream. */
private function terrainRuns(scenario:Scenario, registry:BaseContentRegistry):Array<GeneratedRun> {
	final palette:Map<Int, Int> = [];
	for (entry in scenario.world.palette) {
		final code = registry.blockStorageCode(entry.blockType);
		if (code < 0)
			fail('palette ${entry.code} did not resolve to compact block storage');
		palette.set(entry.code, code);
	}
	final cells = [for (_ in 0...World.VOLUME) -1];
	for (chunk in scenario.world.chunks) {
		var localIndex = 0;
		for (run in chunk.runs) {
			final code = palette.get(run.paletteCode);
			if (code == null)
				fail('chunk ${chunk.id.text()} used an unresolved palette code');
			var remaining = run.count;
			while (remaining > 0) {
				final localX = localIndex % chunk.size.width;
				final localY = Std.int(localIndex / chunk.size.width) % chunk.size.height;
				final localZ = Std.int(localIndex / (chunk.size.width * chunk.size.height));
				final worldIndex = chunk.origin.x + localX + World.WIDTH * (chunk.origin.y + localY + World.HEIGHT * (chunk.origin.z + localZ));
				cells[worldIndex] = code;
				localIndex++;
				remaining--;
			}
		}
	}
	for (code in cells)
		if (code < 0)
			fail("validated chunk coverage did not produce a complete native world");

	final result:Array<GeneratedRun> = [];
	var current = cells[0];
	var count = 1;
	var index = 1;
	while (index < cells.length) {
		if (cells[index] == current) {
			count++;
		} else {
			result.push({code: current, count: count});
			current = cells[index];
			count = 1;
		}
		index++;
	}
	result.push({code: current, count: count});
	return result;
}

/** Put finite volumes before sources, then use stable IDs within each kind. */
private function generatedFluids(scenario:Scenario, registry:BaseContentRegistry):Array<GeneratedFluid> {
	final result = [
		for (fluid in scenario.world.fluids)
			switch fluid.placement {
				case InitialVolume(bounds):
					{
						id: fluid.id.text(),
						kind: 0,
						x: bounds.origin.x,
						y: bounds.origin.y,
						z: bounds.origin.z,
						width: bounds.size.width,
						height: bounds.size.height,
						depth: bounds.size.depth,
						presentationCell: fluidPresentationCell(registry, fluid.fluidType)
					};
				case Source(point):
					{
						id: fluid.id.text(),
						kind: 1,
						x: point.x,
						y: point.y,
						z: point.z,
						width: 1,
						height: 1,
						depth: 1,
						presentationCell: fluidPresentationCell(registry, fluid.fluidType)
					};
			}
	];
	result.sort((left, right) -> left.kind == right.kind ? compareText(left.id, right.id) : left.kind - right.kind);
	return result;
}

/** Resolve one fluid visual through the same generated pack used by validation. */
private function fluidPresentationCell(registry:BaseContentRegistry, id:caxecraft.scenario.ContentId):Int {
	final cell = registry.fluidPresentationCell(id);
	if (cell < 0)
		fail('fluid ${id.text()} omitted its reviewed presentation cell');
	return cell;
}

/** Resolve item placements without teaching the adapter any campaign item name. */
private function generatedItems(scenario:Scenario, registry:BaseContentRegistry):Array<GeneratedItem> {
	final result:Array<GeneratedItem> = [];
	for (object in scenario.objects)
		switch object.placement {
			case Item(itemType, quantity, transform):
				final storageCode = registry.itemStorageCode(itemType);
				if (storageCode < 0)
					fail('item ${object.id.text()} did not resolve to compact pack storage');
				result.push({
					id: object.id.text(),
					storageCode: storageCode,
					quantity: quantity,
					xMilli: transform.xMilli,
					yMilli: transform.yMilli,
					zMilli: transform.zMilli,
					yawDegrees: transform.yawDegrees
				});
			case PlayerSpawn(_) | Checkpoint(_) | Entity(_, _) | Npc(_, _, _) | Prefab(_, _) | TriggerZone(_) | StatefulObject(_, _, _):
		}
	result.sort((left, right) -> compareText(left.id, right.id));
	return result;
}

/** Extract the validator-proven single player spawn as integer milliblocks. */
private function playerSpawn(scenario:Scenario):caxecraft.scenario.ScenarioGeometry.ScenarioTransform {
	for (object in scenario.objects)
		switch object.placement {
			case PlayerSpawn(position):
				return position;
			case Checkpoint(_) | Item(_, _, _) | Entity(_, _) | Npc(_, _, _) | Prefab(_, _) | TriggerZone(_) | StatefulObject(_, _, _):
		}
	return fail("validated first-playable omitted its player spawn");
}

/** Render formatter-stable ordinary Haxe with no copied parser/runtime policy. */
private function render(sourceHash:String, runs:Array<GeneratedRun>, fluids:Array<GeneratedFluid>, items:Array<GeneratedItem>, spawnX:Int, spawnY:Int,
		spawnZ:Int, spawnYaw:Int):String {
	final lines = [
		"package caxecraft.content;",
		"",
		"import caxecraft.domain.GameSession;",
		"import caxecraft.domain.World;",
		"",
		"/**",
		" * Native level facts generated from the validated first-playable CAXEMAP.",
		" *",
		" * The checked-in map is the editable source of truth. Regenerate this typed",
		" * adapter with `python3 examples/caxecraft/level_adapter.py`; do not hand-edit",
		" * terrain runs, fluid/item placement, or spawn values here.",
		" *",
		" * This temporary adapter proves that validated authored data can drive the",
		" * native game while file and text parsing support is still incomplete. Bead",
		" * `haxe_c-xge.39` owns replacing it with runtime CAXEMAP loading; new gameplay",
		" * content must not make this generated module a permanent application API.",
		" */",
		'inline final SOURCE_SHA256:String = "$sourceHash";',
		"",
		"inline final FLUID_INITIAL_VOLUME:Int = 0;",
		"inline final FLUID_SOURCE:Int = 1;",
		"",
		"/** Fill an unpublished session from canonical ascending-index terrain runs. */",
		"function loadTerrain(session:GameSession):Bool {",
		"\tvar index = 0;"
	];
	for (run in runs)
		lines.push('\tindex = writeRun(session, index, ${run.code}, ${run.count});');
	appendLines(lines, [
		"\treturn index == World.VOLUME;",
		"}",
		"",
		"/** Number of validated authored fluid declarations. */",
		"inline function fluidCount():Int"
	]);
	appendLines(lines, ['\treturn ${fluids.length};', ""]);
	appendFluidFunction(lines, "Kind", "kind", fluids, value -> value.kind);
	appendFluidFunction(lines, "X", "x coordinate", fluids, value -> value.x);
	appendFluidFunction(lines, "Y", "y coordinate", fluids, value -> value.y);
	appendFluidFunction(lines, "Z", "z coordinate", fluids, value -> value.z);
	appendFluidFunction(lines, "Width", "width", fluids, value -> value.width);
	appendFluidFunction(lines, "Height", "height", fluids, value -> value.height);
	appendFluidFunction(lines, "Depth", "depth", fluids, value -> value.depth);
	appendFluidFunction(lines, "PresentationCell", "presentation atlas cell", fluids, value -> value.presentationCell);
	appendLines(lines, [
		"/** Number of validated authored item placements. */",
		"inline function itemCount():Int"
	]);
	appendLines(lines, ['\treturn ${items.length};', ""]);
	appendLines(lines, [
		"/** Positive fixed-array capacity, including for a level with no items. */",
		"inline function itemStorageCapacity():Int"
	]);
	appendLines(lines, ['\treturn ${items.length > 0 ? items.length : 1};', ""]);
	appendItemFunction(lines, "StorageCode", "pack-local storage code", items, value -> value.storageCode);
	appendItemFunction(lines, "Quantity", "quantity", items, value -> value.quantity);
	appendItemFunction(lines, "XMilli", "x coordinate in milliblocks", items, value -> value.xMilli);
	appendItemFunction(lines, "YMilli", "y coordinate in milliblocks", items, value -> value.yMilli);
	appendItemFunction(lines, "ZMilli", "z coordinate in milliblocks", items, value -> value.zMilli);
	appendItemFunction(lines, "YawDegrees", "yaw in whole degrees", items, value -> value.yawDegrees);
	appendLines(lines, [
		"/** Authored player spawn x coordinate in integer milliblocks. */",
		"inline function spawnXMilli():Int",
		'\treturn $spawnX;',
		"",
		"/** Authored player spawn y coordinate in integer milliblocks. */",
		"inline function spawnYMilli():Int",
		'\treturn $spawnY;',
		"",
		"/** Authored player spawn z coordinate in integer milliblocks. */",
		"inline function spawnZMilli():Int",
		'\treturn $spawnZ;',
		"",
		"/** Authored player spawn yaw in whole degrees. */",
		"inline function spawnYawDegrees():Int",
		'\treturn $spawnYaw;',
		"",
		"/** Write one already validated compact run and return the next index. */",
		"private function writeRun(session:GameSession, start:Int, code:Int, count:Int):Int {",
		"\tif (start < 0)",
		"\t\treturn -1;",
		"\treturn session.writeTerrainRunDuringLoad(start, code, count);",
		"}",
		""
	]);
	return lines.join("\n");
}

/** Add one primitive indexed item projection without reflective field access. */
private function appendItemFunction(lines:Array<String>, suffix:String, label:String, items:Array<GeneratedItem>, read:GeneratedItem->Int):Void {
	appendLines(lines, [
		'/** Return item $label for one index, or `-1` outside the generated set. */',
		'function item$suffix(index:Int):Int {'
	]);
	for (index in 0...items.length)
		appendLines(lines, ['\tif (index == $index)', '\t\treturn ${read(items[index])};']);
	appendLines(lines, ["\treturn -1;", "}", ""]);
}

/** Add one primitive indexed fluid projection without reflective field access. */
private function appendFluidFunction(lines:Array<String>, suffix:String, label:String, fluids:Array<GeneratedFluid>, read:GeneratedFluid->Int):Void {
	appendLines(lines, [
		'/** Return fluid $label for one index, or `-1` outside the generated set. */',
		'function fluid$suffix(index:Int):Int {'
	]);
	for (index in 0...fluids.length)
		appendLines(lines, ['\tif (index == $index)', '\t\treturn ${read(fluids[index])};']);
	appendLines(lines, ["\treturn -1;", "}", ""]);
}

/** Append generated source lines while keeping `Array.push` target-neutral. */
private function appendLines(target:Array<String>, values:Array<String>):Void {
	for (value in values)
		target.push(value);
}

/** Compare the format's lowercase ASCII IDs without locale dependence. */
private function compareText(left:String, right:String):Int {
	if (left < right)
		return -1;
	return left > right ? 1 : 0;
}

/** Stop generation immediately instead of emitting a plausible stale adapter. */
private function fail<T>(message:String):T
	throw message;
