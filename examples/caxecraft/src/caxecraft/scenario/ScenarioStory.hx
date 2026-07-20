package caxecraft.scenario;

/** Dialogue, journal, objective, and route records for one scenario. */
typedef ScenarioStory = {
	final dialogues:Array<ScenarioDialogue>;
	final journal:Array<ScenarioJournalEntry>;
	final objectives:Array<ScenarioObjective>;
	final routes:Array<ScenarioRoute>;
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
