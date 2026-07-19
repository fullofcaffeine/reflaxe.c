package reflaxe.c.lowering;

import reflaxe.c.ir.HxcIR;

/** Why a reducible branch was admitted as one structured C `if`. */
enum CBodyBranchProof {
	/** Both arms meet at the verified immediate post-dominator. */
	CBPPostDominator(joinBlockId:String);

	/** Every normally continuing arm meets here; other arms complete abruptly. */
	CBPNormalJoin(joinBlockId:String);

	/** The true arm completes or escapes, while false is the continuation. */
	CBPTrueEscape(continuationBlockId:String);

	/** The false arm completes or escapes, while true is the continuation. */
	CBPFalseEscape(continuationBlockId:String);

	/** Both arms complete or escape, so the enclosing sequence is closed. */
	CBPClosed;
}

/** Why a structured switch either rejoins or closes all of its arms. */
enum CBodySwitchProof {
	CSPPostDominator(joinBlockId:String);
	CSPNormalJoin(joinBlockId:String);
	CSPClosed;
}

/** The closed authority set for the rare structured escape that needs a label. */
enum CBodyGotoReason {
	CBGRLoopBreakThroughSwitch;
}

/** One typed C switch label. Labels sharing a target share one arm body. */
enum CBodySwitchLabel {
	CSLValue(value:HxcIRConstant);
	CSLTag(tagName:String);
	CSLDefault;
}

/** A normal or abrupt completion at the end of one structural region. */
enum CBodyControlFlowCompletion {
	/** Control reaches the enclosing region's declared continuation. */
	CFCFallthrough;

	/** Every path was completed by nested structured nodes. */
	CFCClosed;

	CFCReturn(ownerBlockId:String);
	CFCThrow(ownerBlockId:String);
	CFCUnreachable(ownerBlockId:String);
	CFCBreak(ownerBlockId:String, targetBlockId:String);
	CFCContinue(ownerBlockId:String, targetBlockId:String);

	/**
		A bounded structural escape that C cannot spell with the nearest `break`.

		The current admitted use is a loop break nested inside a generated C
		switch. It targets that loop's already-validated exit block; it is not a
		fallback for ordinary sequencing or reducible selection.
	**/
	CFCGoto(ownerBlockId:String, targetBlockId:String, reason:CBodyGotoReason);
}

/** A sequence owns every listed node exactly once and declares how it ends. */
class CBodyControlFlowRegion {
	public final nodes:Array<CBodyControlFlowNode>;
	public final completion:CBodyControlFlowCompletion;

	public function new(nodes:Array<CBodyControlFlowNode>, completion:CBodyControlFlowCompletion) {
		this.nodes = nodes.copy();
		this.completion = completion;
	}

	public static function empty():CBodyControlFlowRegion
		return new CBodyControlFlowRegion([], CFCFallthrough);
}

/** One grouped switch arm and its independently validated structural body. */
class CBodyControlFlowSwitchArm {
	public final targetBlockId:String;
	public final labels:Array<CBodySwitchLabel>;
	public final body:CBodyControlFlowRegion;

	public function new(targetBlockId:String, labels:Array<CBodySwitchLabel>, body:CBodyControlFlowRegion) {
		this.targetBlockId = targetBlockId;
		this.labels = labels.copy();
		this.body = body;
	}
}

/**
	Closed target-owned structure between validated HxcIR and syntax-only CAST.

	Nodes identify HxcIR blocks instead of copying their instructions. This keeps
	the semantic IR authoritative while making the selected C control construct,
	its proof boundary, and its one-time block coverage explicit.
**/
enum CBodyControlFlowNode {
	CFNBlock(blockId:String);
	CFNIf(blockId:String, conditionValueId:String, whenTrue:CBodyControlFlowRegion, whenFalse:CBodyControlFlowRegion, proof:CBodyBranchProof);
	CFNWhile(headerBlockId:String, decisionBlockId:String, conditionValueId:String, continuesWhenTrue:Bool, condition:CBodyControlFlowRegion,
		body:CBodyControlFlowRegion, exitBlockId:String);
	CFNDoWhile(headerBlockId:String, decisionBlockId:String, conditionValueId:String, continuesWhenTrue:Bool, body:CBodyControlFlowRegion,
		condition:CBodyControlFlowRegion, exitBlockId:String);
	CFNSwitch(blockId:String, valueId:String, arms:Array<CBodyControlFlowSwitchArm>, proof:CBodySwitchProof);
	CFNTagSwitch(blockId:String, valueId:String, arms:Array<CBodyControlFlowSwitchArm>, proof:CBodySwitchProof);
}

/** The only two admitted whole-function control-flow policies. */
enum CBodyControlFlowPlan {
	CCFStructured(root:CBodyControlFlowRegion, labeledTargets:Array<String>);

	/** A validated but genuinely irreducible graph retains the legacy CFG form. */
	CCFLegacyIrreducible(entryBlockIds:Array<String>);
}

private typedef CBodyLoopContext = {
	final headerBlockId:String;
	final breakTargetBlockId:String;
	final continueTargetBlockId:Null<String>;
}

private enum CBodyLinearExit {
	CLEBreak;
	CLEContinue;
	CLETerminal;
}

private typedef CBodySwitchArmBuilder = {
	final targetBlockId:String;
	final labels:Array<CBodySwitchLabel>;
}

private typedef CBodyLoopDecision = {
	final blockId:String;
	final conditionValueId:String;
	final insideBlockId:String;
	final exitBlockId:String;
	final continuesWhenTrue:Bool;
	final postTest:Bool;
}

private class CBodyNaturalLoop {
	public final headerBlockId:String;
	public final nodes:Map<String, Bool> = [];
	public final backedgeSources:Map<String, Bool> = [];

	public function new(headerBlockId:String) {
		this.headerBlockId = headerBlockId;
	}
}

/** Builds and independently verifies the structural plan for one HxcIR CFG. */
class CBodyControlFlowPlanner {
	public function new() {}

	public function plan(fn:HxcIRFunction):CBodyControlFlowPlan {
		final analysis = new CBodyControlFlowAnalysis(fn);
		analysis.requireAdmittedGraph();
		if (analysis.irreducibleEntries.length > 0) {
			return CCFLegacyIrreducible(analysis.irreducibleEntries);
		}
		final builder = new CBodyControlFlowBuilder(fn, analysis);
		final result = builder.build();
		new CBodyControlFlowPlanValidator(fn, analysis).requireValid(result);
		return result;
	}
}

/**
	Rechecks a previously built plan against its source HxcIR function.

	The planner invokes the same implementation before returning, while this
	public facade lets focused tests and future phase boundaries prove that a
	plan was not corrupted after construction. Validation only checks facts; it
	does not repair, normalize, or otherwise rewrite the plan.
**/
class CBodyControlFlowPlanVerifier {
	public function new() {}

	public function requireValid(fn:HxcIRFunction, plan:CBodyControlFlowPlan):Void {
		final analysis = new CBodyControlFlowAnalysis(fn);
		analysis.requireAdmittedGraph();
		new CBodyControlFlowPlanValidator(fn, analysis).requireValid(plan);
	}
}

private class CBodyControlFlowBuilder {
	final fn:HxcIRFunction;
	final analysis:CBodyControlFlowAnalysis;
	final planned:Map<String, Bool> = [];
	final labeledTargets:Map<String, Bool> = [];

	public function new(fn:HxcIRFunction, analysis:CBodyControlFlowAnalysis) {
		this.fn = fn;
		this.analysis = analysis;
	}

	public function build():CBodyControlFlowPlan {
		final root = buildSequence(fn.entryBlockId, null, null, true, analysis.reachable, null);
		for (blockId in analysis.orderedReachable) {
			if (!planned.exists(blockId))
				fail('structural plan for `${fn.id}` omitted reachable block `$blockId`');
		}
		final labels = [for (blockId in labeledTargets.keys()) blockId];
		labels.sort(analysis.compareBlockIds);
		return CCFStructured(root, labels);
	}

