package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorSetting;
import caxecraft.editor.EditorTypes.EditorSettings;

/**
	Defines bounded editor-memory policy without owning editor state.

	Callers own each `EditorSettings` value. Module constants and functions make
	that stateless relationship explicit; a static-only policy class would add a
	nominal namespace without creating a policy object or lifetime.
**/
/** Largest undo and redo entry count accepted by one editor session. */
inline final MAX_HISTORY_ENTRIES:Int = 64;

/** Largest combined byte budget accepted for one session's history. */
inline final MAX_HISTORY_BYTES:Int = 67108864;

/** Largest voxel selection one editor command may process. */
inline final MAX_SELECTION_CELLS:Int = 65536;

/** Create the conservative settings used when a caller supplies no override. */
function defaults():EditorSettings
	return {
		historyEntries: 64,
		historyBytes: 33554432,
		selectionCells: 65536
	};

/** Return the first invalid bound, or `null` when all settings are safe. */
function validate(settings:EditorSettings):Null<EditorError> {
	if (settings.historyEntries < 1 || settings.historyEntries > MAX_HISTORY_ENTRIES)
		return InvalidSetting(HistoryEntries, 1, MAX_HISTORY_ENTRIES);
	if (settings.historyBytes < 1 || settings.historyBytes > MAX_HISTORY_BYTES)
		return InvalidSetting(HistoryBytes, 1, MAX_HISTORY_BYTES);
	if (settings.selectionCells < 1 || settings.selectionCells > MAX_SELECTION_CELLS)
		return InvalidSetting(SelectionCells, 1, MAX_SELECTION_CELLS);
	return null;
}
