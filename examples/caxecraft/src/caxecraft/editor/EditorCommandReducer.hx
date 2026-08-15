package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorCommandFamily;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorTypes.EditorSettings;
import caxecraft.editor.EditorWorldGrid.EditorWorldResult;
import caxecraft.editor.EditorWorldGrid.fill as fillWorld;
import caxecraft.editor.EditorWorldGrid.paintMany as paintWorld;
import caxecraft.editor.EditorWorldGrid.resize as resizeWorld;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioMessages;
import caxecraft.scenario.ScenarioMessages.ScenarioLocaleCatalog;
import caxecraft.scenario.ScenarioMessages.ScenarioMessage;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;
import caxecraft.scenario.ScenarioWorld.ScenarioFluid;

/** Internal candidate state produced before a command is committed to history. */
@:noCompletion
typedef EditorReduction = {
	final scenario:Scenario;
	final family:EditorCommandFamily;
}

/** Internal typed result for candidate command application. */
@:noCompletion
enum EditorReductionResult {
	ReductionReady(value:EditorReduction);
	ReductionRejected(error:EditorError);
}

/**
	Applies one closed command to an isolated draft copy.

	The reducer owns no state between calls: every dependency arrives as an
	argument and every accepted edit returns a new candidate value. Module
	functions express that data flow directly; a static-only class would only
	repeat the source module's name.

	`apply` must remain module-public so `EditorSession` can import it from
	another file. Haxe's `@:noCompletion` keeps that internal building block out
	of ordinary IDE suggestions, guiding application code toward the session
	that owns snapshot isolation and history. The metadata is only a completion
	hint: explicit imports remain type-checked normally, and haxe.c emits no
	access check or runtime behavior for it.
**/
@:noCompletion
function apply(scenario:Scenario, command:EditorCommand, settings:EditorSettings):EditorReductionResult {
	return switch command {
		case SetTitle(title):
			setTitle(scenario, title);
		case ResizeWorld(size):
			switch resizeWorld(scenario.world, size) {
				case WorldRejected(error): ReductionRejected(error);
				case WorldReady(world): ready(withWorld(scenario, world), WorldShape);
			}
		case SetPaletteEntry(code, blockType):
			setPaletteEntry(scenario, code, blockType);
		case PaintVoxel(point, paletteCode):
			paint(scenario, [point], paletteCode, settings.selectionCells);
		case EraseVoxel(point):
			paint(scenario, [point], 0, settings.selectionCells);
		case PaintVoxels(points, paletteCode):
			paint(scenario, points, paletteCode, settings.selectionCells);
		case EraseVoxels(points):
			paint(scenario, points, 0, settings.selectionCells);
		case FillBounds(bounds, paletteCode):
			fillBounds(scenario, bounds, paletteCode);
		case PutFluid(fluid):
			ready(withFluids(scenario, putFluid(scenario.world.fluids, fluid)), Fluid);
		case RemoveFluid(id):
			removeWorldFluid(scenario, id);
		case StampPrefab(id, prefabType, tags, transform):
			stampPrefab(scenario, id, prefabType, tags, transform);
		case PutObject(object):
			ready(withObjects(scenario, putObject(scenario.objects, object)), Placement);
		case RemoveObject(id):
			removePlacedObject(scenario, id);
		case PutDialogue(dialogue):
			ready(withDialogues(scenario, putDialogue(scenario.story.dialogues, dialogue)), Dialogue);
		case RemoveDialogue(id):
			removeStoryDialogue(scenario, id);
		case PutObjective(objective):
			ready(withObjectives(scenario, putObjective(scenario.story.objectives, objective)), Objective);
		case RemoveObjective(id):
			removeStoryObjective(scenario, id);
		case PutRule(rule):
			ready(withRules(scenario, putRule(scenario.flow.rules, rule)), Rule);
		case RemoveRule(id):
			removeFlowRule(scenario, id);
		case SetDefaultLocale(locale):
			setDefaultLocale(scenario, locale);
		case PutLocale(locale):
			ready(withMessages(scenario, putLocale(scenario.messages, locale)), Localization);
		case RemoveLocale(locale):
			removeLocale(scenario, locale);
		case PutMessage(locale, message):
			putMessage(scenario, locale, message);
		case RemoveMessage(locale, message):
			removeMessage(scenario, locale, message);
		case RestoreLastPlayable:
			ReductionRejected(NoPlayableScenario);
	}
}

