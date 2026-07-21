package caxecraft.scenario;

/**
	The declaration family whose source coordinate owns a scenario ID.

	This is an implementation-only vocabulary shared by the validator classes.
	`@:noCompletion` keeps it out of normal editor suggestions; it does not make
	the enum private at runtime or grant any extra safety.
**/
@:noCompletion
enum ScenarioIdentitySource {
	ChunkIdentity;
	ObjectIdentity;
	DialogueIdentity;
	JournalIdentity;
	ObjectiveIdentity;
	RouteIdentity;
	VariableIdentity;
	SequenceIdentity;
	SequenceParameterIdentity(sequenceId:ScenarioId);
	RuleIdentity;
}
