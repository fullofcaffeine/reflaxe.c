package caxecraft.app;

#if c
import raylib.Color;
import raylib.Raylib;

/** Tiny allocation-free seven-segment counter renderer. */
final class HudDigits {
	static inline final DIGIT_WIDTH:Int = 8;
	static inline final DIGIT_HEIGHT:Int = 12;
	static inline final HALF_HEIGHT:Int = 6;
	static inline final THICKNESS:Int = 2;

	public static function drawNumber(value:Int, x:Int, y:Int, digits:Int, color:Color):Void {
		var remaining = value;
		var index = digits - 1;
		while (index >= 0) {
			final digit = remaining % 10;
			drawDigit(digit, x + index * (DIGIT_WIDTH + 3), y, color);
			remaining = Std.int(remaining / 10);
			index--;
		}
	}

	static function drawDigit(digit:Int, x:Int, y:Int, color:Color):Void {
		final mask = switch (digit) {
			case 0: 0x3f;
			case 1: 0x06;
			case 2: 0x5b;
			case 3: 0x4f;
			case 4: 0x66;
			case 5: 0x6d;
			case 6: 0x7d;
			case 7: 0x07;
			case 8: 0x7f;
			case 9: 0x6f;
			default: 0;
		};
		if ((mask & 0x01) != 0)
			Raylib.DrawRectangle(x + THICKNESS, y, DIGIT_WIDTH - THICKNESS * 2, THICKNESS, color);
		if ((mask & 0x02) != 0)
			Raylib.DrawRectangle(x + DIGIT_WIDTH - THICKNESS, y + THICKNESS, THICKNESS, HALF_HEIGHT - THICKNESS, color);
		if ((mask & 0x04) != 0)
			Raylib.DrawRectangle(x + DIGIT_WIDTH - THICKNESS, y + HALF_HEIGHT, THICKNESS, HALF_HEIGHT - THICKNESS, color);
		if ((mask & 0x08) != 0)
			Raylib.DrawRectangle(x + THICKNESS, y + DIGIT_HEIGHT - THICKNESS, DIGIT_WIDTH - THICKNESS * 2, THICKNESS, color);
		if ((mask & 0x10) != 0)
			Raylib.DrawRectangle(x, y + HALF_HEIGHT, THICKNESS, HALF_HEIGHT - THICKNESS, color);
		if ((mask & 0x20) != 0)
			Raylib.DrawRectangle(x, y + THICKNESS, THICKNESS, HALF_HEIGHT - THICKNESS, color);
		if ((mask & 0x40) != 0)
			Raylib.DrawRectangle(x + THICKNESS, y + HALF_HEIGHT - 1, DIGIT_WIDTH - THICKNESS * 2, THICKNESS, color);
	}
}
#end