/**
	Replace document metadata without exposing the mutable editor draft.

	An empty literal would produce a blank name in the campaign tree and native
	editor, so the command rejects it before a candidate snapshot is created.
	Message-backed titles keep their typed message identity and are validated by
	the ordinary scenario validator when the draft is prepared for play.
**/
private function setTitle(scenario:Scenario, title:ScenarioText):EditorReductionResult {
	switch title {
		case Literal(text):
			if (text.length == 0)
				return ReductionRejected(InvalidTitle);
		case Message(_):
	}
	return ready(copy(scenario, scenario.messages, title, scenario.world, scenario.objects, scenario.story.dialogues, scenario.story.objectives,
		scenario.flow.rules),
		DocumentMetadata);
}

private function setDefaultLocale(scenario:Scenario, locale:LocaleId):EditorReductionResult {
	return switch scenario.messages {
		case NoMessageCatalog: ReductionRejected(MissingLocale(locale));
		case EmbeddedMessageCatalog(catalog):
			if (!hasLocale(catalog.locales,
				locale)) ReductionRejected(MissingLocale(locale)); else ready(withMessages(scenario,
				EmbeddedMessageCatalog({defaultLocale: locale, locales: copyLocales(catalog.locales)})), Localization);
	};
}

private function removeLocale(scenario:Scenario, locale:LocaleId):EditorReductionResult {
	return switch scenario.messages {
		case NoMessageCatalog: ReductionRejected(MissingLocale(locale));
		case EmbeddedMessageCatalog(catalog):
			if (!hasLocale(catalog.locales,
				locale)) ReductionRejected(MissingLocale(locale)); else if (sameLocale(catalog.defaultLocale, locale)
				&& catalog.locales.length > 1) ReductionRejected(CannotRemoveDefaultLocale(locale)); else {
				final locales = [
					for (value in catalog.locales)
						if (!sameLocale(value.id, locale)) copyLocale(value)
				];
				final messages = locales.length == 0 ? NoMessageCatalog : EmbeddedMessageCatalog({
					defaultLocale: catalog.defaultLocale,
					locales: locales
				});
				ready(withMessages(scenario, messages), Localization);
			}
	};
}

private function putMessage(scenario:Scenario, locale:LocaleId, message:ScenarioMessage):EditorReductionResult {
	return switch scenario.messages {
		case NoMessageCatalog: ReductionRejected(MissingLocale(locale));
		case EmbeddedMessageCatalog(catalog):
			if (!hasLocale(catalog.locales, locale)) ReductionRejected(MissingLocale(locale)); else {
				final locales:Array<ScenarioLocaleCatalog> = [];
				for (value in catalog.locales)
					if (sameLocale(value.id, locale))
						locales.push({id: value.id, messages: putMessageValue(value.messages, message)});
					else
						locales.push(copyLocale(value));
				ready(withMessages(scenario, EmbeddedMessageCatalog({defaultLocale: catalog.defaultLocale, locales: locales})), Localization);
			}
	};
}

private function removeMessage(scenario:Scenario, locale:LocaleId, message:MessageId):EditorReductionResult {
	return switch scenario.messages {
		case NoMessageCatalog: ReductionRejected(MissingLocale(locale));
		case EmbeddedMessageCatalog(catalog):
			final found = findLocale(catalog.locales, locale);
			if (found == null) ReductionRejected(MissingLocale(locale)); else if (!hasMessage(found.messages,
				message)) ReductionRejected(MissingMessage(locale, message)); else {
				final locales:Array<ScenarioLocaleCatalog> = [];
				for (value in catalog.locales)
					if (sameLocale(value.id, locale))
						locales.push({
							id: value.id,
							messages: [
								for (entry in value.messages)
									if (!sameMessage(entry.id, message)) copyMessage(entry)
							]
						});
					else
						locales.push(copyLocale(value));
				ready(withMessages(scenario, EmbeddedMessageCatalog({defaultLocale: catalog.defaultLocale, locales: locales})), Localization);
			}
	};
}

