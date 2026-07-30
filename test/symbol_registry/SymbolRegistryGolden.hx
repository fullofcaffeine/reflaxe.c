import haxe.Json;
import haxe.io.Bytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractField;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.contract.TypedCContract.TypedCDependency;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRegistryError;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.naming.CSymbolRequest.CSymbolKind;
import reflaxe.c.naming.CSymbolRequest.CSymbolNamespace;
import reflaxe.c.naming.CSymbolRequest.CSymbolVisibility;
import reflaxe.c.naming.TypedCNameFinalizer;
import reflaxe.c.plan.CDeclarationPlanner;

/** Builds the reviewable hxc.symbols.json contract and negative corpus. */
class SymbolRegistryGolden {
	static inline final REPORT_PREFIX = "HXC_SYMBOL_REGISTRY=";

	static function main():Void {
		verifyUtf8Order();
		final requests = corpus();
		final forward = registry(requests);
		final reverseRequests = requests.copy();
		reverseRequests.reverse();
		final reverse = registry(reverseRequests);
		if (forward.toJson() != reverse.toJson()) {
			throw "symbol registry changed with discovery order";
		}
		for (request in requests) {
			if (forward.identifierFor(request).value != reverse.identifierFor(request).value) {
				throw 'symbol `${request.sourceSymbol()}` changed with discovery order';
			}
		}
		verifyWarmCacheLifecycle(requests);

		final finalized = new TypedCNameFinalizer().finalizeNames(contract(false));
		final reversedFinalized = new TypedCNameFinalizer().finalizeNames(contract(true));
		if (Json.stringify(finalized) != Json.stringify(reversedFinalized)) {
			throw "typed C name finalization changed with declaration discovery order";
		}
		verifyRepeatedCollisionRound();
		final plan = new CDeclarationPlanner().plan(finalized.snapshot);
		final forwardNames = [];
		for (header in plan.headers) {
			for (entry in header.forwardDeclarations) {
				forwardNames.push({modulePath: entry.modulePath, cName: entry.cName});
			}
		}

		Sys.println(REPORT_PREFIX + Json.stringify({
			symbolTable: forward.finalizeSymbols(),
			readableIdentityProof: readableIdentityProof(),
			contractFinalization: finalized,
			plannedForwardNames: forwardNames,
			diagnostics: {
				exactCollision: collisionDiagnostic(),
				keyword: invalidExactDiagnostic("while"),
				leadingUnderscore: invalidExactDiagnostic("_private"),
				doubleUnderscore: invalidExactDiagnostic("public__name"),
				compilerNamespace: invalidExactDiagnostic("hxc_private"),
				lexical: invalidExactDiagnostic("bad-name"),
				conflictingFacts: conflictingFactsDiagnostic(),
				hostPathIdentity: hostPathIdentityDiagnostic(),
				sealedRegistry: sealedRegistryDiagnostic()
			}
		}));
	}

	/**
			Checks the shared fast comparator against an independent UTF-8 byte walk.

		ASCII alone cannot prove the contract because every common string order
		agrees there. These values cross one-, two-, three-, and four-byte UTF-8
		encodings, so an Eval-host shortcut that used a different Unicode order
		would fail before it could perturb generated artifact order.
	**/
	static function verifyUtf8Order():Void {
		final values = ["z", "é", "Ā", "雪", "🙂", "a", ""];
		for (left in values) {
			for (right in values) {
				final expected = compareUtf8Bytes(Bytes.ofString(left), Bytes.ofString(right));
				final actual = reflaxe.c.CUtf8Order.compare(left, right);
				if ((expected < 0) != (actual < 0) || (expected == 0) != (actual == 0) || (expected > 0) != (actual > 0))
					throw 'shared UTF-8 order differs for `${left}` and `${right}`: expected=$expected actual=$actual';
			}
		}
	}

	static function compareUtf8Bytes(left:Bytes, right:Bytes):Int {
		final length = left.length < right.length ? left.length : right.length;
		for (index in 0...length) {
			final difference = left.get(index) - right.get(index);
			if (difference != 0)
				return difference;
		}
		return left.length - right.length;
	}

