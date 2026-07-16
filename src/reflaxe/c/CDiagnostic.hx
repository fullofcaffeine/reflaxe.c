package reflaxe.c;

import haxe.Json;
#if macro
import haxe.macro.Context;
import haxe.macro.Expr.Position;
#end

/** Stable machine identifiers admitted by `docs/specs/diagnostics.json`. */
enum abstract CDiagnosticId(String) to String {
	var FrameworkUnavailable = "HXC0002";
	var InvalidConfiguration = "HXC0003";
	var LoweringNotImplemented = "HXC1000";
	var UnsupportedExpression = "HXC1001";
	var RuntimePolicyViolation = "HXC2000";
	var RuntimeFeatureSelected = "HXC2001";
	var InvalidAbiBoundary = "HXC3000";
	var RawCForbiddenInExample = "HXC4001";
	var RawCForbiddenInStrictMode = "HXC4002";
	var BootstrapMacroContract = "HXC5001";
	var InvalidTypedCContract = "HXC5002";
	var InternalCompilerError = "HXC9000";
}

enum abstract CDiagnosticSeverity(String) to String {
	var Error = "error";
	var Warning = "warning";
	var Info = "info";
}

enum abstract CDiagnosticPhase(String) to String {
	var Bootstrap = "bootstrap";
	var Configuration = "configuration";
	var Lowering = "lowering";
	var RuntimeAnalysis = "runtime-analysis";
	var Abi = "abi";
	var Interop = "interop";
	var TargetContract = "target-contract";
	var Internal = "internal";
}

enum abstract CDiagnosticKind(String) to String {
	var DependencyFailure = "dependency-failure";
	var InvalidConfigurationKind = "invalid-configuration";
	var CompilerCapability = "compiler-capability";
	var UnsupportedSource = "unsupported-source";
	var RuntimePolicy = "runtime-policy";
	var RuntimeSelection = "runtime-selection";
	var InvalidAbi = "invalid-abi";
	var UnsafeBoundary = "unsafe-boundary";
	var InvalidTargetContract = "invalid-target-contract";
	var InternalCompilerFailure = "internal-compiler-failure";
}

typedef CDiagnosticDefinition = {
	final id:CDiagnosticId;
	final name:String;
	final defaultSeverity:CDiagnosticSeverity;
	final allowedSeverities:Array<CDiagnosticSeverity>;
	final phase:CDiagnosticPhase;
	final kind:CDiagnosticKind;
	final sourcePolicy:String;
	final idStability:String;
	final summary:String;
	final remediation:String;
}

typedef CDiagnosticJsonPoint = {
	final line:Int;
	final column:Int;
}

typedef CDiagnosticSourceJson = {
	final file:String;
	final start:CDiagnosticJsonPoint;
	final end:CDiagnosticJsonPoint;
}

typedef CDiagnosticRelatedLocationJson = {
	final label:String;
	final source:CDiagnosticSourceJson;
}

typedef CDiagnosticRecordJson = {
	final schemaVersion:Int;
	final id:CDiagnosticId;
	final severity:CDiagnosticSeverity;
	final phase:CDiagnosticPhase;
	final kind:CDiagnosticKind;
	final profile:String;
	final message:String;
	final remediation:String;
	final source:CDiagnosticSourceJson;
	final context:String;
	final notes:Array<String>;
	final relatedLocations:Array<CDiagnosticRelatedLocationJson>;
}

typedef CDiagnosticDefinitionJson = {
	final id:CDiagnosticId;
	final name:String;
	final defaultSeverity:CDiagnosticSeverity;
	final allowedSeverities:Array<CDiagnosticSeverity>;
	final phase:CDiagnosticPhase;
	final kind:CDiagnosticKind;
	final sourcePolicy:String;
	final idStability:String;
	final summary:String;
	final remediation:String;
}

/** Repository-relative, one-based, end-exclusive source range. */
class CDiagnosticSourceSpan {
	public final file:String;
	public final startLine:Int;
	public final startColumn:Int;
	public final endLine:Int;
	public final endColumn:Int;