private function setPaletteEntry(scenario:Scenario, code:Int, blockType:ContentId):EditorReductionResult {
	if (code < 0 || code > 255)
		return ReductionRejected(InvalidPaletteCode(code));
	return ready(withWorld(scenario, {
		size: scenario.world.size,
		palette: putPalette(scenario.world.palette, {code: code, blockType: blockType}),
		chunks: scenario.world.chunks.copy(),
		fluids: scenario.world.fluids.copy()
	}), Voxel);
}

private function fillBounds(scenario:Scenario, bounds:VoxelBounds, paletteCode:Int):EditorReductionResult {
	if (!hasPaletteCode(scenario, paletteCode))
		return ReductionRejected(UnknownPaletteCode(paletteCode));
	return switch fillWorld(scenario.world, bounds, paletteCode) {
		case WorldRejected(error): ReductionRejected(error);
		case WorldReady(world): ready(withWorld(scenario, world), Voxel);
	}
}

private function stampPrefab(scenario:Scenario, id:ScenarioId, prefabType:ContentId, tags:Array<ScenarioTag>,
		transform:ScenarioTransform):EditorReductionResult {
	if (hasObject(scenario, id))
		return ReductionRejected(DuplicateObject(id));
	final objects = scenario.objects.copy();
	objects.push({id: id, tags: tags.copy(), placement: Prefab(prefabType, transform)});
	return ready(withObjects(scenario, objects), Prefab);
}

private function removePlacedObject(scenario:Scenario, id:ScenarioId):EditorReductionResult {
	if (!hasObject(scenario, id))
		return ReductionRejected(MissingObject(id));
	return ready(withObjects(scenario, removeObject(scenario.objects, id)), Placement);
}

private function removeWorldFluid(scenario:Scenario, id:ScenarioId):EditorReductionResult {
	if (!hasFluid(scenario, id))
		return ReductionRejected(MissingFluid(id));
	return ready(withFluids(scenario, removeFluid(scenario.world.fluids, id)), Fluid);
}

private function removeStoryDialogue(scenario:Scenario, id:ScenarioId):EditorReductionResult {
	if (!hasDialogue(scenario, id))
		return ReductionRejected(MissingDialogue(id));
	return ready(withDialogues(scenario, removeDialogue(scenario.story.dialogues, id)), Dialogue);
}

private function removeStoryObjective(scenario:Scenario, id:ScenarioId):EditorReductionResult {
	if (!hasObjective(scenario, id))
		return ReductionRejected(MissingObjective(id));
	return ready(withObjectives(scenario, removeObjective(scenario.story.objectives, id)), Objective);
}

private function removeFlowRule(scenario:Scenario, id:ScenarioId):EditorReductionResult {
	if (!hasRule(scenario, id))
		return ReductionRejected(MissingRule(id));
	return ready(withRules(scenario, removeRule(scenario.flow.rules, id)), Rule);
}

private function paint(scenario:Scenario, points:Array<VoxelPoint>, paletteCode:Int, maximumCells:Int):EditorReductionResult {
	if (points.length > maximumCells)
		return ReductionRejected(VoxelEditTooLarge(points.length, maximumCells));
	if (!hasPaletteCode(scenario, paletteCode))
		return ReductionRejected(UnknownPaletteCode(paletteCode));
	return switch paintWorld(scenario.world, points, paletteCode) {
		case WorldRejected(error): ReductionRejected(error);
		case WorldReady(world): ready(withWorld(scenario, world), Voxel);
	}
}

private function ready(scenario:Scenario, family:EditorCommandFamily):EditorReductionResult
	return ReductionReady({scenario: scenario, family: family});

private function hasPaletteCode(scenario:Scenario, code:Int):Bool {
	for (entry in scenario.world.palette)
		if (entry.code == code)
			return true;
	return false;
}

private function hasObject(scenario:Scenario, id:ScenarioId):Bool {
	for (value in scenario.objects)
		if (same(value.id, id))
			return true;
	return false;
}

private function hasFluid(scenario:Scenario, id:ScenarioId):Bool {
	for (value in scenario.world.fluids)
		if (same(value.id, id))
			return true;
	return false;
}

private function hasDialogue(scenario:Scenario, id:ScenarioId):Bool {
	for (value in scenario.story.dialogues)
		if (same(value.id, id))
			return true;
	return false;
}