	static function corpus():Array<CSymbolRequest> {
		final global = CNSOrdinary("translation-unit");
		return [
			new CSymbolRequest(CSKPackage, ["demo"], global),
			new CSymbolRequest(CSKModule, ["demo", "Main"], global),
			new CSymbolRequest(CSKType, ["demo", "Node"], CNSTag("translation-unit")),
			new CSymbolRequest(CSKField, ["demo", "Node", "next_value"], CNSMember("demo.Node")),
			new CSymbolRequest(CSKMethod, ["demo", "Worker", "run"], global, CSVInternal, null, ["c.Int32", "Void"]),
			new CSymbolRequest(CSKMethod, ["demo", "Worker", "run"], global, CSVInternal, null, ["String", "Void"]),
			new CSymbolRequest(CSKLocal, ["demo", "Worker", "run", "value"], CNSOrdinary("demo.Worker.run(c.Int32)")),
			new CSymbolRequest(CSKLocal, ["demo", "Worker", "run", "while"], CNSOrdinary("demo.Worker.run(c.Int32)")),
			new CSymbolRequest(CSKLocal, ["demo", "Worker", "run", "bool"], CNSOrdinary("demo.Worker.run(c.Int32)")),
			new CSymbolRequest(CSKLocal, ["demo", "Worker", "run", "status"], CNSOrdinary("demo.Worker.run(c.Int32)")),
			new CSymbolRequest(CSKTemporary, ["demo", "Worker", "run", "call-result"], CNSOrdinary("demo.Worker.run(c.Int32)"), CSVInternal, null, null, null,
				4),
			new CSymbolRequest(CSKSpecialization, ["demo", "Box", "map"], global, CSVInternal, null, null, ["c.Int32"]),
			new CSymbolRequest(CSKSpecialization, ["demo", "Box", "map"], global, CSVInternal, null, null, ["demo.User"]),
			new CSymbolRequest(CSKClosure, ["demo", "Worker", "run", "lambda"], global, CSVInternal, null, null, null, 2),
			new CSymbolRequest(CSKClosureEnvironment, ["demo", "Worker", "run", "lambda"], CNSTag("translation-unit"), CSVInternal, null, null, null, 2),
			new CSymbolRequest(CSKVTable, ["demo", "Worker"], global),
			new CSymbolRequest(CSKInterfaceTable, ["demo", "Worker", "Runnable"], global),
			new CSymbolRequest(CSKTypeDescriptor, ["demo", "Worker"], global),
			new CSymbolRequest(CSKReflectionEntry, ["demo", "Worker", "run"], global),
			new CSymbolRequest(CSKStaticInitializer, ["demo", "Main"], global),
			new CSymbolRequest(CSKExport, ["demo", "Api", "open"], global, CSVPublic, "demo_open"),
			new CSymbolRequest(CSKRuntimePrivate, ["array", "grow"], global),
			new CSymbolRequest(CSKMethod, ["keywords", "while"], global),
			new CSymbolRequest(CSKType, ["__Reserved"], CNSTag("translation-unit")),
			new CSymbolRequest(CSKType, ["_Upper"], CNSTag("translation-unit")),
			new CSymbolRequest(CSKMethod, ["hxc_owned"], global),
			new CSymbolRequest(CSKMethod, ["strcpy"], global),
			new CSymbolRequest(CSKField, ["demo", "Café", "résumé"], CNSMember("demo.Café")),
			new CSymbolRequest(CSKType, ["compiler", "closed-record", "4aec2e39ec8810b7"], CNSTag("translation-unit"), CSVInternal, null, null, null, null,
				["caxecraft", "domain", "BlockCoord"]),
			new CSymbolRequest(CSKField, ["compiler", "closed-record", "4aec2e39ec8810b7", "x"], CNSMember("compiler.closed-record.4aec2e39ec8810b7"),
				CSVInternal, null, null, null, null, ["x"]),
			new CSymbolRequest(CSKField, ["compiler", "closed-record", "4aec2e39ec8810b7", "NULL"], CNSMember("compiler.closed-record.4aec2e39ec8810b7"),
				CSVInternal, null, null, null, null, ["NULL"]),
			new CSymbolRequest(CSKField, ["compiler", "closed-record", "4aec2e39ec8810b7", "NAN"], CNSMember("compiler.closed-record.4aec2e39ec8810b7"),
				CSVInternal, null, null, null, null, ["NAN"]),
			new CSymbolRequest(CSKType, [
				"very_long_package_component_with_repeated_provenance",
				"another_long_module_component",
				"ATypeWhoseReadableNameExceedsTheLimit"
			],
				CNSTag("translation-unit")),
			new CSymbolRequest(CSKType, ["shared"], CNSTag("translation-unit"), CSVPublic, "shared_name"),
			new CSymbolRequest(CSKMethod, ["shared"], global, CSVPublic, "shared_name"),
			new CSymbolRequest(CSKField, ["first", "value"], CNSMember("first.Owner"), CSVPublic, "value"),
			new CSymbolRequest(CSKField, ["second", "value"], CNSMember("second.Owner"), CSVPublic, "value"),
			new CSymbolRequest(CSKField, ["demo", "LIMIT"], CNSPreprocessor, CSVPublic, "DEMO_LIMIT"),
			new CSymbolRequest(CSKLocal, ["demo", "Worker", "label"], CNSLabel("demo.Worker.run(c.Int32)"))
		];
	}