	function buildSequence(startBlockId:String, stopBlockId:Null<String>, loop:Null<CBodyLoopContext>, breakIsDirect:Bool, allowed:Map<String, Bool>,
			suppressedLoopHeader:Null<String>):CBodyControlFlowRegion {
		final nodes:Array<CBodyControlFlowNode> = [];
		var current:Null<String> = startBlockId;
		while (current != null) {
			if (current == stopBlockId)
				return new CBodyControlFlowRegion(nodes, CFCFallthrough);
			if (!allowed.exists(current))
				fail('structural sequence in `${fn.id}` escaped its admitted region through `$current`');
			if (planned.exists(current))
				fail('structural sequence in `${fn.id}` would emit block `$current` more than once');

			final naturalLoop = analysis.loopsByHeader.get(current);
			if (naturalLoop != null && current != suppressedLoopHeader && loopFits(naturalLoop, allowed)) {
				final lowered = buildLoop(naturalLoop, allowed);
				nodes.push(lowered.node);
				current = lowered.exitBlockId;
				continue;
			}

			final block = analysis.requireBlock(current);
			final terminator = requireTerminator(block);
			switch terminator.kind {
				case IRTJump(edge):
					analysis.requirePlainEdge(edge, block.id);
					claim(block.id);
					nodes.push(CFNBlock(block.id));
					if (edge.targetBlockId == stopBlockId)
						return new CBodyControlFlowRegion(nodes, CFCFallthrough);
					final completion = edgeCompletion(block.id, edge.targetBlockId, loop, breakIsDirect);
					if (completion != null)
						return new CBodyControlFlowRegion(nodes, completion);
					current = edge.targetBlockId;
				case IRTBranch(conditionValueId, whenTrue, whenFalse):
					analysis.requirePlainEdge(whenTrue, block.id);
					analysis.requirePlainEdge(whenFalse, block.id);
					claim(block.id);
					final branch = buildBranch(block, conditionValueId, whenTrue.targetBlockId, whenFalse.targetBlockId, stopBlockId, loop, breakIsDirect,
						allowed, suppressedLoopHeader);
					nodes.push(branch.node);
					if (branch.continuationBlockId == null)
						return new CBodyControlFlowRegion(nodes, CFCClosed);
					current = branch.continuationBlockId;
				case IRTSwitch(valueId, cases, defaultEdge):
					claim(block.id);
					final arms:Array<CBodySwitchArmBuilder> = [];
					for (item in cases) {
						analysis.requirePlainEdge(item.edge, block.id);
						addSwitchLabel(arms, item.edge.targetBlockId, CSLValue(item.value));
					}
					analysis.requirePlainEdge(defaultEdge, block.id);
					addSwitchLabel(arms, defaultEdge.targetBlockId, CSLDefault);
					final lowered = buildSwitch(block, valueId, arms, false, stopBlockId, loop, allowed, suppressedLoopHeader);
					nodes.push(lowered.node);
					if (lowered.continuationBlockId == null)
						return new CBodyControlFlowRegion(nodes, CFCClosed);
					current = lowered.continuationBlockId;
				case IRTTagSwitch(valueId, cases, defaultEdge):
					claim(block.id);
					final arms:Array<CBodySwitchArmBuilder> = [];
					for (item in cases) {
						analysis.requirePlainEdge(item.edge, block.id);
						addSwitchLabel(arms, item.edge.targetBlockId, CSLTag(item.tagName));
					}
					if (defaultEdge != null) {
						analysis.requirePlainEdge(defaultEdge, block.id);
						addSwitchLabel(arms, defaultEdge.targetBlockId, CSLDefault);
					}
					final lowered = buildSwitch(block, valueId, arms, true, stopBlockId, loop, allowed, suppressedLoopHeader);
					nodes.push(lowered.node);
					if (lowered.continuationBlockId == null)
						return new CBodyControlFlowRegion(nodes, CFCClosed);
					current = lowered.continuationBlockId;
				case IRTReturn(_, _):
					claim(block.id);
					nodes.push(CFNBlock(block.id));
					return new CBodyControlFlowRegion(nodes, CFCReturn(block.id));
				case IRTThrow(_, _):
					claim(block.id);
					nodes.push(CFNBlock(block.id));
					return new CBodyControlFlowRegion(nodes, CFCThrow(block.id));
				case IRTUnreachable:
					claim(block.id);
					nodes.push(CFNBlock(block.id));
					return new CBodyControlFlowRegion(nodes, CFCUnreachable(block.id));
			}
		}
		return new CBodyControlFlowRegion(nodes, CFCClosed);
	}

