package caxecraft.scenario;

/** One complete translation of the scenario's message set. */
typedef ScenarioLocaleCatalog = {
	final id:LocaleId;
	final messages:Array<ScenarioMessage>;
}

/** One stable key and its whole translated value. */
typedef ScenarioMessage = {
	final id:MessageId;
	final text:String;
}

/** A catalog always has one non-recursive fallback locale. */
typedef ScenarioMessageCatalog = {
	final defaultLocale:LocaleId;
	final locales:Array<ScenarioLocaleCatalog>;
}

/**
	Whether a scenario carries translated messages.

	Literal-only user maps can stay small. Once a scenario uses
	`ScenarioText.Message`, semantic validation requires an embedded catalog and
	a complete translation in every declared locale.
**/
enum ScenarioMessages {
	NoMessageCatalog;
	EmbeddedMessageCatalog(catalog:ScenarioMessageCatalog);
}

/**
	Resolve an exact locale first, then the one declared default.

	This is a module function because it has no object state. A validated catalog
	contains the same keys in every locale. The fallback also makes a requested
	locale that the scenario does not ship predictable; there is no locale-to-
	locale chain and therefore no fallback cycle.
**/
function resolveScenarioMessage(source:ScenarioMessages, requested:LocaleId, message:MessageId):Null<String> {
	return switch source {
		case NoMessageCatalog: null;
		case EmbeddedMessageCatalog(catalog):
			final exact = findLocale(catalog, requested);
			final value = exact == null ? null : findMessage(exact, message);
			if (value != null) value; else {
				final fallback = findLocale(catalog, catalog.defaultLocale);
				fallback == null ? null : findMessage(fallback, message);
			}
	};
}

private function findLocale(catalog:ScenarioMessageCatalog, id:LocaleId):Null<ScenarioLocaleCatalog> {
	for (locale in catalog.locales)
		if (locale.id.text() == id.text())
			return locale;
	return null;
}

private function findMessage(locale:ScenarioLocaleCatalog, id:MessageId):Null<String> {
	for (message in locale.messages)
		if (message.id.text() == id.text())
			return message.text;
	return null;
}
