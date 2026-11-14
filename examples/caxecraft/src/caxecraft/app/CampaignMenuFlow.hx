package caxecraft.app;

import caxecraft.app.CampaignMenu.CampaignMenuHit;

/**
 * Resolves one campaign-menu frame without owning rendering or campaign data.
 *
 * The application samples devices and supplies the validated level count. This
 * module applies selection and action precedence once, which keeps pointer,
 * keyboard, controller, and pilot behavior aligned outside the main frame loop.
 */
/** Device-neutral inputs for one campaign-menu frame. */
typedef CampaignMenuFrameInput = {
	final selectedLevelIndex:Int;
	final levelCount:Int;
	final selectionEnabled:Bool;
	final cycleSelection:Bool;
	final confirmSelection:Bool;
	final cancelSelection:Bool;
	final pointerHit:CampaignMenuHit;
	final pointerPressed:Bool;
}

/** Selected row and requested action after one campaign-menu frame. */
typedef CampaignMenuFrameOutcome = {
	final selectedLevelIndex:Int;
	final launchRequested:Bool;
	final backRequested:Bool;
}

/** Apply one complete campaign-menu input sample. */
function advanceCampaignMenuFrame(input:CampaignMenuFrameInput):CampaignMenuFrameOutcome {
	var selected = input.selectedLevelIndex;
	if (input.selectionEnabled && input.cycleSelection && input.levelCount > 0)
		selected = (selected + 1) % input.levelCount;
	var launch = input.confirmSelection;
	var back = input.cancelSelection;
	if (input.pointerPressed)
		switch input.pointerHit {
			case NoCampaignMenuHit:
			case LevelHit(index):
				if (input.selectionEnabled)
					selected = index;
			case LaunchHit:
				launch = true;
			case BackHit:
				back = true;
		}
	return {
		selectedLevelIndex: selected,
		launchRequested: launch,
		backRequested: back
	};
}
