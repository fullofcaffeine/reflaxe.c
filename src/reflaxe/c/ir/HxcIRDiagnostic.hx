package reflaxe.c.ir;

enum HxcIRDiagnosticSeverity {
	IRDError;
	IRDWarning;
	IRDInfo;
}

/** A stable, source-positioned diagnostic produced at the HxcIR boundary. */
class HxcIRDiagnostic {
	public final id:String;
	public final severity:HxcIRDiagnosticSeverity;
	public final profile:String;
	public final irPath:String;
	public final message:String;
	public final remediation:String;
	public final source:HxcSourceSpan;

	public function new(id:String, severity:HxcIRDiagnosticSeverity, profile:String, irPath:String, message:String, remediation:String, source:HxcSourceSpan) {
		this.id = id;
		this.severity = severity;
		this.profile = profile;
		this.irPath = irPath;
		this.message = message;
		this.remediation = remediation;
		this.source = source;
	}

	/**
		The typed-AST adapter must return this diagnostic instead of manufacturing
		an opaque IR value for an unhandled expression family.
	 */
	public static function unsupportedTypedAstNode(profile:String, nodeKind:String, context:String, source:HxcSourceSpan):HxcIRDiagnostic {
		final normalizedKind = nodeKind == "" ? "<unknown>" : nodeKind;
		final normalizedContext = context == "" ? "typed expression" : context;
		return new HxcIRDiagnostic("HXC1001", IRDError, profile, normalizedContext,
			'Unsupported typed Haxe node `$normalizedKind` while lowering $normalizedContext.',
			"Use a supported construct or implement its typed IR and C AST lowering with semantic tests.", source.stableForDiagnostic());
	}

	public static function invalidIR(profile:String, irPath:String, message:String, source:HxcSourceSpan):HxcIRDiagnostic {
		return new HxcIRDiagnostic("HXC9000", IRDError, profile, irPath, 'Invalid HxcIR: $message',
			"Report a minimized reproducer, compiler/version manifest, and IR dump; do not bypass validation with raw C.", source.stableForDiagnostic());
	}

	public function render():String {
		final severityName = switch severity {
			case IRDError: "error";
			case IRDWarning: "warning";
			case IRDInfo: "info";
		}
		return '$id $severityName [profile=$profile] ${source.display()} [$irPath] $message Remediation: $remediation';
	}
}
