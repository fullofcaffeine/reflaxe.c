package reflaxe.c.plan;

import haxe.io.Bytes;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.contract.TypedCContract.TypedCContractSnapshot;
import reflaxe.c.contract.TypedCContract.TypedCDeclaration;
import reflaxe.c.contract.TypedCContract.TypedCDependency;
import reflaxe.c.plan.CDeclarationPlan.CHeaderPlan;
import reflaxe.c.plan.CDeclarationPlan.CPlanDeclarationEmission;
import reflaxe.c.plan.CDeclarationPlan.CPlanForwardKind;
import reflaxe.c.plan.CDeclarationPlan.CPlanHeaderVisibility;
import reflaxe.c.plan.CDeclarationPlan.CPlanIncludeKind;
import reflaxe.c.plan.CDeclarationPlan.CPlanIncludeReason;
import reflaxe.c.plan.CDeclarationPlan.CPlannedDeclaration;
import reflaxe.c.plan.CDeclarationPlan.CPlannedForwardDeclaration;
import reflaxe.c.plan.CDeclarationPlan.CPlannedInclude;

/** Pure front end for complete-type, forward-declaration, and include planning. */
class CDeclarationPlanner {
	public function new() {}

	public function plan(snapshot:TypedCContractSnapshot):CDeclarationPlan
		return new PlanningState(snapshot).build();
}

private class PlanningState {
	final snapshot:TypedCContractSnapshot;
	final declarations:Array<TypedCDeclaration>;
	final byModule:Map<String, TypedCDeclaration> = [];
	final dependenciesByModule:Map<String, Array<NormalizedDependency>> = [];
	final ownerIncludes:Map<String, Array<CPlannedInclude>> = [];
	final buildIncludes:Array<CPlannedInclude> = [];

	public function new(snapshot:TypedCContractSnapshot) {
		this.snapshot = snapshot;
		declarations = snapshot.declarations.copy();
		if (snapshot.schemaVersion != 2) {
			throw new CDeclarationPlanningError('typed C contract schema ${snapshot.schemaVersion} is unsupported by declaration planning; expected schema 2');
		}

		declarations.sort((left, right) -> compareUtf8(left.modulePath, right.modulePath));
		indexDeclarations();
		normalizeDependencies();
		collectBuildIncludes(snapshot.buildFacts);
		validateCompleteTypeGraph();
	}

	public function build():CDeclarationPlan {
		final headers:Array<CHeaderPlan> = [];
		for (path in headerPaths()) {
			headers.push(planHeader(path));
		}

		final sourceDeclarations = declarations.filter(declaration -> declaration.headerPath == null);
		final sourceIncludes:Array<CPlannedInclude> = [];
		final sourceForwards:Array<CPlannedForwardDeclaration> = [];
		for (declaration in sourceDeclarations) {
			addOwnedIncludes(sourceIncludes, declaration.modulePath, null);
			planDependencies(declaration, null, PHVPrivate, sourceIncludes, sourceForwards);
		}
		sortIncludes(sourceIncludes);
		sortForwards(sourceForwards);

		return {
			schemaVersion: 1,
			inputSchemaVersion: snapshot.schemaVersion,
			headers: headers,
			sourceIncludes: sourceIncludes,
			sourceForwardDeclarations: sourceForwards,
			sourceDeclarations: orderedDeclarations(sourceDeclarations).map(plannedDeclaration),
			buildIncludes: buildIncludes
		};
	}

	function indexDeclarations():Void {
		for (declaration in declarations) {
			if (declaration.modulePath == "") {
				fail("typed C declaration has an empty module path");
			}
			if (byModule.exists(declaration.modulePath)) {
				fail('duplicate typed C declaration `${declaration.modulePath}`', [declaration.modulePath]);
			}
			validateLayout(declaration);
			validateHeader(declaration);
			byModule.set(declaration.modulePath, declaration);
		}
	}

