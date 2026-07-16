package reflaxe.c.frontend;

#if (macro || reflaxe_runtime)
import haxe.macro.TypedExprTools;
import reflaxe.c.frontend.TypedProgramInput.TypedAstMetadata;

typedef TypedAstInventoryCount = {
	final kind:String;
	final count:Int;
}

typedef TypedAstInventoryMetadata = {
	final name:String;
	final arguments:Array<String>;
}

typedef TypedAstInventoryField = {
	final sourceOrder:Int;
	final name:String;
	final role:String;
	final kind:String;
	final typeKind:String;
	final typeDisplay:String;
	final isPublic:Bool;
	final isExtern:Bool;
	final metadata:Array<TypedAstInventoryMetadata>;
	final hasExpression:Bool;
}

typedef TypedAstInventoryDeclaration = {
	final path:String;
	final name:String;
	final kind:String;
	final ownerModulePath:String;
	final ownership:String;
	final sourcePath:String;
	final isPrivate:Bool;
	final isExtern:Bool;
	final classKind:Null<String>;
	final isInterface:Bool;
	final isFinal:Bool;
	final isAbstract:Bool;
	final representedTypeKind:Null<String>;
	final representedTypeDisplay:Null<String>;
	final metadata:Array<TypedAstInventoryMetadata>;
	final fields:Array<TypedAstInventoryField>;
}

typedef TypedAstInventoryModule = {
	final path:String;
	final sourcePath:String;
	final declarations:Array<TypedAstInventoryDeclaration>;
}

typedef TypedAstInventoryExpressionRoot = {
	final ownerModulePath:String;
	final ownerDeclarationPath:Null<String>;
	final fieldName:Null<String>;
	final role:String;
	final sourceOrder:Int;
	final expressionKind:String;
	final typeKind:String;
}

typedef TypedAstInventoryEntryPoint = {
	final modulePath:Null<String>;
	final declarationPath:Null<String>;
	final expressionKind:String;
}

typedef TypedAstInventoryTotals = {
	final modules:Int;
	final declarations:Int;
	final fields:Int;
	final expressionRoots:Int;
	final expressions:Int;
}

typedef TypedAstInventoryKinds = {
	final declarationKinds:Array<TypedAstInventoryCount>;
	final classKinds:Array<TypedAstInventoryCount>;
	final fieldKinds:Array<TypedAstInventoryCount>;
	final expressionKinds:Array<TypedAstInventoryCount>;
	final typeKinds:Array<TypedAstInventoryCount>;
	final metadataNames:Array<TypedAstInventoryCount>;
}

typedef TypedAstInventorySnapshot = {
	final schemaVersion:Int;
	final status:String;
	final entryPoint:Null<TypedAstInventoryEntryPoint>;
	final totals:TypedAstInventoryTotals;
	final inventory:TypedAstInventoryKinds;
	final modules:Array<TypedAstInventoryModule>;
	final expressionRoots:Array<TypedAstInventoryExpressionRoot>;
}

/** Creates a path-stable, deterministic typed-AST coverage report. */
class TypedAstInventory {
	public static inline final REPORT_DEFINE = "reflaxe_c_typed_ast_report";
	public static inline final REPORT_PREFIX = "HXC_TYPED_AST_INVENTORY=";

