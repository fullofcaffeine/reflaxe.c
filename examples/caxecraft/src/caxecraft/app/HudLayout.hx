package caxecraft.app;

/**
 * Computes the screen regions used by the Caxecraft play interface.
 *
 * Gameplay supplies committed state, this module chooses responsive integer
 * rectangles, and the Raylib owner draws inside them. The geometry stays
 * target-neutral so Eval and native C can prove the same fit and overlap rules.
 */
/** One axis-aligned screen region measured in pixels. */
typedef HudRectangle = {
	/** Left edge measured from the window's left edge. */
	final x:Int;

	/** Top edge measured from the window's top edge. */
	final y:Int;

	/** Positive horizontal extent. */
	final width:Int;

	/** Positive vertical extent. */
	final height:Int;
}

/** Complete responsive geometry for one normal play frame. */
typedef HudLayout = {
	/** True when the viewport needs smaller icons and tighter spacing. */
	final compact:Bool;

	/** Current localized objective in the upper-left corner. */
	final objective:HudRectangle;

	/** Optional developer metrics below all normal top-left regions. */
	final diagnostics:HudRectangle;

	/** Three player-health glyphs in the upper-right corner. */
	final health:HudRectangle;

	/** Reserved equipped-item glyph immediately before player health. */
	final equipment:HudRectangle;

	/** Enemy danger notice in free space or below the top status row. */
	final alert:HudRectangle;

	/** One contextual action or short feedback notice above the hotbar. */
	final action:HudRectangle;

	/** Ten breath indicators above the action notice. */
	final breath:HudRectangle;

	/** Crosshair plus its shape-based target ring at the viewport center. */
	final crosshair:HudRectangle;

	/** Bounded inventory row along the lower edge. */
	final hotbar:HudRectangle;

	/** Pixel size of one hotbar slot. */
	final slotSize:Int;

	/** Gap between adjacent hotbar slots. */
	final slotGap:Int;

	/** Pixel size of one health glyph. */
	final heartSize:Int;

	/** Horizontal gap between adjacent health glyphs. */
	final heartGap:Int;

	/** Pixel size of the reviewed crosshair glyph. */
	final crosshairSize:Int;
}

/** Responsive geometry for one replacement panel. */
typedef HudModalLayout = {
	/** Blocking conversation panel. */
	final conversation:HudRectangle;

	/** Portrait inside the conversation panel. */
	final conversationPortrait:HudRectangle;

	/** Wrapped dialogue body above the conversation help. */
	final conversationText:HudRectangle;

	/** Localized conversation controls along the panel's lower edge. */
	final conversationHelp:HudRectangle;

	/** Pause or journal panel selected by the caller's state. */
	final pause:HudRectangle;

	/** Defeat and recovery panel. */
	final defeat:HudRectangle;
}

/** One mutually exclusive full-screen HUD presentation state. */
enum HudReplacementState {
	/** Draw the ordinary play interface. */
	NormalHud;

	/** Replace ordinary play with the active conversation. */
	ConversationHud;

	/** Replace ordinary play with the pause or journal panel. */
	PausedHud;

	/** Replace every other panel with the recovery prompt. */
	DefeatedHud;
}

/** Shape selected for one breath position without relying on color. */
enum HudBreathGlyph {
	/** A filled circle represents remaining breath. */
	FilledBreath;

	/** An outlined square represents depleted breath. */
	DepletedBreath;
}

/**
 * Fit one play interface to the current supported window.
 *
 * The application admits 800 by 450 as its minimum. The inventory supplies
 * its exact slot count so the layout cannot drift from the gameplay hotbar.
 */
