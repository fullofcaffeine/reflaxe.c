import raygui.GuiTextBoxState;

/**
 * Exercises text ownership without opening a native window.
 *
 * Eval proves the ordinary Haxe value semantics quickly. The generated-C
 * fixture separately proves that the same owner lends its storage directly to
 * pinned raygui when `draw` is reached.
 */
final class Main {
	static function main():Void {
		assert(GuiTextBoxState.create(0, "") == null, "zero capacity was accepted");
		assert(GuiTextBoxState.create(4, "four") == null, "missing NUL room was accepted");
		assert(GuiTextBoxState.create(16, "a" + String.fromCharCode(0) + "b") == null, "embedded NUL was accepted");

		final maybeState = GuiTextBoxState.create(16, "Ivvy");
		assert(maybeState != null, "valid text box was rejected");
		final state:GuiTextBoxState = maybeState;
		assert(state.capacity() == 16, "capacity drifted");
		assert(state.text() == "Ivvy", "initial text drifted");
		assert(!state.isEditing(), "editing should begin disabled");
		state.setEditing(true);
		assert(state.isEditing(), "editing state did not change");
		assert(state.replace("Niña 🐈"), "valid UTF-8 replacement was rejected");
		assert(state.text() == "Niña 🐈", "UTF-8 replacement drifted");
		assert(!state.replace("0123456789abcdef"), "oversized replacement was accepted");
		assert(state.text() == "Niña 🐈", "failed replacement changed the previous text");
		state.clear();
		assert(state.text() == "", "clear did not produce empty text");
	}

	static function assert(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