	function buildBranch(block:HxcIRBlock, conditionValueId:String, trueTarget:String, falseTarget:String, stopBlockId:Null<String>,
			loop:Null<CBodyLoopContext>, breakIsDirect:Bool, allowed:Map<String, Bool>, suppressedLoopHeader:Null<String>):{
		final node:CBodyControlFlowNode;
		final continuationBlockId:Null<String>;
	} {
		final provenJoin = analysis.immediatePostDominator(block.id);
		if (provenJoin != null && allowed.exists(provenJoin) && provenJoin != loopBreakTarget(loop) && provenJoin != loopContinueTarget(loop)) {
			final trueRegion = trueTarget == provenJoin ? CBodyControlFlowRegion.empty() : buildSequence(trueTarget, provenJoin, loop, breakIsDirect, allowed,
				suppressedLoopHeader);
			final falseRegion = falseTarget == provenJoin ? CBodyControlFlowRegion.empty() : buildSequence(falseTarget, provenJoin, loop, breakIsDirect,
				allowed, suppressedLoopHeader);
			return {
				node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPPostDominator(provenJoin)),
				continuationBlockId: provenJoin
			};
		}
		final normalJoin = findNormalJoin([trueTarget, falseTarget], allowed, loop, stopBlockId);
		if (normalJoin != null) {
			final trueRegion = buildNormalJoinArm(block.id, trueTarget, normalJoin, loop, breakIsDirect, allowed, suppressedLoopHeader);
			final falseRegion = buildNormalJoinArm(block.id, falseTarget, normalJoin, loop, breakIsDirect, allowed, suppressedLoopHeader);
			return {
				node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPNormalJoin(normalJoin)),
				continuationBlockId: normalJoin
			};
		}
		final trueExit = linearExit(trueTarget, falseTarget, stopBlockId, loop, allowed);
		final falseExit = linearExit(falseTarget, trueTarget, stopBlockId, loop, allowed);
		if (trueExit != null || falseExit != null) {
			final trueRegion = trueExit == null ? CBodyControlFlowRegion.empty() : buildEdgeRegion(trueTarget, block.id, loop, breakIsDirect, allowed,
				suppressedLoopHeader);
			final falseRegion = falseExit == null ? CBodyControlFlowRegion.empty() : buildEdgeRegion(falseTarget, block.id, loop, breakIsDirect, allowed,
				suppressedLoopHeader);
			if (trueExit != null && falseExit != null) {
				return {
					node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPClosed),
					continuationBlockId: null
				};
			}
			if (trueExit != null) {
				return {
					node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPTrueEscape(falseTarget)),
					continuationBlockId: falseTarget
				};
			}
			return {
				node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPFalseEscape(trueTarget)),
				continuationBlockId: trueTarget
			};
		}

		var join = analysis.immediatePostDominator(block.id);
		if (join != null && (!allowed.exists(join) || join == loopBreakTarget(loop) || join == loopContinueTarget(loop)))
			join = null;
		if (join == null) {
			final trueRegion = buildSequence(trueTarget, null, loop, breakIsDirect, allowed, suppressedLoopHeader);
			final falseRegion = buildSequence(falseTarget, null, loop, breakIsDirect, allowed, suppressedLoopHeader);
			if (fallsThrough(trueRegion.completion) || fallsThrough(falseRegion.completion))
				fail('reducible branch `${block.id}` in `${fn.id}` has no verified continuation');
			return {
				node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPClosed),
				continuationBlockId: null
			};
		}
		final trueRegion = trueTarget == join ? CBodyControlFlowRegion.empty() : buildSequence(trueTarget, join, loop, breakIsDirect, allowed,
			suppressedLoopHeader);
		final falseRegion = falseTarget == join ? CBodyControlFlowRegion.empty() : buildSequence(falseTarget, join, loop, breakIsDirect, allowed,
			suppressedLoopHeader);
		return {
			node: CFNIf(block.id, conditionValueId, trueRegion, falseRegion, CBPPostDominator(join)),
			continuationBlockId: join
		};
	}

	function buildNormalJoinArm(ownerBlockId:String, targetBlockId:String, joinBlockId:String, loop:Null<CBodyLoopContext>, breakIsDirect:Bool,
			allowed:Map<String, Bool>, suppressedLoopHeader:Null<String>):CBodyControlFlowRegion {
		if (targetBlockId == joinBlockId)
			return CBodyControlFlowRegion.empty();
		final completion = edgeCompletion(ownerBlockId, targetBlockId, loop, breakIsDirect);
		return completion == null ? buildSequence(targetBlockId, joinBlockId, loop, breakIsDirect, allowed,
			suppressedLoopHeader) : new CBodyControlFlowRegion([], completion);
	}

	function buildSwitch(block:HxcIRBlock, valueId:String, builders:Array<CBodySwitchArmBuilder>, tagged:Bool, stopBlockId:Null<String>,
			loop:Null<CBodyLoopContext>, allowed:Map<String, Bool>, suppressedLoopHeader:Null<String>):{
		final node:CBodyControlFlowNode;
		final continuationBlockId:Null<String>;
	} {
		var join = analysis.immediatePostDominator(block.id);
		var proof:CBodySwitchProof = join == null ? CSPClosed : CSPPostDominator(join);
		if (join != null && (!allowed.exists(join) || join == loopBreakTarget(loop)))
			join = null;
		if (join == null) {
			join = findNormalJoin(builders.map(builder -> builder.targetBlockId), allowed, loop, stopBlockId);
			proof = join == null ? CSPClosed : CSPNormalJoin(join);
		}
		final arms:Array<CBodyControlFlowSwitchArm> = [];
		for (builder in builders) {
			final body = if (join != null && builder.targetBlockId == join) {
				CBodyControlFlowRegion.empty();
			} else {
				final completion = edgeCompletion(block.id, builder.targetBlockId, loop, false);
				completion == null ? buildSequence(builder.targetBlockId, join, loop, false, allowed,
					suppressedLoopHeader) : new CBodyControlFlowRegion([], completion);
			};
			arms.push(new CBodyControlFlowSwitchArm(builder.targetBlockId, builder.labels, body));
		}
		if (join == null) {
			for (arm in arms)
				if (fallsThrough(arm.body.completion))
					fail('switch `${block.id}` in `${fn.id}` has an arm without a verified completion');
		}
		return {
			node: tagged ? CFNTagSwitch(block.id, valueId, arms, proof) : CFNSwitch(block.id, valueId, arms, proof),
			continuationBlockId: join
		};
	}

	function findNormalJoin(starts:Array<String>, allowed:Map<String, Bool>, loop:Null<CBodyLoopContext>, preferredCandidate:Null<String>):Null<String> {
		final escapeTargets:Map<String, Bool> = [];
		if (loop != null) {
			escapeTargets.set(loop.breakTargetBlockId, true);
			if (loop.continueTargetBlockId != null)
				escapeTargets.set(loop.continueTargetBlockId, true);
		}
		return analysis.normalJoin(starts, allowed, escapeTargets, planned, preferredCandidate);
	}

	function buildLoop(loop:CBodyNaturalLoop, outerAllowed:Map<String, Bool>):{final node:CBodyControlFlowNode; final exitBlockId:String;} {
		final decision = loopDecision(loop);
		final context:CBodyLoopContext = {
			headerBlockId: loop.headerBlockId,
			breakTargetBlockId: decision.exitBlockId,
			continueTargetBlockId: decision.postTest ? null : loop.headerBlockId
		};
		if (decision.postTest) {
			final body = buildSequence(loop.headerBlockId, decision.blockId, context, true, loop.nodes, loop.headerBlockId);
			final condition = decisionRegion(decision);
			return {
				node: CFNDoWhile(loop.headerBlockId, decision.blockId, decision.conditionValueId, decision.continuesWhenTrue, body, condition,
					decision.exitBlockId),
				exitBlockId: decision.exitBlockId
			};
		}

		final conditionPrefix = loop.headerBlockId == decision.blockId ? CBodyControlFlowRegion.empty() : buildSequence(loop.headerBlockId, decision.blockId,
			null, true, loop.nodes, loop.headerBlockId);
		if (!fallsThrough(conditionPrefix.completion))
			fail('pre-test loop `${loop.headerBlockId}` in `${fn.id}` has a non-completing condition prefix');
		final conditionNodes = conditionPrefix.nodes.copy();
		claim(decision.blockId);
		conditionNodes.push(CFNBlock(decision.blockId));
		final condition = new CBodyControlFlowRegion(conditionNodes, CFCFallthrough);
		final body = buildSequence(decision.insideBlockId, loop.headerBlockId, context, true, loop.nodes, null);
		return {
			node: CFNWhile(loop.headerBlockId, decision.blockId, decision.conditionValueId, decision.continuesWhenTrue, condition, body, decision.exitBlockId),
			exitBlockId: decision.exitBlockId
		};
	}

	function decisionRegion(decision:CBodyLoopDecision):CBodyControlFlowRegion {
		claim(decision.blockId);
		return new CBodyControlFlowRegion([CFNBlock(decision.blockId)], CFCFallthrough);
	}

	function loopDecision(loop:CBodyNaturalLoop):CBodyLoopDecision {
		final post:Array<CBodyLoopDecision> = [];
		final pre:Array<CBodyLoopDecision> = [];
		for (blockId in analysis.orderedReachable) {
			if (!loop.nodes.exists(blockId))
				continue;
			final block = analysis.requireBlock(blockId);
			final terminator = requireTerminator(block);
			switch terminator.kind {
				case IRTBranch(conditionValueId, whenTrue, whenFalse):
					final trueInside = loop.nodes.exists(whenTrue.targetBlockId);
					final falseInside = loop.nodes.exists(whenFalse.targetBlockId);
					if (trueInside == falseInside)
						continue;
					final inside = trueInside ? whenTrue.targetBlockId : whenFalse.targetBlockId;
					final outside = trueInside ? whenFalse.targetBlockId : whenTrue.targetBlockId;
					final value:CBodyLoopDecision = {
						blockId: blockId,
						conditionValueId: conditionValueId,
						insideBlockId: inside,
						exitBlockId: outside,
						continuesWhenTrue: trueInside,
						postTest: inside == loop.headerBlockId
					};
					if (value.postTest) {
						post.push(value);
					} else if (dominatesBackedges(blockId, loop)) {
						pre.push(value);
					}
				case _:
			}
		}
		final candidates = if (post.length > 0) {
			post;
		} else {
			// A body-level `if (...) break` also dominates every surviving
			// backedge, so it satisfies the broad pre-test predicate. The loop
			// condition is the unique outermost candidate: it dominates each
			// nested exit decision. Keeping that proof explicit admits ordinary
			// early breaks without guessing from block names.
			pre.filter(candidate -> {
				var outermost = true;
				for (other in pre) {
					if (other.blockId != candidate.blockId && !analysis.dominates(candidate.blockId, other.blockId)) {
						outermost = false;
						break;
					}
				}
				outermost;
			});
		};
		if (candidates.length != 1)
			fail('natural loop `${loop.headerBlockId}` in `${fn.id}` has ${candidates.length} structural decisions; expected one');
		final result = candidates[0];
		absorbLinearEscapeArms(loop, result.exitBlockId);
		final exits:Map<String, Bool> = [];
		for (blockId in loop.nodes.keys())
			for (target in analysis.successors(blockId))
				if (!loop.nodes.exists(target))
					exits.set(target, true);
		final exitIds = [for (id in exits.keys()) id];
		exitIds.sort(analysis.compareBlockIds);
		if (exitIds.length != 1 || exitIds[0] != result.exitBlockId)
			fail('natural loop `${loop.headerBlockId}` in `${fn.id}` has unowned exits [${exitIds.join(", ")}], expected `${result.exitBlockId}`');
		return result;
	}

	/**
		Natural-loop sets omit acyclic break and terminal arms.

		Absorb only header-dominated linear chains that either reach the proven
		loop exit or end in an explicit HxcIR return/throw/unreachable. This lets
		the region builder own an early break/return exactly once without treating
		an arbitrary second loop exit as structured.
	**/
	function absorbLinearEscapeArms(loop:CBodyNaturalLoop, exitBlockId:String):Void {
		final absorbed:Map<String, Bool> = [];
		var changed = true;
		while (changed) {
			changed = false;
			final sources = [for (id in loop.nodes.keys()) id];
			sources.sort(analysis.compareBlockIds);
			for (source in sources) {
				final targets = analysis.successors(source);
				targets.sort(analysis.compareBlockIds);
				for (target in targets) {
					if (target == exitBlockId || loop.nodes.exists(target))
						continue;
					var chain = linearChainToExit(target, exitBlockId, loop, absorbed);
					if (chain == null)
						chain = linearChainToTerminal(target, loop, absorbed);
					if (chain == null)
						continue;
					for (blockId in chain) {
						if (!loop.nodes.exists(blockId)) {
							loop.nodes.set(blockId, true);
							absorbed.set(blockId, true);
							changed = true;
						}
					}
				}
			}
		}
	}

	function linearChainToExit(startBlockId:String, exitBlockId:String, loop:CBodyNaturalLoop, absorbed:Map<String, Bool>):Null<Array<String>> {
		final result:Array<String> = [];
		final seen:Map<String, Bool> = [];
		var current = startBlockId;
		while (current != exitBlockId) {
			if (absorbed.exists(current))
				return result;
			if (seen.exists(current) || loop.nodes.exists(current) || !analysis.dominates(loop.headerBlockId, current))
				return null;
			seen.set(current, true);
			result.push(current);
			final block = analysis.requireBlock(current);
			final terminator = requireTerminator(block);
			current = switch terminator.kind {
				case IRTJump(edge): edge.targetBlockId;
				case _: return null;
			};
		}
		return result;
	}

	function linearChainToTerminal(startBlockId:String, loop:CBodyNaturalLoop, absorbed:Map<String, Bool>):Null<Array<String>> {
		final result:Array<String> = [];
		final seen:Map<String, Bool> = [];
		var current = startBlockId;
		while (true) {
			if (absorbed.exists(current))
				return result;
			if (seen.exists(current) || loop.nodes.exists(current) || !analysis.dominates(loop.headerBlockId, current))
				return null;
			seen.set(current, true);
			result.push(current);
			final block = analysis.requireBlock(current);
			final terminator = requireTerminator(block);
			switch terminator.kind {
				case IRTJump(edge):
					current = edge.targetBlockId;
				case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable:
					return result;
				case _:
					return null;
			}
		}
	}

	function dominatesBackedges(blockId:String, loop:CBodyNaturalLoop):Bool {
		for (source in loop.backedgeSources.keys())
			if (!analysis.dominates(blockId, source))
				return false;
		return true;
	}

	function buildEdgeRegion(targetBlockId:String, ownerBlockId:String, loop:Null<CBodyLoopContext>, breakIsDirect:Bool, allowed:Map<String, Bool>,
			suppressedLoopHeader:Null<String>):CBodyControlFlowRegion {
		final completion = edgeCompletion(ownerBlockId, targetBlockId, loop, breakIsDirect);
		return completion == null ? buildSequence(targetBlockId, null, loop, breakIsDirect, allowed,
			suppressedLoopHeader) : new CBodyControlFlowRegion([], completion);
	}

	function edgeCompletion(ownerBlockId:String, targetBlockId:String, loop:Null<CBodyLoopContext>, breakIsDirect:Bool):Null<CBodyControlFlowCompletion> {
		if (loop == null)
			return null;
		if (targetBlockId == loop.breakTargetBlockId) {
			if (breakIsDirect)
				return CFCBreak(ownerBlockId, targetBlockId);
			labeledTargets.set(targetBlockId, true);
			return CFCGoto(ownerBlockId, targetBlockId, CBGRLoopBreakThroughSwitch);
		}
		if (loop.continueTargetBlockId != null && targetBlockId == loop.continueTargetBlockId)
			return CFCContinue(ownerBlockId, targetBlockId);
		return null;
	}

	function linearExit(startBlockId:String, forbiddenBlockId:String, stopBlockId:Null<String>, loop:Null<CBodyLoopContext>,
			allowed:Map<String, Bool>):Null<CBodyLinearExit> {
		if (loop != null) {
			if (startBlockId == loop.breakTargetBlockId)
				return CLEBreak;
			if (loop.continueTargetBlockId != null && startBlockId == loop.continueTargetBlockId)
				return CLEContinue;
		}
		var current = startBlockId;
		final seen:Map<String, Bool> = [];
		while (true) {
			if (current == forbiddenBlockId || current == stopBlockId || seen.exists(current) || planned.exists(current) || !allowed.exists(current))
				return null;
			seen.set(current, true);
			final block = analysis.requireBlock(current);
			final terminator = requireTerminator(block);
			switch terminator.kind {
				case IRTJump(edge):
					if (loop != null && edge.targetBlockId == loop.breakTargetBlockId)
						return CLEBreak;
					if (loop != null && loop.continueTargetBlockId != null && edge.targetBlockId == loop.continueTargetBlockId)
						return CLEContinue;
					current = edge.targetBlockId;
				case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable:
					return CLETerminal;
				case _:
					return null;
			}
		}
	}

	function addSwitchLabel(arms:Array<CBodySwitchArmBuilder>, targetBlockId:String, label:CBodySwitchLabel):Void {
		for (arm in arms) {
			if (arm.targetBlockId == targetBlockId) {
				arm.labels.push(label);
				return;
			}
		}
		arms.push({targetBlockId: targetBlockId, labels: [label]});
	}

	function loopFits(loop:CBodyNaturalLoop, allowed:Map<String, Bool>):Bool {
		for (blockId in loop.nodes.keys())
			if (!allowed.exists(blockId))
				return false;
		return true;
	}

	function claim(blockId:String):Void {
		if (planned.exists(blockId))
			fail('structural plan for `${fn.id}` claimed block `$blockId` twice');
		planned.set(blockId, true);
	}

	static function loopBreakTarget(loop:Null<CBodyLoopContext>):Null<String>
		return loop == null ? null : loop.breakTargetBlockId;

	static function loopContinueTarget(loop:Null<CBodyLoopContext>):Null<String>
		return loop == null ? null : loop.continueTargetBlockId;

	static function fallsThrough(completion:CBodyControlFlowCompletion):Bool
		return completion == CFCFallthrough;

	static function requireTerminator(block:HxcIRBlock):HxcIRTerminator {
		if (block.terminator == null)
			return fail('validated block `${block.id}` lost its terminator before control-flow planning');
		return block.terminator;
	}

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);
}

