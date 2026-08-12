package caxecraft.scenario;

/** Player-facing names, dialogue, journal, objective, and route records for one scenario. */
typedef ScenarioStory = {
	/** Localized display names keyed by the same NPC identity used by dialogue. */
	final speakerNames:Array<ScenarioSpeakerName>;

	final dialogues:Array<ScenarioDialogue>;
	final journal:Array<ScenarioJournalEntry>;
	final objectives:Array<ScenarioObjective>;
	final routes:Array<ScenarioRoute>;
}

/** A reloadable player-facing name for one authored NPC identity. */
typedef ScenarioSpeakerName = {
	final speaker:ScenarioId;
	final name:ScenarioText;
}

typedef ScenarioDialogue = {
	final id:ScenarioId;
	final lines:Array<ScenarioDialogueLine>;
}

typedef ScenarioDialogueLine = {
	final speaker:Null<ScenarioId>;
	final text:ScenarioText;
}

typedef ScenarioJournalEntry = {
	final id:ScenarioId;
	final title:ScenarioText;
	final body:ScenarioText;
}

typedef ScenarioObjective = {
	final id:ScenarioId;
	final title:ScenarioText;
	final body:ScenarioText;
	final initialState:ObjectiveState;
}

enum ObjectiveState {
	Hidden;
	Active;
	Complete;
	Failed;
}

typedef ScenarioRoute = {
	final id:ScenarioId;
	final title:ScenarioText;
	final objectives:Array<ScenarioId>;
}
