import haxe.Json;
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

		final finalized = new TypedCNameFinalizer().finalizeNames(contract(false));
		final reversedFinalized = new TypedCNameFinalizer().finalizeNames(contract(true));
		if (Json.stringify(finalized) != Json.stringify(reversedFinalized)) {
			throw "typed C name finalization changed with declaration discovery order";
		}
		final plan = new CDeclarationPlanner().plan(finalized.snapshot);
		final forwardNames = [];
		for (header in plan.headers) {
			for (entry in header.forwardDeclarations) {
				forwardNames.push({modulePath: entry.modulePath, cName: entry.cName});
			}
		}

		Sys.println(REPORT_PREFIX + Json.stringify({
			symbolTable: forward.finalizeSymbols(),
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

	static function registry(requests:Array<CSymbolRequest>):CSymbolRegistry {
		final result = new CSymbolRegistry();
		result.registerAll(requests);
		result.finalizeSymbols();
		return result;
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
			schemaVersion: 2,
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
