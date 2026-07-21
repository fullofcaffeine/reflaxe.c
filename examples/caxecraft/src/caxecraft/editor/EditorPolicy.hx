package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorSetting;
import caxecraft.editor.EditorTypes.EditorSettings;

/** Hard editor-memory limits shared by the future UI and headless tests. */
final class EditorPolicy {
	public static inline final MAX_HISTORY_ENTRIES:Int = 64;
	public static inline final MAX_HISTORY_BYTES:Int = 67108864;
	public static inline final MAX_SELECTION_CELLS:Int = 65536;

	public static function defaults():EditorSettings
		return {
			historyEntries: 64,
			historyBytes: 33554432,
			selectionCells: 65536
		};

	public static function validate(settings:EditorSettings):Null<EditorError> {
		if (settings.historyEntries < 1 || settings.historyEntries > MAX_HISTORY_ENTRIES)
			return InvalidSetting(HistoryEntries, 1, MAX_HISTORY_ENTRIES);
		if (settings.historyBytes < 1 || settings.historyBytes > MAX_HISTORY_BYTES)
			return InvalidSetting(HistoryBytes, 1, MAX_HISTORY_BYTES);
		if (settings.selectionCells < 1 || settings.selectionCells > MAX_SELECTION_CELLS)
			return InvalidSetting(SelectionCells, 1, MAX_SELECTION_CELLS);
		return null;
	}
}