private function hasObjective(scenario:Scenario, id:ScenarioId):Bool {
	for (value in scenario.story.objectives)
		if (same(value.id, id))
			return true;
	return false;
}

private function hasRule(scenario:Scenario, id:ScenarioId):Bool {
	for (value in scenario.flow.rules)
		if (same(value.id, id))
			return true;
	return false;
}

private function putPalette(values:Array<BlockPaletteEntry>, entry:BlockPaletteEntry):Array<BlockPaletteEntry> {
	final result:Array<BlockPaletteEntry> = [];
	var replaced = false;
	for (value in values)
		if (value.code == entry.code) {
			if (!replaced)
				result.push(entry);
			replaced = true;
		} else
			result.push(value);
	if (!replaced)
		result.push(entry);
	return result;
}

private function putObject(values:Array<ScenarioObject>, replacement:ScenarioObject):Array<ScenarioObject> {
	final result:Array<ScenarioObject> = [];
	var replaced = false;
	for (value in values)
		if (same(value.id, replacement.id)) {
			if (!replaced)
				result.push({id: replacement.id, tags: replacement.tags.copy(), placement: replacement.placement});
			replaced = true;
		} else
			result.push(value);
	if (!replaced)
		result.push({id: replacement.id, tags: replacement.tags.copy(), placement: replacement.placement});
	return result;
}

private function putFluid(values:Array<ScenarioFluid>, replacement:ScenarioFluid):Array<ScenarioFluid> {
	final result = [for (value in values) if (!same(value.id, replacement.id)) value];
	result.push(replacement);
	return result;
}

private function removeFluid(values:Array<ScenarioFluid>, id:ScenarioId):Array<ScenarioFluid>
	return [for (value in values) if (!same(value.id, id)) value];

private function removeObject(values:Array<ScenarioObject>, id:ScenarioId):Array<ScenarioObject>
	return [for (value in values) if (!same(value.id, id)) value];

private function putDialogue(values:Array<ScenarioDialogue>, replacement:ScenarioDialogue):Array<ScenarioDialogue> {
	final result = [for (value in values) if (!same(value.id, replacement.id)) value];
	result.push({id: replacement.id, lines: replacement.lines.copy()});
	return result;
}

private function removeDialogue(values:Array<ScenarioDialogue>, id:ScenarioId):Array<ScenarioDialogue>
	return [for (value in values) if (!same(value.id, id)) value];

private function putObjective(values:Array<ScenarioObjective>, replacement:ScenarioObjective):Array<ScenarioObjective> {
	final result = [for (value in values) if (!same(value.id, replacement.id)) value];
	result.push(replacement);
	return result;
}

private function removeObjective(values:Array<ScenarioObjective>, id:ScenarioId):Array<ScenarioObjective>
	return [for (value in values) if (!same(value.id, id)) value];

private function putRule(values:Array<FlowRule>, replacement:FlowRule):Array<FlowRule> {
	final result = [for (value in values) if (!same(value.id, replacement.id)) value];
	result.push({
		id: replacement.id,
		priority: replacement.priority,
		repeat: replacement.repeat,
		event: replacement.event,
		predicate: replacement.predicate,
		actions: replacement.actions.copy()
	});
	return result;
}

private function removeRule(values:Array<FlowRule>, id:ScenarioId):Array<FlowRule>
	return [for (value in values) if (!same(value.id, id)) value];

private function putLocale(source:ScenarioMessages, replacement:ScenarioLocaleCatalog):ScenarioMessages {
	return switch source {
		case NoMessageCatalog:
			EmbeddedMessageCatalog({defaultLocale: replacement.id, locales: [copyLocale(replacement)]});
		case EmbeddedMessageCatalog(catalog):
			final locales = [
				for (value in catalog.locales)
					if (!sameLocale(value.id, replacement.id)) copyLocale(value)
			];
			locales.push(copyLocale(replacement));
			EmbeddedMessageCatalog({defaultLocale: catalog.defaultLocale, locales: locales});
	};
}

private function putMessageValue(values:Array<ScenarioMessage>, replacement:ScenarioMessage):Array<ScenarioMessage> {
	final result = [
		for (value in values)
			if (!sameMessage(value.id, replacement.id)) copyMessage(value)
	];
	result.push(copyMessage(replacement));
	return result;
}

