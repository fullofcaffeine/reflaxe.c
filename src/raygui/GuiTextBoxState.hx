package raygui;

import haxe.io.Bytes;

/**
 * Owns the durable Haxe state behind one immediate-mode raygui text box.
 *
 * Raygui redraws the control every frame and edits a caller-supplied C buffer;
 * it does not create or own a widget object. This class keeps that buffer and
 * the editing flag together so their lifetimes and byte capacity cannot drift.
 * Application code receives copied `String` snapshots, never the mutable
 * pointer that the native call uses.
 *
 * A class is appropriate here because the text and focus state have identity
 * across frames. Stateless controls remain module-like facade calls instead of
 * acquiring pretend widget objects.
 */
final class GuiTextBoxState {
	final storage:Bytes;
	var editing:Bool;

	/**
	 * Create an empty or prefilled text box with one fixed byte capacity.
	 *
	 * `capacity` counts the complete native allocation, including one byte
	 * reserved for the final NUL terminator required by C. The initial Haxe
	 * string is encoded as UTF-8. This returns `null` when capacity is not
	 * positive, the encoded text would not leave room for the terminator, or the
	 * string contains an embedded NUL that would make C observe only a prefix.
	 */
	public static function create(capacity:Int, initialText:String = ""):Null<GuiTextBoxState> {
		if (capacity <= 0)
			return null;
		final value = new GuiTextBoxState(Bytes.alloc(capacity));
		if (!value.replace(initialText))
			return null;
		return value;
	}

	private function new(storage:Bytes) {
		this.storage = storage;
		editing = false;
	}

	/** Return the full native byte capacity, including room for the final NUL. */
	public inline function capacity():Int
		return storage.length;

	/** Report whether keyboard input currently edits this control. */
	public inline function isEditing():Bool
		return editing;

	/**
	 * Enter or leave editing explicitly.
	 *
	 * Production screens normally let `draw` toggle this state from raygui's
	 * click, Enter, and outside-click result. The explicit setter also lets a
	 * screen restore presentation state and lets deterministic pilots exercise
	 * the same control without fabricating operating-system input.
	 */
	public inline function setEditing(value:Bool):Void
		editing = value;

	/**
	 * Replace the text while preserving the fixed native allocation.
	 *
	 * The method first validates the complete replacement, so failure leaves the
	 * previous text untouched. A successful write clears unused bytes, copies
	 * the UTF-8 payload, and therefore leaves at least one NUL byte inside the
	 * allocation for raygui's `strlen`-based reader.
	 */
	public function replace(value:String):Bool {
		final encoded = Bytes.ofString(value);
		if (encoded.length >= storage.length)
			return false;
		for (index in 0...encoded.length)
			if (encoded.get(index) == 0)
				return false;
		storage.fill(0, storage.length, 0);
		storage.blit(0, encoded, 0, encoded.length);
		return true;
	}

	/** Clear the text without changing capacity or edit state. */
	public function clear():Void
		storage.fill(0, storage.length, 0);

	/**
	 * Copy the current NUL-terminated UTF-8 payload into an immutable String.
	 *
	 * The pinned raygui implementation inserts and pastes complete UTF-8 code
	 * points only when they fit before the terminator. haxe.c still validates
	 * the bytes during `getString`; malformed native output fails at that exact
	 * boundary instead of becoming an invalid Haxe String.
	 */
	public function text():String {
		var length = 0;
		while (length < storage.length && storage.get(length) != 0)
			length++;
		return storage.getString(0, length);
	}

	#if c
	/**
	 * Draw one frame, let raygui edit the owned bytes, and update edit state.
	 *
	 * `CStringBufferRef.to(storage)` lends the buffer only to this direct call.
	 * The exact allocation length is passed as raygui's `textSize`, whose
	 * contract counts bytes including the final NUL. The generated C therefore
	 * performs a checked borrow followed immediately by `GuiTextBox`; the native
	 * library cannot retain the pointer or acquire ownership of the Bytes value.
	 */
	public function draw(bounds:raylib.Rectangle):GuiResult {
		final result = GuiResult.fromRaw(c.IntConvert.exact(raygui.raw.Raygui.GuiTextBox(bounds, c.CStringBufferRef.to(storage),
			c.IntConvert.exact(storage.length), editing)));
		if (result.has(GuiResult.Pressed))
			editing = !editing;
		return result;
	}
	#end
}