	public function new(file:String, startLine:Int, startColumn:Int, endLine:Int, endColumn:Int) {
		if (!isNormalizedFile(file)) {
			throw 'diagnostic source path must be normalized and repository-relative: `$file`';
		}
		if (startLine < 1
			|| startColumn < 1
			|| endLine < startLine
			|| endColumn < 1
			|| (endLine == startLine && endColumn < startColumn)) {
			throw 'diagnostic source range is malformed: $file:$startLine:$startColumn-$endLine:$endColumn';
		}
		this.file = file;
		this.startLine = startLine;
		this.startColumn = startColumn;
		this.endLine = endLine;
		this.endColumn = endColumn;
	}

	public function display():String
		return '$file:$startLine:$startColumn-$endLine:$endColumn';

	public function toJsonValue():CDiagnosticSourceJson {
		return {
			file: file,
			start: {line: startLine, column: startColumn},
			end: {line: endLine, column: endColumn}
		};
	}

	public static function isNormalizedFile(value:String):Bool {
		if (value == "" || StringTools.startsWith(value, "/") || StringTools.startsWith(value, "~") || value.indexOf("\\") != -1
			|| value.indexOf("\x00") != -1 || ~/^[A-Za-z]:/.match(value)) {
			return false;
		}
		for (part in value.split("/")) {
			if (part == "" || part == "." || part == "..") {
				return false;
			}
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

class CDiagnosticRelatedLocation {
	public final label:String;
	public final source:CDiagnosticSourceSpan;

	public function new(label:String, source:CDiagnosticSourceSpan) {
		if (StringTools.trim(label) == "") {
			throw "diagnostic related-location label must not be empty";
		}
		this.label = label;
		this.source = source;
	}

	public function toJsonValue():CDiagnosticRelatedLocationJson
		return {label: label, source: source.toJsonValue()};
}

/** A deterministic schema-1 diagnostic value; stream framing remains E8.T09. */
class CDiagnosticRecord {
	public final id:CDiagnosticId;
	public final severity:CDiagnosticSeverity;
	public final phase:CDiagnosticPhase;
	public final kind:CDiagnosticKind;
	public final profile:String;
	public final message:String;
	public final remediation:String;
	public final source:CDiagnosticSourceSpan;
	public final context:String;
	public final notes:Array<String>;
	public final relatedLocations:Array<CDiagnosticRelatedLocation>;

	public function new(id:CDiagnosticId, severity:CDiagnosticSeverity, profile:String, message:String, source:CDiagnosticSourceSpan, ?context:String,
			?notes:Array<String>, ?relatedLocations:Array<CDiagnosticRelatedLocation>) {
		final definition = CDiagnostic.definition(id);
		CDiagnostic.requireAllowedSeverity(definition, severity);
		if (StringTools.trim(message) == "") {
			throw "diagnostic message must not be empty";
		}
		this.id = id;
		this.severity = severity;
		this.phase = definition.phase;
		this.kind = definition.kind;
		this.profile = CDiagnostic.normalizeProfile(profile);
		this.message = message;
		this.remediation = definition.remediation;
		this.source = source;
		this.context = context == null ? "" : context;
		this.notes = notes == null ? [] : notes.copy();
		this.relatedLocations = relatedLocations == null ? [] : relatedLocations.copy();
		for (note in this.notes) {
			if (StringTools.trim(note) == "") {
				throw "diagnostic notes must not contain empty entries";
			}
		}
	}

	public function render():String {
		final contextSuffix = context == "" ? "" : ' [$context]';
		return '$id $severity [profile=$profile] ${source.display()}$contextSuffix $message Remediation: $remediation';
	}

	public function toJsonValue():CDiagnosticRecordJson {
		return {
			schemaVersion: CDiagnostic.EVENT_SCHEMA_VERSION,
			id: id,
			severity: severity,
			phase: phase,
			kind: kind,
			profile: profile,
			message: message,
			remediation: remediation,
			source: source.toJsonValue(),
			context: context,
			notes: notes.copy(),
			relatedLocations: relatedLocations.map(location -> location.toJsonValue())
		};
	}

	public function toJson():String
		return Json.stringify(toJsonValue());
}

/** Typed emission and serialization boundary for every target-owned diagnostic. */
class CDiagnostic {
	public static inline final EVENT_SCHEMA_VERSION = 1;

	public static function definitions():Array<CDiagnosticDefinition> {
		return [
			definition(FrameworkUnavailable),
			definition(InvalidConfiguration),
			definition(LoweringNotImplemented),
			definition(UnsupportedExpression),
			definition(RuntimePolicyViolation),
			definition(RuntimeFeatureSelected),
			definition(InvalidAbiBoundary),
			definition(RawCForbiddenInExample),
			definition(RawCForbiddenInStrictMode),
			definition(BootstrapMacroContract),
			definition(InvalidTypedCContract),
			definition(InternalCompilerError)
		];
	}

	public static function definition(id:CDiagnosticId):CDiagnosticDefinition {
		return switch id {
			case FrameworkUnavailable:
				makeDefinition(id, "framework-unavailable", Error, [Error], Bootstrap, DependencyFailure,
					"The Reflaxe framework could not be resolved during compiler bootstrap.",
					"Use the scoped library HXML, pin or vendor Reflaxe, or install the compatible library revision.");
			case InvalidConfiguration:
				makeDefinition(id, "invalid-configuration", Error, [Error], Configuration, InvalidConfigurationKind,
					"A target identity, carrier capability, profile, runtime, environment, standard, or related build option is invalid or contradictory.",
					"Correct the named option or carrier mismatch; use `hxc inspect build` once M8 implements configuration inspection.");
			case LoweringNotImplemented:
				makeDefinition(id, "lowering-not-implemented", Error, [Error], Lowering, CompilerCapability,
					"The scaffold reached a language-lowering path that is intentionally not implemented.",
					"Implement and test the corresponding Beads task; scaffold output is never executable program output.");
			case UnsupportedExpression:
				makeDefinition(id, "unsupported-expression", Error, [Error], Lowering, UnsupportedSource,
					"A typed Haxe expression has no supported C lowering.",
					"Use a supported construct or implement its typed IR and C AST lowering with semantic tests.");
			case RuntimePolicyViolation:
				makeDefinition(id, "runtime-policy-violation", Error, [Error], RuntimeAnalysis, RuntimePolicy,
					"Source semantics require a runtime feature forbidden by the selected runtime policy.",
					"Remove the source requirement, select an allowed runtime policy, or provide an explicit metal implementation.");
			case RuntimeFeatureSelected:
				makeDefinition(id, "runtime-feature-selected", Info, [Info, Warning], RuntimeAnalysis, RuntimeSelection,
					"A root source semantic requirement selected an hxrt feature after direct C and local specialization were ineligible.",
					"Inspect hxc.runtime-plan.json; accept the explained feature, choose a semantics-preserving typed C alternative, or select a stricter runtime policy.");
			case InvalidAbiBoundary:
				makeDefinition(id, "invalid-abi-boundary", Error, [Error], Abi, InvalidAbi,
					"An exported or imported declaration cannot be represented by the selected stable C ABI contract.",
					"Use fixed ABI types, opaque handles, views, callbacks with context, and explicit status or error outputs.");
			case RawCForbiddenInExample:
				makeDefinition(id, "raw-c-forbidden-in-example", Error, [Error], Interop, UnsafeBoundary,
					"An example or snapshot uses raw `__c__()` target-code injection.", "Add a typed `c.*` facade or compiler lowering instead.");
			case RawCForbiddenInStrictMode:
				makeDefinition(id, "raw-c-forbidden-in-strict-mode", Error, [Error], Interop, UnsafeBoundary,
					"Strict mode found application-side raw `__c__()` target-code injection.",
					"Use typed interop, a reviewed framework macro, or disable strict mode only for an isolated migration fixture.");
			case BootstrapMacroContract:
				makeDefinition(id, "bootstrap-macro-contract", Error, [Error], TargetContract, InvalidTargetContract,
					"A public target macro is present as an API contract but its validated implementation is not complete.",
					"Complete the owning Beads milestone before using the macro in production code.");
			case InvalidTypedCContract:
				makeDefinition(id, "invalid-typed-c-contract", Error, [Error], TargetContract, InvalidTargetContract,
					"A typed C declaration, header, layout, ownership, build, macro, or DSL contract is invalid for the resolved target configuration.",
					"Correct the reported typed declaration or metadata at its Haxe source span; do not bypass validation with raw C.");
			case InternalCompilerError:
				makeDefinition(id, "internal-compiler-error", Error, [Error], Internal, InternalCompilerFailure,
					"A required compiler lifecycle invariant or internal object is missing or malformed.",
					"Report a minimized reproducer, compiler/version manifest, and the relevant stack trace or IR dump; do not bypass validation with raw C.");
			case _:
				throw 'unregistered diagnostic ID `$id`';
		};
	}

	public static function create(id:CDiagnosticId, profile:String, message:String, source:CDiagnosticSourceSpan, ?severity:CDiagnosticSeverity,
			?context:String, ?notes:Array<String>, ?relatedLocations:Array<CDiagnosticRelatedLocation>):CDiagnosticRecord {
		final definition = definition(id);
		return new CDiagnosticRecord(id, severity == null ? definition.defaultSeverity : severity, profile, message, source, context, notes, relatedLocations);
	}

	public static function catalogSnapshot():Array<CDiagnosticDefinitionJson> {
		return definitions().map(definition -> {
			id: definition.id,
			name: definition.name,
			defaultSeverity: definition.defaultSeverity,
			allowedSeverities: definition.allowedSeverities.copy(),
			phase: definition.phase,
			kind: definition.kind,
			sourcePolicy: definition.sourcePolicy,
			idStability: definition.idStability,
			summary: definition.summary,
			remediation: definition.remediation
		});
	}

	/** Stable code prefix without prose contracts, for inspectable exception values. */
	public static function codeMessage(id:CDiagnosticId, detail:String):String
		return '$id: $detail';

	/** Human compiler text. Consumers parse the ID, never the prose or Haxe formatting. */
	public static function humanMessage(id:CDiagnosticId, detail:String, ?profile:String):String {
		final registered = definition(id);
		return '${codeMessage(id, detail)} [profile=${currentProfile(profile)}] Remediation: ${registered.remediation}';
	}

	#if macro
	public static function fatal<T>(id:CDiagnosticId, detail:String, pos:Position, ?profile:String):T {
		requireAllowedSeverity(definition(id), Error);
		Context.fatalError(humanMessage(id, detail, profile), pos);
		throw new haxe.Exception("Context.fatalError returned after a fatal compiler diagnostic");
	}

	public static function error(id:CDiagnosticId, detail:String, pos:Position, ?profile:String):Void {
		requireAllowedSeverity(definition(id), Error);
		Context.error(humanMessage(id, detail, profile), pos);
	}

	public static function warning(id:CDiagnosticId, detail:String, pos:Position, ?profile:String):Void {
		requireAllowedSeverity(definition(id), Warning);
		Context.warning(humanMessage(id, detail, profile), pos);
	}

	public static function info(id:CDiagnosticId, detail:String, pos:Position, ?profile:String):Void {
		requireAllowedSeverity(definition(id), Info);
		Context.info(humanMessage(id, detail, profile), pos);
	}
	#end

	public static function normalizeProfile(value:String):String {
		return switch value {
			case "portable" | "metal" | "unresolved": value;
			case _: throw 'invalid diagnostic profile `$value`; expected portable, metal, or unresolved';
		};
	}

	public static function requireAllowedSeverity(definition:CDiagnosticDefinition, severity:CDiagnosticSeverity):Void {
		if (!definition.allowedSeverities.contains(severity)) {
			throw 'diagnostic ${definition.id} does not allow severity `$severity`';
		}
	}

	static function currentProfile(explicit:Null<String>):String {
		if (explicit != null) {
			return normalizeProfile(explicit);
		}
		#if macro
		final configured = Context.definedValue("reflaxe_c_profile");
		return switch configured {
			case null | "" | "portable": "portable";
			case "metal": "metal";
			case _: "unresolved";
		};
		#else
		return "portable";
		#end
	}

	static function makeDefinition(id:CDiagnosticId, name:String, defaultSeverity:CDiagnosticSeverity, allowedSeverities:Array<CDiagnosticSeverity>,
			phase:CDiagnosticPhase, kind:CDiagnosticKind, summary:String, remediation:String):CDiagnosticDefinition {
		return {
			id: id,
			name: name,
			defaultSeverity: defaultSeverity,
			allowedSeverities: allowedSeverities,
			phase: phase,
			kind: kind,
			sourcePolicy: "required",
			idStability: "stable",
			summary: summary,
			remediation: remediation
		};
	}
}