private class CBodyControlFlowPlanValidator {
	final fn:HxcIRFunction;
	final analysis:CBodyControlFlowAnalysis;
	final counts:Map<String, Int> = [];
	final labeled:Map<String, Bool> = [];
	final usedLabels:Map<String, Bool> = [];

	public function new(fn:HxcIRFunction, analysis:CBodyControlFlowAnalysis) {
		this.fn = fn;
		this.analysis = analysis;
	}

	public function requireValid(plan:CBodyControlFlowPlan):Void {
		switch plan {
			case CCFLegacyIrreducible(entries):
				if (analysis.irreducibleEntries.length == 0)
					fail('legacy control-flow fallback for reducible function `${fn.id}` is not admitted');
				if (entries.length != analysis.irreducibleEntries.length)
					fail('legacy control-flow fallback for `${fn.id}` has an incomplete irreducible entry proof');
				for (index in 0...entries.length)
					if (entries[index] != analysis.irreducibleEntries[index])
						fail('legacy control-flow fallback for `${fn.id}` has invalid irreducible entry `${entries[index]}`');
			case CCFStructured(root, labeledTargets):
				if (analysis.irreducibleEntries.length > 0)
					fail('structured plan for irreducible function `${fn.id}` is not admitted');
				final orderedLabeledTargets = labeledTargets.copy();
				orderedLabeledTargets.sort(analysis.compareBlockIds);
				for (target in orderedLabeledTargets) {
					if (labeled.exists(target) || !analysis.reachable.exists(target))
						fail('structured plan for `${fn.id}` has an invalid labeled target `$target`');
					labeled.set(target, true);
				}
				validateRegion(root, null, true);
				requireRegionEntry(root, fn.entryBlockId, fn.entryBlockId, null);
				if (fallsThrough(root.completion))
					fail('structured root for `${fn.id}` has no explicit completion');
				for (blockId in analysis.orderedReachable) {
					final count = counts.get(blockId);
					if (count == null || count != 1)
						fail('structured plan for `${fn.id}` covers block `$blockId` ${count == null ? 0 : count} times');
				}
				for (target in orderedLabeledTargets)
					if (!usedLabels.exists(target))
						fail('structured plan for `${fn.id}` declares unused label target `$target`');
		}
	}

	function validateRegion(region:CBodyControlFlowRegion, loop:Null<CBodyLoopContext>, breakIsDirect:Bool):Void {
		for (node in region.nodes)
			validateNode(node, loop, breakIsDirect);
		validateCompletion(region.completion, loop, breakIsDirect);
		validateRegionSequence(region);
	}

	function validateNode(node:CBodyControlFlowNode, loop:Null<CBodyLoopContext>, breakIsDirect:Bool):Void {
		switch node {
			case CFNBlock(blockId):
				count(blockId);
			case CFNIf(blockId, conditionValueId, whenTrue, whenFalse, proof):
				count(blockId);
				final block = analysis.requireBlock(blockId);
				final targets = switch requireTerminator(block).kind {
					case IRTBranch(actual, whenTrueEdge, whenFalseEdge) if (actual == conditionValueId):
						{whenTrue: whenTrueEdge.targetBlockId, whenFalse: whenFalseEdge.targetBlockId};
					case _: fail('if region `$blockId` in `${fn.id}` no longer matches its HxcIR branch');
				};
				analysis.requireValueAtTerminator(block, conditionValueId);
				validateRegion(whenTrue, loop, breakIsDirect);
				validateRegion(whenFalse, loop, breakIsDirect);
				switch proof {
					case CBPPostDominator(joinBlockId):
						if (analysis.immediatePostDominator(blockId) != joinBlockId)
							fail('if region `$blockId` in `${fn.id}` lost post-dominator `$joinBlockId`');
						requireRegionEntry(whenTrue, targets.whenTrue, blockId, joinBlockId);
						requireRegionEntry(whenFalse, targets.whenFalse, blockId, joinBlockId);
						requireRegionFallthrough(whenTrue, joinBlockId, blockId);
						requireRegionFallthrough(whenFalse, joinBlockId, blockId);
					case CBPNormalJoin(joinBlockId):
						if (!analysis.isNormalJoin(joinBlockId, [targets.whenTrue, targets.whenFalse], analysis.reachable, escapeTargets(loop)))
							fail('if region `$blockId` in `${fn.id}` lost normal join `$joinBlockId`');
						requireRegionEntry(whenTrue, targets.whenTrue, blockId, joinBlockId);
						requireRegionEntry(whenFalse, targets.whenFalse, blockId, joinBlockId);
						requireRegionFallthrough(whenTrue, joinBlockId, blockId);
						requireRegionFallthrough(whenFalse, joinBlockId, blockId);
					case CBPTrueEscape(continuationBlockId):
						if (continuationBlockId != targets.whenFalse)
							fail('if region `$blockId` in `${fn.id}` has a false continuation that is not its HxcIR edge');
						if (fallsThrough(whenTrue.completion))
							fail('if region `$blockId` in `${fn.id}` claims a falling-through true escape');
						requireRegionEntry(whenTrue, targets.whenTrue, blockId, continuationBlockId);
						requireRegionEntry(whenFalse, targets.whenFalse, blockId, continuationBlockId);
						requireRegionFallthrough(whenFalse, continuationBlockId, blockId);
					case CBPFalseEscape(continuationBlockId):
						if (continuationBlockId != targets.whenTrue)
							fail('if region `$blockId` in `${fn.id}` has a true continuation that is not its HxcIR edge');
						if (fallsThrough(whenFalse.completion))
							fail('if region `$blockId` in `${fn.id}` claims a falling-through false escape');
						requireRegionEntry(whenTrue, targets.whenTrue, blockId, continuationBlockId);
						requireRegionEntry(whenFalse, targets.whenFalse, blockId, continuationBlockId);
						requireRegionFallthrough(whenTrue, continuationBlockId, blockId);
					case CBPClosed:
						if (fallsThrough(whenTrue.completion) || fallsThrough(whenFalse.completion))
							fail('closed if region `$blockId` in `${fn.id}` has a falling-through arm');
						requireRegionEntry(whenTrue, targets.whenTrue, blockId, null);
						requireRegionEntry(whenFalse, targets.whenFalse, blockId, null);
				}
			case CFNWhile(headerBlockId, decisionBlockId, conditionValueId, continuesWhenTrue, condition, body, exitBlockId):
				validateLoop(headerBlockId, decisionBlockId, conditionValueId, continuesWhenTrue, condition, body, exitBlockId, false);
			case CFNDoWhile(headerBlockId, decisionBlockId, conditionValueId, continuesWhenTrue, body, condition, exitBlockId):
				validateLoop(headerBlockId, decisionBlockId, conditionValueId, continuesWhenTrue, condition, body, exitBlockId, true);
			case CFNSwitch(blockId, valueId, arms, proof):
				validateSwitch(blockId, valueId, arms, proof, false, loop);
			case CFNTagSwitch(blockId, valueId, arms, proof):
				validateSwitch(blockId, valueId, arms, proof, true, loop);
		}
	}

