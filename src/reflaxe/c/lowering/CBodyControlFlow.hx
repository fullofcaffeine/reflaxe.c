package reflaxe.c.lowering;

import reflaxe.c.ir.HxcIR;
#if (macro || reflaxe_runtime)
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CDetailTimingId;
#end

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
		One structured edge into a shared abrupt tail.

		Haxe may merge identical `return`, `throw`, or unreachable cleanup tails
		from mutually exclusive source paths. C has no expression that names such
		a tail without `goto`; repeating the validated tail at each exit is the
		clear handwritten-C shape. The planner ranks a valid continuing join ahead
		of an abrupt join; otherwise a sequence of guard clauses would become a
		deeply nested `if (!condition) { ... }` chain. A shared abrupt join remains
		valid when every arm only computes the value returned by that one tail, as
		in a value-producing switch. This completion owns the remaining abrupt-only
		arm or a loop-local early exit without inventing a label. It never admits a
		normally continuing target, so it cannot duplicate later work or change
		evaluation order.
	**/
	CFCSharedAbrupt(ownerBlockId:String, targetBlockId:String);

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

	/**
		The block that ends this structural iteration for join analysis.

		For `while`, this is also the C `continue` target. For `do ... while`, it
		is the trailing condition block: structured emission reaches that code by
		fallthrough because a literal C `continue` would skip the synthesized
		condition statements.
	**/
	final iterationBoundaryBlockId:String;

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

private typedef CBodyNormalJoinCandidate = {
	final blockId:String;
	final abrupt:Bool;
	final reachCount:Int;
	final maximumDistance:Int;
	final totalDistance:Int;
}

private class CBodyNaturalLoop {
	public final headerBlockId:String;
	public final nodes:Map<String, Bool> = [];
	public final backedgeSources:Map<String, Bool> = [];

	public function new(headerBlockId:String) {
		this.headerBlockId = headerBlockId;
	}
}

/** One natural-loop completion rule waiting for its remaining exits. */
private class CBodyCompletionLoopRule {
	public final nodes:Array<String>;
	public var remaining:Int;
	public var resolved:Bool = false;

	public function new(nodes:Array<String>, remaining:Int) {
		this.nodes = nodes;
		this.remaining = remaining;
	}
}

/** Builds and independently verifies the structural plan for one HxcIR CFG. */
class CBodyControlFlowPlanner {
	public function new() {}

	public function plan(fn:HxcIRFunction):CBodyControlFlowPlan {
		return planWithWorkReport(fn).plan;
	}

	/**
		Build a plan and expose deterministic algorithmic-work counts for focused
		performance tests. The counts describe graph searches, not elapsed time,
		so a busy machine cannot make the regression flaky.
	**/
	public function planWithWorkReport(fn:HxcIRFunction):CBodyControlFlowPlanningResult {
		#if (macro || reflaxe_runtime)
		final analysisTimer = CPhaseTiming.startDetail(CDTBodyControlFlowAnalysis, fn.id);
		#end
		final analysis = new CBodyControlFlowAnalysis(fn);
		analysis.requireAdmittedGraph();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(analysisTimer);
		#end
		if (analysis.irreducibleEntries.length > 0) {
			return new CBodyControlFlowPlanningResult(CCFLegacyIrreducible(analysis.irreducibleEntries), analysis.workReport());
		}
		#if (macro || reflaxe_runtime)
		final constructionTimer = CPhaseTiming.startDetail(CDTBodyControlFlowConstruction, fn.id);
		#end
		final builder = new CBodyControlFlowBuilder(fn, analysis);
		final result = builder.build();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(constructionTimer);
		final validationTimer = CPhaseTiming.startDetail(CDTBodyControlFlowValidation, fn.id);
		#end
		new CBodyControlFlowPlanValidator(fn, analysis).requireValid(result);
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(validationTimer);
		#end
		return new CBodyControlFlowPlanningResult(result, analysis.workReport());
	}
}

/** A structural plan paired with stable work counts from the same request. */
@:noCompletion
class CBodyControlFlowPlanningResult {
	public final plan:CBodyControlFlowPlan;
	public final work:CBodyControlFlowWorkReport;

	public function new(plan:CBodyControlFlowPlan, work:CBodyControlFlowWorkReport) {
		this.plan = plan;
		this.work = work;
	}
}

/**
	Machine-independent evidence for the normal-join search.

	A "candidate proof" is the expensive check that every normal path reaches a
	possible join and that branch prefixes do not overlap. A "distance search"
	walks the admitted graph once from one branch start. Each completion-set
	search scans the admitted blocks once to seed a reverse worklist, then
	dequeues a block only when all of that block's required successors have
	completed. An *immediate post-dominator* is the first block that every path
	from a source block must reach; plan construction and validation may ask for
	that same settled graph fact. Its query, computation, and cache-hit counts
	prove that repeated questions reuse the fact without reusing the builder's
	plan verdict. Keeping these counts separate makes repeated graph work visible
	without relying on machine timing.
**/
@:noCompletion
class CBodyControlFlowWorkReport {
	public final normalJoinSearches:Int;
	public final normalJoinCandidateProofs:Int;
	public final normalJoinDistanceSearches:Int;
	public final normalJoinDistanceBlockVisits:Int;
	public final completionSetSearches:Int;
	public final completionSetInitialBlockScans:Int;
	public final completionSetWorklistDequeues:Int;
	public final abruptCompletionSetSearches:Int;
	public final abruptCompletionSetInitialBlockScans:Int;
	public final abruptCompletionSetWorklistDequeues:Int;
	public final forwardReachabilitySearches:Int;
	public final forwardReachabilityBlockVisits:Int;
	public final prefixDisjointSearches:Int;
	public final prefixDisjointBlockVisits:Int;
	public final immediatePostDominatorQueries:Int;
	public final immediatePostDominatorComputations:Int;
	public final immediatePostDominatorCacheHits:Int;

