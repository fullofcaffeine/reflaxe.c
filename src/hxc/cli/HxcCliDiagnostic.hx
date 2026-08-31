package hxc.cli;

import reflaxe.c.CDiagnostic.CDiagnosticRelatedLocation;
import reflaxe.c.CDiagnostic.CDiagnosticRelatedLocationJson;
import reflaxe.c.CDiagnostic.CDiagnosticKind;
import reflaxe.c.CDiagnostic.CDiagnosticPhase;
import reflaxe.c.CDiagnostic.CDiagnosticRecord;
import reflaxe.c.CDiagnostic.CDiagnosticSeverity;
import reflaxe.c.CDiagnostic.CDiagnosticSourceJson;
import reflaxe.c.CDiagnostic.CDiagnosticSourceSpan;

/** Adds public-tool phases while accepting the compiler's typed phase directly. */
abstract HxcCliDiagnosticPhase(String) to String {
	inline function new(value:String)
		this = value;

	@:from public static inline function fromCompiler(value:CDiagnosticPhase):HxcCliDiagnosticPhase
		return new HxcCliDiagnosticPhase(value);

	public static final Cli = new HxcCliDiagnosticPhase("cli");
	public static final NativeTool = new HxcCliDiagnosticPhase("native-tool");
	public static final Bindgen = new HxcCliDiagnosticPhase("bindgen");
	public static final Test = new HxcCliDiagnosticPhase("test");
}

/** Names the producer and stream of one exact attached text value. */
enum abstract HxcCliDiagnosticAttachmentSource(String) to String {
	var Compiler = "compiler";
	var NativeTool = "native-tool";
	var Bindgen = "bindgen";
	var Abi = "abi";
	var Test = "test";
}

/** Distinguishes text streams without parsing their content. */
enum abstract HxcCliDiagnosticAttachmentChannel(String) to String {
	var Stdout = "stdout";
	var Stderr = "stderr";
	var Report = "report";
}

/** JSON command identity carried by every standalone diagnostic event. */
typedef HxcCliDiagnosticCommandJson = {
	final command:String;
	final arguments:Array<String>;
}

/** JSON attachment that preserves compiler, native, ABI, or test text exactly. */
typedef HxcCliDiagnosticAttachmentJson = {
	final source:HxcCliDiagnosticAttachmentSource;
	final channel:HxcCliDiagnosticAttachmentChannel;
	final tool:String;
	final text:String;
}

/** Schema-1 JSON shape for one standalone `hxc` diagnostic event. */
typedef HxcCliDiagnosticJson = {
	final schemaVersion:Int;
	final id:String;
	final severity:CDiagnosticSeverity;
	final phase:HxcCliDiagnosticPhase;
	final profile:Null<String>;
	final kind:Null<CDiagnosticKind>;
	final message:String;
	final remediation:String;
	final source:Null<CDiagnosticSourceJson>;
	final context:Null<String>;
	final notes:Array<String>;
	final relatedLocations:Array<CDiagnosticRelatedLocationJson>;
	final commandContext:HxcCliDiagnosticCommandJson;
	final attachments:Array<HxcCliDiagnosticAttachmentJson>;
}

/** Named input for one diagnostic, which avoids ambiguous positional metadata. */
typedef HxcCliDiagnosticSpec = {
	final id:String;
	final severity:CDiagnosticSeverity;
	final phase:HxcCliDiagnosticPhase;
	final message:String;
	final remediation:String;
	final commandContext:HxcCliDiagnosticCommand;
	final ?profile:String;
	final ?kind:CDiagnosticKind;
	final ?source:CDiagnosticSourceSpan;
	final ?context:String;
	final ?notes:Array<String>;
	final ?relatedLocations:Array<CDiagnosticRelatedLocation>;
	final ?attachments:Array<HxcCliDiagnosticAttachment>;
}

/** Copy-owned command name and arguments that make one event reproducible. */
class HxcCliDiagnosticCommand {
	public final command:String;
	public final arguments:Array<String>;

	public function new(command:String, arguments:Array<String>) {
		if (StringTools.trim(command) == "" || command.indexOf("\x00") >= 0)
			throw "CLI diagnostic command must be non-empty and contain no NUL";
		for (argument in arguments)
			if (argument.indexOf("\x00") >= 0)
				throw "CLI diagnostic arguments must contain no NUL";
		this.command = command;
		this.arguments = arguments.copy();
	}

