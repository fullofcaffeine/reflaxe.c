package caxecraft.input;

/**
 * Converts one device snapshot into rate-limited, device-neutral UI commands.
 *
 * A device adapter reports held directions plus one-frame confirm/cancel
 * presses. `NavigationRepeater` emits the first direction immediately, waits
 * before repeating it, then repeats at a steady rate. Menus and editors can
 * therefore share predictable navigation without knowing about Raylib,
 * controller brands, keyboard keys, or operating-system input delivery.
 */
enum abstract NavigationCommand(Int) {
	var None = 0;
	var Up = 1;
	var Right = 2;
	var Down = 3;
	var Left = 4;
	var Confirm = 5;
	var Cancel = 6;
}

/**
 * One normalized UI-input observation from any keyboard, controller, or pilot.
 *
 * `horizontal` and `vertical` use the conventional `-1.0` to `1.0` stick
 * range. Digital directions are kept separately so an adapter never has to
 * invent analog values for a D-pad. Confirm and cancel are press edges rather
 * than held values, preventing one long press from activating several screens.
 */
typedef NavigationSample = {
	final connected:Bool;
	final up:Bool;
	final right:Bool;
	final down:Bool;
	final left:Bool;
	final confirmPressed:Bool;
	final cancelPressed:Bool;
	final horizontal:Float;
	final vertical:Float;
}

/**
 * Owns the small amount of time state needed for held-direction repetition.
 *
 * This is an instance class, rather than stateless module functions, because
 * each local input device needs an independent held direction and repeat
 * clock. It allocates once when the application creates it; `advance` mutates
 * those scalar fields and returns one integer-backed command per frame.
 */
final class NavigationRepeater {
	public static inline final DEAD_ZONE:Float = 0.35;
	public static inline final INITIAL_REPEAT_DELAY_SECONDS:Float = 0.35;
	public static inline final REPEAT_INTERVAL_SECONDS:Float = 0.10;

	var held:NavigationCommand;
	var heldSeconds:Float;
	var nextRepeatSeconds:Float;

	/** Begin disconnected, with no direction waiting to repeat. */
	public function new() {
		held = None;
		heldSeconds = 0.0;
		nextRepeatSeconds = INITIAL_REPEAT_DELAY_SECONDS;
	}

	/**
	 * Produce at most one UI command from the current frame.
	 *
	 * Confirm and cancel win over movement because they are deliberate press
	 * edges. A newly held direction moves immediately; an unchanged direction
	 * repeats after 350 ms and then every 100 ms. Releasing the direction,
	 * entering the dead zone, pressing opposite directions together, or losing
	 * the device resets the clock. Large frame durations still emit at most one
	 * move, so a stalled frame cannot skip across several controls.
	 */
	public function advance(sample:NavigationSample, frameSeconds:Float):NavigationCommand {
		if (!sample.connected) {
			reset();
			return None;
		}
		if (sample.cancelPressed)
			return Cancel;
		if (sample.confirmPressed)
			return Confirm;

		final direction = directionOf(sample);
		if (direction == None) {
			reset();
			return None;
		}
		if (direction != held) {
			held = direction;
			heldSeconds = 0.0;
			nextRepeatSeconds = INITIAL_REPEAT_DELAY_SECONDS;
			return direction;
		}

		if (frameSeconds > 0.0)
			heldSeconds += frameSeconds;
		if (heldSeconds < nextRepeatSeconds)
			return None;
		nextRepeatSeconds = heldSeconds + REPEAT_INTERVAL_SECONDS;
		return direction;
	}

	/**
	 * Clear input remembered by a screen that is no longer active.
	 *
	 * The application calls this while another screen owns input. Returning to
	 * a menu or editor then treats a still-held direction as a fresh press
	 * instead of inheriting an invisible repeat clock from the prior screen.
	 */
	public function release():Void
		reset();

	/** Forget a released or disconnected direction and its partial delay. */
	function reset():Void {
		held = None;
		heldSeconds = 0.0;
		nextRepeatSeconds = INITIAL_REPEAT_DELAY_SECONDS;
	}

	/**
	 * Resolve digital and analog input to one direction.
	 *
	 * D-pad input and stick movement outside the 35% dead zone are equivalent.
	 * Opposite directions cancel one another. Vertical input wins only when it
	 * is unambiguous; otherwise horizontal input is considered, giving diagonal
	 * stick motion one deterministic result rather than two UI moves.
	 */
	static function directionOf(sample:NavigationSample):NavigationCommand {
		final up = sample.up || sample.vertical < -DEAD_ZONE;
		final down = sample.down || sample.vertical > DEAD_ZONE;
		if (up != down)
			return up ? Up : Down;
		final left = sample.left || sample.horizontal < -DEAD_ZONE;
		final right = sample.right || sample.horizontal > DEAD_ZONE;
		if (left != right)
			return left ? Left : Right;
		return None;
	}
}
