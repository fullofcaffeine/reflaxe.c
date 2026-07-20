package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexRecord;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexToken;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexTokenKind;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioStory.ObjectiveState;

typedef ParsedScenarioText = {
	final value:ScenarioText;
	final next:Int;
}

/** Small, context-free value readers shared by the record-level grammar. */
// Used only by the CAXEMAP parser. Game and editor code should call
// ScenarioParser.parse(...) instead of calling this class.

@:noCompletion
final class ScenarioTokenGrammar {
	public static inline function hasTokenCount(record:ScenarioLexRecord, expected:Int):Bool
		return record.tokens.length == expected;

	public static inline function isBare(token:ScenarioLexToken, expected:String):Bool
		return token.kind == BareToken && token.text == expected;

	public static inline function isQuoted(token:ScenarioLexToken):Bool
		return token.kind == QuotedText;

	public static inline function bareText(token:ScenarioLexToken):Null<String>
		return token.kind == BareToken ? token.text : null;

	public static inline function firstText(record:ScenarioLexRecord):String
		return record.tokens.length == 0 ? "" : (record.tokens[0].kind == BareToken ? record.tokens[0].text : "");

	public static inline function isEnd(record:ScenarioLexRecord, kind:String):Bool
		return isEndAt(record, kind, 0);

	public static function isEndAt(record:ScenarioLexRecord, kind:String, indent:Int):Bool
		return record.indent == indent && hasTokenCount(record, 2) && firstText(record) == "end" && isBare(record.tokens[1], kind);

	public static function text(record:ScenarioLexRecord, at:Int):Null<ParsedScenarioText> {
		if (at + 1 >= record.tokens.length)
			return null;
		return switch bareText(record.tokens[at]) {
			case "message":
				final id = messageId(record.tokens[at + 1]);
				id == null ? null : {value: Message(id), next: at + 2};
			case "literal" if (isQuoted(record.tokens[at + 1])):
				{value: Literal(record.tokens[at + 1].text), next: at + 2};
			case _:
				null;
		}
	}

	public static function transform(record:ScenarioLexRecord, at:Int):Null<ScenarioTransform> {
		if (at + 3 >= record.tokens.length)
			return null;
		final x = integer(record.tokens[at]);
		final y = integer(record.tokens[at + 1]);
		final z = integer(record.tokens[at + 2]);
		final yaw = integer(record.tokens[at + 3]);
		return x == null || y == null || z == null || yaw == null ? null : {
			xMilli: x,
			yMilli: y,
			zMilli: z,
			yawDegrees: yaw
		};
	}

	public static function point(record:ScenarioLexRecord, at:Int):Null<VoxelPoint> {
		if (at + 2 >= record.tokens.length)
			return null;
		final x = integer(record.tokens[at]);
		final y = integer(record.tokens[at + 1]);
		final z = integer(record.tokens[at + 2]);
		return x == null || y == null || z == null ? null : {x: x, y: y, z: z};
	}

	public static function size(record:ScenarioLexRecord, at:Int):Null<VoxelSize> {
		if (at + 2 >= record.tokens.length)
			return null;
		final width = integer(record.tokens[at]);
		final height = integer(record.tokens[at + 1]);
		final depth = integer(record.tokens[at + 2]);
		return width == null || height == null || depth == null ? null : {width: width, height: height, depth: depth};
	}

	/** Parse the one canonical spelling of a signed 32-bit decimal integer. */
	public static function integer(token:ScenarioLexToken):Null<Int> {
		final value = token.text;
		if (token.kind != BareToken || value.length == 0 || value.charCodeAt(0) == 43)
			return null;
		var negative = false;
		var at = 0;
		if (value.charCodeAt(0) == 45) {
			negative = true;
			at = 1;
		}
		if (at == value.length || (value.charCodeAt(at) == 48 && at + 1 != value.length) || (negative && value == "-0"))
			return null;
		final limit = negative ? -2147483648 : -2147483647;
		final multiplyLimit = -214748364;
		var result = 0;
		while (at < value.length) {
			final code = value.charCodeAt(at);
			if (code < 48 || code > 57 || result < multiplyLimit)
				return null;
			result *= 10;
			final digit = code - 48;
			if (result < limit + digit)
				return null;
			result -= digit;
			at++;
		}
		return negative ? result : -result;
	}

	public static function boolean(token:ScenarioLexToken):Null<Bool>
		return switch bareText(token) {
			case "true": true;
			case "false": false;
			case _: null;
		}

	public static function objectiveState(token:ScenarioLexToken):Null<ObjectiveState>
		return switch bareText(token) {
			case "hidden": Hidden;
			case "active": Active;
			case "complete": Complete;
			case "failed": Failed;
			case _: null;
		}

	public static function comparison(token:ScenarioLexToken):Null<FlowComparison>
		return switch bareText(token) {
			case "equal": Equal;
			case "not-equal": NotEqual;
			case "less": Less;
			case "less-or-equal": LessOrEqual;
			case "greater": Greater;
			case "greater-or-equal": GreaterOrEqual;
			case _: null;
		}

	public static function scenarioId(token:ScenarioLexToken):Null<ScenarioId>
		return token.kind == BareToken && validId(token.text) ? new ScenarioId(token.text) : null;

	public static function scenarioTag(token:ScenarioLexToken):Null<ScenarioTag>
		return token.kind == BareToken && validId(token.text) ? new ScenarioTag(token.text) : null;

	public static function messageId(token:ScenarioLexToken):Null<MessageId>
		return token.kind == BareToken && validId(token.text) ? new MessageId(token.text) : null;

	public static function contentId(token:ScenarioLexToken):Null<ContentId> {
		if (token.kind != BareToken)
			return null;
		final separator = token.text.indexOf(":");
		return separator <= 0
			|| separator != token.text.lastIndexOf(":")
			|| !validId(token.text.substring(0, separator))
			|| !validId(token.text.substring(separator + 1)) ? null : new ContentId(token.text);
	}

	public static function logicalPath(token:ScenarioLexToken):Null<LogicalPath> {
		if (token.kind != BareToken || token.text.length == 0 || token.text.indexOf("\\") != -1 || token.text.charAt(0) == "/")
			return null;
		for (part in token.text.split("/"))
			if (part == "." || part == ".." || !validId(part))
				return null;
		return new LogicalPath(token.text);
	}

	static function validId(value:String):Bool {
		if (value.length == 0)
			return false;
		var expectLetter = true;
		for (at in 0...value.length) {
			final code = value.charCodeAt(at);
			if (expectLetter) {
				if (code < 97 || code > 122)
					return false;
				expectLetter = false;
			} else if (code == 46 || code == 95 || code == 45) {
				expectLetter = true;
			} else if (!((code >= 97 && code <= 122) || (code >= 48 && code <= 57))) {
				return false;
			}
		}
		return !expectLetter;
	}
}
