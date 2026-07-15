package reflaxe.c.naming;

import haxe.io.Bytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractField;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.contract.TypedCContract.TypedCDependency;
import reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
import reflaxe.c.naming.CSymbolRequest.CSymbolKind;
import reflaxe.c.naming.CSymbolRequest.CSymbolNamespace;
import reflaxe.c.naming.CSymbolRequest.CSymbolVisibility;

typedef TypedCNameFinalization = {
	final snapshot:TypedCContractSnapshot;
	final symbols:CSymbolTableSnapshot;
}

/**
	Finalizes default and explicit names in a typed C contract snapshot.

	This is the structural handoff from the ratified Haxe-facing vocabulary to
	declaration planning. It copies the snapshot, changes only finalized `cName`
	facts and status, and preserves the empty runtime-feature effect.
 */
class TypedCNameFinalizer {
	public function new() {}

	public function finalizeNames(snapshot:TypedCContractSnapshot):TypedCNameFinalization {
		if (snapshot.schemaVersion != 2) {
			contractFailure('typed C contract schema ${snapshot.schemaVersion} is unsupported by name finalization; expected schema 2');
		}

		final registry = new CSymbolRegistry();
		final seenDeclarations:Map<String, Bool> = [];
		final externalOwners = externalOwnerSet(snapshot.buildFacts);
		final orderedDeclarations = snapshot.declarations.copy();
		orderedDeclarations.sort((left, right) -> compareUtf8(left.modulePath, right.modulePath));
		for (declaration in orderedDeclarations) {
			final external = isExternalDeclaration(declaration, externalOwners);
			if (seenDeclarations.exists(declaration.modulePath)) {
				contractFailure('duplicate typed C declaration `${declaration.modulePath}` cannot receive one deterministic C name', [declaration.modulePath]);
			}
			seenDeclarations.set(declaration.modulePath, true);
			registry.register(declarationRequest(declaration, external));

			final seenFields:Map<String, Bool> = [];
			for (field in declaration.fields) {
				final fieldKey = field.kind + "\x00" + field.name;
				if (seenFields.exists(fieldKey)) {
					contractFailure('duplicate typed C ${field.kind} `${declaration.modulePath}.${field.name}` cannot receive one deterministic C name',
						['${declaration.modulePath}.${field.name}']);
				}
				seenFields.set(fieldKey, true);
				registry.register(fieldRequest(declaration, field, external));
			}
		}

		final symbols = registry.finalizeSymbols();
		final declarations:Array<TypedCDeclaration> = [];
		for (declaration in orderedDeclarations) {
			final external = isExternalDeclaration(declaration, externalOwners);
			final fields:Array<TypedCContractField> = [];
			for (field in declaration.fields) {
				fields.push(copyField(field, registry.identifierFor(fieldRequest(declaration, field, external)).value));
			}
			declarations.push(copyDeclaration(declaration, registry.identifierFor(declarationRequest(declaration, external)).value, fields));
		}

		return {
			snapshot: {
				schemaVersion: snapshot.schemaVersion,
				status: "contract-seed-names-finalized-no-lowering",
				declarations: declarations,
				buildFacts: copyBuildFacts(snapshot.buildFacts),
				effects: {
					allocation: snapshot.effects.allocation,
					ownership: snapshot.effects.ownership,
					unsafe: snapshot.effects.unsafe,
					portability: snapshot.effects.portability,
					runtimeFeatures: snapshot.effects.runtimeFeatures.copy()
				}
			},
			symbols: symbols
		};
	}

	static function declarationRequest(declaration:TypedCDeclaration, external:Bool):CSymbolRequest {
		return new CSymbolRequest(CSKType, splitQualified(declaration.modulePath), declarationNamespace(declaration),
			visibility(declaration.headerVisibility == "public" || declaration.exported, external), declaration.cName);
	}

	static function fieldRequest(declaration:TypedCDeclaration, field:TypedCContractField, externalOwner:Bool):CSymbolRequest {
		final kind = field.kind == "function" ? CSKMethod : CSKField;
		final namespace = field.kind == "field" ? CNSMember(declaration.modulePath) : CNSOrdinary("translation-unit");
		return new CSymbolRequest(kind, splitQualified(declaration.modulePath).concat([field.name]), namespace,
			visibility(field.exported || declaration.headerVisibility == "public", externalOwner), field.cName);
	}