	function validateLoop(headerBlockId:String, decisionBlockId:String, conditionValueId:String, continuesWhenTrue:Bool, condition:CBodyControlFlowRegion,
			body:CBodyControlFlowRegion, exitBlockId:String, postTest:Bool):Void {
		final optionalLoop = analysis.loopsByHeader.get(headerBlockId);
		if (optionalLoop == null)
			return fail('loop region `$headerBlockId` in `${fn.id}` lost its natural-loop proof');
		final naturalLoop:CBodyNaturalLoop = optionalLoop;
		if (!naturalLoop.nodes.exists(decisionBlockId))
			fail('loop region `$headerBlockId` in `${fn.id}` lost decision `$decisionBlockId`');
		final decision = analysis.requireBlock(decisionBlockId);
		final insideBlockId = switch requireTerminator(decision).kind {
			case IRTBranch(actual, whenTrue, whenFalse)
				if (actual == conditionValueId && (continuesWhenTrue ? whenFalse.targetBlockId : whenTrue.targetBlockId) == exitBlockId):
				continuesWhenTrue ? whenTrue.targetBlockId : whenFalse.targetBlockId;
			case _: fail('loop decision `$decisionBlockId` in `${fn.id}` no longer owns exit `$exitBlockId`');
		};
		if (!naturalLoop.nodes.exists(insideBlockId) || (postTest && insideBlockId != headerBlockId))
			fail('loop decision `$decisionBlockId` in `${fn.id}` no longer enters its natural loop');
		analysis.requireValueAtTerminator(decision, conditionValueId);
		final context:CBodyLoopContext = {
			headerBlockId: headerBlockId,
			breakTargetBlockId: exitBlockId,
			continueTargetBlockId: postTest ? null : headerBlockId
		};
		validateRegion(condition, context, true);
		validateRegion(body, context, true);
		if (!fallsThrough(condition.completion))
			fail('loop condition `$decisionBlockId` in `${fn.id}` does not reach its owned decision');
		requireRegionEntry(condition, postTest ? decisionBlockId : headerBlockId, decisionBlockId, null);
		requireRegionEntry(body, postTest ? headerBlockId : insideBlockId, decisionBlockId, postTest ? decisionBlockId : headerBlockId);
		if (fallsThrough(body.completion))
			requireRegionFallthrough(body, postTest ? decisionBlockId : headerBlockId, decisionBlockId);
	}

	function validateSwitch(blockId:String, valueId:String, arms:Array<CBodyControlFlowSwitchArm>, proof:CBodySwitchProof, tagged:Bool,
			loop:Null<CBodyLoopContext>):Void {
		count(blockId);
		final block = analysis.requireBlock(blockId);
		final expected:Array<CBodySwitchArmBuilder> = [];
		final matches = switch requireTerminator(block).kind {
			case IRTSwitch(actual, cases, defaultEdge) if (!tagged && actual == valueId):
				for (item in cases)
					addExpectedSwitchLabel(expected, item.edge.targetBlockId, CSLValue(item.value));
				addExpectedSwitchLabel(expected, defaultEdge.targetBlockId, CSLDefault);
				true;
			case IRTTagSwitch(actual, cases, defaultEdge) if (tagged && actual == valueId):
				for (item in cases)
					addExpectedSwitchLabel(expected, item.edge.targetBlockId, CSLTag(item.tagName));
				if (defaultEdge != null)
					addExpectedSwitchLabel(expected, defaultEdge.targetBlockId, CSLDefault);
				true;
			case _: false;
		};
		if (!matches)
			fail('switch region `$blockId` in `${fn.id}` no longer matches its HxcIR terminator');
		analysis.requireValueAtTerminator(block, valueId);
		if (arms.length != expected.length)
			fail('switch region `$blockId` in `${fn.id}` has ${arms.length} arms, expected ${expected.length}');
		for (index in 0...arms.length) {
			final arm = arms[index];
			final expectedArm = expected[index];
			if (arm.targetBlockId != expectedArm.targetBlockId || arm.labels.length != expectedArm.labels.length)
				fail('switch region `$blockId` in `${fn.id}` has a target/label shape that differs from HxcIR');
			for (labelIndex in 0...arm.labels.length)
				if (!sameSwitchLabel(arm.labels[labelIndex], expectedArm.labels[labelIndex]))
					fail('switch region `$blockId` in `${fn.id}` has a label that differs from HxcIR');
			validateRegion(arm.body, loop, false);
		}
		var join:Null<String> = null;
		switch proof {
			case CSPPostDominator(joinBlockId):
				if (analysis.immediatePostDominator(blockId) != joinBlockId)
					fail('switch region `$blockId` in `${fn.id}` lost post-dominator `$joinBlockId`');
				join = joinBlockId;
			case CSPNormalJoin(joinBlockId):
				if (!analysis.isNormalJoin(joinBlockId, arms.map(arm -> arm.targetBlockId), analysis.reachable, escapeTargets(loop)))
					fail('switch region `$blockId` in `${fn.id}` lost normal join `$joinBlockId`');
				join = joinBlockId;
			case CSPClosed:
				for (arm in arms)
					if (fallsThrough(arm.body.completion))
						fail('closed switch region `$blockId` in `${fn.id}` has a falling-through arm');
		}
		for (arm in arms)
			requireRegionEntry(arm.body, arm.targetBlockId, blockId, join);
		if (join != null)
			for (arm in arms)
				requireRegionFallthrough(arm.body, join, blockId);
	}

	function validateCompletion(completion:CBodyControlFlowCompletion, loop:Null<CBodyLoopContext>, breakIsDirect:Bool):Void {
		switch completion {
			case CFCFallthrough | CFCClosed:
			case CFCReturn(ownerBlockId):
				requireCompletion(ownerBlockId, completion, kind -> switch kind {
					case IRTReturn(_, _): true;
					case _: false;
				});
			case CFCThrow(ownerBlockId):
				requireCompletion(ownerBlockId, completion, kind -> switch kind {
					case IRTThrow(_, _): true;
					case _: false;
				});
			case CFCUnreachable(ownerBlockId):
				requireCompletion(ownerBlockId, completion, kind -> kind == IRTUnreachable);
			case CFCBreak(ownerBlockId, targetBlockId):
				if (loop == null || !breakIsDirect || loop.breakTargetBlockId != targetBlockId)
					fail('structured break `$ownerBlockId` -> `$targetBlockId` in `${fn.id}` is outside its proven loop');
				requireEdgeCompletion(ownerBlockId, targetBlockId);
			case CFCContinue(ownerBlockId, targetBlockId):
				if (loop == null || loop.continueTargetBlockId == null || loop.continueTargetBlockId != targetBlockId)
					fail('structured continue `$ownerBlockId` -> `$targetBlockId` in `${fn.id}` is outside its proven loop');
				requireEdgeCompletion(ownerBlockId, targetBlockId);
			case CFCGoto(ownerBlockId, targetBlockId, reason):
				if (reason != CBGRLoopBreakThroughSwitch
					|| loop == null
					|| breakIsDirect
					|| loop.breakTargetBlockId != targetBlockId
					|| !labeled.exists(targetBlockId))
					fail('structured goto `$ownerBlockId` -> `$targetBlockId` in `${fn.id}` has no admitted bounded reason');
				usedLabels.set(targetBlockId, true);
				requireEdgeCompletion(ownerBlockId, targetBlockId);
		}
	}

	static function escapeTargets(loop:Null<CBodyLoopContext>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		if (loop != null) {
			result.set(loop.breakTargetBlockId, true);
			if (loop.continueTargetBlockId != null)
				result.set(loop.continueTargetBlockId, true);
		}
		return result;
	}

	function requireRegionEntry(region:CBodyControlFlowRegion, expectedBlockId:String, ownerBlockId:String, fallthroughTarget:Null<String>):Void {
		if (region.nodes.length > 0) {
			if (nodeEntryBlockId(region.nodes[0]) != expectedBlockId)
				fail('region owned by `$ownerBlockId` in `${fn.id}` does not begin at HxcIR edge `$expectedBlockId`');
			return;
		}
		final matches = switch region.completion {
			case CFCFallthrough: fallthroughTarget != null && expectedBlockId == fallthroughTarget;
			case CFCBreak(owner, target) | CFCContinue(owner, target) | CFCGoto(owner, target, _): owner == ownerBlockId && target == expectedBlockId;
			case _: false;
		};
		if (!matches)
			fail('empty region owned by `$ownerBlockId` in `${fn.id}` does not represent HxcIR edge `$expectedBlockId`');
	}

