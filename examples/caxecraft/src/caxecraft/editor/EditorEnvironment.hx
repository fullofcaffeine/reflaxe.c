package caxecraft.editor;

import caxecraft.scenario.ScenarioEnvironment;
import caxecraft.scenario.ScenarioEnvironment.ScenarioCloudLayer;
import caxecraft.scenario.ScenarioEnvironment.ScenarioEnvironmentProfile;
import caxecraft.scenario.ScenarioEnvironment.ScenarioHorizonEdge;
import caxecraft.scenario.ScenarioEnvironment.ScenarioRgb;
import caxecraft.scenario.ScenarioEnvironment.ScenarioSun;

/**
	Edits the optional CAXEMAP environment without replacing unrelated fields.

	The visual panel selects one closed control and one direction. This module
	then copies the current typed value and changes only that field. An absent
	environment stays absent until the creator enables it or changes a field.
	All returned values obey the same bounds as the CAXEMAP parser.
**/
enum abstract EditorEnvironmentControl(Int) {
	var Enabled = 0;
	var SkyRed = 1;
	var SkyGreen = 2;
	var SkyBlue = 3;
	var SunEnabled = 4;
	var SunX = 5;
	var SunY = 6;
	var SunZ = 7;
	var SunRadius = 8;
	var CloudCount = 9;
	var CloudSpeed = 10;
	var CloudSeed = 11;
	var NorthEdge = 12;
	var SouthEdge = 13;
	var EastEdge = 14;
	var WestEdge = 15;
	var ContinueWater = 16;
	var Done = 17;
}

/** A decrement or increment requested by pointer, keyboard, or controller. */
enum abstract EditorEnvironmentDirection(Int) {
	var Decrease = 0;
	var Increase = 1;
}

/** Begin environment-panel navigation at the explicit enabled state. */
function firstEnvironmentControl():EditorEnvironmentControl
	return Enabled;

/** Move through every visible environment control and wrap at either end. */
function moveEnvironmentControl(current:EditorEnvironmentControl, direction:EditorEnvironmentDirection):EditorEnvironmentControl {
	return switch direction {
		case Increase:
			switch current {
				case Enabled: SkyRed;
				case SkyRed: SkyGreen;
				case SkyGreen: SkyBlue;
				case SkyBlue: SunEnabled;
				case SunEnabled: SunX;
				case SunX: SunY;
				case SunY: SunZ;
				case SunZ: SunRadius;
				case SunRadius: CloudCount;
				case CloudCount: CloudSpeed;
				case CloudSpeed: CloudSeed;
				case CloudSeed: NorthEdge;
				case NorthEdge: SouthEdge;
				case SouthEdge: EastEdge;
				case EastEdge: WestEdge;
				case WestEdge: ContinueWater;
				case ContinueWater: Done;
				case Done: Enabled;
			}
		case Decrease:
			switch current {
				case Enabled: Done;
				case SkyRed: Enabled;
				case SkyGreen: SkyRed;
				case SkyBlue: SkyGreen;
				case SunEnabled: SkyBlue;
				case SunX: SunEnabled;
				case SunY: SunX;
				case SunZ: SunY;
				case SunRadius: SunZ;
				case CloudCount: SunRadius;
				case CloudSpeed: CloudCount;
				case CloudSeed: CloudSpeed;
				case NorthEdge: CloudSeed;
				case SouthEdge: NorthEdge;
				case EastEdge: SouthEdge;
				case WestEdge: EastEdge;
				case ContinueWater: WestEdge;
				case Done: ContinueWater;
			}
	};
}

