package reflaxe.c.ir;

import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CDiagnostic.CDiagnosticSeverity;

/** A stable, source-positioned diagnostic produced at the HxcIR boundary. */
class HxcIRDiagnostic {
	public final id:CDiagnosticId;
	public final severity:CDiagnosticSeverity;
	public final profile:String;
	public final irPath:String;
	public final message:String;
	public final remediation:String;
	public final source:HxcSourceSpan;

	public function new(id:CDiagnosticId, severity:CDiagnosticSeverity, profile:String, irPath:String, message:String, remediation:String,
			source:HxcSourceSpan) {
		CDiagnostic.requireAllowedSeverity(CDiagnostic.definition(id), severity);
		this.id = id;
		this.severity = severity;
		this.profile = CDiagnostic.normalizeProfile(profile);
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
		return new HxcIRDiagnostic(CDiagnosticId.UnsupportedExpression, CDiagnosticSeverity.Error, profile, normalizedContext,
			'Unsupported typed Haxe node `$normalizedKind` while lowering $normalizedContext.',
			"Use a supported construct or implement its typed IR and C AST lowering with semantic tests.", source.stableForDiagnostic());
	}

	public static function invalidIR(profile:String, irPath:String, message:String, source:HxcSourceSpan):HxcIRDiagnostic {
		return new HxcIRDiagnostic(CDiagnosticId.InternalCompilerError, CDiagnosticSeverity.Error, profile, irPath, 'Invalid HxcIR: $message',
			"Report a minimized reproducer, compiler/version manifest, and IR dump; do not bypass validation with raw C.", source.stableForDiagnostic());
	}

	public function render():String {
		return '$id $severity [profile=$profile] ${source.display()} [$irPath] $message Remediation: $remediation';
	}
}