function hudLayout(screenWidth:Int, screenHeight:Int, hotbarSlots:Int):HudLayout {
	final width = screenWidth < 800 ? 800 : screenWidth;
	final height = screenHeight < 450 ? 450 : screenHeight;
	final slots = hotbarSlots < 1 ? 1 : hotbarSlots;
	final compact = width < 900 || height < 600;
	final margin = compact ? 12 : 18;
	final slotSize = compact ? 48 : 64;
	final slotGap = compact ? 3 : 4;
	final hotbarWidth = slots * slotSize + (slots - 1) * slotGap;
	final hotbarY = height - slotSize - (compact ? 12 : 24);
	final actionWidth = compact ? 260 : 320;
	final actionHeight = compact ? 36 : 42;
	final actionY = hotbarY - actionHeight - (compact ? 10 : 14);
	final heartSize = compact ? 34 : 42;
	final heartGap = compact ? 5 : 8;
	final healthWidth = 3 * heartSize + 2 * heartGap;
	final health = rectangle(width - margin - healthWidth, margin, healthWidth, heartSize);
	final equipmentSize = compact ? 34 : 42;
	final equipment = rectangle(health.x - equipmentSize - 10, margin, equipmentSize, equipmentSize);
	final objectiveWidth = compact ? 300 : 390;
	final objectiveHeight = compact ? 58 : 68;
	final objective = rectangle(margin, margin, objectiveWidth, objectiveHeight);
	final alertWidth = compact ? 280 : 340;
	final alertHeight = compact ? 34 : 38;
	final freeLeft = objective.x + objective.width + 8;
	final freeRight = equipment.x - 8;
	final freeWidth = freeRight - freeLeft;
	final alertFitsTop = freeWidth >= alertWidth;
	final topStatusBottom = maximum(objective.y + objective.height, maximum(health.y + health.height, equipment.y + equipment.height));
	final alertX = alertFitsTop ? freeLeft + Std.int((freeWidth - alertWidth) / 2) : Std.int((width - alertWidth) / 2);
	final alertY = alertFitsTop ? margin : topStatusBottom + 8;
	final alert = rectangle(alertX, alertY, alertWidth, alertHeight);
	final diagnosticsY = maximum(objective.y + objective.height, alert.y + alert.height) + 8;
	final crosshairSize = compact ? 20 : 24;
	final crosshairRingSize = crosshairSize + 6;
	final breathWidth = 10 * 18 - 4;
	final breathCenterY = actionY - (compact ? 14 : 18);
	return {
		compact: compact,
		objective: objective,
		diagnostics: rectangle(margin, diagnosticsY, objectiveWidth, compact ? 54 : 60),
		health: health,
		equipment: equipment,
		alert: alert,
		action: rectangle(Std.int((width - actionWidth) / 2), actionY, actionWidth, actionHeight),
		breath: rectangle(Std.int((width - breathWidth) / 2), breathCenterY - 6, breathWidth, 12),
		crosshair: rectangle(Std.int((width - crosshairRingSize) / 2), Std.int((height - crosshairRingSize) / 2), crosshairRingSize, crosshairRingSize),
		hotbar: rectangle(Std.int((width - hotbarWidth) / 2), hotbarY, hotbarWidth, slotSize),
		slotSize: slotSize,
		slotGap: slotGap,
		heartSize: heartSize,
		heartGap: heartGap,
		crosshairSize: crosshairSize
	};
}