	public static function snapshot(program:TypedProgramInput):TypedAstInventorySnapshot {
		final declarationKinds:Map<String, Int> = [];
		final classKinds:Map<String, Int> = [];
		final fieldKinds:Map<String, Int> = [];
		final expressionKinds:Map<String, Int> = [];
		final typeKinds:Map<String, Int> = [];
		final metadataNames:Map<String, Int> = [];
		var fieldCount = 0;

		final modules:Array<TypedAstInventoryModule> = [];
		for (module in program.modules) {
			final declarations:Array<TypedAstInventoryDeclaration> = [];
			for (declaration in module.declarations) {
				increment(declarationKinds, declaration.kind);
				if (declaration.classKind != null) {
					increment(classKinds, declaration.classKind);
				}
				if (declaration.representedTypeKind != null) {
					increment(typeKinds, declaration.representedTypeKind);
				}
				countMetadata(metadataNames, declaration.metadata);

				final fields:Array<TypedAstInventoryField> = [];
				for (field in declaration.fields) {
					fieldCount++;
					increment(fieldKinds, field.kind);
					increment(typeKinds, field.typeKind);
					countMetadata(metadataNames, field.metadata);
					fields.push({
						sourceOrder: field.sourceOrder,
						name: field.name,
						role: field.role,
						kind: field.kind,
						typeKind: field.typeKind,
						typeDisplay: field.typeDisplay,
						isPublic: field.isPublic,
						isExtern: field.isExtern,
						metadata: metadataReport(field.metadata),
						hasExpression: field.expression != null
					});
				}

				declarations.push({
					path: declaration.path,
					name: declaration.name,
					kind: declaration.kind,
					ownerModulePath: declaration.ownerModulePath,
					ownership: declaration.ownership,
					sourcePath: declaration.sourcePath,
					isPrivate: declaration.isPrivate,
					isExtern: declaration.isExtern,
					classKind: declaration.classKind,
					isInterface: declaration.isInterface,
					isFinal: declaration.isFinal,
					isAbstract: declaration.isAbstract,
					representedTypeKind: declaration.representedTypeKind,
					representedTypeDisplay: declaration.representedTypeDisplay,
					metadata: metadataReport(declaration.metadata),
					fields: fields
				});
			}
			modules.push({path: module.path, sourcePath: module.sourcePath, declarations: declarations});
		}

		var expressionCount = 0;
		final expressionRoots:Array<TypedAstInventoryExpressionRoot> = [];
		for (root in program.expressionRoots) {
			expressionRoots.push({
				ownerModulePath: root.ownerModulePath,
				ownerDeclarationPath: root.ownerDeclarationPath,
				fieldName: root.fieldName,
				role: root.role,
				sourceOrder: root.sourceOrder,
				expressionKind: TypedAstNormalizer.expressionKind(root.expression),
				typeKind: TypedAstNormalizer.typeKind(root.expression.t)
			});
			function visit(expression:haxe.macro.Type.TypedExpr):Void {
				expressionCount++;
				increment(expressionKinds, TypedAstNormalizer.expressionKind(expression));
				increment(typeKinds, TypedAstNormalizer.typeKind(expression.t));
				switch expression.expr {
					case TMeta(metadata, _):
						increment(metadataNames, metadata.name);
					case _:
				}
				TypedExprTools.iter(expression, visit);
			}
			visit(root.expression);
		}

		final entryPoint:Null<TypedAstInventoryEntryPoint> = program.entryPoint == null ? null : {
			modulePath: program.entryPoint.modulePath,
			declarationPath: program.entryPoint.declarationPath,
			expressionKind: TypedAstNormalizer.expressionKind(program.entryPoint.expression)
		};
		return {
			schemaVersion: 1,
			status: "normalized-typed-input-before-body-lowering",
			entryPoint: entryPoint,
			totals: {
				modules: program.modules.length,
				declarations: program.declarations.length,
				fields: fieldCount,
				expressionRoots: program.expressionRoots.length,
				expressions: expressionCount
			},
			inventory: {
				declarationKinds: counts(declarationKinds),
				classKinds: counts(classKinds),
				fieldKinds: counts(fieldKinds),
				expressionKinds: counts(expressionKinds),
				typeKinds: counts(typeKinds),
				metadataNames: counts(metadataNames)
			},
			expressionRoots: expressionRoots,
			modules: modules
		};
	}

	static function countMetadata(countsByName:Map<String, Int>, entries:Array<TypedAstMetadata>):Void {
		for (entry in entries) {
			increment(countsByName, entry.name);
		}
	}

	static function metadataReport(entries:Array<TypedAstMetadata>):Array<TypedAstInventoryMetadata> {
		return [for (entry in entries) {name: entry.name, arguments: entry.arguments.copy()}];
	}

	static function increment(countsByKind:Map<String, Int>, kind:String):Void {
		final existing = countsByKind.get(kind);
		countsByKind.set(kind, existing == null ? 1 : existing + 1);
	}

	static function counts(countsByKind:Map<String, Int>):Array<TypedAstInventoryCount> {
		final kinds = [for (kind in countsByKind.keys()) kind];
		kinds.sort(compareStrings);
		return [for (kind in kinds) {kind: kind, count: countsByKind.get(kind)}];
	}

	static function compareStrings(left:String, right:String):Int {
		return left < right ? -1 : (left > right ? 1 : 0);
	}
}
#else
class TypedAstInventory {}
#end
