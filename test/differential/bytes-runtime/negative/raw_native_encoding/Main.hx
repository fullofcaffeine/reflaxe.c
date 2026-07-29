import haxe.io.Bytes;
import haxe.io.Encoding;

/**
 * Keeps platform-native String memory spelling outside the portable C slice.
 *
 * UTF-8 is the only cross-platform encoding admitted by the current
 * Bytes-to-String runtime contract. `RawNative` can mean different byte orders
 * and widths on different targets, so it must fail at this exact source call
 * instead of silently behaving like UTF-8.
 */
final class Main {
	static function main():Void {
		final bytes = Bytes.ofString("text");
		bytes.getString(0, bytes.length, Encoding.RawNative);
	}
}
