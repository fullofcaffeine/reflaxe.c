package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorChangeId;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorTypes.EditorTerrainChange;
import haxe.io.Bytes;

/**
	The exact reversible state owned by one accepted command.

	It is shared between editor source files but hidden from ordinary code
	completion because only `EditorSession` may commit or restore it.
**/
@:noCompletion
typedef EditorHistoryEntry = {
	final family:EditorCommandFamily;
	final changes:Array<EditorChangeId>;

	/** Smallest safe terrain refresh after restoring `before`. */
	final undoTerrain:EditorTerrainChange;

	/** Smallest safe terrain refresh after restoring `after`. */
	final redoTerrain:EditorTerrainChange;

	/** Stable identity of the draft restored by Undo. */
	final beforeStateIdentity:Int;

	/** Stable identity of the draft restored by Redo. */
	final afterStateIdentity:Int;

	final before:Bytes;
	final after:Bytes;
	final byteCost:Int;
}

/**
	Bounded undo/redo storage.

	Both stacks share one byte and entry budget. Adding a new command drops the
	redo branch, then evicts the oldest undo entries until the configured bounds
	are satisfied. A single command too large for the byte budget is rejected
	before the live draft changes.

	`@:noCompletion` keeps this storage mechanism out of the public editing API;
	it does not change runtime visibility or type safety.
**/
@:noCompletion
final class EditorHistory {
	final settings:EditorSettings;
	final undoEntries:Array<EditorHistoryEntry> = [];
	final redoEntries:Array<EditorHistoryEntry> = [];
	var usedBytes:Int = 0;

	public function new(settings:EditorSettings)
		this.settings = settings;

	public inline function undoDepth():Int
		return undoEntries.length;

	public inline function redoDepth():Int
		return redoEntries.length;

	public inline function entryCount():Int
		return undoEntries.length + redoEntries.length;

	public inline function byteCount():Int
		return usedBytes;

	#if caxecraft_editor_probe
	/** Count distinct private byte buffers retained by focused allocation tests. */
	public function byteBufferCount():Int {
		final buffers:Array<Bytes> = [];
		for (entry in undoEntries) {
			rememberByteBuffer(buffers, entry.before);
			rememberByteBuffer(buffers, entry.after);
		}
		for (entry in redoEntries) {
			rememberByteBuffer(buffers, entry.before);
			rememberByteBuffer(buffers, entry.after);
		}
		return buffers.length;
	}

	/** Add one buffer by object identity so equal snapshots remain distinguishable. */
	function rememberByteBuffer(buffers:Array<Bytes>, candidate:Bytes):Void {
		for (buffer in buffers)
			if (buffer == candidate)
				return;
		buffers.push(candidate);
	}
	#end

	public inline function canRecord(byteCost:Int):Bool
		return byteCost <= settings.historyBytes;

	public function record(entry:EditorHistoryEntry):Void {
		for (value in redoEntries)
			usedBytes -= value.byteCost;
		redoEntries.resize(0);
		undoEntries.push(entry);
		usedBytes += entry.byteCost;
		while (entryCount() > settings.historyEntries || usedBytes > settings.historyBytes) {
			final removed = undoEntries.shift();
			if (removed == null)
				break;
			usedBytes -= removed.byteCost;
		}
	}

	public function takeUndo():Null<EditorHistoryEntry> {
		final entry = undoEntries.pop();
		if (entry != null)
			redoEntries.push(entry);
		return entry;
	}

	public function takeRedo():Null<EditorHistoryEntry> {
		final entry = redoEntries.pop();
		if (entry != null)
			undoEntries.push(entry);
		return entry;
	}
}
