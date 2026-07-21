package caxecraft.scenario;

import caxecraft.scenario.ScenarioMessages.ScenarioLocaleCatalog;

/**
	Checks the scenario-owned translations and every message reference.

	The default locale defines the complete key set. Every other locale must
	translate exactly that set: missing and extra keys are errors instead of
	silently producing a partly translated campaign.

	`@:noCompletion` keeps this implementation helper out of editor suggestions.
	Scenario authors and tools call `ScenarioValidator`, which runs this check as
	part of the complete map validation process.
**/
@:noCompletion
final class ScenarioMessageValidator {
	final context:ScenarioValidationContext;

	public function new(context:ScenarioValidationContext)
		this.context = context;

	public function validate():Void {
		switch context.scenario.messages {
			case NoMessageCatalog:
				validateReferences(null);
			case EmbeddedMessageCatalog(catalog):
				if (catalog.locales.length > ScenarioLimits.MAX_LOCALES)
					context.addAtCoordinate(LimitExceeded(Locales, ScenarioLimits.MAX_LOCALES), context.coordinateForDefaultLocale());
				final locales:Map<String, ScenarioLocaleCatalog> = [];
				for (locale in catalog.locales) {
					if (locales.exists(locale.id.text()))
						context.addAtCoordinate(DuplicateLocale(locale.id), context.coordinateForLocale(locale.id));
					else
						locales.set(locale.id.text(), locale);
					validateMessageIdentities(locale);
				}
				final base = locales.get(catalog.defaultLocale.text());
				if (base == null) {
					context.addAtCoordinate(UnknownDefaultLocale(catalog.defaultLocale), context.coordinateForDefaultLocale());
					validateReferences(null);
				} else {
					final baseIds = messageIds(base);
					for (locale in catalog.locales)
						if (locale.id.text() != catalog.defaultLocale.text())
							validateTranslationSet(base, baseIds, locale);
					validateReferences(baseIds);
				}
		}
	}

	function validateMessageIdentities(locale:ScenarioLocaleCatalog):Void {
		if (locale.messages.length > ScenarioLimits.MAX_MESSAGES_PER_LOCALE)
			context.addAtCoordinate(LimitExceeded(MessagesPerLocale, ScenarioLimits.MAX_MESSAGES_PER_LOCALE), context.coordinateForLocale(locale.id));
		final seen:Map<String, Bool> = [];
		for (message in locale.messages) {
			if (seen.exists(message.id.text()))
				context.addAtCoordinate(DuplicateMessage(locale.id, message.id), context.coordinateForLocaleMessage(locale.id, message.id));
			seen.set(message.id.text(), true);
		}
	}

	static function messageIds(locale:ScenarioLocaleCatalog):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (message in locale.messages)
			result.set(message.id.text(), true);
		return result;
	}

	function validateTranslationSet(base:ScenarioLocaleCatalog, baseIds:Map<String, Bool>, locale:ScenarioLocaleCatalog):Void {
		final translated = messageIds(locale);
		for (message in base.messages)
			if (!translated.exists(message.id.text())) {
				context.addAtCoordinate(MissingTranslation(locale.id, message.id), context.coordinateForLocale(locale.id));
			}
		for (message in locale.messages)
			if (!baseIds.exists(message.id.text()))
				context.addAtCoordinate(UnknownTranslation(locale.id, message.id), context.coordinateForLocaleMessage(locale.id, message.id));
	}

	function validateReferences(available:Null<Map<String, Bool>>):Void {
		validateText(context.scenario.title, available);
		for (dialogue in context.scenario.story.dialogues)
			for (line in dialogue.lines)
				validateText(line.text, available);
		for (entry in context.scenario.story.journal) {
			validateText(entry.title, available);
			validateText(entry.body, available);
		}
		for (objective in context.scenario.story.objectives) {
			validateText(objective.title, available);
			validateText(objective.body, available);
		}
		for (route in context.scenario.story.routes)
			validateText(route.title, available);
	}

	function validateText(text:ScenarioText, available:Null<Map<String, Bool>>):Void {
		switch text {
			case Literal(_):
			case Message(id):
				if (available == null || !available.exists(id.text()))
					context.addAtCoordinate(UnresolvedMessage(id), context.coordinateForMessageReference(id));
		}
	}
}