	function validateLayout(declaration:TypedCDeclaration):Void {
		switch declaration.layout {
			case null | "struct" | "union" | "opaque" | "enum":
			case invalid:
				fail('declaration `${declaration.modulePath}` has unknown C layout `$invalid`', [declaration.modulePath]);
		}
	}

	function validateHeader(declaration:TypedCDeclaration):Void {
		final path = declaration.headerPath;
		final visibility = declaration.headerVisibility;
		if (path == null) {
			if (visibility != null) {
				fail('declaration `${declaration.modulePath}` has header visibility without a header path', [declaration.modulePath]);
			}
			return;
		}

		validatePath(path, true, 'generated header for `${declaration.modulePath}`');
		if (visibility != "public" && visibility != "private") {
			fail('declaration `${declaration.modulePath}` has unknown header visibility `${visibility == null ? "null" : visibility}`',
				[declaration.modulePath]);
		}
	}

	function normalizeDependencies():Void {
		for (declaration in declarations) {
			final requirements:Map<String, String> = [];
			for (dependency in declaration.dependencies) {
				validateDependency(declaration, dependency);
				final existing = requirements.get(dependency.modulePath);
				if (existing == null || dependency.requirement == "complete") {
					requirements.set(dependency.modulePath, dependency.requirement);
				}
			}

			final targets:Array<String> = [];
			for (dependency in declaration.dependencies) {
				if (targets.indexOf(dependency.modulePath) == -1) {
					targets.push(dependency.modulePath);
				}
			}
			targets.sort(compareUtf8);
			final normalized:Array<NormalizedDependency> = [];
			for (target in targets) {
				final requirement = requirements.get(target);
				if (requirement == null) {
					throw "unreachable missing normalized dependency requirement";
				}
				normalized.push({modulePath: target, requirement: requirement});
			}
			dependenciesByModule.set(declaration.modulePath, normalized);
		}
	}

	function validateDependency(owner:TypedCDeclaration, dependency:TypedCDependency):Void {
		if (!byModule.exists(dependency.modulePath)) {
			fail('declaration `${owner.modulePath}` depends on unknown typed C declaration `${dependency.modulePath}`',
				[owner.modulePath, dependency.modulePath]);
		}
		if (dependency.requirement != "forward" && dependency.requirement != "complete") {
			fail('declaration `${owner.modulePath}` has unknown dependency requirement `${dependency.requirement}` for `${dependency.modulePath}`',
				[owner.modulePath, dependency.modulePath]);
		}
	}

	function collectBuildIncludes(facts:Array<TypedCBuildFact>):Void {
		for (fact in facts) {
			if (fact.kind != "include") {
				continue;
			}
			validatePath(fact.name, false, "typed C include");
			if (fact.valueKind != "enum" || (fact.value != "system" && fact.value != "local")) {
				fail('include `${fact.name}` must carry c.IncludeKind.System or c.IncludeKind.Local');
			}
			if (fact.ownerModulePaths.length == 0) {
				fail('include `${fact.name}` has no declaration provenance');
			}

			final owners = sortedUnique(fact.ownerModulePaths);
			final kind = fact.value == "system" ? PIKSystem : PIKLocal;
			for (owner in owners) {
				if (!byModule.exists(owner)) {
					fail('include `${fact.name}` is attributed to unknown declaration `$owner`', [owner]);
				}
				addInclude(buildIncludes, fact.name, kind, PIRBuildFact(owner));
				var includes = ownerIncludes.get(owner);
				if (includes == null) {
					includes = [];
					ownerIncludes.set(owner, includes);
				}
				addInclude(includes, fact.name, kind, PIRBuildFact(owner));
			}
		}
		sortIncludes(buildIncludes);
		for (declaration in declarations) {
			final includes = ownerIncludes.get(declaration.modulePath);
			if (includes != null) {
				sortIncludes(includes);
			}
		}
	}

	function headerPaths():Array<String> {
		final paths:Array<String> = [];
		for (declaration in declarations) {
			if (declaration.headerPath != null && paths.indexOf(declaration.headerPath) == -1) {
				paths.push(declaration.headerPath);
			}
		}
		paths.sort(compareUtf8);
		return paths;
	}

