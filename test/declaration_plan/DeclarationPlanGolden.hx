import haxe.Json;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.contract.TypedCContract.TypedCDependency;
import reflaxe.c.plan.CDeclarationPlan;
import reflaxe.c.plan.CDeclarationPlan.CHeaderPlan;
import reflaxe.c.plan.CDeclarationPlan.CPlanDeclarationEmission;
import reflaxe.c.plan.CDeclarationPlan.CPlanForwardKind;
import reflaxe.c.plan.CDeclarationPlan.CPlanHeaderVisibility;
import reflaxe.c.plan.CDeclarationPlan.CPlanIncludeKind;
import reflaxe.c.plan.CDeclarationPlan.CPlanIncludeReason;
import reflaxe.c.plan.CDeclarationPlan.CPlannedDeclaration;
import reflaxe.c.plan.CDeclarationPlan.CPlannedForwardDeclaration;
import reflaxe.c.plan.CDeclarationPlan.CPlannedInclude;
import reflaxe.c.plan.CDeclarationPlanner;
import reflaxe.c.plan.CDeclarationPlanningError;

/** Builds an inspectable planner report and strict-C11 header corpus. */
class DeclarationPlanGolden {
	static inline final REPORT_PREFIX = "HXC_DECLARATION_PLAN=";

	static function main():Void {
		final planner = new CDeclarationPlanner();
		final forward = planner.plan(snapshot(false));
		final reverse = planner.plan(snapshot(true));
		final forwardDump = dumpPlan(forward);
		if (Json.stringify(forwardDump) != Json.stringify(dumpPlan(reverse))) {
			throw "declaration plan changed with input order";
		}

		final files = renderHeaders(forward);
		final reverseFiles = renderHeaders(reverse);
		if (Json.stringify(files) != Json.stringify(reverseFiles)) {
			throw "planned headers changed with input order";
		}

		Sys.println(REPORT_PREFIX + Json.stringify({
			plan: forwardDump,
			diagnostics: {
				byValueCycle: cycleDiagnostic(),
				sourceOnlyComplete: sourceOnlyDiagnostic(),
				privateComplete: privateCompleteDiagnostic(),
				mixedHeaderVisibility: mixedHeaderVisibilityDiagnostic(),
				missingForwardName: missingForwardNameDiagnostic(),
				includeKindConflict: includeKindConflictDiagnostic()
			},
			files: files
		}));
	}

	static function snapshot(reverse:Bool):TypedCContractSnapshot {
		final declarations = [
			declaration("ApiEnvelope", "struct", "api_envelope", "api.h", "public", [complete("ApiRequest")]),
			declaration("ApiRequest", "struct", "api_request", "api.h", "public",
				[
					complete("Value"),
					forward("ExternalClock"),
					forward("HiddenHandle"),
					forward("NodeA"),
					forward("PrivateState")
				]),
			declaration("ExternalClock", "opaque", "external_clock", null, null),
			declaration("HiddenHandle", "opaque", "hidden_handle", null, null),
			declaration("Implementation", null, null, null, null, [complete("Value"), forward("NodeB")]),
			declaration("NodeA", "struct", "node_a", "nodes.h", "public", [forward("NodeB")]),
			declaration("NodeB", "struct", "node_b", "nodes.h", "public", [forward("NodeA")]),
			declaration("PrivateState", "struct", "private_state", "detail/state.h", "private", [complete("Value"), forward("HiddenHandle")]),
			declaration("Value", "struct", "value", "types/value.h", "public")
		];
		final buildFacts = [
			includeFact("project/config.h", "local", ["PrivateState", "ApiRequest", "ApiRequest"]),
			includeFact("stdint.h", "system", ["Value", "NodeB", "NodeA", "NodeA"]),
			includeFact("types/value.h", "local", ["ApiRequest"]),
			includeFact("clock_api.h", "system", ["ExternalClock", "ExternalClock"]),
			includeFact("stddef.h", "system", ["ApiRequest"]),
			includeFact("stdint.h", "system", ["NodeA", "Value"]),
			includeFact("project/config.h", "local", ["ApiRequest"])
		];
		if (reverse) {
			declarations.reverse();
			for (item in declarations) {
				item.dependencies.reverse();
			}
			buildFacts.reverse();
			for (fact in buildFacts) {
				fact.ownerModulePaths.reverse();
			}
		}
		return contract(declarations, buildFacts);
	}

	static function cycleDiagnostic():String {
		return expectPlanningFailure(contract([
			declaration("CycleA", "struct", "cycle_a", "cycle.h", "public", [complete("CycleB")]),
			declaration("CycleB", "struct", "cycle_b", "cycle.h", "public", [complete("CycleA")])
		], []));
	}

	static function sourceOnlyDiagnostic():String {
		return expectPlanningFailure(contract([
			declaration("PublicValue", "struct", "public_value", "public.h", "public", [complete("SourceValue")]),
			declaration("SourceValue", "struct", "source_value", null, null)
		], []));
	}