	public function new(normalJoinSearches:Int, normalJoinCandidateProofs:Int, normalJoinDistanceSearches:Int, normalJoinDistanceBlockVisits:Int,
			completionSetSearches:Int, completionSetInitialBlockScans:Int, completionSetWorklistDequeues:Int, abruptCompletionSetSearches:Int,
			abruptCompletionSetInitialBlockScans:Int, abruptCompletionSetWorklistDequeues:Int, forwardReachabilitySearches:Int,
			forwardReachabilityBlockVisits:Int, prefixDisjointSearches:Int, prefixDisjointBlockVisits:Int, immediatePostDominatorQueries:Int,
			immediatePostDominatorComputations:Int, immediatePostDominatorCacheHits:Int) {
		this.normalJoinSearches = normalJoinSearches;
		this.normalJoinCandidateProofs = normalJoinCandidateProofs;
		this.normalJoinDistanceSearches = normalJoinDistanceSearches;
		this.normalJoinDistanceBlockVisits = normalJoinDistanceBlockVisits;
		this.completionSetSearches = completionSetSearches;
		this.completionSetInitialBlockScans = completionSetInitialBlockScans;
		this.completionSetWorklistDequeues = completionSetWorklistDequeues;
		this.abruptCompletionSetSearches = abruptCompletionSetSearches;
		this.abruptCompletionSetInitialBlockScans = abruptCompletionSetInitialBlockScans;
		this.abruptCompletionSetWorklistDequeues = abruptCompletionSetWorklistDequeues;
		this.forwardReachabilitySearches = forwardReachabilitySearches;
		this.forwardReachabilityBlockVisits = forwardReachabilityBlockVisits;
		this.prefixDisjointSearches = prefixDisjointSearches;
		this.prefixDisjointBlockVisits = prefixDisjointBlockVisits;
		this.immediatePostDominatorQueries = immediatePostDominatorQueries;
		this.immediatePostDominatorComputations = immediatePostDominatorComputations;
		this.immediatePostDominatorCacheHits = immediatePostDominatorCacheHits;
	}