	function validateRegionSequence(region:CBodyControlFlowRegion):Void {
		if (region.nodes.length > 1)
			for (index in 0...(region.nodes.length - 1))
				requireNodeContinuation(region.nodes[index], nodeEntryBlockId(region.nodes[index + 1]));
		if (region.nodes.length == 0) {
			if (region.completion == CFCClosed)
				fail('structured region in `${fn.id}` is empty but claims closed completion');
			return;
		}
		final last = region.nodes[region.nodes.length - 1];
		switch region.completion {
			case CFCFallthrough:
			case CFCClosed:
				if (nodeContinuation(last) != null)
					fail('closed structured region in `${fn.id}` retains a normal continuation');
			case CFCReturn(ownerBlockId) | CFCThrow(ownerBlockId) | CFCUnreachable(ownerBlockId):
				requireLastBlockOwner(last, ownerBlockId);
			case CFCBreak(ownerBlockId, targetBlockId) | CFCContinue(ownerBlockId, targetBlockId) | CFCGoto(ownerBlockId, targetBlockId, _):
				requireLastBlockOwner(last, ownerBlockId);
				requireNodeContinuation(last, targetBlockId);
		}
	}

	function requireRegionFallthrough(region:CBodyControlFlowRegion, targetBlockId:String, ownerBlockId:String):Void {
		if (!fallsThrough(region.completion))
			return;
		if (region.nodes.length > 0) {
			requireNodeContinuation(region.nodes[region.nodes.length - 1], targetBlockId);
		} else if (targetBlockId == ownerBlockId) {
			fail('empty region in `${fn.id}` cannot fall through to its own owner `$ownerBlockId`');
		}
	}

	function requireNodeContinuation(node:CBodyControlFlowNode, expectedBlockId:String):Void {
		final actual = nodeContinuation(node);
		final actualLabel = actual == null ? "none" : actual;
		if (actual != expectedBlockId)
			fail('structured node `${nodeEntryBlockId(node)}` in `${fn.id}` continues to `$actualLabel`, expected `$expectedBlockId`');
	}

	function nodeContinuation(node:CBodyControlFlowNode):Null<String>
		return switch node {
			case CFNBlock(blockId):
				switch requireTerminator(analysis.requireBlock(blockId)).kind {
					case IRTJump(edge): edge.targetBlockId;
					case _: null;
				}
			case CFNIf(_, _, _, _, CBPPostDominator(joinBlockId)) | CFNIf(_, _, _, _, CBPNormalJoin(joinBlockId)) |
				CFNIf(_, _, _, _, CBPTrueEscape(joinBlockId)) | CFNIf(_, _, _, _, CBPFalseEscape(joinBlockId)):
				joinBlockId;
			case CFNIf(_, _, _, _, CBPClosed): null;
			case CFNWhile(_, _, _, _, _, _, exitBlockId) | CFNDoWhile(_, _, _, _, _, _, exitBlockId): exitBlockId;
			case CFNSwitch(_, _, _, CSPPostDominator(joinBlockId)) | CFNTagSwitch(_, _, _, CSPPostDominator(joinBlockId)) |
				CFNSwitch(_, _, _, CSPNormalJoin(joinBlockId)) | CFNTagSwitch(_, _, _, CSPNormalJoin(joinBlockId)):
				joinBlockId;
			case CFNSwitch(_, _, _, CSPClosed) | CFNTagSwitch(_, _, _, CSPClosed): null;
		};

	function requireLastBlockOwner(node:CBodyControlFlowNode, ownerBlockId:String):Void {
		switch node {
			case CFNBlock(blockId) if (blockId == ownerBlockId):
			case _:
				fail('structured completion owner `$ownerBlockId` in `${fn.id}` is not the region final HxcIR block');
		}
	}

	static function nodeEntryBlockId(node:CBodyControlFlowNode):String
		return switch node {
			case CFNBlock(blockId) | CFNIf(blockId, _, _, _, _) | CFNSwitch(blockId, _, _, _) | CFNTagSwitch(blockId, _, _, _): blockId;
			case CFNWhile(headerBlockId, _, _, _, _, _, _) | CFNDoWhile(headerBlockId, _, _, _, _, _, _): headerBlockId;
		};

	static function addExpectedSwitchLabel(arms:Array<CBodySwitchArmBuilder>, targetBlockId:String, label:CBodySwitchLabel):Void {
		for (arm in arms) {
			if (arm.targetBlockId == targetBlockId) {
				arm.labels.push(label);
				return;
			}
		}
		arms.push({targetBlockId: targetBlockId, labels: [label]});
	}

	static function sameSwitchLabel(left:CBodySwitchLabel, right:CBodySwitchLabel):Bool
		return switch [left, right] {
			case [CSLValue(leftValue), CSLValue(rightValue)]: sameConstant(leftValue, rightValue);
			case [CSLTag(leftName), CSLTag(rightName)]: leftName == rightName;
			case [CSLDefault, CSLDefault]: true;
			case _: false;
		};

	static function sameConstant(left:HxcIRConstant, right:HxcIRConstant):Bool
		return switch [left, right] {
			case [IRCInt(leftValue), IRCInt(rightValue)] | [IRCFloat(leftValue), IRCFloat(rightValue)]: leftValue == rightValue;
			case [IRCBool(leftValue), IRCBool(rightValue)]: leftValue == rightValue;
			case [IRCString(leftValue, leftLength), IRCString(rightValue, rightLength)] | [
				IRCCStringLiteral(leftValue, leftLength),
				IRCCStringLiteral(rightValue, rightLength)
			]: leftValue == rightValue && leftLength == rightLength;
			case [IRCNativeConstant(leftId), IRCNativeConstant(rightId)]: leftId == rightId;
			case [IRCNull, IRCNull]: true;
			case _: false;
		};

	function requireCompletion(ownerBlockId:String, completion:CBodyControlFlowCompletion, accepts:HxcIRTerminatorKind->Bool):Void {
		final block = analysis.requireBlock(ownerBlockId);
		if (!accepts(requireTerminator(block).kind))
			fail('completion `$completion` in `${fn.id}` does not match owner block `$ownerBlockId`');
	}

	function requireEdgeCompletion(ownerBlockId:String, targetBlockId:String):Void {
		if (analysis.successors(ownerBlockId).indexOf(targetBlockId) == -1)
			fail('structured escape `$ownerBlockId` -> `$targetBlockId` in `${fn.id}` is not an HxcIR edge');
	}

	function count(blockId:String):Void {
		analysis.requireBlock(blockId);
		final prior = counts.get(blockId);
		counts.set(blockId, prior == null ? 1 : prior + 1);
	}

	static function fallsThrough(completion:CBodyControlFlowCompletion):Bool
		return completion == CFCFallthrough;

	static function requireTerminator(block:HxcIRBlock):HxcIRTerminator {
		if (block.terminator == null)
			return fail('validated block `${block.id}` lost its terminator before plan validation');
		return block.terminator;
	}

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);
}

private class CBodyControlFlowAnalysis {
	static inline final EXIT_ID = "\x00hxc-control-flow-exit";

	public final fn:HxcIRFunction;
	public final blocks:Map<String, HxcIRBlock> = [];
	public final blockOrder:Map<String, Int> = [];
	public final orderedReachable:Array<String> = [];
	public final reachable:Map<String, Bool> = [];
	public final predecessors:Map<String, Array<String>> = [];
	public final dominators:Map<String, Map<String, Bool>> = [];
	public final postDominators:Map<String, Map<String, Bool>> = [];
	public final loopsByHeader:Map<String, CBodyNaturalLoop> = [];
	public final irreducibleEntries:Array<String> = [];

	public function new(fn:HxcIRFunction) {
		this.fn = fn;
		for (index => block in fn.blocks) {
			if (blocks.exists(block.id))
				fail('control-flow analysis for `${fn.id}` received duplicate block `${block.id}`');
			blocks.set(block.id, block);
			blockOrder.set(block.id, index);
			predecessors.set(block.id, []);
		}
		computeReachability();
		computePredecessors();
		computeDominators();
		computePostDominators();
		computeNaturalLoops();
		computeIrreducibleEntries();
	}

	public function requireAdmittedGraph():Void {
		if (fn.blocks.length == 0 || fn.blocks[0].id != fn.entryBlockId)
			fail('control-flow planning requires entry-first blocks in `${fn.id}`');
		if (orderedReachable.length != fn.blocks.length)
			fail('control-flow planning for `${fn.id}` rejects unreachable HxcIR blocks');
		for (blockId in orderedReachable) {
			final block = requireBlock(blockId);
			if (block.parameters.length != 0)
				fail('control-flow planning for `${fn.id}` does not admit block parameters at `$blockId`');
			for (instruction in block.instructions)
				requireAdmittedInstructionFailure(block, instruction);
			final terminator = requireBlockTerminator(block);
			switch terminator.kind {
				case IRTJump(edge):
					requirePlainEdge(edge, blockId);
				case IRTBranch(_, whenTrue, whenFalse):
					requirePlainEdge(whenTrue, blockId);
					requirePlainEdge(whenFalse, blockId);
				case IRTSwitch(_, cases, defaultEdge):
					for (item in cases)
						requirePlainEdge(item.edge, blockId);
					requirePlainEdge(defaultEdge, blockId);
				case IRTTagSwitch(_, cases, defaultEdge):
					for (item in cases)
						requirePlainEdge(item.edge, blockId);
					if (defaultEdge != null)
						requirePlainEdge(defaultEdge, blockId);
				case IRTThrow(_, failure):
					switch failure.target {
						case IRFTBlock(target):
							fail('control-flow planning for `${fn.id}` does not yet admit throw-to-block edge `$blockId` -> `$target`');
						case IRFTPropagate | IRFTAbort:
					}
				case IRTReturn(_, _) | IRTUnreachable:
			}
		}
	}