	static function declarationNamespace(declaration:TypedCDeclaration):CSymbolNamespace {
		return switch declaration.layout {
			case "struct" | "union" | "opaque" | "enum": CNSTag("translation-unit");
			case _: CNSOrdinary("translation-unit");
		};
	}

	static function visibility(isPublic:Bool, external:Bool):CSymbolVisibility
		return external ? CSVExternal : (isPublic ? CSVPublic : CSVInternal);

	static function externalOwnerSet(buildFacts:Array<TypedCBuildFact>):Map<String, Bool> {
		final owners:Map<String, Bool> = [];
		for (fact in buildFacts) {
			if (fact.kind == "include") {
				for (owner in fact.ownerModulePaths) {
					owners.set(owner, true);
				}
			}
		}
		return owners;
	}

	static function isExternalDeclaration(declaration:TypedCDeclaration, externalOwners:Map<String, Bool>):Bool
		return declaration.layout == "opaque" && declaration.headerPath == null && externalOwners.exists(declaration.modulePath);

	static function splitQualified(value:String):Array<String> {
		final parts = value.split(".");
		for (part in parts) {
			if (part == "") {
				contractFailure('typed C module path `$value` is not a normalized Haxe qualified name', [value]);
			}
		}
		return parts;
	}

	static function copyDeclaration(declaration:TypedCDeclaration, cName:String, fields:Array<TypedCContractField>):TypedCDeclaration {
		return {
			modulePath: declaration.modulePath,
			sourceKind: declaration.sourceKind,
			layout: declaration.layout,
			cName: cName,
			headerPath: declaration.headerPath,
			headerVisibility: declaration.headerVisibility,
			pack: declaration.pack,
			align: declaration.align,
			exported: declaration.exported,
			linkage: declaration.linkage,
			callingConvention: declaration.callingConvention,
			visibility: declaration.visibility,
			section: declaration.section,
			dependencies: copyDependencies(declaration.dependencies),
			fields: fields
		};
	}

	static function copyField(field:TypedCContractField, cName:String):TypedCContractField {
		return {
			name: field.name,
			cName: cName,
			kind: field.kind,
			type: field.type,
			bitWidth: field.bitWidth,
			align: field.align,
			exported: field.exported,
			linkage: field.linkage,
			callingConvention: field.callingConvention,
			visibility: field.visibility,
			section: field.section
		};
	}

	static function copyDependencies(dependencies:Array<TypedCDependency>):Array<TypedCDependency> {
		final result = dependencies.map(dependency -> {
			modulePath: dependency.modulePath,
			requirement: dependency.requirement
		});
		result.sort((left, right) -> compareUtf8(left.modulePath + "\x00" + left.requirement, right.modulePath + "\x00" + right.requirement));
		return result;
	}

	static function copyBuildFacts(facts:Array<TypedCBuildFact>):Array<TypedCBuildFact> {
		final result:Array<TypedCBuildFact> = [];
		for (fact in facts) {
			final owners:Array<String> = [];
			for (owner in fact.ownerModulePaths) {
				if (owners.indexOf(owner) == -1) {
					owners.push(owner);
				}
			}
			owners.sort(compareUtf8);
			result.push({
				kind: fact.kind,
				name: fact.name,
				value: fact.value,
				valueKind: fact.valueKind,
				ownerModulePaths: owners
			});
		}
		result.sort(compareBuildFacts);
		return result;
	}

	static function compareBuildFacts(left:TypedCBuildFact, right:TypedCBuildFact):Int {
		for (order in [
			compareUtf8(left.kind, right.kind),
			compareUtf8(left.name, right.name),
			compareNullable(left.valueKind, right.valueKind),
			compareNullable(left.value, right.value)
		]) {
			if (order != 0) {
				return order;
			}
		}
		final length = left.ownerModulePaths.length < right.ownerModulePaths.length ? left.ownerModulePaths.length : right.ownerModulePaths.length;
		for (index in 0...length) {
			final order = compareUtf8(left.ownerModulePaths[index], right.ownerModulePaths[index]);
			if (order != 0) {
				return order;
			}
		}
		return left.ownerModulePaths.length - right.ownerModulePaths.length;
	}

	static function compareNullable(left:Null<String>, right:Null<String>):Int {
		if (left == null) {
			return right == null ? 0 : -1;
		}
		return right == null ? 1 : compareUtf8(left, right);
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	static function contractFailure(detail:String, ?sources:Array<String>):Dynamic
		throw new CSymbolRegistryError(CDiagnosticId.InvalidTypedCContract, detail, sources);
}