	function planHeader(path:String):CHeaderPlan {
		final group = declarations.filter(declaration -> declaration.headerPath == path);
		final visibility = headerVisibility(group[0]);
		for (declaration in group) {
			if (headerVisibility(declaration) != visibility) {
				fail('generated header `$path` mixes public and private declarations; assign one visibility to the whole header',
					group.map(item -> item.modulePath));
			}
		}

		final includes:Array<CPlannedInclude> = [];
		final forwards:Array<CPlannedForwardDeclaration> = [];
		for (declaration in group) {
			addOwnedIncludes(includes, declaration.modulePath, path);
			if (declaration.layout == "opaque") {
				addForward(forwards, declaration);
			}
			planDependencies(declaration, path, visibility, includes, forwards);
		}
		sortIncludes(includes);
		sortForwards(forwards);

		return {
			path: path,
			visibility: visibility,
			guard: guardFor(path),
			includes: includes,
			forwardDeclarations: forwards,
			declarations: orderedDeclarations(group).map(plannedDeclaration)
		};
	}

	function planDependencies(consumer:TypedCDeclaration, consumerHeader:Null<String>, consumerVisibility:CPlanHeaderVisibility,
			includes:Array<CPlannedInclude>, forwards:Array<CPlannedForwardDeclaration>):Void {
		for (dependency in dependencies(consumer)) {
			final target = declaration(dependency.modulePath);
			if (isExternallyProvided(target)) {
				for (include in ownedIncludes(target.modulePath)) {
					addInclude(includes, include.path, include.kind, PIRExternalType(consumer.modulePath, target.modulePath));
				}
				continue;
			}

			if (isCompleteRequirement(dependency, target)) {
				planCompleteDependency(consumer, consumerHeader, consumerVisibility, target, includes);
			} else {
				addForward(forwards, target);
			}
		}
	}

	function planCompleteDependency(consumer:TypedCDeclaration, consumerHeader:Null<String>, consumerVisibility:CPlanHeaderVisibility,
			target:TypedCDeclaration, includes:Array<CPlannedInclude>):Void {
		if (target.layout == "opaque") {
			fail('declaration `${consumer.modulePath}` requires opaque `${target.modulePath}` by value; use c.Ptr/c.Ref or provide an authoritative external header',
				[consumer.modulePath, target.modulePath]);
		}

		if (consumerHeader == null) {
			if (target.headerPath != null) {
				addInclude(includes, target.headerPath, PIKLocal, PIRCompleteType(consumer.modulePath, target.modulePath));
			}
			return;
		}

		final targetHeader = target.headerPath;
		if (targetHeader == consumerHeader) {
			return;
		}
		if (targetHeader == null) {
			fail('header `$consumerHeader` needs complete type `${target.modulePath}`, but that declaration is source-only; move it to a generated header or use an opaque pointer',
				[consumer.modulePath, target.modulePath]);
			return;
		}
		if (consumerVisibility == PHVPublic && headerVisibility(target) == PHVPrivate) {
			fail('public header `$consumerHeader` needs complete private type `${target.modulePath}` from `$targetHeader`; expose the type publicly or use an opaque pointer',
				[consumer.modulePath, target.modulePath]);
		}
		addInclude(includes, targetHeader, PIKLocal, PIRCompleteType(consumer.modulePath, target.modulePath));
	}

	function addOwnedIncludes(output:Array<CPlannedInclude>, owner:String, headerPath:Null<String>):Void {
		for (include in ownedIncludes(owner)) {
			if (headerPath != null && include.kind == PIKLocal && include.path == headerPath) {
				fail('generated header `$headerPath` directly includes itself through declaration `$owner`', [owner]);
			}
			for (reason in include.reasons) {
				addInclude(output, include.path, include.kind, reason);
			}
		}
	}

	function ownedIncludes(modulePath:String):Array<CPlannedInclude> {
		final includes = ownerIncludes.get(modulePath);
		return includes == null ? [] : includes;
	}