	static function missingForwardNameDiagnostic():String {
		return expectPlanningFailure(contract([
			declaration("Consumer", "struct", "consumer", "consumer.h", "public", [forward("Unnamed")]),
			declaration("Unnamed", "struct", null, "unnamed.h", "public")
		], []));
	}

	static function privateCompleteDiagnostic():String {
		return expectPlanningFailure(contract([
			declaration("PublicConsumer", "struct", "public_consumer", "public.h", "public", [complete("PrivateValue")]),
			declaration("PrivateValue", "struct", "private_value", "private.h", "private")
		], []));
	}

	static function mixedHeaderVisibilityDiagnostic():String {
		return expectPlanningFailure(contract([
			declaration("PrivatePart", "struct", "private_part", "mixed.h", "private"),
			declaration("PublicPart", "struct", "public_part", "mixed.h", "public")
		], []));
	}

	static function includeKindConflictDiagnostic():String {
		return expectPlanningFailure(contract([declaration("Owner", "struct", "owner", "owner.h", "public")], [
			includeFact("same.h", "system", ["Owner"]),
			includeFact("same.h", "local", ["Owner"])
		]));
	}

	static function expectPlanningFailure(input:TypedCContractSnapshot):String {
		try {
			new CDeclarationPlanner().plan(input);
		} catch (error:CDeclarationPlanningError) {
			return error.message;
		}
		throw "invalid declaration plan unexpectedly succeeded";
	}

	static function contract(declarations:Array<TypedCDeclaration>, buildFacts:Array<TypedCBuildFact>):TypedCContractSnapshot {
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
			?dependencies:Array<TypedCDependency>):TypedCDeclaration {
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
			fields: []
		};
	}

	static function complete(modulePath:String):TypedCDependency
		return {modulePath: modulePath, requirement: "complete"};

	static function forward(modulePath:String):TypedCDependency
		return {modulePath: modulePath, requirement: "forward"};

	static function includeFact(path:String, kind:String, owners:Array<String>):TypedCBuildFact {
		return {
			kind: "include",
			name: path,
			value: kind,
			valueKind: "enum",
			ownerModulePaths: owners
		};
	}

	static function dumpPlan(plan:CDeclarationPlan):DeclarationPlanDump {
		return {
			schemaVersion: plan.schemaVersion,
			inputSchemaVersion: plan.inputSchemaVersion,
			headers: plan.headers.map(header -> {
				path: header.path,
				visibility: visibilityName(header.visibility),
				guard: header.guard,
				includes: header.includes.map(dumpInclude),
				forwardDeclarations: header.forwardDeclarations.map(forwardDeclaration -> {
					modulePath: forwardDeclaration.modulePath,
					cName: forwardDeclaration.cName,
					kind: forwardKindName(forwardDeclaration.kind)
				}),
				declarations: header.declarations.map(dumpDeclaration)
			}),
			sourceIncludes: plan.sourceIncludes.map(dumpInclude),
			sourceForwardDeclarations: plan.sourceForwardDeclarations.map(forwardDeclaration -> {
				modulePath: forwardDeclaration.modulePath,
				cName: forwardDeclaration.cName,
				kind: forwardKindName(forwardDeclaration.kind)
			}),
			sourceDeclarations: plan.sourceDeclarations.map(dumpDeclaration),
			buildIncludes: plan.buildIncludes.map(dumpInclude)
		};
	}

	static function dumpInclude(include:CPlannedInclude):DeclarationIncludeDump {
		return {
			path: include.path,
			kind: include.kind == PIKSystem ? "system" : "local",
			reasons: include.reasons.map(reasonName)
		};
	}

	static function dumpDeclaration(declaration:CPlannedDeclaration):DeclarationDump {
		return {
			modulePath: declaration.modulePath,
			cName: declaration.cName,
			layout: declaration.layout,
			emission: switch declaration.emission {
				case PDEComplete: "complete";
				case PDEOpaque: "opaque";
				case PDEExternal: "external";
			}
		};
	}

	static function reasonName(reason:CPlanIncludeReason):String {
		return switch reason {
			case PIRBuildFact(owner): 'build:$owner';
			case PIRCompleteType(consumer, target): 'complete:$consumer->$target';
			case PIRExternalType(consumer, target): 'external:$consumer->$target';
		}
	}

	static function visibilityName(visibility:CPlanHeaderVisibility):String
		return visibility == PHVPublic ? "public" : "private";

	static function forwardKindName(kind:CPlanForwardKind):String {
		return switch kind {
			case PFKStruct: "struct";
			case PFKUnion: "union";
			case PFKOpaque: "opaque";
		}
	}

	static function renderHeaders(plan:CDeclarationPlan):Array<RenderedHeader> {
		return plan.headers.map(header -> {
			path: header.path,
			content: renderHeader(header)
		});
	}

	static function renderHeader(header:CHeaderPlan):String {
		final unit = new CTranslationUnit();
		for (include in header.includes) {
			unit.includes.push({
				path: include.path,
				kind: include.kind == PIKSystem ? System : Local
			});
		}

		final preamble = headerPreamble(header);
		for (entry in preamble) {
			final identifier = id(entry.cName);
			if (entry.kind == PFKUnion) {
				unit.declarations.push(DForwardUnion(identifier, []));
				unit.declarations.push(DTypedef(type(TUnion(identifier)), name(entry.cName), []));
			} else {
				unit.declarations.push(DForwardStruct(identifier, []));
				unit.declarations.push(DTypedef(type(TStruct(identifier)), name(entry.cName), []));
			}
		}
		for (declaration in header.declarations) {
			if (declaration.emission == PDEComplete) {
				unit.declarations.push(definition(declaration.modulePath));
			}
		}

		final body = new CASTPrinter().printTranslationUnit(unit);
		return '#ifndef ${header.guard}\n#define ${header.guard}\n\n$body\n#endif /* ${header.guard} */\n';
	}

	static function headerPreamble(header:CHeaderPlan):Array<PreambleEntry> {
		final entries:Array<PreambleEntry> = [];
		for (forwardDeclaration in header.forwardDeclarations) {
			addPreamble(entries, forwardDeclaration.modulePath, forwardDeclaration.cName, forwardDeclaration.kind);
		}
		for (declaration in header.declarations) {
			if (declaration.cName == null) {
				continue;
			}
			switch declaration.layout {
				case "struct":
					addPreamble(entries, declaration.modulePath, declaration.cName, PFKStruct);
				case "union":
					addPreamble(entries, declaration.modulePath, declaration.cName, PFKUnion);
				case "opaque":
					addPreamble(entries, declaration.modulePath, declaration.cName, PFKOpaque);
				case _:
			}
		}
		entries.sort((left, right) -> left.cName < right.cName ? -1 : (left.cName > right.cName ? 1 : 0));
		return entries;
	}

	static function addPreamble(entries:Array<PreambleEntry>, modulePath:String, cName:String, kind:CPlanForwardKind):Void {
		for (entry in entries) {
			if (entry.cName == cName) {
				return;
			}
		}
		entries.push({modulePath: modulePath, cName: cName, kind: kind});
	}

	static function definition(modulePath:String):CDecl {
		return switch modulePath {
			case "ApiRequest": DStruct(id("api_request"), [
					field(named("value"), name("payload")),
					field(named("node_a"), pointer("node")),
					field(named("hidden_handle"), pointer("hidden")),
					field(named("external_clock"), pointer("clock")),
					field(named("private_state"), pointer("state")),
					field(named("size_t"), name("count")),
					field(named("project_config_tag"), name("config_tag"))
				], []);
			case "ApiEnvelope": DStruct(id("api_envelope"), [field(named("api_request"), name("request"))], []);
			case "NodeA": DStruct(id("node_a"), [
					field(named("node_b"), pointer("next")),
					field(type(TInt(32, true)), name("value"))
				], []);
			case "NodeB": DStruct(id("node_b"), [
					field(named("node_a"), pointer("previous")),
					field(type(TInt(32, true)), name("value"))
				], []);
			case "PrivateState": DStruct(id("private_state"), [
					field(named("value"), name("current")),
					field(named("hidden_handle"), pointer("hidden")),
					field(named("project_config_tag"), name("config_tag"))
				], []);
			case "Value": DStruct(id("value"), [field(type(TInt(32, true)), name("scalar"))], []);
			case _: throw 'missing structural C fixture declaration for `$modulePath`';
		}
	}

	static function id(value:String):CIdentifier
		return new CIdentifier(value);

	static function type(spec:CTypeSpec):CType
		return new CType(spec);

	static function named(value:String):CType
		return type(TNamed(id(value)));

	static function name(value:String):CDeclarator
		return DName(id(value));

	static function pointer(value:String):CDeclarator
		return DPointer(name(value), []);

	static function field(fieldType:CType, declarator:CDeclarator):CField {
		return {
			type: fieldType,
			declarator: declarator,
			bitWidth: null,
			alignments: [],
			attributes: []
		};
	}
}

