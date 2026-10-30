import hxc.cli.HxcCli;
import hxc.cli.HxcCliDiagnostic;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticAttachment;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticAttachmentChannel;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticAttachmentSource;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticCommand;
import hxc.cli.HxcCliDiagnostic.HxcCliDiagnosticPhase;
import hxc.cli.HxcCliExecution;
import hxc.cli.HxcCliExecutor;
import hxc.cli.HxcCliExitCategory;
import hxc.cli.HxcCliRequest;
import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CDiagnostic.CDiagnosticRelatedLocation;
import reflaxe.c.CDiagnostic.CDiagnosticSeverity;
import reflaxe.c.CDiagnostic.CDiagnosticSourceSpan;

/** Eval probe that gives the router a deterministic child-process observer. */
class CliProbe {
	static function main():Void {
		final response = new HxcCli(new ProbeExecutor()).route(Sys.args());
		Sys.stdout().writeString(response.renderStdout());
		Sys.stderr().writeString(response.renderStderr());
		Sys.exit(response.exitCode);
	}
}

/** Synthetic command owner used only to prove exact output and exit propagation. */
private class ProbeExecutor implements HxcCliExecutor {
	public function new() {}

	/** Admit every routed product command for exact execution propagation tests. */
	public function isAvailable(command:hxc.cli.HxcCliCommand):Bool
		return true;

	public function execute(request:HxcCliRequest):HxcCliExecution {
		final command:String = request.command;
		final childStderr = "native line one\nnative line two\n";
		final commandContext = new HxcCliDiagnosticCommand(request.command, request.arguments);
		final compilerSource = new CDiagnosticSourceSpan("test/hxc_cli/CliProbe.hx", 25, 1, 25, 2);
		final compilerRecord = CDiagnostic.create(CDiagnosticId.UnsupportedExpression, "portable", "synthetic compiler failure", compilerSource, null,
			"synthetic compiler context", ["The compiler note remains structured."], [
				new CDiagnosticRelatedLocation("synthetic compiler owner", new CDiagnosticSourceSpan("test/hxc_cli/CliProbe.hx", 21, 1, 21, 2))
			]);
		return new HxcCliExecution(HxcCliExitCategory.Command, 23, 'child-out:$command:${request.arguments.join("|")}\n', childStderr, "SIGTERM",
			["probe-log"], [
				new HxcCliDiagnostic({
					id: "HXC-CLI-0099",
					severity: CDiagnosticSeverity.Error,
					phase: HxcCliDiagnosticPhase.NativeTool,
					message: "synthetic child failure",
					remediation: "Inspect the propagated child result.",
					source: new CDiagnosticSourceSpan("test/hxc_cli/CliProbe.hx", 25, 1, 25, 2),
					notes: ["The fixture keeps this note separate from native stderr."],
					relatedLocations: [
						new CDiagnosticRelatedLocation("synthetic command owner", new CDiagnosticSourceSpan("test/hxc_cli/CliProbe.hx", 21, 1, 21, 2))
					],
					commandContext: commandContext,
					attachments: [
						new HxcCliDiagnosticAttachment(HxcCliDiagnosticAttachmentSource.NativeTool, HxcCliDiagnosticAttachmentChannel.Stderr,
							"synthetic-native-tool", childStderr)
					]
				}),
				HxcCliDiagnostic.fromCompiler(compilerRecord, commandContext)
			]);
	}
}