	function validateCompleteTypeGraph():Void {
		final visiting:Map<String, Bool> = [];
		final visited:Map<String, Bool> = [];
		final stack:Array<String> = [];

		function visit(modulePath:String):Void {
			if (visited.exists(modulePath)) {
				return;
			}
			visiting.set(modulePath, true);
			stack.push(modulePath);
			for (dependency in dependencies(declaration(modulePath))) {
				final target = declaration(dependency.modulePath);
				if (!isCompleteRequirement(dependency, target) || isExternallyProvided(target)) {
					continue;
				}
				if (visiting.exists(target.modulePath)) {
					final start = stack.indexOf(target.modulePath);
					final cycle = stack.slice(start);
					cycle.push(target.modulePath);
					fail('impossible complete-type declaration cycle: ${cycle.map(path -> "`" + path + "`").join(" -> ")}; replace at least one by-value edge with c.Ptr/c.Ref or an opaque handle',
						cycle);
				}
				visit(target.modulePath);
			}
			stack.pop();
			visiting.remove(modulePath);
			visited.set(modulePath, true);
		}

		for (declaration in declarations) {
			visit(declaration.modulePath);
		}
	}

	function orderedDeclarations(group:Array<TypedCDeclaration>):Array<TypedCDeclaration> {
		final inGroup:Map<String, Bool> = [];
		for (declaration in group) {
			inGroup.set(declaration.modulePath, true);
		}
		final ordered:Array<TypedCDeclaration> = [];
		final visited:Map<String, Bool> = [];

		function visit(item:TypedCDeclaration):Void {
			if (visited.exists(item.modulePath)) {
				return;
			}
			visited.set(item.modulePath, true);
			for (dependency in dependencies(item)) {
				final target = declaration(dependency.modulePath);
				if (inGroup.exists(target.modulePath) && isCompleteRequirement(dependency, target) && !isExternallyProvided(target)) {
					visit(target);
				}
			}
			ordered.push(item);
		}

		final stable = group.copy();
		stable.sort((left, right) -> compareUtf8(left.modulePath, right.modulePath));
		for (item in stable) {
			visit(item);
		}
		return ordered;
	}

	function plannedDeclaration(declaration:TypedCDeclaration):CPlannedDeclaration {
		return {
			modulePath: declaration.modulePath,
			cName: declaration.cName,
			layout: declaration.layout,
			emission: isExternallyProvided(declaration) ? PDEExternal : (declaration.layout == "opaque" ? PDEOpaque : PDEComplete)
		};
	}

	function addForward(output:Array<CPlannedForwardDeclaration>, target:TypedCDeclaration):Void {
		if (!isForwardDeclarable(target)) {
			fail('declaration `${target.modulePath}` cannot be forward-declared in strict C11; a complete definition is required', [target.modulePath]);
		}
		final cName = target.cName;
		if (cName == null) {
			fail('declaration `${target.modulePath}` needs a finalized C name before it can be forward-declared; add @:c.name or finalize names before planning',
				[target.modulePath]);
			return;
		}
		for (existing in output) {
			if (existing.modulePath == target.modulePath) {
				return;
			}
		}
		output.push({
			modulePath: target.modulePath,
			cName: cName,
			kind: switch target.layout {
				case "struct": PFKStruct;
				case "union": PFKUnion;
				case "opaque": PFKOpaque;
				case _: throw "unreachable forward declaration layout";
			}
		});
	}

	function addInclude(output:Array<CPlannedInclude>, path:String, kind:CPlanIncludeKind, reason:CPlanIncludeReason):Void {
		for (include in output) {
			if (include.path != path) {
				continue;
			}
			if (include.kind != kind) {
				fail('include `$path` is requested as both system and local; choose one canonical include kind');
			}
			final key = reasonKey(reason);
			for (existingReason in include.reasons) {
				if (reasonKey(existingReason) == key) {
					return;
				}
			}
			include.reasons.push(reason);
			include.reasons.sort((left, right) -> compareUtf8(reasonKey(left), reasonKey(right)));
			return;
		}
		output.push({path: path, kind: kind, reasons: [reason]});
	}

