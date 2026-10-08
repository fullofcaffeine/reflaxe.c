package hxc.inspect;

import haxe.crypto.Sha256;
import haxe.io.Bytes;
import hxc.config.HxcJsonCodec.jsonArray;
import hxc.config.HxcJsonCodec.jsonBool;
import hxc.config.HxcJsonCodec.jsonField;
import hxc.config.HxcJsonCodec.jsonInt;
import hxc.config.HxcJsonCodec.jsonObject;
import hxc.config.HxcJsonCodec.jsonString;
import hxc.config.HxcJsonCodec.objectField;
import hxc.config.HxcJsonCodec.redactHostPaths;
import hxc.config.HxcJsonParser;
import hxc.config.HxcJsonValue.HxcJsonField;
import hxc.config.HxcJsonValue.HxcJsonNode;
import sys.FileSystem;
import sys.io.File;

/** One manifest-owned artifact after confinement and content-address validation. */
private class HxcInspectedArtifact {
	public final path:String;
	public final kind:String;
	public final sha256:String;
	public final bytes:Bytes;

	public function new(path:String, kind:String, sha256:String, bytes:Bytes) {
		this.path = path;
		this.kind = kind;
		this.sha256 = sha256;
		this.bytes = bytes;
	}
}

/**
	Read one compiler manifest and expose deterministic inspection projections.

	Construction validates every referenced artifact before any report is shown.
	This makes a stale, traversing, symlink-escaped, or modified build fail closed
	instead of combining facts from different compiler runs.
**/
class HxcInspectManifest {
	public static inline final REPORT_SCHEMA_VERSION = 1;

	final manifestPath:String;
	final rootPath:String;
	final manifest:HxcJsonNode;
	final artifacts:Array<HxcInspectedArtifact>;

	public function new(requestedManifestPath:String) {
		if (!FileSystem.exists(requestedManifestPath) || FileSystem.isDirectory(requestedManifestPath))
			fail('manifest `$requestedManifestPath` is not a readable file', "Build the project or pass `--manifest <path>`.");
		manifestPath = FileSystem.fullPath(requestedManifestPath);
		rootPath = FileSystem.fullPath(haxe.io.Path.directory(manifestPath));
		manifest = parseJson(File.getContent(manifestPath), "hxc.manifest.json");
		requireNumber(requireField(manifest, "schemaVersion"), "schemaVersion", "1");
		artifacts = loadArtifacts(requireField(manifest, "artifacts"));
	}

	/** Build a schema-versioned view without changing any compiler artifact. */
	public function report(report:HxcInspectReport, showSensitive:Bool):HxcJsonNode {
		final data = project(report);
		return jsonObject([
			jsonField("schemaVersion", jsonInt(REPORT_SCHEMA_VERSION)),
			jsonField("report", jsonString(report)),
			jsonField("redacted", jsonBool(!showSensitive)),
			jsonField("manifest", jsonString(showSensitive ? manifestPath : "$PROJECT/hxc.manifest.json")),
			jsonField("data", showSensitive ? data : redactHostPaths(data))
		]);
	}

	function project(report:HxcInspectReport):HxcJsonNode {
		return switch report {
			case Manifest: manifest;
			case Config: requireField(manifest, "configuration");
			case TypedInventory: artifactJson("typed-inventory");
			case HxcIR: artifactJson("hxcir");
			case CAst: artifactJson("c-ast");
			case Runtime: artifactJson("runtime-plan");
			case Symbols: artifactJson("symbol-table");
			case Abi: artifactJson("abi-manifest");
			case Stdlib: artifactJson("stdlib-report");
			case Declarations: artifactJson("declaration-report");
			case Build: requireField(manifest, "build");
			case Includes:
				final build = requireField(manifest, "build");
				jsonObject([
					jsonField("includeDirectories", requireField(build, "includeDirectories")),
					jsonField("requiredHeaders", requireField(build, "requiredHeaders")),
					jsonField("publicHeaders", requireField(build, "publicHeaders")),
					jsonField("privateHeaders", requireField(build, "privateHeaders")),
					jsonField("runtimeHeaders", requireField(build, "runtimeHeaders"))
				]);
			case Lowering:
				jsonObject([
					jsonField("hxcir", artifactJson("hxcir")),
					jsonField("cAst", artifactJson("c-ast")),
					jsonField("runtime", artifactJson("runtime-plan")),
					jsonField("dispatch", optionalArtifactJson("dispatch-report")),
					jsonField("initialization", optionalArtifactJson("initialization-plan")),
					jsonField("specializations", optionalArtifactJson("specialization-report"))
				]);
			case Macros:
				jsonObject([
					jsonField("typedProducts", artifactJson("declaration-report")),
					jsonField("typedDeclarationsAndMetadata", artifactJson("typed-inventory")),
					jsonField("specializations", optionalArtifactJson("specialization-report"))
				]);
			case Sizes: sizeReport();
			case All:
				final fields:Array<HxcJsonField> = [];
				for (name in HxcInspectReport.all())
					if (name != All)
						fields.push(jsonField(name, project(name)));
				jsonObject(fields);
		};
	}

