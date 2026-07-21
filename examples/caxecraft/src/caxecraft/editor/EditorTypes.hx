package caxecraft.editor;

import caxecraft.editor.EditorSession;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioTag;
import haxe.io.Bytes;

/** Resource limits selected when an editor session opens. */
typedef EditorSettings = {
	final historyEntries:Int;
	final historyBytes:Int;
	final selectionCells:Int;
}

/** The editor's closed, renderer-independent command vocabulary. */
enum EditorCommand {
	ResizeWorld(size:VoxelSize);
	SetPaletteEntry(code:Int, blockType:ContentId);
	PaintVoxel(point:VoxelPoint, paletteCode:Int);
	EraseVoxel(point:VoxelPoint);
	PaintVoxels(points:Array<VoxelPoint>, paletteCode:Int);
	EraseVoxels(points:Array<VoxelPoint>);
	Select(bounds:VoxelBounds);
	ClearSelection;
	FillSelection(paletteCode:Int);
	StampPrefab(id:ScenarioId, prefabType:ContentId, tags:Array<ScenarioTag>, transform:ScenarioTransform);
	PutObject(object:ScenarioObject);
	RemoveObject(id:ScenarioId);
	PutDialogue(dialogue:ScenarioDialogue);
	RemoveDialogue(id:ScenarioId);
	PutObjective(objective:ScenarioObjective);
	RemoveObjective(id:ScenarioId);
	PutRule(rule:FlowRule);
	RemoveRule(id:ScenarioId);
	RestoreLastPlayable;
}

/** Stable command groups used by history, UI labels, and acceptance traces. */
enum EditorCommandFamily {
	WorldShape;
	Voxel;
	Selection;
	Prefab;
	Placement;
	Dialogue;
	Objective;
	Rule;
	Recovery;
}

enum EditorSetting {
	HistoryEntries;
	HistoryBytes;
	SelectionCells;
}

/** Exact reasons an edit can be refused before it changes the draft. */
enum EditorError {
	InvalidSetting(setting:EditorSetting, minimum:Int, maximum:Int);
	UnsupportedFormatVersion(actual:Int, supported:Int);
	SnapshotRejected(diagnostics:Array<ScenarioDiagnostic>);
	NestedChoiceIsNotRepresentable;
	DraftWorldIsNotEditable;
	NotEditing;
	InvalidWorldSize(size:VoxelSize);
	PointOutsideWorld(point:VoxelPoint);
	BoundsOutsideWorld(bounds:VoxelBounds);
	SelectionTooLarge(cells:Int, maximum:Int);
	VoxelEditTooLarge(cells:Int, maximum:Int);
	NoSelection;
	InvalidPaletteCode(code:Int);
	UnknownPaletteCode(code:Int);
	DuplicateObject(id:ScenarioId);
	MissingObject(id:ScenarioId);
	MissingDialogue(id:ScenarioId);
	MissingObjective(id:ScenarioId);
	MissingRule(id:ScenarioId);
	HistoryEntryTooLarge(bytes:Int, maximum:Int);
	NothingToUndo;
	NothingToRedo;
	NoPlayableScenario;
}

enum EditorOpenResult {
	EditorOpened(session:EditorSession);
	EditorOpenRejected(error:EditorError);
}

enum EditorEditResult {
	EditApplied(family:EditorCommandFamily, undoDepth:Int, redoDepth:Int);
	EditUnchanged(family:EditorCommandFamily);
	EditRejected(error:EditorError);
}

enum EditorHistoryResult {
	HistoryApplied(family:EditorCommandFamily, undoDepth:Int, redoDepth:Int);
	HistoryRejected(error:EditorError);
}

enum EditorValidationResult {
	ValidationPassed(canonical:Bytes);
	ValidationFailed(diagnostics:Array<ScenarioDiagnostic>);
	ValidationBlocked(error:EditorError);
}

enum EditorTestPlayResult {
	TestPlayStarted;
	TestPlayRejected(diagnostics:Array<ScenarioDiagnostic>);
	TestPlayBlocked(error:EditorError);
}
