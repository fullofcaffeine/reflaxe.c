package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorChangeId;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;

/**
	Chooses the smallest correct terrain-presentation refresh for one command.

	The canonical editor session still owns every mutation. After that mutation
	succeeds, the native view uses this closed request to keep unrelated edits
	away from the terrain cache and to patch a single painted voxel without
	rebuilding the complete fixed-layout world. Broad terrain changes retain the
	safe complete refresh path.
**/
enum EditorTerrainRefreshRequest {
	/** The command cannot change terrain bytes or their runtime interpretation. */
	KeepTerrain;

	/** One accepted command changed exactly this authored voxel to this palette code. */
	RefreshTerrainVoxel(point:VoxelPoint, paletteCode:Int);

	/** The command can change several cells, dimensions, palette meaning, or the complete draft. */
	RefreshAllTerrain;
}

/** Return the narrowest safe presentation refresh after one accepted command. */
function forCommand(command:EditorCommand):EditorTerrainRefreshRequest {
	return switch command {
		case PaintVoxel(point, paletteCode): RefreshTerrainVoxel(point, paletteCode);
		case EraseVoxel(point): RefreshTerrainVoxel(point, 0);
		case ResizeWorld(_) | SetPaletteEntry(_, _) | PaintVoxels(_, _) | EraseVoxels(_) | FillBounds(_, _) | RestoreLastPlayable:
			RefreshAllTerrain;
		case SetTitle(_) | SetEnvironment(_) | PutFluid(_) | RemoveFluid(_) | StampPrefab(_, _, _, _) | PutObject(_) | MoveObjectBy(_, _) |
			RotateObjectBy(_, _) | ResizeTriggerTo(_, _) | RemoveObject(_) | PutDialogue(_) | RemoveDialogue(_) | PutObjective(_) | RemoveObjective(_) |
			PutRule(_) | RemoveRule(_) | SetDefaultLocale(_) | PutLocale(_) | RemoveLocale(_) | PutMessage(_, _) | RemoveMessage(_, _):
			KeepTerrain;
	};
}

/**
	Return the safe terrain refresh for one atomic command batch.

	A batch that contains any terrain mutation uses the complete path because one
	history entry can carry several changed coordinates. Object-only and metadata
	batches leave the retained terrain presentation untouched.
**/
function forBatch(commands:Array<EditorCommand>):EditorTerrainRefreshRequest {
	for (command in commands)
		switch forCommand(command) {
			case KeepTerrain:
			case RefreshTerrainVoxel(_, _) | RefreshAllTerrain:
				return RefreshAllTerrain;
		}
	return KeepTerrain;
}

/**
	Return the safe terrain refresh after Undo or Redo.

	History reports stable change IDs from its original command. Metadata and
	authored objects cannot change voxel bytes or palette meaning, so they retain
	the current projection. Terrain, palette, world-shape, and full-document
	restores use the complete path because history does not report exact cells.
**/
function forChanges(changes:Array<EditorChangeId>):EditorTerrainRefreshRequest {
	for (change in changes)
		switch change {
			case ChangedDocument | ChangedWorldShape | ChangedTerrain | ChangedPalette(_):
				return RefreshAllTerrain;
			case ChangedTitle | ChangedFluid(_) | ChangedObject(_) | ChangedDialogue(_) | ChangedObjective(_) | ChangedRule(_) | ChangedLocalization |
				ChangedLocale(_) | ChangedMessage(_, _):
		}
	return KeepTerrain;
}
