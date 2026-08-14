package caxecraft.app;

import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.app.ActivePlayableLevel.PlayableLevelPreparationError;
import caxecraft.app.ActivePlayableLevel.PlayableLevelStageResult;
import caxecraft.app.ActivePlayableLevel.StagedPlayableLevel;
import caxecraft.content.CampaignManifest.CampaignLevel;
import caxecraft.content.CampaignManifest.CampaignManifest;
import caxecraft.content.CampaignManifest.CampaignTransition;
import caxecraft.content.CampaignManifest.CampaignTransitionHandoff;
import caxecraft.content.CampaignRuntime.CampaignLevelLoadError;
import caxecraft.content.CampaignRuntime.CampaignLevelLoadResult;
import caxecraft.content.CampaignRuntime.loadCampaignLevel;
import caxecraft.content.ContentPackageStore;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.LoadedContentGeneration.ContentGenerationId;
import caxecraft.content.ResolvedLevelPlan.LevelPlayerOptions;
import caxecraft.domain.World;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioId;

/**
	Aligns one inactive destination with an authored source doorway.

	Both maps name trigger-zone anchors. This module checks that the anchors sit
	on opposite world edges, then computes one visual translation. It does not
	load, simulate, or publish either map. The frame loop can draw the inactive
	session and later hand the same staged value to `ActivePlayableLevel`.
**/
/** Why an admitted seamless edge could not become a safe visual portal. */
enum CampaignPortalPreparationError {
	/** The manifest selected the ordinary loading-card path. */
	PortalNotRequested;

	/** The source map does not contain the manifest's named trigger zone. */
	PortalSourceAnchorMissing(id:ScenarioId);

	/** The destination map does not contain the manifest's named entrance zone. */
	PortalDestinationAnchorMissing(id:ScenarioId);

	/** An anchor is too far from a world boundary to represent a doorway. */
	PortalAnchorAwayFromBoundary(id:ScenarioId);

	/** Both anchors face the same direction instead of meeting each other. */
	PortalAnchorsDoNotFace;
}

/** One complete inactive portal, or the exact alignment rejection. */
enum CampaignPortalPreparationResult {
	/** The staged destination can be previewed and published. */
	CampaignPortalPrepared(portal:PreparedCampaignPortal);

	/** Ordinary loading remains available; no partly aligned portal escaped. */
	CampaignPortalRejected(error:CampaignPortalPreparationError);
}

/** Why one declared portal could not be staged from package content. */
enum CampaignPortalPreloadError {
	/** More than one seamless edge leaves the bounded one-preview owner ambiguous. */
	PortalPreloadAmbiguous;

	/** The destination disappeared after manifest validation. */
	PortalPreloadDestinationMissing;

	/** Exact package bytes did not become a runtime candidate. */
	PortalPreloadLevelRejected(error:CampaignLevelLoadError);

	/** The current app shell could not prepare the complete inactive view. */
	PortalPreloadViewRejected(error:PlayableLevelPreparationError);

	/** Both complete levels loaded, but their authored doorway anchors did not align. */
	PortalPreloadAlignmentRejected(error:CampaignPortalPreparationError);
}

/** Result of looking ahead by at most one seamless campaign edge. */
enum CampaignPortalPreloadResult {
	/** No seamless edge leaves this level; ordinary loading remains unchanged. */
	CampaignPortalNotDeclared;

	/** One exact destination is ready for preview and publication. */
	CampaignPortalReady(portal:PreparedCampaignPortal);

	/** The source stays playable and ordinary loading remains a safe fallback. */
	CampaignPortalPreloadRejected(error:CampaignPortalPreloadError);
}

/** All inputs needed to stage one portal destination without publishing it. */
typedef CampaignPortalPreloadRequest = {
	/** Exact package store used by ordinary campaign loading. */
	final store:ContentPackageStore;

	/** Validated campaign that owns the possible outgoing edge. */
	final campaign:CampaignManifest;

	/** Active source level whose outgoing edge may request a portal. */
	final sourceLevel:CampaignLevel;

	/** Read-only source view used to resolve the authored doorway anchor. */
	final sourceView:PlayableLevelView;

	/** Generation reserved for the inactive destination. */
	final nextGeneration:ContentGenerationId;

	/** Player setup that the ordinary destination loader accepts. */
	final playerOptions:LevelPlayerOptions;
}

/** One of the four horizontal CAXEMAP boundaries. */
private enum PortalEdge {
	PortalNorth;
	PortalSouth;
	PortalEast;
	PortalWest;
}

