package caxecraft.app;

/**
	Owns the device-neutral cursor for one blocking, reloadable conversation.

	The active map still owns line text, speakers, and ordering. This state keeps
	only the current line, reveal progress, and deliberate skip hold. UI elapsed
	time advances the reveal, while fixed simulation can remain stopped.
**/
typedef ConversationState = {
	/** Zero-based line currently shown by the conversation panel. */
	final lineIndex:Int;

	/** Number of source string characters currently visible. */
	final visibleCharacters:Int;

	/** Milliseconds retained until they form another reveal step. */
	final revealRemainderMilliseconds:Int;

	/** Consecutive milliseconds for which the separate skip action was held. */
	final skipHeldMilliseconds:Int;
}

/** Result of applying one rendered-frame input sample to a conversation. */
enum ConversationAdvance {
	/** Keep the blocking panel open with this complete next state. */
	ConversationContinues(state:ConversationState);

	/** Close the conversation without advancing any world action. */
	ConversationCloses;
}

/** Start one conversation before its first character is visible. */
function beginConversation():ConversationState
	return state(0, 0, 0, 0);

/**
	Advance reveal, normal action, and held skip for one UI frame.

	A normal action first completes a partly shown line. A later action advances
	to the next authored line or closes the conversation. Holding the separate
	skip action for 600 milliseconds closes it. This order prevents one press
	from both completing and advancing a line.
**/
function advanceConversation(current:ConversationState, lineLength:Int, lineCount:Int, elapsedMilliseconds:Int, actionPressed:Bool,
		skipHeld:Bool):ConversationAdvance {
	final safeLength = lineLength < 0 ? 0 : lineLength;
	final safeLineCount = lineCount < 1 ? 1 : lineCount;
	final elapsed = elapsedMilliseconds < 0 ? 0 : elapsedMilliseconds;
	final held = skipHeld ? current.skipHeldMilliseconds + elapsed : 0;
	if (held >= 600)
		return ConversationCloses;

	if (actionPressed) {
		if (current.visibleCharacters < safeLength)
			return ConversationContinues(state(current.lineIndex, safeLength, 0, held));
		if (current.lineIndex + 1 >= safeLineCount)
			return ConversationCloses;
		return ConversationContinues(state(current.lineIndex + 1, 0, 0, held));
	}

	if (current.visibleCharacters >= safeLength)
		return ConversationContinues(state(current.lineIndex, safeLength, 0, held));
	final revealMilliseconds = current.revealRemainderMilliseconds + elapsed;
	var visible = current.visibleCharacters + Std.int(revealMilliseconds / 25);
	if (visible > safeLength)
		visible = safeLength;
	final remainder = visible >= safeLength ? 0 : revealMilliseconds % 25;
	return ConversationContinues(state(current.lineIndex, visible, remainder, held));
}

/** Copy scalar state in one place so transitions cannot retain mixed cursors. */
private inline function state(lineIndex:Int, visibleCharacters:Int, revealRemainderMilliseconds:Int, skipHeldMilliseconds:Int):ConversationState
	return {
		lineIndex: lineIndex,
		visibleCharacters: visibleCharacters,
		revealRemainderMilliseconds: revealRemainderMilliseconds,
		skipHeldMilliseconds: skipHeldMilliseconds
	};