	function requireAdmittedInstructionFailure(block:HxcIRBlock, instruction:HxcIRInstruction):Void {
		final failure:Null<HxcIRFailureEdge> = switch instruction.kind {
			case IRIOCall(call): call.failure;
			case IRIOConvert(_, _, _, _, edge) | IRIOAllocate(_, _, _, edge): edge;
			case _: null;
		};
		if (failure == null)
			return;
		switch failure.target {
			case IRFTBlock(targetBlockId):
				fail('control-flow planning for `${fn.id}` does not yet admit instruction failure edge `${block.id}`/`${instruction.id}` -> `$targetBlockId`');
			case IRFTPropagate | IRFTAbort:
		}
	}

	public function requirePlainEdge(edge:HxcIRBlockEdge, ownerBlockId:String):Void {
		if (edge.arguments.length != 0 || edge.cleanup.length != 0)
			fail('structural edge `$ownerBlockId` -> `${edge.targetBlockId}` in `${fn.id}` carries block arguments or cleanup');
		if (!blocks.exists(edge.targetBlockId))
			fail('structural edge `$ownerBlockId` in `${fn.id}` targets unknown block `${edge.targetBlockId}`');
	}

	public function requireBlock(blockId:String):HxcIRBlock {
		final block = blocks.get(blockId);
		if (block == null)
			return fail('control-flow plan for `${fn.id}` refers to unknown block `$blockId`');
		return block;
	}

	public function successors(blockId:String):Array<String> {
		final block = requireBlock(blockId);
		if (block.terminator == null)
			return [];
		final result:Array<String> = [];
		function add(target:String):Void {
			if (result.indexOf(target) == -1)
				result.push(target);
		}
		switch block.terminator.kind {
			case IRTJump(edge):
				add(edge.targetBlockId);
			case IRTBranch(_, whenTrue, whenFalse):
				add(whenTrue.targetBlockId);
				add(whenFalse.targetBlockId);
			case IRTSwitch(_, cases, defaultEdge):
				for (item in cases)
					add(item.edge.targetBlockId);
				add(defaultEdge.targetBlockId);
			case IRTTagSwitch(_, cases, defaultEdge):
				for (item in cases)
					add(item.edge.targetBlockId);
				if (defaultEdge != null)
					add(defaultEdge.targetBlockId);
			case IRTThrow(_, failure):
				switch failure.target {
					case IRFTBlock(target): add(target);
					case IRFTPropagate | IRFTAbort:
				}
			case IRTReturn(_, _) | IRTUnreachable:
		}
		return result;
	}

	public function dominates(dominator:String, blockId:String):Bool {
		final values = dominators.get(blockId);
		return values != null && values.exists(dominator);
	}

	public function postDominates(postDominator:String, blockId:String):Bool {
		final values = postDominators.get(blockId);
		return values != null && values.exists(postDominator);
	}

	public function immediatePostDominator(blockId:String):Null<String> {
		final values = postDominators.get(blockId);
		if (values == null)
			return null;
		final candidates = [
			for (candidate in orderedReachable)
				if (candidate != blockId && values.exists(candidate)) candidate
		];
		for (candidate in candidates) {
			var immediate = true;
			for (other in candidates) {
				if (other == candidate)
					continue;
				final otherSet = postDominators.get(other);
				if (otherSet != null && otherSet.exists(candidate)) {
					immediate = false;
					break;
				}
			}
			if (immediate)
				return candidate;
		}
		return null;
	}

	public function requireValueAtTerminator(block:HxcIRBlock, valueId:String):Void {
		for (parameter in fn.parameters)
			if (parameter.id == valueId)
				return;
		for (parameter in block.parameters)
			if (parameter.id == valueId)
				return;
		for (instruction in block.instructions)
			if (instruction.result != null && instruction.result.id == valueId)
				return;
		fail('terminator in `${fn.id}` block `${block.id}` cannot resolve value `$valueId` locally');
	}

	/**
		Find the nearest block reached by every normally continuing start.

		A path may avoid the join only by reaching an explicit terminal or one
		of the active structured escape targets. Closed cycles are rejected
		conservatively instead of being guessed to complete. An abrupt block
		reachable from the candidate belongs to its continuation and therefore
		cannot justify bypassing the candidate. Distinct arm occurrences must
		also own disjoint pre-candidate block sets; otherwise building a tree
		would duplicate the shared block. Prefer a valid enclosing stop so a
		nested plan cannot consume its parent's continuation.
	**/
	public function normalJoin(starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>, unavailable:Map<String, Bool>,
			preferredCandidate:Null<String>):Null<String> {
		if (preferredCandidate != null
			&& allowed.exists(preferredCandidate)
			&& !escapeTargets.exists(preferredCandidate)
			&& !unavailable.exists(preferredCandidate)
			&& isNormalJoinWithAvailability(preferredCandidate, starts, allowed, escapeTargets, unavailable))
			return preferredCandidate;
		var best:Null<String> = null;
		var bestReachCount = -1;
		var bestMaximum = 0x3FFFFFFF;
		var bestTotal = 0x3FFFFFFF;
		for (candidate in orderedReachable) {
			if (!allowed.exists(candidate)
				|| escapeTargets.exists(candidate)
				|| unavailable.exists(candidate)
				|| !isNormalJoinWithAvailability(candidate, starts, allowed, escapeTargets, unavailable))
				continue;
			var reachCount = 0;
			var maximum = 0;
			var total = 0;
			for (start in starts) {
				final distance = shortestDistance(start, candidate, allowed);
				if (distance == null)
					continue;
				reachCount++;
				if (distance > maximum)
					maximum = distance;
				total += distance;
			}
			if (reachCount > bestReachCount
				|| reachCount == bestReachCount
				&& (maximum < bestMaximum || maximum == bestMaximum && total < bestTotal)) {
				best = candidate;
				bestReachCount = reachCount;
				bestMaximum = maximum;
				bestTotal = total;
			}
		}
		return best;
	}