/** Immutable facts that belong to one prepared doorway and staged level. */
private typedef PreparedCampaignPortalData = {
	final transition:CampaignTransition;
	final destination:CampaignLevel;
	final staged:StagedPlayableLevel;
	final offsetX:Float;
	final offsetY:Float;
	final offsetZ:Float;
	final sourceEdgeMask:Int;
	final sourceCenterX:Float;
	final sourceCenterZ:Float;
}

/**
	Owns one staged level and its source-world translation.

	A class is useful because the staged generation has one identity and lifetime.
	The scalar offsets are immutable facts derived from that same generation.
**/
final class PreparedCampaignPortal {
	static inline final PREVIEW_RADIUS_SQUARED = 100.0;

	final transitionValue:CampaignTransition;
	final destinationValue:CampaignLevel;
	final stagedValue:StagedPlayableLevel;
	final offsetXValue:Float;
	final offsetYValue:Float;
	final offsetZValue:Float;
	final sourceEdgeMaskValue:Int;
	final sourceCenterXValue:Float;
	final sourceCenterZValue:Float;

	/** Retain one complete checked alignment. */
	@:allow(caxecraft.app.CampaignPortal)
	private function new(data:PreparedCampaignPortalData) {
		transitionValue = data.transition;
		destinationValue = data.destination;
		stagedValue = data.staged;
		offsetXValue = data.offsetX;
		offsetYValue = data.offsetY;
		offsetZValue = data.offsetZ;
		sourceEdgeMaskValue = data.sourceEdgeMask;
		sourceCenterXValue = data.sourceCenterX;
		sourceCenterZValue = data.sourceCenterZ;
	}

	/** True when an authored exit names this exact prepared edge. */
	public inline function matches(transition:CampaignTransition):Bool
		return transitionValue.exit.text() == transition.exit.text()
			&& transitionValue.sourceLevel.text() == transition.sourceLevel.text()
			&& destinationValue.id.text() == transition.destinationLevel.text();

	/** Destination record whose exact bytes produced the staged generation. */
	public inline function destination():CampaignLevel
		return destinationValue;

	/** Inactive level that may be previewed or atomically selected. */
	public inline function staged():StagedPlayableLevel
		return stagedValue;

	/** Add this X translation when drawing destination-local geometry. */
	public inline function offsetX():Float
		return offsetXValue;

	/** Add this Y translation when drawing destination-local geometry. */
	public inline function offsetY():Float
		return offsetYValue;

	/** Add this Z translation when drawing destination-local geometry. */
	public inline function offsetZ():Float
		return offsetZValue;

	/** Bit removed from the source horizon so it does not cover the doorway. */
	public inline function sourceHorizonEdgeMask():Int
		return sourceEdgeMaskValue;

	/** Show the inactive destination only near its source doorway. */
	public inline function visibleFrom(x:Float, z:Float):Bool {
		final dx = x - sourceCenterXValue;
		final dz = z - sourceCenterZValue;
		return dx * dx + dz * dz <= PREVIEW_RADIUS_SQUARED;
	}
}

/** Check both content anchors and compute one destination preview translation. */
function prepareCampaignPortal(transition:CampaignTransition, destination:CampaignLevel, source:PlayableLevelView,
		staged:StagedPlayableLevel):CampaignPortalPreparationResult {
	final sourceAnchor = switch transition.handoff {
		case LoadingScreen: return CampaignPortalRejected(PortalNotRequested);
		case SeamlessPortal(anchor): anchor;
	};
	final destinationAnchor = new ScenarioId(transition.destinationEntrance.text());
	final sourceBounds = source.presentation().triggerZoneBounds(sourceAnchor);
	if (sourceBounds == null)
		return CampaignPortalRejected(PortalSourceAnchorMissing(sourceAnchor));
	final destinationBounds = staged.level().presentation().triggerZoneBounds(destinationAnchor);
	if (destinationBounds == null)
		return CampaignPortalRejected(PortalDestinationAnchorMissing(destinationAnchor));
	final sourceEdge = boundaryEdge(sourceBounds, source.authoredWorldWidth());
	if (sourceEdge == null)
		return CampaignPortalRejected(PortalAnchorAwayFromBoundary(sourceAnchor));
	final destinationEdge = boundaryEdge(destinationBounds, staged.level().authoredWorldWidth());
	if (destinationEdge == null)
		return CampaignPortalRejected(PortalAnchorAwayFromBoundary(destinationAnchor));
	if (!edgesFace(sourceEdge, destinationEdge))
		return CampaignPortalRejected(PortalAnchorsDoNotFace);
	final sourceCenterX = anchorCenterX(sourceBounds);
	final sourceCenterZ = anchorCenterZ(sourceBounds);
	return CampaignPortalPrepared(new PreparedCampaignPortal({
		transition: transition,
		destination: destination,
		staged: staged,
		offsetX: sourceCenterX - anchorCenterX(destinationBounds),
		offsetY: sourceBounds.origin.y - destinationBounds.origin.y,
		offsetZ: sourceCenterZ - anchorCenterZ(destinationBounds),
		sourceEdgeMask: edgeMask(sourceEdge),
		sourceCenterX: sourceCenterX,
		sourceCenterZ: sourceCenterZ
	}));
}