	/** Report that an exact cache hit performed no graph searches this request. */
	public static function zero():CBodyControlFlowWorkReport
		return new CBodyControlFlowWorkReport(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
				fail('structural sequence in `${fn.id}` escaped its admitted region through `$current`'
					+ ' [start=$startBlockId, stop=${stopBlockId == null ? "none" : stopBlockId}, loop=${loop == null ? "none" : loop.headerBlockId}]');
			if (planned.exists(current)) {
				final predecessors = analysis.predecessorIds(current);
				predecessors.sort(analysis.compareBlockIds);
				fail('structural sequence in `${fn.id}` would emit block `$current` more than once'
					+ ' [start=$startBlockId, stop=${stopBlockId == null ? "none" : stopBlockId}, loop=${loop == null ? "none" : loop.headerBlockId}, '
					+ 'predecessors=${predecessors.join(",")}]');
			}

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
		// A whole-function post-dominator can sit in a later loop iteration.
		// Inside a loop, use the iteration-bounded normal-join proof below so a
		// continue/header edge cannot make that future block look local.
		final provenJoin = loop == null ? analysis.immediatePostDominator(block.id) : null;
		if (provenJoin != null
			&& allowed.exists(provenJoin)
			&& provenJoin != loopBreakTarget(loop)
			&& provenJoin != loopIterationBoundaryTarget(loop)) {
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

		var join = loop == null ? analysis.immediatePostDominator(block.id) : null;
		if (join != null && (!allowed.exists(join) || join == loopBreakTarget(loop) || join == loopIterationBoundaryTarget(loop)))
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
		// See the branch rule above: loop-local switch joins must not cross a
		// continue edge into a later iteration.
		var join = loop == null ? analysis.immediatePostDominator(block.id) : null;
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
		final iterationBypassTargets:Map<String, Bool> = [];
		if (loop != null) {
			escapeTargets.set(loop.breakTargetBlockId, true);
			escapeTargets.set(loop.iterationBoundaryBlockId, true);
			iterationBypassTargets.set(loop.iterationBoundaryBlockId, true);
		}
		return analysis.normalJoin(starts, allowed, escapeTargets, iterationBypassTargets, planned, preferredCandidate);
	}

	function buildLoop(loop:CBodyNaturalLoop, outerAllowed:Map<String, Bool>):{final node:CBodyControlFlowNode; final exitBlockId:String;} {
		final decision = loopDecision(loop);
		final context:CBodyLoopContext = {
			headerBlockId: loop.headerBlockId,
			breakTargetBlockId: decision.exitBlockId,
			iterationBoundaryBlockId: decision.postTest ? decision.blockId : loop.headerBlockId,
			// A post-test source `continue` is structuralized as fallthrough to
			// the separately emitted trailing condition; see the context contract.
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
		absorbAcyclicEscapeRegions(loop, result.exitBlockId);
		final exits:Map<String, Bool> = [];
		for (blockId in loop.nodes.keys())
			for (target in analysis.successors(blockId))
				if (!loop.nodes.exists(target) && target != result.exitBlockId && !isAbruptTarget(target))
					exits.set(target, true);
		final exitIds = [for (id in exits.keys()) id];
		exitIds.sort(analysis.compareBlockIds);
		if (exitIds.length != 0) {
			final details = exitIds.map(id -> describeUnownedExit(loop, id));
			fail('natural loop `${loop.headerBlockId}` in `${fn.id}` has unowned exits [${exitIds.join(", ")}], expected only '
				+ '`${result.exitBlockId}` or explicit abrupt tails; ${details.join("; ")}');
		}
		return result;
	}

	/**
		Describe why one edge left a natural loop without guessing its meaning.

		A block name alone does not tell a contributor whether the edge represents
		a `break`, an early `return`, or an unrelated continuation. The diagnostic
		reports which loop blocks enter the target and the target's exact HxcIR
		terminator shape. This evidence guides a structural fix; it does not make
		the edge legal by itself.
	**/
	function describeUnownedExit(loop:CBodyNaturalLoop, targetBlockId:String):String {
		final owners = analysis.predecessorIds(targetBlockId).filter(id -> loop.nodes.exists(id));
		owners.sort(analysis.compareBlockIds);
		final terminator = requireTerminator(analysis.requireBlock(targetBlockId));
		final shape = switch terminator.kind {
			case IRTJump(edge):
				'jump->${edge.targetBlockId}';
			case IRTBranch(_, whenTrue, whenFalse):
				'branch->${whenTrue.targetBlockId},${whenFalse.targetBlockId}';
			case IRTSwitch(_, cases, defaultEdge):
				final targets = cases.map(item -> item.edge.targetBlockId);
				targets.push(defaultEdge.targetBlockId);
				'switch->[${targets.join(",")}]';
			case IRTTagSwitch(_, cases, defaultEdge):
				final targets = cases.map(item -> item.edge.targetBlockId);
				if (defaultEdge != null)
					targets.push(defaultEdge.targetBlockId);
				'tag-switch->[${targets.join(",")}]';
			case IRTReturn(_, _):
				"return";
			case IRTThrow(_, _):
				"throw";
			case IRTUnreachable:
				"unreachable";
		};
		return '$targetBlockId from [${owners.join(",")}] is $shape';
	}

	/**
		Natural-loop sets omit acyclic break and abrupt-return regions.

		Haxe may compute an early return value with branches before reaching its
		`return`, so requiring a straight-line chain rejects ordinary source code.
		Absorb a region only when the loop header dominates it, it has no cycle,
		and every path reaches the one proven loop exit or an explicit
		return/throw/unreachable terminator. The structural builder can then own
		the complete `if`/switch-shaped escape without treating an arbitrary second
		loop exit as legal.
	**/
	function absorbAcyclicEscapeRegions(loop:CBodyNaturalLoop, exitBlockId:String):Void {
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
					final region = acyclicEscapeRegion(target, exitBlockId, loop, absorbed);
					if (region == null)
						continue;
					for (blockId in region) {
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

	/**
		Return the complete bounded escape region rooted at `startBlockId`.

		`null` means at least one path cycles, re-enters the natural loop, escapes
		to another destination, or is not owned by this loop header. Already
		absorbed nodes are valid shared tails; they were proven by the same rule in
		an earlier deterministic iteration.
	**/
	function acyclicEscapeRegion(startBlockId:String, exitBlockId:String, loop:CBodyNaturalLoop, absorbed:Map<String, Bool>):Null<Array<String>> {
		final result:Array<String> = [];
		final visiting:Map<String, Bool> = [];
		final complete:Map<String, Bool> = [];

		function visit(blockId:String):Bool {
			if (blockId == exitBlockId || absorbed.exists(blockId))
				return true;
			if (loop.nodes.exists(blockId) || visiting.exists(blockId) || !analysis.dominates(loop.headerBlockId, blockId))
				return false;
			if (complete.exists(blockId))
				return true;

			visiting.set(blockId, true);
			final block = analysis.requireBlock(blockId);
			final terminator = requireTerminator(block);
			final bounded = switch terminator.kind {
				case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable:
					true;
				case _:
					final targets = analysis.successors(blockId);
					if (targets.length == 0) {
						false;
					} else {
						var allBounded = true;
						for (target in targets)
							if (!visit(target)) {
								allBounded = false;
								break;
							}
						allBounded;
					}
			};
			visiting.remove(blockId);
			if (!bounded)
				return false;
			complete.set(blockId, true);
			result.push(blockId);
			return true;
		}

		return visit(startBlockId) ? result : null;
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
		if (loop != null) {
			if (targetBlockId == loop.breakTargetBlockId) {
				if (breakIsDirect)
					return CFCBreak(ownerBlockId, targetBlockId);
				labeledTargets.set(targetBlockId, true);
				return CFCGoto(ownerBlockId, targetBlockId, CBGRLoopBreakThroughSwitch);
			}
			if (loop.continueTargetBlockId != null && targetBlockId == loop.continueTargetBlockId)
				return CFCContinue(ownerBlockId, targetBlockId);
		}
		if (isAbruptTarget(targetBlockId)) {
			if (!planned.exists(targetBlockId))
				claim(targetBlockId);
			return CFCSharedAbrupt(ownerBlockId, targetBlockId);
		}
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
			/*
				A return, throw, or unreachable block is a valid branch escape
				even when it sits just outside the active natural-loop set. Test
				that semantic boundary before rejecting unrelated out-of-region
				blocks; otherwise a direct early return is mistaken for a normal
				continuation.
			 */
			if (isAbruptTarget(current))
				return CLETerminal;
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

	static function loopIterationBoundaryTarget(loop:Null<CBodyLoopContext>):Null<String>
		return loop == null ? null : loop.iterationBoundaryBlockId;

	static function fallsThrough(completion:CBodyControlFlowCompletion):Bool
		return completion == CFCFallthrough;

	static function requireTerminator(block:HxcIRBlock):HxcIRTerminator {
		if (block.terminator == null)
			return fail('validated block `${block.id}` lost its terminator before control-flow planning');
		return block.terminator;
	}

	static function fail<T>(detail:String):T
		throw new CBodyEmissionError(detail);

	function isAbruptTarget(blockId:String):Bool
		return switch requireTerminator(analysis.requireBlock(blockId)).kind {
			case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable: true;
			case _: false;
		};
}

private class CBodyControlFlowPlanValidator {
	final fn:HxcIRFunction;
	final analysis:CBodyControlFlowAnalysis;
	final counts:Map<String, Int> = [];
	final labeled:Map<String, Bool> = [];
	final usedLabels:Map<String, Bool> = [];
	final sharedAbruptTargets:Map<String, Bool> = [];

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
						if (!isNormalOrBoundaryJoin(joinBlockId, [targets.whenTrue, targets.whenFalse], loop))
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
			iterationBoundaryBlockId: postTest ? decisionBlockId : headerBlockId,
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
				if (!isNormalOrBoundaryJoin(joinBlockId, arms.map(arm -> arm.targetBlockId), loop))
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
			case CFCSharedAbrupt(ownerBlockId, targetBlockId):
				requireEdgeCompletion(ownerBlockId, targetBlockId);
				switch requireTerminator(analysis.requireBlock(targetBlockId)).kind {
					case IRTReturn(_, _) | IRTThrow(_, _) | IRTUnreachable:
					case _:
						fail('shared abrupt completion `$ownerBlockId` -> `$targetBlockId` in `${fn.id}` reaches a normally continuing target');
				}
				// Several mutually exclusive completions may render the same
				// abrupt tail, but it remains one HxcIR block with one semantic
				// owner for coverage and diagnostics.
				if (!sharedAbruptTargets.exists(targetBlockId)) {
					sharedAbruptTargets.set(targetBlockId, true);
					count(targetBlockId);
				}
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
			result.set(loop.iterationBoundaryBlockId, true);
		}
		return result;
	}

	/** Name the one escape that skips work by starting the next iteration. */
	static function iterationBypassTargets(loop:Null<CBodyLoopContext>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		if (loop != null)
			result.set(loop.iterationBoundaryBlockId, true);
		return result;
	}

	/** Recheck the exact ordinary-join or enclosing-iteration-boundary proof. */
	function isNormalOrBoundaryJoin(candidate:String, starts:Array<String>, loop:Null<CBodyLoopContext>):Bool {
		final escapes = escapeTargets(loop);
		return loop != null
			&& candidate == loop.iterationBoundaryBlockId ? analysis.isBoundaryJoin(candidate, starts, analysis.reachable,
				escapes) : analysis.isNormalJoin(candidate, starts, analysis.reachable, escapes, iterationBypassTargets(loop));
	}

	function requireRegionEntry(region:CBodyControlFlowRegion, expectedBlockId:String, ownerBlockId:String, fallthroughTarget:Null<String>):Void {
		if (region.nodes.length > 0) {
			if (nodeEntryBlockId(region.nodes[0]) != expectedBlockId)
				fail('region owned by `$ownerBlockId` in `${fn.id}` does not begin at HxcIR edge `$expectedBlockId`');
			return;
		}
		final matches = switch region.completion {
			case CFCFallthrough: fallthroughTarget != null && expectedBlockId == fallthroughTarget;
			case CFCBreak(owner, target) | CFCContinue(owner, target) | CFCSharedAbrupt(owner, target) | CFCGoto(owner, target, _): owner == ownerBlockId && target == expectedBlockId;
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
			case CFCBreak(ownerBlockId, targetBlockId) | CFCContinue(ownerBlockId, targetBlockId) | CFCSharedAbrupt(ownerBlockId, targetBlockId) |
				CFCGoto(ownerBlockId, targetBlockId, _):
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

	final successorsByBlock:Map<String, Array<String>> = [];
	// These facts belong only to this immutable function analysis. The separate
	// map records null results too, so "no post-dominator" is computed once.
	final immediatePostDominatorByBlock:Map<String, String> = [];
	final immediatePostDominatorComputed:Map<String, Bool> = [];

	public final orderedReachable:Array<String> = [];
	public final reachable:Map<String, Bool> = [];
	public final predecessors:Map<String, Array<String>> = [];
	public final dominators:Map<String, Map<String, Bool>> = [];
	public final postDominators:Map<String, Map<String, Bool>> = [];
	public final loopsByHeader:Map<String, CBodyNaturalLoop> = [];
	public final irreducibleEntries:Array<String> = [];

	var normalJoinSearches:Int = 0;
	var normalJoinCandidateProofs:Int = 0;
	var normalJoinDistanceSearches:Int = 0;
	var normalJoinDistanceBlockVisits:Int = 0;
	var completionSetSearches:Int = 0;
	var completionSetInitialBlockScans:Int = 0;
	var completionSetWorklistDequeues:Int = 0;
	var abruptCompletionSetSearches:Int = 0;
	var abruptCompletionSetInitialBlockScans:Int = 0;
	var abruptCompletionSetWorklistDequeues:Int = 0;
	var forwardReachabilitySearches:Int = 0;
	var forwardReachabilityBlockVisits:Int = 0;
	var prefixDisjointSearches:Int = 0;
	var prefixDisjointBlockVisits:Int = 0;
	var immediatePostDominatorQueries:Int = 0;
	var immediatePostDominatorComputations:Int = 0;
	var immediatePostDominatorCacheHits:Int = 0;

	public function new(fn:HxcIRFunction) {
		this.fn = fn;
		#if (macro || reflaxe_runtime)
		final indexingTimer = CPhaseTiming.startDetail(CDTBodyControlFlowIndexing, fn.id);
		#end
		for (index => block in fn.blocks) {
			if (blocks.exists(block.id))
				fail('control-flow analysis for `${fn.id}` received duplicate block `${block.id}`');
			blocks.set(block.id, block);
			blockOrder.set(block.id, index);
			predecessors.set(block.id, []);
		}
		for (block in fn.blocks)
			successorsByBlock.set(block.id, collectSuccessors(block));
		computeReachability();
		computePredecessors();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(indexingTimer);
		final dominatorTimer = CPhaseTiming.startDetail(CDTBodyControlFlowDominators, fn.id);
		#end
		computeDominators();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(dominatorTimer);
		final postDominatorTimer = CPhaseTiming.startDetail(CDTBodyControlFlowPostDominators, fn.id);
		#end
		computePostDominators();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(postDominatorTimer);
		final loopAnalysisTimer = CPhaseTiming.startDetail(CDTBodyControlFlowLoopAnalysis, fn.id);
		#end
		computeNaturalLoops();
		computeIrreducibleEntries();
		#if (macro || reflaxe_runtime)
		CPhaseTiming.stopDetail(loopAnalysisTimer);
		#end
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
		final result = successorsByBlock.get(block.id);
		if (result == null)
			return fail('control-flow analysis for `${fn.id}` lost successor set `$blockId`');
		return result;
	}

	/** Return a defensive copy of the blocks with an edge into `blockId`. */
	public function predecessorIds(blockId:String):Array<String>
		return requirePredecessors(blockId).copy();

	function collectSuccessors(block:HxcIRBlock):Array<String> {
		final result:Array<String> = [];
		if (block.terminator == null)
			return result;
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

	public function workReport():CBodyControlFlowWorkReport {
		return new CBodyControlFlowWorkReport(normalJoinSearches, normalJoinCandidateProofs, normalJoinDistanceSearches, normalJoinDistanceBlockVisits,
			completionSetSearches, completionSetInitialBlockScans, completionSetWorklistDequeues, abruptCompletionSetSearches,
			abruptCompletionSetInitialBlockScans, abruptCompletionSetWorklistDequeues, forwardReachabilitySearches, forwardReachabilityBlockVisits,
			prefixDisjointSearches, prefixDisjointBlockVisits, immediatePostDominatorQueries, immediatePostDominatorComputations,
			immediatePostDominatorCacheHits);
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
		immediatePostDominatorQueries++;
		if (immediatePostDominatorComputed.exists(blockId)) {
			immediatePostDominatorCacheHits++;
			return immediatePostDominatorByBlock.get(blockId);
		}
		immediatePostDominatorComputations++;
		final values = postDominators.get(blockId);
		if (values == null) {
			immediatePostDominatorComputed.set(blockId, true);
			return null;
		}
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
			if (immediate) {
				immediatePostDominatorByBlock.set(blockId, candidate);
				immediatePostDominatorComputed.set(blockId, true);
				return candidate;
			}
		}
		immediatePostDominatorComputed.set(blockId, true);
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
	public function normalJoin(starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>, iterationBypassTargets:Map<String, Bool>,
			unavailable:Map<String, Bool>, preferredCandidate:Null<String>):Null<String> {
		normalJoinSearches++;
		var abruptCompletion:Null<Map<String, Bool>> = null;
		function requireAbruptCompletion():Map<String, Bool> {
			if (abruptCompletion == null)
				abruptCompletion = abruptCompletionSet(allowed);
			return abruptCompletion;
		}
		if (preferredCandidate != null
			&& escapeTargets.exists(preferredCandidate)
			&& !unavailable.exists(preferredCandidate)
			&& isBoundaryJoinWithAvailability(preferredCandidate, starts, allowed, escapeTargets, unavailable))
			return preferredCandidate;
		final distances = distancesForStarts(starts, allowed, escapeTargets);
		if (preferredCandidate != null
			&& allowed.exists(preferredCandidate)
			&& !escapeTargets.exists(preferredCandidate)
			&& !unavailable.exists(preferredCandidate)
			&& isNormalJoinWithAvailability(preferredCandidate, starts, allowed, escapeTargets, iterationBypassTargets, unavailable, distances,
				requireAbruptCompletion))
			return preferredCandidate;

		final candidates:Array<CBodyNormalJoinCandidate> = [];
		for (candidate in orderedReachable) {
			if (!allowed.exists(candidate) || escapeTargets.exists(candidate) || unavailable.exists(candidate))
				continue;
			var reachCount = 0;
			var maximum = 0;
			var total = 0;
			for (distanceByBlock in distances) {
				final distance = distanceByBlock.get(candidate);
				if (distance == null)
					continue;
				reachCount++;
				if (distance > maximum)
					maximum = distance;
				total += distance;
			}
			candidates.push({
				blockId: candidate,
				abrupt: isAbruptTerminal(requireBlock(candidate)),
				reachCount: reachCount,
				maximumDistance: maximum,
				totalDistance: total
			});
		}
		// This is the exact score used by the previous exhaustive search. Sorting
		// first is semantics-preserving: the first valid candidate has the same
		// score and source block-order tie break as the former retained minimum.
		candidates.sort((left, right) -> {
			if (left.abrupt != right.abrupt)
				return left.abrupt ? 1 : -1;
			if (left.reachCount != right.reachCount)
				return right.reachCount - left.reachCount;
			if (left.maximumDistance != right.maximumDistance)
				return left.maximumDistance - right.maximumDistance;
			if (left.totalDistance != right.totalDistance)
				return left.totalDistance - right.totalDistance;
			return compareBlockIds(left.blockId, right.blockId);
		});
		for (candidate in candidates)
			if (isNormalJoinWithAvailability(candidate.blockId, starts, allowed, escapeTargets, iterationBypassTargets, unavailable, distances,
				requireAbruptCompletion))
				return candidate.blockId;
		return null;
	}

	public function isNormalJoin(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			iterationBypassTargets:Map<String, Bool>):Bool {
		final unavailable:Map<String, Bool> = [];
		return isNormalJoinWithAvailability(candidate, starts, allowed, escapeTargets, iterationBypassTargets, unavailable,
			distancesForStarts(starts, allowed, escapeTargets), null);
	}

	/**
		Prove that every normal arm reaches the enclosing loop-iteration boundary.

		The boundary is intentionally an escape target for ordinary join ranking,
		because searching through it would inspect a future iteration. For the exact
		enclosing stop, however, reaching it means ordinary structural fallthrough.
		Remove only that candidate from the escape set, then reuse the complete
		disjoint-prefix and completion proof.
	**/
	public function isBoundaryJoin(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Bool {
		final unavailable:Map<String, Bool> = [];
		return isBoundaryJoinWithAvailability(candidate, starts, allowed, escapeTargets, unavailable);
	}

	function isBoundaryJoinWithAvailability(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			unavailable:Map<String, Bool>):Bool {
		if (!escapeTargets.exists(candidate) || !allowed.exists(candidate))
			return false;
		final remainingEscapes:Map<String, Bool> = [];
		for (target in escapeTargets.keys())
			if (target != candidate)
				remainingEscapes.set(target, true);
		final noIterationBypasses:Map<String, Bool> = [];
		return isNormalJoinWithAvailability(candidate, starts, allowed, remainingEscapes, noIterationBypasses, unavailable,
			distancesForStarts(starts, allowed, remainingEscapes), null);
	}

	function isNormalJoinWithAvailability(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			iterationBypassTargets:Map<String, Bool>, unavailable:Map<String, Bool>, distances:Array<Map<String, Int>>,
			abruptCompletion:Null<Void->Map<String, Bool>>):Bool {
		normalJoinCandidateProofs++;
		if (!allowed.exists(candidate) || escapeTargets.exists(candidate))
			return false;
		if (isLinearEscapePrefix(candidate, allowed, escapeTargets)) {
			final abrupt = abruptCompletion == null ? abruptCompletionSet(allowed) : abruptCompletion();
			for (index => distance in distances)
				// A switch may have value-producing arms that share work before
				// `continue` and a default arm that continues directly. Moving the
				// shared work after the switch is safe only for that next-iteration
				// edge. A loop `break` or return remains inside its own switch arm;
				// treating either as a missing normal arm can turn a readable
				// `continue` into switch fallthrough.
				if (!distance.exists(candidate)
					&& !abrupt.exists(starts[index])
					&& !isLinearEscapePrefix(starts[index], allowed, iterationBypassTargets))
					return false;
		}
		final continuation = forwardReachable(candidate, allowed, escapeTargets);
		final completing = completionSet(candidate, allowed, escapeTargets, continuation);
		var hasContinuingPath = false;
		for (index => start in starts) {
			if (distances[index].exists(candidate))
				hasContinuingPath = true;
			if (!escapeTargets.exists(start) && !completing.exists(start))
				return false;
		}
		final disjoint = prefixesAreDisjoint(candidate, starts, allowed, escapeTargets, unavailable, continuation);
		return hasContinuingPath && disjoint;
	}

	function completionSet(candidate:String, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>, continuation:Map<String, Bool>):Map<String, Bool> {
		completionSetSearches++;
		final result:Map<String, Bool> = [];
		result.set(candidate, true);
		// A loop's continue/header target starts a later iteration. Do not walk
		// through it while deciding which abrupt paths belong to this iteration;
		// otherwise a later iteration can make the current return arm look like
		// part of the candidate's continuation and hide the real local join.
		for (blockId in orderedReachable)
			if (allowed.exists(blockId) && !continuation.exists(blockId) && isAbruptTerminal(requireBlock(blockId)))
				result.set(blockId, true);

		/*
			The old fixed point rescanned every reachable block until no new block
			completed. A long function with a backward source order could therefore
			visit the same 339 blocks thousands of times for one candidate. Instead,
			count each rule's unresolved successors once. When one successor becomes
			complete, only its direct predecessor rules and loop-exit rules wake up.
			This computes the same least fixed point without depending on source
			order.
		 */
		final remainingByBlock:Map<String, Int> = [];
		final ready:Array<String> = [];
		final queued:Map<String, Bool> = [];
		for (blockId in orderedReachable) {
			completionSetInitialBlockScans++;
			if (!allowed.exists(blockId) || result.exists(blockId))
				continue;
			final outgoing = successors(blockId);
			if (outgoing.length == 0)
				continue;
			var remaining = 0;
			for (target in outgoing)
				if (!escapeTargets.exists(target) && target != candidate && (!allowed.exists(target) || !result.exists(target)))
					remaining++;
			remainingByBlock.set(blockId, remaining);
			if (remaining == 0) {
				ready.push(blockId);
				queued.set(blockId, true);
			}
		}

		final loopRulesByExit:Map<String, Array<CBodyCompletionLoopRule>> = [];
		final immediatelyReadyLoops:Array<CBodyCompletionLoopRule> = [];
		for (headerId in orderedReachable) {
			final loop = loopsByHeader.get(headerId);
			if (loop == null)
				continue;
			var admitted = true;
			final nodes = [for (blockId in loop.nodes.keys()) blockId];
			nodes.sort(compareBlockIds);
			for (blockId in nodes)
				if (!allowed.exists(blockId)) {
					admitted = false;
					break;
				}
			if (!admitted)
				continue;
			final unresolvedExits:Map<String, Bool> = [];
			var hasExit = false;
			for (blockId in nodes)
				for (target in successors(blockId)) {
					if (loop.nodes.exists(target))
						continue;
					hasExit = true;
					if (!escapeTargets.exists(target) && target != candidate && !result.exists(target))
						unresolvedExits.set(target, true);
				}
			if (!hasExit)
				continue;
			var unresolvedExitCount = 0;
			for (_ in unresolvedExits.keys())
				unresolvedExitCount++;
			final rule = new CBodyCompletionLoopRule(nodes, unresolvedExitCount);
			if (rule.remaining == 0) {
				immediatelyReadyLoops.push(rule);
			} else {
				for (target in unresolvedExits.keys()) {
					if (!allowed.exists(target))
						continue;
					var rules = loopRulesByExit.get(target);
					if (rules == null) {
						rules = [];
						loopRulesByExit.set(target, rules);
					}
					rules.push(rule);
				}
			}
		}

		function markComplete(blockId:String):Void {
			if (!result.exists(blockId)) {
				result.set(blockId, true);
				if (!queued.exists(blockId)) {
					ready.push(blockId);
					queued.set(blockId, true);
				}
			}
		}
		function resolveLoop(rule:CBodyCompletionLoopRule):Void {
			if (rule.resolved || rule.remaining != 0)
				return;
			rule.resolved = true;
			for (blockId in rule.nodes)
				markComplete(blockId);
		}
		for (rule in immediatelyReadyLoops)
			resolveLoop(rule);
		var readyIndex = 0;
		while (readyIndex < ready.length) {
			completionSetWorklistDequeues++;
			final completedId = ready[readyIndex++];
			if (!result.exists(completedId))
				result.set(completedId, true);
			for (predecessorId in requirePredecessors(completedId)) {
				final remaining = remainingByBlock.get(predecessorId);
				if (remaining == null || remaining <= 0 || result.exists(predecessorId))
					continue;
				final next = remaining - 1;
				remainingByBlock.set(predecessorId, next);
				if (next == 0)
					markComplete(predecessorId);
			}
			final loopRules = loopRulesByExit.get(completedId);
			if (loopRules != null)
				for (rule in loopRules) {
					if (rule.resolved || rule.remaining <= 0)
						continue;
					rule.remaining--;
					resolveLoop(rule);
				}
		}
		return result;
	}

	/**
		Find blocks whose every path ends in an explicit abrupt terminator.

		This intentionally does not count a loop break/continue boundary as
		abrupt. That distinction lets a return-only switch arm sit beside a shared
		normal join while preventing one case-specific `continue` prefix from being
		moved after a sibling that owns different work.
	**/
	function abruptCompletionSet(allowed:Map<String, Bool>):Map<String, Bool> {
		abruptCompletionSetSearches++;
		final result:Map<String, Bool> = [];
		for (blockId in orderedReachable)
			if (allowed.exists(blockId) && isAbruptTerminal(requireBlock(blockId)))
				result.set(blockId, true);
		final remainingByBlock:Map<String, Int> = [];
		final ready:Array<String> = [];
		for (blockId in orderedReachable) {
			abruptCompletionSetInitialBlockScans++;
			if (!allowed.exists(blockId) || result.exists(blockId))
				continue;
			final outgoing = successors(blockId);
			if (outgoing.length == 0)
				continue;
			var remaining = 0;
			for (target in outgoing)
				if (!allowed.exists(target) || !result.exists(target))
					remaining++;
			remainingByBlock.set(blockId, remaining);
			if (remaining == 0)
				ready.push(blockId);
		}
		var readyIndex = 0;
		while (readyIndex < ready.length) {
			abruptCompletionSetWorklistDequeues++;
			final completedId = ready[readyIndex++];
			if (result.exists(completedId))
				continue;
			result.set(completedId, true);
			for (predecessorId in requirePredecessors(completedId)) {
				final remaining = remainingByBlock.get(predecessorId);
				if (remaining == null || remaining <= 0 || result.exists(predecessorId))
					continue;
				final next = remaining - 1;
				remainingByBlock.set(predecessorId, next);
				if (next == 0)
					ready.push(predecessorId);
			}
		}
		return result;
	}

	function prefixesAreDisjoint(candidate:String, starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>,
			unavailable:Map<String, Bool>, continuation:Map<String, Bool>):Bool {
		prefixDisjointSearches++;
		final ownerByBlock:Map<String, Int> = [];
		// A block reached after the candidate belongs to the shared continuation.
		// If another arm reaches that same block before the candidate (for
		// example, by continuing the loop), choosing this candidate would place
		// the block both inside that arm and after the structured branch.
		for (armIndex => start in starts) {
			if (start == candidate || escapeTargets.exists(start))
				continue;
			final pending:Array<String> = [start];
			final seen:Map<String, Bool> = [];
			var index = 0;
			while (index < pending.length) {
				prefixDisjointBlockVisits++;
				final current = pending[index++];
				if (current == candidate || escapeTargets.exists(current) || seen.exists(current))
					continue;
				/*
					A return/throw/unreachable target is a shared completion, not
					part of either arm's normally continuing prefix. Its dedicated
					CFCSharedAbrupt owner accounts for the block once even when
					several mutually exclusive paths reach it.
				 */
				if (isAbruptTerminal(requireBlock(current)))
					continue;
				if (!allowed.exists(current) || unavailable.exists(current))
					return false;
				if (continuation.exists(current))
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

	function forwardReachable(start:String, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Map<String, Bool> {
		forwardReachabilitySearches++;
		final result:Map<String, Bool> = [];
		final pending:Array<String> = [start];
		var index = 0;
		while (index < pending.length) {
			forwardReachabilityBlockVisits++;
			final current = pending[index++];
			if (!allowed.exists(current) || result.exists(current))
				continue;
			result.set(current, true);
			for (target in successors(current))
				if (!escapeTargets.exists(target) && !result.exists(target))
					pending.push(target);
		}
		return result;
	}

	/**
		Recognize a straight-line arm that ends only by leaving the active loop iteration.

		Its blocks belong inside that branch even when another arm eventually
		reaches the same break/continue boundary. Selecting one as a normal join
		would move the escape after its sibling and can make planning walk into a
		later iteration.
	**/
	function isLinearEscapePrefix(start:String, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Bool {
		var current = start;
		final seen:Map<String, Bool> = [];
		while (true) {
			if (escapeTargets.exists(current))
				return true;
			if (!allowed.exists(current) || seen.exists(current))
				return false;
			seen.set(current, true);
			final outgoing = successors(current);
			if (outgoing.length != 1)
				return false;
			current = outgoing[0];
		}
	}

	/**
		Measure paths that remain inside the current structural region.

		A loop's break and continue targets end the current iteration. Walking
		through `continue` and back into the header would rank blocks from a future
		iteration as possible joins for the current branch. The later join proof
		already treats these targets as completion boundaries; distance ranking
		must use the same boundary or it can prefer a valid-but-wrong candidate.
	**/
	function distancesForStarts(starts:Array<String>, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Array<Map<String, Int>> {
		final result:Array<Map<String, Int>> = [];
		for (start in starts) {
			normalJoinDistanceSearches++;
			result.push(distancesFromStart(start, allowed, escapeTargets));
		}
		return result;
	}

	function distancesFromStart(start:String, allowed:Map<String, Bool>, escapeTargets:Map<String, Bool>):Map<String, Int> {
		final distances:Map<String, Int> = [];
		if (!allowed.exists(start) || escapeTargets.exists(start))
			return distances;
		final pending:Array<String> = [start];
		distances.set(start, 0);
		var index = 0;
		while (index < pending.length) {
			normalJoinDistanceBlockVisits++;
			final current = pending[index++];
			final distance = distances.get(current);
			if (distance == null)
				return fail('normal-join distance in `${fn.id}` lost block `$current`');
			for (target in successors(current)) {
				if (allowed.exists(target) && !escapeTargets.exists(target) && !distances.exists(target)) {
					distances.set(target, distance + 1);
					pending.push(target);
				}
			}
		}
		return distances;
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
		final bitCount = orderedReachable.length;
		final wordCount = bitWordCount(bitCount);
		final all = fullBitWords(bitCount);
		final bitsByBlock:Map<String, Array<Int>> = [];
		for (blockId in orderedReachable) {
			final words = blockId == fn.entryBlockId ? emptyBitWords(wordCount) : all.copy();
			if (blockId == fn.entryBlockId)
				setBit(words, requireInt(blockOrder, blockId));
			bitsByBlock.set(blockId, words);
		}
		final scratch = emptyBitWords(wordCount);
		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in orderedReachable) {
				if (blockId == fn.entryBlockId)
					continue;
				final incoming = predecessors.get(blockId);
				if (incoming == null || incoming.length == 0)
					continue;
				copyBitWords(requireBitWords(bitsByBlock, incoming[0], "dominator"), scratch);
				for (index in 1...incoming.length)
					intersectBitWords(scratch, requireBitWords(bitsByBlock, incoming[index], "dominator"));
				setBit(scratch, requireInt(blockOrder, blockId));
				final current = requireBitWords(bitsByBlock, blockId, "dominator");
				if (replaceBitWordsWhenChanged(current, scratch))
					changed = true;
			}
		}
		for (blockId in orderedReachable)
			dominators.set(blockId, bitWordsToSet(requireBitWords(bitsByBlock, blockId, "dominator"), orderedReachable));
	}

	function computePostDominators():Void {
		final blockCount = orderedReachable.length;
		final bitCount = blockCount + 1;
		final wordCount = bitWordCount(bitCount);
		final all = fullBitWords(bitCount);
		final bitsByBlock:Map<String, Array<Int>> = [];
		final exitWords = emptyBitWords(wordCount);
		setBit(exitWords, blockCount);
		bitsByBlock.set(EXIT_ID, exitWords);
		for (blockId in orderedReachable)
			bitsByBlock.set(blockId, all.copy());
		final scratch = emptyBitWords(wordCount);
		var changed = true;
		while (changed) {
			changed = false;
			for (blockId in orderedReachable) {
				var foundOutgoing = false;
				for (target in successors(blockId)) {
					if (!reachable.exists(target))
						continue;
					final targetWords = requireBitWords(bitsByBlock, target, "post-dominator");
					if (foundOutgoing)
						intersectBitWords(scratch, targetWords);
					else {
						copyBitWords(targetWords, scratch);
						foundOutgoing = true;
					}
				}
				if (!foundOutgoing)
					copyBitWords(exitWords, scratch);
				setBit(scratch, requireInt(blockOrder, blockId));
				final current = requireBitWords(bitsByBlock, blockId, "post-dominator");
				if (replaceBitWordsWhenChanged(current, scratch))
					changed = true;
			}
		}
		final identities = orderedReachable.copy();
		identities.push(EXIT_ID);
		postDominators.set(EXIT_ID, bitWordsToSet(exitWords, identities));
		for (blockId in orderedReachable)
			postDominators.set(blockId, bitWordsToSet(requireBitWords(bitsByBlock, blockId, "post-dominator"), identities));
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

	/*
		Dominator sets are dense: most blocks initially contain nearly every
		block. A string-keyed Map therefore allocates one hash entry per bit for
		every fixed-point candidate. These helpers keep the same set operation in
		32-bit words and reuse one scratch array while converging, then materialize
		the public string sets once.
	 */
	static inline function bitWordCount(bitCount:Int):Int
		return (bitCount + 31) >> 5;

	static function emptyBitWords(wordCount:Int):Array<Int>
		return [for (_ in 0...wordCount) 0];

	static function fullBitWords(bitCount:Int):Array<Int> {
		final result = [for (_ in 0...bitWordCount(bitCount)) -1];
		final remainder = bitCount & 31;
		if (remainder != 0)
			result[result.length - 1] = (1 << remainder) - 1;
		return result;
	}

	static inline function setBit(words:Array<Int>, index:Int):Void {
		final wordIndex = index >> 5;
		words[wordIndex] = words[wordIndex] | (1 << (index & 31));
	}

	static inline function hasBit(words:Array<Int>, index:Int):Bool
		return (words[index >> 5] & (1 << (index & 31))) != 0;

	static function copyBitWords(source:Array<Int>, target:Array<Int>):Void {
		if (source.length != target.length)
			fail('control-flow dense set width changed from ${source.length} to ${target.length}');
		for (index in 0...source.length)
			target[index] = source[index];
	}

	static function intersectBitWords(target:Array<Int>, other:Array<Int>):Void {
		if (target.length != other.length)
			fail('control-flow dense intersection width changed from ${target.length} to ${other.length}');
		for (index in 0...target.length)
			target[index] = target[index] & other[index];
	}

	static function replaceBitWordsWhenChanged(target:Array<Int>, candidate:Array<Int>):Bool {
		if (target.length != candidate.length)
			fail('control-flow dense assignment width changed from ${target.length} to ${candidate.length}');
		var changed = false;
		for (index in 0...target.length)
			if (target[index] != candidate[index]) {
				changed = true;
				break;
			}
		if (changed)
			copyBitWords(candidate, target);
		return changed;
	}

	static function bitWordsToSet(words:Array<Int>, identities:Array<String>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (index => identity in identities)
			if (hasBit(words, index))
				result.set(identity, true);
		return result;
	}

	static function requireBitWords(index:Map<String, Array<Int>>, key:String, label:String):Array<Int> {
		final value = index.get(key);
		if (value == null)
			return fail('control-flow analysis lost dense $label set `$key`');
		return value;
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