private function findLocale(values:Array<ScenarioLocaleCatalog>, id:LocaleId):Null<ScenarioLocaleCatalog> {
	for (value in values)
		if (sameLocale(value.id, id))
			return value;
	return null;
}

private inline function hasLocale(values:Array<ScenarioLocaleCatalog>, id:LocaleId):Bool
	return findLocale(values, id) != null;

private function hasMessage(values:Array<ScenarioMessage>, id:MessageId):Bool {
	for (value in values)
		if (sameMessage(value.id, id))
			return true;
	return false;
}

private function copyLocales(values:Array<ScenarioLocaleCatalog>):Array<ScenarioLocaleCatalog>
	return [for (value in values) copyLocale(value)];

private function copyLocale(value:ScenarioLocaleCatalog):ScenarioLocaleCatalog
	return {id: value.id, messages: [for (message in value.messages) copyMessage(message)]};

private inline function copyMessage(value:ScenarioMessage):ScenarioMessage
	return {id: value.id, text: value.text};

private inline function sameLocale(left:LocaleId, right:LocaleId):Bool
	return left.text() == right.text();

private inline function sameMessage(left:MessageId, right:MessageId):Bool
	return left.text() == right.text();

private inline function same(left:ScenarioId, right:ScenarioId):Bool
	return left.text() == right.text();

private function withWorld(scenario:Scenario, world:caxecraft.scenario.ScenarioWorld):Scenario
	return copy(scenario, scenario.messages, scenario.title, world, scenario.objects, scenario.story.dialogues, scenario.story.objectives, scenario.flow.rules);

private function withFluids(scenario:Scenario, fluids:Array<ScenarioFluid>):Scenario
	return withWorld(scenario, {
		size: scenario.world.size,
		palette: scenario.world.palette.copy(),
		chunks: scenario.world.chunks.copy(),
		fluids: fluids
	});

private function withObjects(scenario:Scenario, objects:Array<ScenarioObject>):Scenario
	return copy(scenario, scenario.messages, scenario.title, scenario.world, objects, scenario.story.dialogues, scenario.story.objectives, scenario.flow.rules);

private function withDialogues(scenario:Scenario, dialogues:Array<ScenarioDialogue>):Scenario
	return copy(scenario, scenario.messages, scenario.title, scenario.world, scenario.objects, dialogues, scenario.story.objectives, scenario.flow.rules);

private function withObjectives(scenario:Scenario, objectives:Array<ScenarioObjective>):Scenario
	return copy(scenario, scenario.messages, scenario.title, scenario.world, scenario.objects, scenario.story.dialogues, objectives, scenario.flow.rules);

private function withRules(scenario:Scenario, rules:Array<FlowRule>):Scenario
	return copy(scenario, scenario.messages, scenario.title, scenario.world, scenario.objects, scenario.story.dialogues, scenario.story.objectives, rules);

private function withMessages(scenario:Scenario, messages:ScenarioMessages):Scenario
	return copy(scenario, messages, scenario.title, scenario.world, scenario.objects, scenario.story.dialogues, scenario.story.objectives, scenario.flow.rules);

private function copy(scenario:Scenario, messages:ScenarioMessages, title:ScenarioText, world:caxecraft.scenario.ScenarioWorld, objects:Array<ScenarioObject>,
		dialogues:Array<ScenarioDialogue>, objectives:Array<ScenarioObjective>, rules:Array<FlowRule>):Scenario {
	return {
		formatVersion: scenario.formatVersion,
		requiredFeatures: scenario.requiredFeatures.copy(),
		optionalFeatures: scenario.optionalFeatures.copy(),
		id: scenario.id,
		assetPack: scenario.assetPack,
		messages: messages,
		title: title,
		mode: scenario.mode,
		environment: scenario.environment,
		world: world,
		objects: objects,
		story: {
			speakerNames: scenario.story.speakerNames.copy(),
			dialogues: dialogues,
			journal: scenario.story.journal.copy(),
			objectives: objectives,
			routes: scenario.story.routes.copy()
		},
		flow: {
			variables: scenario.flow.variables.copy(),
			sequences: scenario.flow.sequences.copy(),
			rules: rules
		},
		extensions: scenario.extensions.copy()
	};
}