/**
	Load and prepare at most one seamless destination while the source stays active.

	The package receipt, CAXEMAP parser, level builder, and app preflight are the
	same owners used by an ordinary loading transition. Failure returns a closed
	reason and never receives publication authority.
**/
function preloadCampaignPortal(request:CampaignPortalPreloadRequest, validationRegistry:ScenarioContentRegistry,
		resolutionRegistry:LevelContentResolver):CampaignPortalPreloadResult {
	var transition:Null<CampaignTransition> = null;
	for (index in 0...request.campaign.transitionCount()) {
		final candidate = request.campaign.transitionAt(index);
		if (candidate.sourceLevel.text() == request.sourceLevel.id.text())
			switch candidate.handoff {
				case LoadingScreen:
				case SeamlessPortal(_):
					if (transition != null)
						return CampaignPortalPreloadRejected(PortalPreloadAmbiguous);
					transition = candidate;
			}
	}
	if (transition == null)
		return CampaignPortalNotDeclared;
	final destination = request.campaign.level(transition.destinationLevel);
	if (destination == null)
		return CampaignPortalPreloadRejected(PortalPreloadDestinationMissing);
	final candidate = switch loadCampaignLevel(request.store, destination, request.nextGeneration, validationRegistry, resolutionRegistry,
		request.playerOptions) {
		case CampaignLevelReady(value): value;
		case CampaignLevelRejected(error): return CampaignPortalPreloadRejected(PortalPreloadLevelRejected(error));
	};
	final staged = switch ActivePlayableLevel.stage(candidate) {
		case PlayableLevelStaged(value): value;
		case PlayableLevelStageRejected(error): return CampaignPortalPreloadRejected(PortalPreloadViewRejected(error));
	};
	return switch prepareCampaignPortal(transition, destination, request.sourceView, staged) {
		case CampaignPortalPrepared(portal): CampaignPortalReady(portal);
		case CampaignPortalRejected(error): CampaignPortalPreloadRejected(PortalPreloadAlignmentRejected(error));
	};
}

/** Select the closest edge only when the trigger is within four cells of it. */
private function boundaryEdge(bounds:VoxelBounds, worldWidth:Int):Null<PortalEdge> {
	final north = bounds.origin.z;
	final south = World.DEPTH - (bounds.origin.z + bounds.size.depth);
	final east = worldWidth - (bounds.origin.x + bounds.size.width);
	final west = bounds.origin.x;
	var distance = north;
	var edge = PortalNorth;
	if (south < distance) {
		distance = south;
		edge = PortalSouth;
	}
	if (east < distance) {
		distance = east;
		edge = PortalEast;
	}
	if (west < distance) {
		distance = west;
		edge = PortalWest;
	}
	return distance >= 0 && distance <= 4 ? edge : null;
}

/** Opposite boundaries form one connected doorway. */
private function edgesFace(source:PortalEdge, destination:PortalEdge):Bool {
	return switch [source, destination] {
		case [PortalNorth, PortalSouth] | [PortalSouth, PortalNorth] | [PortalEast, PortalWest] | [PortalWest, PortalEast]: true;
		case _: false;
	};
}

/** Match the authored environment bit layout used by `DistantHorizonRenderer`. */
private function edgeMask(edge:PortalEdge):Int {
	return switch edge {
		case PortalNorth: 1;
		case PortalSouth: 2;
		case PortalEast: 4;
		case PortalWest: 8;
	};
}

/** Center a half-open trigger box on the horizontal X plane. */
private inline function anchorCenterX(bounds:VoxelBounds):Float
	return bounds.origin.x + bounds.size.width * 0.5;

/** Center a half-open trigger box on the horizontal Z plane. */
private inline function anchorCenterZ(bounds:VoxelBounds):Float
	return bounds.origin.z + bounds.size.depth * 0.5;
