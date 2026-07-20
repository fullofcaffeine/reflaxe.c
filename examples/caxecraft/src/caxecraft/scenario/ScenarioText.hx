package caxecraft.scenario;

/**
	Player-visible scenario text.

	Built-in content uses `Message` so English and Spanish remain equal. User
	maps may use `Literal`; translated display text is never used as identity.
**/
enum ScenarioText {
	Message(id:MessageId);
	Literal(text:String);
}