	/**
		Prove that a hash-suffixed name which collides again advances safely.

		Ordinary collisions settle after one suffix. This fixture deliberately
		gives a third generated symbol the exact spelling of another symbol's
		first suffix, forcing a second round. It protects the rare correctness
		path while production finalization revisits only names changed by the
		previous round.
	**/
	static function verifyRepeatedCollisionRound():Void {
		final namespace = CNSOrdinary("translation-unit");
		final first = new CSymbolRequest(CSKSpecialization, ["demo", "Box", "map"], namespace, CSVInternal, null, null, ["c.Int32"]);
		final second = new CSymbolRequest(CSKSpecialization, ["demo", "Box", "map"], namespace, CSVInternal, null, null, ["demo.User"]);
		final occupant = new CSymbolRequest(CSKMethod, ["collision", "occupant"], namespace, CSVInternal, null, null, null, null,
			["demo", "Box", "map", "h8297ed1a605e"]);
		final registry = new CSymbolRegistry();
		registry.registerAll([first, second, occupant]);
		final snapshot = registry.finalizeSymbols();
		final firstName = registry.identifierFor(first).value;
		final secondName = registry.identifierFor(second).value;
		final occupantName = registry.identifierFor(occupant).value;
		if (!StringTools.endsWith(firstName, "_h8297ed1a605e5954")
			|| !StringTools.endsWith(secondName, "_h6b7e6eb9dc80")
			|| occupantName.indexOf("hxc_demo_Box_map_h8297ed1a605e_h") != 0
			|| snapshot.collisions.length != 2) {
			throw 'a generated C name that collided after its first hash suffix was not advanced deterministically: first=$firstName second=$secondName occupant=$occupantName collisions=${snapshot.collisions.length}';
		}
	}

	static function readableIdentityProof():String {
		final semantic = ["compiler", "closed-record", "semantic-digest"];
		final first = new CSymbolRequest(CSKType, semantic, CNSTag("translation-unit"), CSVInternal, null, null, null, null, ["demo", "FirstDisplay"]);
		final second = new CSymbolRequest(CSKType, semantic, CNSTag("translation-unit"), CSVInternal, null, null, null, null, ["demo", "SecondDisplay"]);
		if (first.stableKey() != second.stableKey())
			throw "readable spelling incorrectly changed semantic symbol identity";
		if (first.namingFingerprint() == second.namingFingerprint())
			throw "readable spelling was omitted from the conflicting-facts boundary";
		return "readable-display-is-not-semantic-identity-but-is-a-validated-naming-fact";
	}

	static function registry(requests:Array<CSymbolRequest>):CSymbolRegistry {
		final result = new CSymbolRegistry();
		result.registerAll(requests);
		result.finalizeSymbols();
		return result;
	}