private typedef PreambleEntry = {
	final modulePath:String;
	final cName:String;
	final kind:CPlanForwardKind;
}

private typedef DeclarationIncludeDump = {
	final path:String;
	final kind:String;
	final reasons:Array<String>;
}

private typedef DeclarationForwardDump = {
	final modulePath:String;
	final cName:String;
	final kind:String;
}

private typedef DeclarationDump = {
	final modulePath:String;
	final cName:Null<String>;
	final layout:Null<String>;
	final emission:String;
}

private typedef DeclarationHeaderDump = {
	final path:String;
	final visibility:String;
	final guard:String;
	final includes:Array<DeclarationIncludeDump>;
	final forwardDeclarations:Array<DeclarationForwardDump>;
	final declarations:Array<DeclarationDump>;
}

private typedef DeclarationPlanDump = {
	final schemaVersion:Int;
	final inputSchemaVersion:Int;
	final headers:Array<DeclarationHeaderDump>;
	final sourceIncludes:Array<DeclarationIncludeDump>;
	final sourceForwardDeclarations:Array<DeclarationForwardDump>;
	final sourceDeclarations:Array<DeclarationDump>;
	final buildIncludes:Array<DeclarationIncludeDump>;
}

private typedef RenderedHeader = {
	final path:String;
	final content:String;
}