	public function isNormalJoin(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Bool {
		final unavailable:Map<String, Bool> = [];
		return isNormalJoinWithAvailability(candidate, starts, allowed, escapeTargets, unavailable);
	}

	function isNormalJoinWithAvailability(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			unavailable:Map<String, Bool>):Bool {
		if (!allowed.exists(candidate) || escapeTargets.exists(candidate))
			return false;
		final completing = completionSet(candidate, allowed, escapeTargets);
		var hasContinuingPath = false;
		for (start in starts) {
			if (shortestDistance(start, candidate, allowed) != null)
				hasContinuingPath = true;
			if (!escapeTargets.exists(start) && !completing.exists(start))
				return false;
		}
		return hasContinuingPath && prefixesAreDisjoint(candidate, starts, allowed, escapeTargets, unavailable);
	}

	function completionSet(candidate:String, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		result.set(candidate, true);
		final continuation = forwardReachable(candidate, allowed);
		for (blockId in orderedReachable)
			if (allowed.exists(blockId) && !continuation.exists(blockId) && isAbruptTerminal(requireBlock(blockId)))
				result.set(blockId, true);
		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in orderedReachable) {
				if (!allowed.exists(blockId) || result.exists(blockId))
					continue;
				final outgoing = successors(blockId);
				if (outgoing.length == 0)
					continue;
				var complete = true;
				for (target in outgoing) {
					if (!escapeTargets.exists(target) && target != candidate && (!allowed.exists(target) || !result.exists(target))) {
						complete = false;
						break;
					}
				}
				if (complete) {
					result.set(blockId, true);
					changed = true;
				}
			}
		}
		return result;
	}

	function prefixesAreDisjoint(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			unavailable:Map<String, Bool>):Bool {
		final ownerByBlock:Map<String, Int> = [];
		for (armIndex => start in starts) {
			if (start == candidate || escapeTargets.exists(start))
				continue;
			final pending:Array<String> = [start];
			final seen:Map<String, Bool> = [];
			var index = 0;
			while (index < pending.length) {
				final current = pending[index++];
				if (current == candidate || escapeTargets.exists(current) || seen.exists(current))
					continue;
				if (!allowed.exists(current) || unavailable.exists(current))
					return false;
				seen.set(current, true);
				final owner = ownerByBlock.get(current);
				if (owner != null && owner != armIndex)
					return false;
				ownerByBlock.set(current, armIndex);
				for (target in successors(current))
					if (target != candidate && !escapeTargets.exists(target) && !seen.exists(target))
						pending.push(target);
			}
		}
		return true;
	}

	function forwardReachable(start:String, allowed:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		final pending:Array<String> = [start];
		var index = 0;
		while (index < pending.length) {
			final current = pending[index++];
			if (!allowed.exists(current) || result.exists(current))
				continue;
			result.set(current, true);
			for (target in successors(current))
				if (!result.exists(target))
					pending.push(target);
		}
		return result;
	}

	function shortestDistance(start:String, candidate:String, allowed:Map<String, Bool>):Null<Int> {
		if (start == candidate)
			return 0;
		if (!allowed.exists(start))
			return null;
		final pending:Array<String> = [start];
		final distances:Map<String, Int> = [start => 0];
		var index = 0;
		while (index < pending.length) {
			final current = pending[index++];
			final distance = distances.get(current);
			if (distance == null)
				return fail('normal-join distance in `${fn.id}` lost block `$current`');
			for (target in successors(current)) {
				if (target == candidate)
					return distance + 1;
				if (allowed.exists(target) && !distances.exists(target)) {
					distances.set(target, distance + 1);
					pending.push(target);
				}
			}
		}
		return null;
	}

	static function isAbruptTerminal(block:HxcIRBlock):Bool {
		if (block.terminator == null)
			return false;
		return switch block.terminator.kind {
			case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable: true;
			case _: false;
		};
	}

	public function compareBlockIds(left:String, right:String):Int {
		final leftOrder = blockOrder.get(left);
		final rightOrder = blockOrder.get(right);
		if (leftOrder == null || rightOrder == null)
			return left < right ? -1 : (left > right ? 1 : 0);
		return leftOrder - rightOrder;
	}

	function computeReachability():Void {
		if (!blocks.exists(fn.entryBlockId))
			fail('control-flow analysis for `${fn.id}` cannot find entry `${fn.entryBlockId}`');
		final pending:Array<String> = [fn.entryBlockId];
		var index = 0;
		while (index < pending.length) {
			final blockId = pending[index++];
			if (reachable.exists(blockId))
				continue;
			reachable.set(blockId, true);
			for (target in successors(blockId))
				if (!reachable.exists(target))
					pending.push(target);
		}
		for (block in fn.blocks)
			if (reachable.exists(block.id))
				orderedReachable.push(block.id);
	}

	function computePredecessors():Void {
		for (blockId in orderedReachable) {
			for (target in successors(blockId)) {
				if (!reachable.exists(target))
					continue;
				final values = requirePredecessors(target);
				if (values.indexOf(blockId) == -1)
					values.push(blockId);
			}
		}
	}

	function requirePredecessors(blockId:String):Array<String> {
		final values = predecessors.get(blockId);
		if (values == null)
			return fail('control-flow analysis for `${fn.id}` lost predecessor set `$blockId`');
		return values;
	}

	function requireBlockTerminator(block:HxcIRBlock):HxcIRTerminator {
		final value = block.terminator;
		if (value == null)
			return fail('control-flow planning for `${fn.id}` received unterminated block `${block.id}`');
		return value;
	}

	function computeDominators():Void {
		final all = setOf(orderedReachable);
		for (blockId in orderedReachable)
			dominators.set(blockId, blockId == fn.entryBlockId ? singleton(blockId) : copySet(all));
		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in orderedReachable) {
				if (blockId == fn.entryBlockId)
					continue;
				final incoming = predecessors.get(blockId);
				if (incoming == null || incoming.length == 0)
					continue;
				var next = copySet(requireSet(dominators, incoming[0], "dominator"));
				for (index in 1...incoming.length)
					next = intersect(next, requireSet(dominators, incoming[index], "dominator"));
				next.set(blockId, true);
				if (!sameSet(next, requireSet(dominators, blockId, "dominator"))) {
					dominators.set(blockId, next);
					changed = true;
				}
			}
		}
	}

	function computePostDominators():Void {
		final universe = orderedReachable.copy();
		universe.push(EXIT_ID);
		final all = setOf(universe);
		postDominators.set(EXIT_ID, singleton(EXIT_ID));
		for (blockId in orderedReachable)
			postDominators.set(blockId, copySet(all));
		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in orderedReachable) {
				var outgoing = successors(blockId).filter(target -> reachable.exists(target));
				if (outgoing.length == 0)
					outgoing = [EXIT_ID];
				var next = copySet(requireSet(postDominators, outgoing[0], "post-dominator"));
				for (index in 1...outgoing.length)
					next = intersect(next, requireSet(postDominators, outgoing[index], "post-dominator"));
				next.set(blockId, true);
				if (!sameSet(next, requireSet(postDominators, blockId, "post-dominator"))) {
					postDominators.set(blockId, next);
					changed = true;
				}
			}
		}
	}

	function computeNaturalLoops():Void {
		for (source in orderedReachable) {
			for (target in successors(source)) {
				if (!reachable.exists(target) || !dominates(target, source))
					continue;
				var loop = loopsByHeader.get(target);
				if (loop == null) {
					loop = new CBodyNaturalLoop(target);
					loopsByHeader.set(target, loop);
				}
				loop.backedgeSources.set(source, true);
				loop.nodes.set(target, true);
				loop.nodes.set(source, true);
				final pending:Array<String> = source == target ? [] : [source];
				while (pending.length > 0) {
					final current = pending.pop();
					if (current == null)
						break;
					final incoming = predecessors.get(current);
					if (incoming == null)
						continue;
					for (predecessor in incoming) {
						if (!loop.nodes.exists(predecessor)) {
							loop.nodes.set(predecessor, true);
							if (predecessor != target)
								pending.push(predecessor);
						}
					}
				}
			}
		}
	}

	function computeIrreducibleEntries():Void {
		final entries:Map<String, Bool> = [];
		scanIrreducibleComponents(reachable, entries);
		for (blockId in orderedReachable)
			if (entries.exists(blockId))
				irreducibleEntries.push(blockId);
	}

	function scanIrreducibleComponents(admitted:Map<String, Bool>, entries:Map<String, Bool>):Void {
		for (component in stronglyConnectedComponents(admitted)) {
			final members = setOf(component);
			final cyclic = component.length > 1 || successors(component[0]).indexOf(component[0]) != -1;
			if (!cyclic)
				continue;
			final componentEntries:Map<String, Bool> = [];
			for (member in component) {
				if (member == fn.entryBlockId)
					componentEntries.set(member, true);
				final incoming = predecessors.get(member);
				if (incoming != null)
					for (predecessor in incoming)
						if (!members.exists(predecessor))
							componentEntries.set(member, true);
			}
			final orderedEntries = [for (blockId in orderedReachable) if (componentEntries.exists(blockId)) blockId];
			if (orderedEntries.length > 1) {
				for (entry in orderedEntries)
					entries.set(entry, true);
				continue;
			}
			if (orderedEntries.length == 1) {
				// A single-entry outer SCC can still hide a multi-entry SCC below its
				// unique header. Remove only that header and analyze the induced graph
				// recursively instead of treating the maximal SCC as sufficient proof.
				final nested = copySet(members);
				nested.remove(orderedEntries[0]);
				if ([for (_ in nested.keys()) 1].length > 0)
					scanIrreducibleComponents(nested, entries);
			}
		}
	}

	function stronglyConnectedComponents(admitted:Map<String, Bool>):Array<Array<String>> {
		final indexByBlock:Map<String, Int> = [];
		final lowByBlock:Map<String, Int> = [];
		final onStack:Map<String, Bool> = [];
		final stack:Array<String> = [];
		var nextIndex = 0;
		final components:Array<Array<String>> = [];
		function strongConnect(blockId:String):Void {
			indexByBlock.set(blockId, nextIndex);
			lowByBlock.set(blockId, nextIndex);
			nextIndex++;
			stack.push(blockId);
			onStack.set(blockId, true);
			for (target in successors(blockId)) {
				if (!admitted.exists(target))
					continue;
				if (!indexByBlock.exists(target)) {
					strongConnect(target);
					lowByBlock.set(blockId, minInt(requireInt(lowByBlock, blockId), requireInt(lowByBlock, target)));
				} else if (onStack.exists(target)) {
					lowByBlock.set(blockId, minInt(requireInt(lowByBlock, blockId), requireInt(indexByBlock, target)));
				}
			}
			if (requireInt(lowByBlock, blockId) == requireInt(indexByBlock, blockId)) {
				final component:Array<String> = [];
				while (stack.length > 0) {
					final item = stack.pop();
					if (item == null)
						break;
					onStack.remove(item);
					component.push(item);
					if (item == blockId)
						break;
				}
				components.push(component);
			}
		}
		for (blockId in orderedReachable)
			if (admitted.exists(blockId) && !indexByBlock.exists(blockId))
				strongConnect(blockId);
		return components;
	}

	static function setOf(values:Array<String>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (value in values)
			result.set(value, true);
		return result;
	}

	static function singleton(value:String):Map<String, Bool>
		return setOf([value]);

	static function copySet(source:Map<String, Bool>):Map<String, Bool>
		return [for (key in source.keys()) key => true];

	static function intersect(left:Map<String, Bool>, right:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (key in left.keys())
			if (right.exists(key))
				result.set(key, true);
		return result;
	}

	static function sameSet(left:Map<String, Bool>, right:Map<String, Bool>):Bool {
		for (key in left.keys())
			if (!right.exists(key))
				return false;
		for (key in right.keys())
			if (!left.exists(key))
				return false;
		return true;
	}

	static function requireSet(index:Map<String, Map<String, Bool>>, key:String, label:String):Map<String, Bool> {
		final value = index.get(key);
		if (value == null)
			return fail('control-flow analysis lost $label set `$key`');
		return value;
	}

	static function requireInt(index:Map<String, Int>, key:String):Int {
		final value = index.get(key);
		if (value == null)
			return fail('control-flow analysis lost integer fact `$key`');
		return value;
	}

	static inline function minInt(left:Int, right:Int):Int
		return left < right ? left : right;

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);
}