	/**
		Prove that warm reuse is exact, bounded, and failure-atomic.

		The cache may skip expensive naming work only after the current canonical
		request sequence matches the preceding successful sequence in full.
		Discovery order is intentionally irrelevant; adding, removing, or changing
		one naming fact is a miss. Aborting that miss must leave the prior exact
		table available to the next request.
	**/
	static function verifyWarmCacheLifecycle(requests:Array<CSymbolRequest>):Void {
		CSymbolRegistry.abortCacheRequest();

		CSymbolRegistry.beginCacheRequest(true);
		final cold = registry(requests);
		final coldJson = cold.toJson();
		final coldStats = CSymbolRegistry.completeCacheRequest();
		requireCacheStats("cold", coldStats, true, 0, 1, requests.length);

		final reversed = requests.copy();
		reversed.reverse();
		CSymbolRegistry.beginCacheRequest(true);
		final warm = registry(reversed);
		if (warm.toJson() != coldJson)
			throw "warm symbol-table reuse changed the finalized report";
		for (request in requests)
			if (warm.identifierFor(request).value != cold.identifierFor(request).value)
				throw 'warm symbol-table reuse changed `${request.sourceSymbol()}`';
		requireCacheStats("reordered warm", CSymbolRegistry.completeCacheRequest(), true, 1, 0, requests.length);

		final added = requests.concat([
			new CSymbolRequest(CSKMethod, ["demo", "Worker", "newMethod"], CNSOrdinary("translation-unit"))
		]);
		CSymbolRegistry.beginCacheRequest(true);
		registry(added);
		requireCacheStats("added request", CSymbolRegistry.completeCacheRequest(), true, 0, 1, added.length);

		// Replacing the one retained generation is observable: the older request
		// set misses once, then becomes the sole new predecessor.
		CSymbolRegistry.beginCacheRequest(true);
		registry(requests);
		requireCacheStats("bounded replacement", CSymbolRegistry.completeCacheRequest(), true, 0, 1, requests.length);

		final removed = requests.copy();
		removed.pop();
		CSymbolRegistry.beginCacheRequest(true);
		registry(removed);
		CSymbolRegistry.abortCacheRequest();

		final renamed = requests.copy();
		renamed[0] = new CSymbolRequest(CSKPackage, ["demo"], CNSOrdinary("translation-unit"), CSVInternal, null, null, null, null, ["renamed"]);
		CSymbolRegistry.beginCacheRequest(true);
		registry(renamed);
		CSymbolRegistry.abortCacheRequest();

		CSymbolRegistry.beginCacheRequest(true);
		registry(requests);
		requireCacheStats("aborted misses preserve prior success", CSymbolRegistry.completeCacheRequest(), true, 1, 0, requests.length);

		CSymbolRegistry.beginCacheRequest(true);
		final failed = new CSymbolRegistry();
		failed.register(new CSymbolRequest(CSKMethod, ["alpha", "open"], CNSOrdinary("translation-unit"), CSVPublic, "same_symbol"));
		failed.register(new CSymbolRequest(CSKMethod, ["beta", "open"], CNSOrdinary("translation-unit"), CSVPublic, "same_symbol"));
		expectRegistryFailure(() -> failed.finalizeSymbols());
		CSymbolRegistry.abortCacheRequest();

		CSymbolRegistry.beginCacheRequest(true);
		registry(requests);
		requireCacheStats("failed request preserves prior success", CSymbolRegistry.completeCacheRequest(), true, 1, 0, requests.length);

		CSymbolRegistry.beginCacheRequest(false);
		registry(requests);
		requireCacheStats("disabled", CSymbolRegistry.completeCacheRequest(), false, 0, 0, requests.length);

		CSymbolRegistry.beginCacheRequest(true);
		registry(requests);
		requireCacheStats("disabled request does not replace prior success", CSymbolRegistry.completeCacheRequest(), true, 1, 0, requests.length);
	}

	/** Check one lifecycle observation without exposing cache internals. */
	static function requireCacheStats(label:String, stats:reflaxe.c.naming.CSymbolRegistry.CSymbolTableCacheStats, enabled:Bool, hits:Int, misses:Int,
			retainedRequests:Int):Void {
		if (stats.enabled != enabled
			|| stats.hits != hits
			|| stats.misses != misses
			|| stats.retainedRequests != retainedRequests
			|| stats.retainedKeyCodeUnits <= 0)
			throw '$label symbol-table cache stats drifted: ${Json.stringify(stats)}';
	}

	static function collisionDiagnostic():SymbolRegistryFailureDump {
		final result = new CSymbolRegistry();
		result.register(new CSymbolRequest(CSKMethod, ["alpha", "open"], CNSOrdinary("translation-unit"), CSVPublic, "same_symbol"));
		result.register(new CSymbolRequest(CSKMethod, ["beta", "open"], CNSOrdinary("translation-unit"), CSVPublic, "same_symbol"));
		return expectRegistryFailure(() -> result.finalizeSymbols());
	}

	static function invalidExactDiagnostic(name:String):SymbolRegistryFailureDump {
		final result = new CSymbolRegistry();
		return expectRegistryFailure(() -> result.register(new CSymbolRequest(CSKExport, ["api", "invalid"], CNSOrdinary("translation-unit"), CSVPublic,
			name)));
	}

