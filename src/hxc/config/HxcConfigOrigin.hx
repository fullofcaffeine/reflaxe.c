package hxc.config;

/** Stable provenance families in increasing precedence order. */
enum abstract HxcConfigOriginKind(String) to String {
	var CompilerDefault = "compiler-default";
	var ProfilePreset = "profile-preset";
	var EnvironmentPreset = "environment-preset";
	var ProjectFile = "project-file";
	var NamedOverlay = "named-overlay";
	var CliFlag = "cli-flag";
	var DirectDefine = "direct-define";
}

typedef HxcConfigOriginJson = {
	final kind:HxcConfigOriginKind;
	final detail:String;
	final priority:Int;
}

/** One inspectable source for one resolved configuration value. */
class HxcConfigOrigin {
	public final kind:HxcConfigOriginKind;
	public final detail:String;
	public final priority:Int;

	public function new(kind:HxcConfigOriginKind, detail:String) {
		if (!validDetail(detail)) {
			throw new HxcConfigError('configuration provenance detail is unsafe or empty: `$detail`');
		}
		this.kind = kind;
		this.detail = detail;
		this.priority = priorityFor(kind);
	}

	public function toJsonValue():HxcConfigOriginJson
		return {kind: kind, detail: detail, priority: priority};

	static function priorityFor(kind:HxcConfigOriginKind):Int {
		return switch kind {
			case CompilerDefault | ProfilePreset: 0;
			case EnvironmentPreset: 1;
			case ProjectFile: 2;
			case NamedOverlay: 3;
			case CliFlag: 4;
			case DirectDefine: 5;
			case _: throw new HxcConfigError('unknown configuration provenance kind `$kind`');
		};
	}

	static function validDetail(value:String):Bool {
		if (value == null || value == "" || StringTools.trim(value) != value || StringTools.startsWith(value, "/") || value.indexOf("\\") != -1
			|| value.indexOf("\x00") != -1 || ~/^[A-Za-z]:/.match(value)) {
			return false;
		}
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				return false;
			}
		}
		return true;
	}
}