	public function toJsonValue():HxcCliDiagnosticCommandJson
		return {command: command, arguments: arguments.copy()};
}

/** One copy-owned text attachment whose line breaks and trailing newline remain exact. */
class HxcCliDiagnosticAttachment {
	public final source:HxcCliDiagnosticAttachmentSource;
	public final channel:HxcCliDiagnosticAttachmentChannel;
	public final tool:String;
	public final text:String;

	public function new(source:HxcCliDiagnosticAttachmentSource, channel:HxcCliDiagnosticAttachmentChannel, tool:String, text:String) {
		if (StringTools.trim(tool) == "" || text == "")
			throw "CLI diagnostic attachments require a tool and exact non-empty text";
		this.source = source;
		this.channel = channel;
		this.tool = tool;
		this.text = text;
	}

	public function toJsonValue():HxcCliDiagnosticAttachmentJson
		return {
			source: source,
			channel: channel,
			tool: tool,
			text: text
		};
}

/** One validated event with unchanged one-line human rendering. */
class HxcCliDiagnostic {
	public static inline final SCHEMA_VERSION = 1;

	public final id:String;
	public final severity:CDiagnosticSeverity;
	public final phase:HxcCliDiagnosticPhase;
	public final profile:Null<String>;
	public final kind:Null<CDiagnosticKind>;
	public final message:String;
	public final remediation:String;
	public final source:Null<CDiagnosticSourceSpan>;
	public final context:Null<String>;
	public final notes:Array<String>;
	public final relatedLocations:Array<CDiagnosticRelatedLocation>;
	public final commandContext:HxcCliDiagnosticCommand;
	public final attachments:Array<HxcCliDiagnosticAttachment>;

	public function new(spec:HxcCliDiagnosticSpec) {
		if (!~/^(HXC[0-9]{4}|HXC-CLI-[0-9]{4})$/.match(spec.id)
			|| StringTools.trim(spec.message) == ""
			|| StringTools.trim(spec.remediation) == "")
			throw "CLI diagnostics require a stable ID, message, and remediation";
		this.id = spec.id;
		this.severity = spec.severity;
		this.phase = spec.phase;
		this.profile = spec.profile;
		this.kind = spec.kind;
		this.message = spec.message;
		this.remediation = spec.remediation;
		this.source = spec.source;
		this.context = spec.context;
		this.notes = spec.notes == null ? [] : spec.notes.copy();
		this.relatedLocations = spec.relatedLocations == null ? [] : spec.relatedLocations.copy();
		this.commandContext = spec.commandContext;
		this.attachments = spec.attachments == null ? [] : spec.attachments.copy();
		for (note in notes)
			if (StringTools.trim(note) == "")
				throw "CLI diagnostic notes must not contain empty entries";
	}

	/** Adapt one compiler-owned record without copying its catalog or ID policy. */
	public static function fromCompiler(record:CDiagnosticRecord, commandContext:HxcCliDiagnosticCommand,
			?attachments:Array<HxcCliDiagnosticAttachment>):HxcCliDiagnostic
		return new HxcCliDiagnostic({
			id: record.id,
			severity: record.severity,
			phase: record.phase,
			profile: record.profile,
			kind: record.kind,
			message: record.message,
			remediation: record.remediation,
			source: record.source,
			context: record.context == "" ? null : record.context,
			notes: record.notes,
			relatedLocations: record.relatedLocations,
			commandContext: commandContext,
			attachments: attachments
		});

	/** Render the existing one-line human contract without attached machine text. */
	public function render():String
		return '$id: $message Remediation: $remediation';

	/** Return the standalone schema value used in the ordered response event stream. */
	public function toJsonValue():HxcCliDiagnosticJson {
		return {
			schemaVersion: SCHEMA_VERSION,
			id: id,
			severity: severity,
			phase: phase,
			profile: profile,
			kind: kind,
			message: message,
			remediation: remediation,
			source: source == null ? null : source.toJsonValue(),
			context: context,
			notes: notes.copy(),
			relatedLocations: relatedLocations.map(location -> location.toJsonValue()),
			commandContext: commandContext.toJsonValue(),
			attachments: attachments.map(attachment -> attachment.toJsonValue())
		};
	}
}