	static function conflictingFactsDiagnostic():SymbolRegistryFailureDump {
		final result = new CSymbolRegistry();
		result.register(new CSymbolRequest(CSKExport, ["api", "open"], CNSOrdinary("translation-unit"), CSVPublic, "api_open"));
		return expectRegistryFailure(() -> result.register(new CSymbolRequest(CSKExport, ["api", "open"], CNSOrdinary("translation-unit"), CSVPublic,
			"api_open_v2")));
	}

	static function sealedRegistryDiagnostic():SymbolRegistryFailureDump {
		final result = new CSymbolRegistry();
		result.register(new CSymbolRequest(CSKType, ["Ready"], CNSTag("translation-unit")));
		result.finalizeSymbols();
		return expectRegistryFailure(() -> result.register(new CSymbolRequest(CSKType, ["Late"], CNSTag("translation-unit"))));
	}

	static function hostPathIdentityDiagnostic():SymbolRegistryFailureDump {
		return expectRegistryFailure(() -> new CSymbolRequest(CSKType, ["/tmp/checkout/Main"], CNSTag("translation-unit")));
	}

	static function expectRegistryFailure(operation:() -> Void):SymbolRegistryFailureDump {
		try {
			operation();
		} catch (error:CSymbolRegistryError) {
			return {
				id: error.diagnosticId,
				message: error.message,
				sourceSymbols: error.sourceSymbols
			};
		}
		throw "invalid symbol registry operation unexpectedly succeeded";
	}

	static function contract(reverse:Bool):TypedCContractSnapshot {
		final declarations = [
			declaration("api.Service", null, null, "api.h", "public", [], [field("open", "function", "service_open", true), field("while", "function")]),
			declaration("demo.Node", "struct", null, "node.h", "public", [forward("demo.Value")], [field("next_value", "field")]),
			declaration("demo.Value", "struct", "demo_value", "value.h", "public", [], [field("value", "field")]),
			declaration("foreign.Handle", "opaque", "foreign_handle", null, null)
		];
		if (reverse) {
			declarations.reverse();
		}
		final buildFacts = [
			includeFact("foreign.h", ["foreign.Handle"]),
			includeFact("shared.h", ["demo.Node"]),
			includeFact("shared.h", ["demo.Value"]),
			includeFact("stdint.h", ["demo.Value", "demo.Node", "demo.Value"])
		];
		if (reverse) {
			buildFacts.reverse();
			for (fact in buildFacts) {
				fact.ownerModulePaths.reverse();
			}
		}
		return {
			schemaVersion: 3,
			status: "contract-seed-no-lowering",
			declarations: declarations,
			buildFacts: buildFacts,
			effects: {
				allocation: "none",
				ownership: "declarative-only",
				unsafe: "none",
				portability: "c-target-only",
				runtimeFeatures: []
			}
		};
	}

	static function declaration(modulePath:String, layout:Null<String>, cName:Null<String>, headerPath:Null<String>, headerVisibility:Null<String>,
			?dependencies:Array<TypedCDependency>, ?fields:Array<TypedCContractField>):TypedCDeclaration {
		return {
			modulePath: modulePath,
			sourceKind: "class",
			layout: layout,
			cName: cName,
			headerPath: headerPath,
			headerVisibility: headerVisibility,
			pack: null,
			align: null,
			exported: false,
			linkage: null,
			callingConvention: null,
			visibility: null,
			section: null,
			dependencies: dependencies == null ? [] : dependencies,
			fields: fields == null ? [] : fields
		};
	}

	static function field(name:String, kind:String, ?cName:String, ?exported:Bool = false):TypedCContractField {
		return {
			name: name,
			cName: cName,
			kind: kind,
			type: {kind: "abstract", name: "c.Int32", arguments: []},
			bitWidth: null,
			align: null,
			exported: exported,
			linkage: null,
			callingConvention: null,
			variadic: false,
			visibility: null,
			section: null
		};
	}

	static function forward(modulePath:String):TypedCDependency
		return {modulePath: modulePath, requirement: "forward"};

	static function includeFact(path:String, owners:Array<String>):TypedCBuildFact {
		return {
			kind: "include",
			name: path,
			value: "system",
			valueKind: "enum",
			ownerModulePaths: owners
		};
	}
}

private typedef SymbolRegistryFailureDump = {
	final id:CDiagnosticId;
	final message:String;
	final sourceSymbols:Array<String>;
}
