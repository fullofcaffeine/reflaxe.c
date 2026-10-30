package caxecraft.text;

import haxe.io.Bytes;

/**
 * Turns bounded external bytes into one valid Haxe string.
 *
 * Content files enter the game as bytes, but parsers work with Unicode scalar
 * values. This decoder rejects malformed, overlong, surrogate, byte-order-mark,
 * and NUL input before a parser sees text. CAXEMAP and JSON share this module so
 * the same external bytes cannot be accepted by one content path and rejected
 * by another.
 */
/**
 * Result of decoding one complete external byte vector.
 *
 * A closed enum keeps malformed input on an ordinary typed failure edge. The
 * decoder does not throw, retain the input `Bytes`, or publish partial text.
 */
enum Utf8DecodeResult {
	/**
	 * Every byte formed a valid admitted Unicode scalar sequence.
	 *
	 * `scalars` preserves the same text as direct code points. A bounded parser
	 * can traverse that array in linear time instead of repeatedly asking the C
	 * String runtime to find a scalar index from the start of its UTF-8 storage.
	 */
	Utf8Decoded(text:String, scalars:Array<Int>);

	/** Decoding stopped at the first malformed byte offset. */
	Utf8Rejected(byteOffset:Int);
}

/**
 * Stateless bounded UTF-8 decoder shared by repository-owned content codecs.
 *
 * A static-only class is retained here because the existing native-proven
 * CAXEMAP codec already uses this shape. A module-level function would be
 * simpler source, but changing the representation is useful only after the
 * compiler's module-field cleanup has its own focused evidence.
 */
final class Utf8Decoder {
	/**
	 * Decode all bytes or reject the first malformed sequence.
	 *
	 * `maximumBytes` is checked before allocation. A non-positive limit rejects
	 * every non-empty input, which keeps the caller's configured bound explicit.
	 * Valid non-NUL ASCII runs are copied together; multibyte input still takes
	 * the scalar path that enforces canonical UTF-8 and exact error offsets.
	 */
	public static function decode(input:Bytes, maximumBytes:Int):Utf8DecodeResult {
		if (input.length > maximumBytes)
			return Utf8Rejected(maximumBytes);
		if (input.length >= 3 && input.get(0) == 0xef && input.get(1) == 0xbb && input.get(2) == 0xbf)
			return Utf8Rejected(0);
		final output = new StringBuf();
		final scalars:Array<Int> = [];
		var offset = 0;
		while (offset < input.length) {
			final asciiStart = offset;
			while (offset < input.length) {
				final value = input.get(offset);
				if (value == 0 || value > 0x7f)
					break;
				offset++;
			}
			if (offset > asciiStart) {
				output.add(input.getString(asciiStart, offset - asciiStart));
				for (asciiOffset in asciiStart...offset)
					scalars.push(input.get(asciiOffset));
			}
			if (offset == input.length)
				break;
			final first = input.get(offset);
			var scalar = 0;
			var width = 0;
			if (first >= 0xc2 && first <= 0xdf) {
				scalar = first & 0x1f;
				width = 2;
			} else if (first >= 0xe0 && first <= 0xef) {
				scalar = first & 0x0f;
				width = 3;
			} else if (first >= 0xf0 && first <= 0xf4) {
				scalar = first & 0x07;
				width = 4;
			} else {
				return Utf8Rejected(offset);
			}
			if (offset + width > input.length)
				return Utf8Rejected(offset);
			for (index in 1...width) {
				final continuation = input.get(offset + index);
				if ((continuation & 0xc0) != 0x80)
					return Utf8Rejected(offset + index);
				scalar = (scalar << 6) | (continuation & 0x3f);
			}
			final overlong = (width == 2 && scalar < 0x80) || (width == 3 && scalar < 0x800) || (width == 4 && scalar < 0x10000);
			if (overlong || scalar > 0x10ffff || (scalar >= 0xd800 && scalar <= 0xdfff))
				return Utf8Rejected(offset);
			output.addChar(scalar);
			scalars.push(scalar);
			offset += width;
		}
		return Utf8Decoded(output.toString(), scalars);
	}
}
