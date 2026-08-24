package caxecraft.qa;

import caxecraft.domain.CharacterBody;
import caxecraft.domain.DynamicCollisionBox;
import caxecraft.domain.PlayerCamera.PlayerCameraMode;
import caxecraft.domain.PlayerCamera.PlayerCameraView;
import caxecraft.domain.PlayerCamera.resolvePlayerCamera;
import caxecraft.domain.PlayerCamera.togglePlayerCamera;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldView;
import caxecraft.domain.WorldVolume;
#if c
import c.CArray;
import c.UInt8;
#end

/**
	Cross-target specification for the player camera and unchanged gameplay aim.

	The probe runs on Eval and native C. It checks the two admitted modes, terrain
	and authored-box obstruction, avatar visibility, and the exact interaction ray
	without opening Raylib or depending on campaign content.
**/
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck();
	#else
	Sys.println(selfCheck());
	#end
}

/** Return zero, or the stable number of the first broken camera rule. */
function selfCheck():Int {
	#if c
	var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	var cells:WorldCells = storage.span();
	var view:WorldView = storage.constSpan();
	#else
	var cells:WorldCells = [];
	for (_ in 0...World.VOLUME)
		cells.push(0);
	var view:WorldView = WorldView.borrow(cells);
	#end
	final player = body(10.0, 2.0, 10.0);
	final noCollisions:Array<DynamicCollisionBox> = [];
	final first = resolvePlayerCamera(view, noCollisions, PlayerCameraMode.FirstPerson, player, 0.0, 0.0, -2.0);
	if (!near(first.positionX, 10.0))
		return 1;
	if (!near(first.positionY, 3.62))
		return 2;
	if (!near(first.positionZ, 10.0))
		return 3;
	if (!near(first.interactionDirectionZ, -1.0))
		return 4;
	if (first.avatarVisible)
		return 5;
	if (first.boomDistance != 0.0)
		return 6;
	if (togglePlayerCamera(PlayerCameraMode.FirstPerson) != PlayerCameraMode.BehindPlayer
		|| togglePlayerCamera(PlayerCameraMode.BehindPlayer) != PlayerCameraMode.FirstPerson)
		return 7;
	final behind = resolvePlayerCamera(view, noCollisions, PlayerCameraMode.BehindPlayer, player, 0.0, 0.0, -2.0);
	if (!near(behind.positionZ, 13.8)
		|| !near(behind.boomDistance, 3.8)
		|| !behind.avatarVisible
		|| !sameInteractionRay(first, behind))
		return 8;
	if (!World.replace(cells, World.coord(10, 3, 12), caxecraft.domain.BlockKind.Stone))
		return 9;
	final terrainBlocked = resolvePlayerCamera(view, noCollisions, PlayerCameraMode.BehindPlayer, player, 0.0, 0.0, -1.0);
	if (terrainBlocked.boomDistance < 1.6 || terrainBlocked.boomDistance > 1.9 || !sameInteractionRay(first, terrainBlocked))
		return 10;
	if (!World.replace(cells, World.coord(10, 3, 12), caxecraft.domain.BlockKind.Air))
		return 11;
	final authoredSolid:Array<DynamicCollisionBox> = [
		{
			minimumX: 9.5,
			maximumX: 10.5,
			minimumY: 3.0,
			maximumY: 4.0,
			minimumZ: 11.8,
			maximumZ: 12.2
		}
	];
	final objectBlocked = resolvePlayerCamera(view, authoredSolid, PlayerCameraMode.BehindPlayer, player, 0.0, 0.0, -1.0);
	if (objectBlocked.boomDistance < 1.5 || objectBlocked.boomDistance > 1.7 || !sameInteractionRay(first, objectBlocked))
		return 12;
	final zeroLook = resolvePlayerCamera(view, noCollisions, PlayerCameraMode.FirstPerson, player, 0.0, 0.0, 0.0);
	if (!near(zeroLook.interactionDirectionX, 0.0)
		|| !near(zeroLook.interactionDirectionY, 0.0)
		|| !near(zeroLook.interactionDirectionZ, -1.0))
		return 13;
	return 0;
}

/** Compare the complete world-action ray across presentation modes. */
function sameInteractionRay(left:PlayerCameraView, right:PlayerCameraView):Bool
	return near(left.interactionOriginX, right.interactionOriginX)
		&& near(left.interactionOriginY, right.interactionOriginY)
		&& near(left.interactionOriginZ, right.interactionOriginZ)
		&& near(left.interactionDirectionX, right.interactionDirectionX)
		&& near(left.interactionDirectionY, right.interactionDirectionY)
		&& near(left.interactionDirectionZ, right.interactionDirectionZ);

/** Build one committed-body-shaped fixture. */
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

/** Small deterministic tolerance shared by Eval and native C. */
inline function near(actual:Float, expected:Float):Bool {
	final difference = actual - expected;
	return difference >= -0.000001 && difference <= 0.000001;
}