	function sizeReport():HxcJsonNode {
		final values:Array<HxcJsonNode> = [];
		var total = 0;
		for (artifact in artifacts) {
			total += artifact.bytes.length;
			values.push(jsonObject([
				jsonField("path", jsonString(artifact.path)),
				jsonField("kind", jsonString(artifact.kind)),
				jsonField("bytes", jsonInt(artifact.bytes.length)),
				jsonField("sha256", jsonString(artifact.sha256))
			]));
		}
		return jsonObject([
			jsonField("artifactBytes", jsonInt(total)),
			jsonField("artifacts", jsonArray(values)),
			jsonField("allocationEvidence", artifactJson("runtime-plan"))
		]);
	}

	function loadArtifacts(node:HxcJsonNode):Array<HxcInspectedArtifact> {
		final records = switch node.value {
			case JArray(values): values;
			case _: return malformed("manifest `artifacts` must be an array");
		};
		final loaded:Array<HxcInspectedArtifact> = [];
		final seen:Map<String, Bool> = [];
		for (record in records) {
			final path = requireString(requireField(record, "path"), "artifact path");
			final kind = requireString(requireField(record, "kind"), "artifact kind");
			final expected = requireString(requireField(record, "sha256"), "artifact sha256");
			if (!isNormalizedRelativePath(path) || seen.exists(path))
				malformed('artifact path `$path` is unsafe or duplicated');
			if (!~/^[0-9a-f]{64}$/.match(expected))
				malformed('artifact `$path` has an invalid SHA-256 address');
			final requested = haxe.io.Path.join([rootPath, path]);
			if (!FileSystem.exists(requested) || FileSystem.isDirectory(requested))
				fail('manifest artifact `$path` is missing', "Rebuild before inspecting this output.");
			final resolved = FileSystem.fullPath(requested);
			if (resolved != rootPath && !StringTools.startsWith(resolved, rootPath + "/"))
				fail('manifest artifact `$path` escapes the output root through a symlink', "Remove the escaped artifact and rebuild.");
			final bytes = File.getBytes(resolved);
			final actual = Sha256.make(bytes).toHex();
			if (actual != expected)
				fail('manifest artifact `$path` does not match its SHA-256 address', "Rebuild before inspecting this output.");
			seen.set(path, true);
			loaded.push(new HxcInspectedArtifact(path, kind, expected, bytes));
		}
		return loaded;
	}

	function artifactJson(kind:String):HxcJsonNode {
		final artifact = findArtifact(kind);
		if (artifact == null)
			fail('inspection report requires the missing `$kind` artifact', kind == "typed-inventory"
				|| kind == "hxcir"
				|| kind == "c-ast"
				|| kind == "declaration-report" ? "Rebuild with `-D hxc_inspection_reports`." : "Rebuild the project with complete compiler reports.");
		return parseJson(artifact.bytes.toString(), artifact.path);
	}

	function optionalArtifactJson(kind:String):HxcJsonNode {
		final artifact = findArtifact(kind);
		return artifact == null ? new HxcJsonNode(JNull, 0, 0) : parseJson(artifact.bytes.toString(), artifact.path);
	}

	function findArtifact(kind:String):Null<HxcInspectedArtifact> {
		for (artifact in artifacts)
			if (artifact.kind == kind)
				return artifact;
		return null;
	}

	function parseJson(text:String, source:String):HxcJsonNode {
		try {
			return new HxcJsonParser(text, source).parse();
		} catch (error:hxc.config.HxcConfigError) {
			return malformed('$source is not valid deterministic JSON: ${error.message}');
		}
	}

	function requireField(node:HxcJsonNode, name:String):HxcJsonNode {
		final value = objectField(node, name);
		return value == null ? malformed('required JSON field `$name` is missing') : value;
	}

	function requireString(node:HxcJsonNode, label:String):String
		return switch node.value {
			case JString(value): value;
			case _: malformed('$label must be a string');
		};

	function requireNumber(node:HxcJsonNode, label:String, expected:String):Void {
		switch node.value {
			case JNumber(value) if (value == expected):
			case _:
				malformed('$label must equal $expected');
		}
	}

	function malformed<T>(message:String):T {
		throw new HxcInspectError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0702", message, "Rebuild with a compatible compiler and inspect again.");
	}

	function fail<T>(message:String, remediation:String):T
		throw new HxcInspectError(hxc.cli.HxcCliExitCategory.Command, "HXC-CLI-0703", message, remediation);

	function isNormalizedRelativePath(path:String):Bool {
		if (path == "" || StringTools.startsWith(path, "/") || StringTools.startsWith(path, "~") || path.indexOf("\\") != -1 || ~/^[A-Za-z]:/.match(path))
			return false;
		for (part in path.split("/"))
			if (part == "" || part == "." || part == "..")
				return false;
		return true;
	}
}