/**
	Change one field and retain every other authored environment choice.

	`Decrease` disables Boolean choices and `Increase` enables them. Numeric
	choices move by one exact source unit and saturate at their parser bounds.
	Changing a field while the environment is absent starts from the generic
	editor default. `Done` never changes the document.
**/
function editEnvironment(current:Null<ScenarioEnvironment>, control:EditorEnvironmentControl, direction:EditorEnvironmentDirection):Null<ScenarioEnvironment> {
	if (control == Done)
		return current == null ? null : copyEnvironment(current);
	if (control == Enabled && direction == Decrease)
		return null;
	var base = defaultEnvironment();
	if (current != null)
		base = copyEnvironment(current);
	if (control == Enabled)
		return base;

	var sky = copyRgb(base.sky);
	var sun:Null<ScenarioSun> = copySun(base.sun);
	var clouds = copyClouds(base.clouds);
	var edges = base.edges.copy();
	var continueWater = base.continueWater;
	final amount = direction == Increase ? 1 : -1;
	switch control {
		case Enabled | Done:
		case SkyRed:
			sky = {red: boundedAdd(sky.red, amount, 0, 255), green: sky.green, blue: sky.blue};
		case SkyGreen:
			sky = {red: sky.red, green: boundedAdd(sky.green, amount, 0, 255), blue: sky.blue};
		case SkyBlue:
			sky = {red: sky.red, green: sky.green, blue: boundedAdd(sky.blue, amount, 0, 255)};
		case SunEnabled:
			if (direction == Decrease)
				sun = null;
			else if (sun == null)
				sun = defaultSun();
		case SunX:
			final value = sunOrDefault(sun);
			sun = {
				x: safeIntAdd(value.x, amount),
				y: value.y,
				z: value.z,
				radiusMilli: value.radiusMilli
			};
		case SunY:
			final value = sunOrDefault(sun);
			sun = {
				x: value.x,
				y: boundedAdd(value.y, amount, 1, 2147483647),
				z: value.z,
				radiusMilli: value.radiusMilli
			};
		case SunZ:
			final value = sunOrDefault(sun);
			sun = {
				x: value.x,
				y: value.y,
				z: safeIntAdd(value.z, amount),
				radiusMilli: value.radiusMilli
			};
		case SunRadius:
			final value = sunOrDefault(sun);
			sun = {
				x: value.x,
				y: value.y,
				z: value.z,
				radiusMilli: boundedAdd(value.radiusMilli, amount, 250, 10000)
			};
		case CloudCount:
			clouds = {count: boundedAdd(clouds.count, amount, 0, 12), speedMilli: clouds.speedMilli, seed: clouds.seed};
		case CloudSpeed:
			clouds = {count: clouds.count, speedMilli: boundedAdd(clouds.speedMilli, amount, 0, 5000), seed: clouds.seed};
		case CloudSeed:
			clouds = {count: clouds.count, speedMilli: clouds.speedMilli, seed: safeIntAdd(clouds.seed, amount)};
		case NorthEdge:
			setEdge(edges, North, direction == Increase);
		case SouthEdge:
			setEdge(edges, South, direction == Increase);
		case EastEdge:
			setEdge(edges, East, direction == Increase);
		case WestEdge:
			setEdge(edges, West, direction == Increase);
		case ContinueWater:
			continueWater = direction == Increase;
	}
	return {
		profile: base.profile,
		sky: sky,
		sun: sun,
		clouds: clouds,
		edges: edges,
		continueWater: continueWater
	};
}

/** Generic starting point for a level that opts into visual surroundings. */
function defaultEnvironment():ScenarioEnvironment {
	return {
		profile: ScenarioEnvironmentProfile.VoxelHorizon,
		sky: {red: 112, green: 182, blue: 224},
		sun: defaultSun(),
		clouds: {count: 0, speedMilli: 0, seed: 0},
		edges: [],
		continueWater: false
	};
}

private function defaultSun():ScenarioSun
	return {
		x: -300,
		y: 700,
		z: 250,
		radiusMilli: 300
	};

private function copyEnvironment(value:ScenarioEnvironment):ScenarioEnvironment {
	return {
		profile: value.profile,
		sky: copyRgb(value.sky),
		sun: copySun(value.sun),
		clouds: copyClouds(value.clouds),
		edges: value.edges.copy(),
		continueWater: value.continueWater
	};
}

private function copyRgb(value:ScenarioRgb):ScenarioRgb
	return {red: value.red, green: value.green, blue: value.blue};

private function copySun(value:Null<ScenarioSun>):Null<ScenarioSun> {
	if (value == null)
		return null;
	return {
		x: value.x,
		y: value.y,
		z: value.z,
		radiusMilli: value.radiusMilli
	};
}

private function sunOrDefault(value:Null<ScenarioSun>):ScenarioSun {
	if (value == null)
		return defaultSun();
	return value;
}

private function copyClouds(value:ScenarioCloudLayer):ScenarioCloudLayer
	return {count: value.count, speedMilli: value.speedMilli, seed: value.seed};

private function setEdge(edges:Array<ScenarioHorizonEdge>, edge:ScenarioHorizonEdge, enabled:Bool):Void {
	for (index in 0...edges.length)
		if (edges[index] == edge) {
			if (!enabled)
				edges.splice(index, 1);
			return;
		}
	if (enabled)
		edges.push(edge);
}

private function boundedAdd(value:Int, amount:Int, minimum:Int, maximum:Int):Int {
	if (amount > 0)
		return value >= maximum ? maximum : value + 1;
	return value <= minimum ? minimum : value - 1;
}

private function safeIntAdd(value:Int, amount:Int):Int {
	if (amount > 0)
		return value == 2147483647 ? value : value + 1;
	final minimum = -2147483647 - 1;
	return value == minimum ? value : value - 1;
}