/** Fit conversation, pause, journal, and defeat panels to one supported window. */
function hudModalLayout(screenWidth:Int, screenHeight:Int, hasJournal:Bool):HudModalLayout {
	final width = screenWidth < 800 ? 800 : screenWidth;
	final height = screenHeight < 450 ? 450 : screenHeight;
	final compact = width < 900 || height < 600;
	final conversationMargin = compact ? 18 : 48;
	final conversationHeight = compact ? 210 : 184;
	final conversation = rectangle(conversationMargin, height - conversationHeight - 34, width - conversationMargin * 2, conversationHeight);
	final portraitSize = conversation.height - 34;
	final portrait = rectangle(conversation.x + 16, conversation.y + 16, portraitSize, portraitSize);
	final help = rectangle(portrait.x
		+ portrait.width
		+ 20, conversation.y
		+ conversation.height
		- 36,
		conversation.x
		+ conversation.width
		- 16
		- (portrait.x + portrait.width + 20), 18);
	final textX = portrait.x + portrait.width + 20;
	final textRight = conversation.x + conversation.width - 92;
	final textY = conversation.y + 52;
	final text = rectangle(textX, textY, textRight - textX, help.y - 8 - textY);
	final pauseDesiredWidth = hasJournal ? 660 : 340;
	final pauseWidth = pauseDesiredWidth < width - 24 ? pauseDesiredWidth : width - 24;
	final pauseHeight = hasJournal ? 220 : 96;
	final pause = rectangle(Std.int((width - pauseWidth) / 2), Std.int((height - pauseHeight) / 2), pauseWidth, pauseHeight);
	final defeatWidth = width - 24 < 500 ? width - 24 : 500;
	return {
		conversation: conversation,
		conversationPortrait: portrait,
		conversationText: text,
		conversationHelp: help,
		pause: pause,
		defeat: rectangle(Std.int((width - defeatWidth) / 2), Std.int((height - 148) / 2), defeatWidth, 148)
	};
}

/**
 * Choose the only replacement state that may draw for one frame.
 *
 * Defeat has priority because hiding its recovery command behind pause would
 * leave the player unable to continue. Pause then blocks conversation and the
 * normal play interface.
 */
function hudReplacementState(defeated:Bool, paused:Bool, conversationVisible:Bool):HudReplacementState {
	if (defeated)
		return DefeatedHud;
	if (paused)
		return PausedHud;
	if (conversationVisible)
		return ConversationHud;
	return NormalHud;
}

/** Select one breath shape from the bounded count computed by gameplay. */
function hudBreathGlyph(index:Int, filledCount:Int):HudBreathGlyph
	return index >= 0 && index < filledCount ? FilledBreath : DepletedBreath;

/** Return the maximum conservative line count for one text rectangle. */
function hudWrappedLineLimit(text:HudRectangle, lineHeight:Int):Int
	return lineHeight < 1 ? 0 : Std.int(text.height / lineHeight);

/** Return the conservative character count for one default-font text rectangle. */
function hudWrappedCharacterLimit(text:HudRectangle, fontSize:Int):Int {
	if (fontSize < 1)
		return 0;
	final averageCharacterWidth = Std.int(fontSize / 2);
	return averageCharacterWidth < 1 ? 0 : Std.int(text.width / averageCharacterWidth);
}

/** True when space-delimited text fits the same conservative wrapper as the renderer. */
function hudWrappedTextFits(value:String, maximumCharacters:Int, maximumLines:Int):Bool {
	if (maximumCharacters < 1 || maximumLines < 1)
		return value.length == 0;
	var lineLength = 0;
	var lineCount = 0;
	var wordLength = 0;
	var index = 0;
	while (index <= value.length) {
		if (index < value.length && value.charCodeAt(index) != 32) {
			wordLength++;
		} else if (wordLength > 0) {
			if (wordLength > maximumCharacters)
				return false;
			final candidateLength = lineLength == 0 ? wordLength : lineLength + 1 + wordLength;
			if (lineLength > 0 && candidateLength > maximumCharacters) {
				lineCount++;
				if (lineCount >= maximumLines)
					return false;
				lineLength = wordLength;
			} else {
				lineLength = candidateLength;
			}
			wordLength = 0;
		}
		index++;
	}
	return true;
}

/** True when two positive rectangles cover any common pixel area. */
function hudRectanglesOverlap(left:HudRectangle, right:HudRectangle):Bool
	return left.x < right.x + right.width && right.x < left.x + left.width && left.y < right.y + right.height && right.y < left.y + left.height;

/** Return the larger of two integer pixel positions. */
private inline function maximum(left:Int, right:Int):Int
	return left > right ? left : right;

/** Construct one immutable pixel rectangle after its dimensions are known. */
private inline function rectangle(x:Int, y:Int, width:Int, height:Int):HudRectangle
	return {
		x: x,
		y: y,
		width: width,
		height: height
	};