	function sortIncludes(includes:Array<CPlannedInclude>):Void {
		for (include in includes) {
			include.reasons.sort((left, right) -> compareUtf8(reasonKey(left), reasonKey(right)));
		}
		includes.sort((left, right) -> {
			final kindOrder = includeKindOrder(left.kind) - includeKindOrder(right.kind);
			return kindOrder != 0 ? kindOrder : compareUtf8(left.path, right.path);
		});
	}

	function sortForwards(forwards:Array<CPlannedForwardDeclaration>):Void {
		forwards.sort((left, right) -> {
			final moduleOrder = compareUtf8(left.modulePath, right.modulePath);
			return moduleOrder != 0 ? moduleOrder : compareUtf8(left.cName, right.cName);
		});
	}

	function dependencies(owner:TypedCDeclaration):Array<NormalizedDependency> {
		final result = dependenciesByModule.get(owner.modulePath);
		return result == null ? [] : result;
	}

	function declaration(modulePath:String):TypedCDeclaration {
		final result = byModule.get(modulePath);
		if (result == null) {
			throw "unreachable missing typed C declaration";
		}
		return result;
	}

	function isCompleteRequirement(dependency:NormalizedDependency, target:TypedCDeclaration):Bool
		return dependency.requirement == "complete" || !isForwardDeclarable(target);

	function isForwardDeclarable(declaration:TypedCDeclaration):Bool
		return declaration.layout == "struct" || declaration.layout == "union" || declaration.layout == "opaque";

	function isExternallyProvided(declaration:TypedCDeclaration):Bool
		return declaration.headerPath == null && declaration.layout == "opaque" && ownedIncludes(declaration.modulePath).length > 0;

	function headerVisibility(declaration:TypedCDeclaration):CPlanHeaderVisibility
		return declaration.headerVisibility == "public" ? PHVPublic : PHVPrivate;

	function guardFor(path:String):String {
		final bytes = Bytes.ofString(path);
		final encoded = new StringBuf();
		for (index in 0...bytes.length) {
			encoded.add(StringTools.hex(bytes.get(index), 2));
		}
		return 'HXC_GENERATED_PATH_${encoded.toString()}_INCLUDED';
	}

	function validatePath(path:String, requireHeaderSuffix:Bool, label:String):Void {
		if (path == "" || StringTools.startsWith(path, "/") || path.indexOf("\\") != -1 || path.indexOf("\"") != -1 || path.indexOf("<") != -1
			|| path.indexOf(">") != -1) {
			fail('$label has non-portable path `$path`');
		}
		for (segment in path.split("/")) {
			if (segment == "" || segment == "." || segment == "..") {
				fail('$label has non-normalized path `$path`');
			}
		}
		for (index in 0...path.length) {
			final code = path.charCodeAt(index);
			if (code == null || code < 0x20 || code == 0x7F) {
				fail('$label contains a control character');
			}
		}
		if (requireHeaderSuffix && !StringTools.endsWith(path, ".h")) {
			fail('$label must end in `.h`: `$path`');
		}
	}

	function sortedUnique(values:Array<String>):Array<String> {
		final output:Array<String> = [];
		for (value in values) {
			if (output.indexOf(value) == -1) {
				output.push(value);
			}
		}
		output.sort(compareUtf8);
		return output;
	}

	function reasonKey(reason:CPlanIncludeReason):String {
		return switch reason {
			case PIRBuildFact(owner): '0\x00$owner';
			case PIRCompleteType(consumer, target): '1\x00$consumer\x00$target';
			case PIRExternalType(consumer, target): '2\x00$consumer\x00$target';
		}
	}

	function includeKindOrder(kind:CPlanIncludeKind):Int
		return kind == PIKSystem ? 0 : 1;

	function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final commonLength = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...commonLength) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}

	function fail(message:String, ?modulePaths:Array<String>):Void
		throw new CDeclarationPlanningError(message, modulePaths);
}

private typedef NormalizedDependency = {
	final modulePath:String;
	final requirement:String;
}
