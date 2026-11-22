package reflaxe.c.emit;

#if macro
import haxe.io.Path;
import haxe.crypto.Sha256;
import haxe.io.Bytes;
import haxe.macro.Context;
import hxc.config.HxcJsonCodec.objectField;
import hxc.config.HxcJsonParser;
import hxc.config.HxcJsonValue.HxcJsonNode;
import sys.io.File;
#end

/**
	Loads generated-output license facts from the authoritative data ledger.

	The compiler projects these facts into application manifests. It does not
	keep a second hand-authored table of license expressions or notice paths.
**/
typedef COutputLicenseRule = {
	final licenseConcluded:String;
	final projectCopyrightClaim:Bool;
}

/** Validated policy facts needed by deterministic project emission. */
typedef COutputLicensePolicySnapshot = {
	final decision:String;
	final spdxLicenseListVersion:String;
	final outputSupportLicenseExpression:String;
	final outputSupportLicenseFile:String;
	final outputSupportLicenseText:String;
	final compiledProgram:COutputLicenseRule;
	final compilerSidecars:COutputLicenseRule;
	final repositoryCopies:COutputLicenseRule;
}

#if macro
/** Macro-host boundary that parses and validates the repository policy once. */
class COutputLicensePolicyLoader {
	static var cached:Null<COutputLicensePolicySnapshot> = null;

	/** Parse, confine, and content-check the policy and its required notice. */
	public static function load():COutputLicensePolicySnapshot {
		if (cached != null)
			return cached;
		final modulePath = Context.resolvePath("reflaxe/c/emit/COutputLicensePolicy.hx");
		final repositoryRoot = Path.normalize(Path.join([Path.directory(modulePath), "..", "..", "..", ".."]));
		final policyPath = Path.join([repositoryRoot, "docs", "specs", "third-party-provenance.json"]);
		final document = new HxcJsonParser(File.getContent(policyPath), policyPath).parse();
		final policy = field(document, "policy");
		final output = field(document, "generatedOutput");
		final decision = text(field(policy, "generatedOutputDecision"), "generated-output decision");
		final spdxVersion = text(field(output, "spdxLicenseListVersion"), "SPDX license-list version");
		final supportExpression = text(field(output, "outputSupportLicenseExpression"), "output-support license expression");
		final supportFile = text(field(output, "outputSupportLicenseFile"), "output-support license file");
		if (!GeneratedFile.isNormalizedRelativePath(supportFile) || !StringTools.startsWith(supportFile, "LICENSES/"))
			invalid("output-support license file must stay under LICENSES/");
		final supportHash = text(field(output, "outputSupportLicenseSha256"), "output-support license hash");
		final supportPath = Path.normalize(Path.join([repositoryRoot, supportFile]));
		final supportText = File.getContent(supportPath);
		if (Sha256.make(Bytes.ofString(supportText)).toHex() != supportHash)
			invalid("output-support license file does not match its reviewed hash");
		final compiledProgram = rule(field(output, "compiledProgramWithoutUserAssertion"), "compiled-program rule");
		final compilerSidecars = rule(field(output, "compilerSidecars"), "compiler-sidecar rule");
		final repositoryCopies = rule(field(output, "repositoryCopies"), "repository-copy rule");
		cached = {
			decision: decision,
			spdxLicenseListVersion: spdxVersion,
			outputSupportLicenseExpression: supportExpression,
			outputSupportLicenseFile: supportFile,
			outputSupportLicenseText: supportText,
			compiledProgram: compiledProgram,
			compilerSidecars: compilerSidecars,
			repositoryCopies: repositoryCopies
		};
		return cached;
	}

	static function rule(node:HxcJsonNode, label:String):COutputLicenseRule {
		final licenseConcluded = text(field(node, "licenseConcluded"), '$label license conclusion');
		final projectCopyrightClaim = switch field(node, "projectCopyrightClaim").value {
			case JBool(value): value;
			case _: invalid('$label project copyright claim must be boolean');
		};
		return {licenseConcluded: licenseConcluded, projectCopyrightClaim: projectCopyrightClaim};
	}

	static function field(node:HxcJsonNode, name:String):HxcJsonNode {
		final value = objectField(node, name);
		return value == null ? invalid('generated-output policy field `$name` is absent') : value;
	}

	static function text(node:HxcJsonNode, label:String):String {
		final value = switch node.value {
			case JString(found): found;
			case _: invalid('$label must be text in the generated-output policy');
		};
		if (value == "" || value.indexOf("\x00") != -1 || value.indexOf("\r") != -1 || value.indexOf("\n") != -1)
			invalid('$label is absent or unsafe in the generated-output policy');
		return value;
	}

	static function invalid<T>(detail:String):T
		throw new ProjectEmissionError(detail);
}
#end
