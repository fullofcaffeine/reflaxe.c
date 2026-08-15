package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorChangeId;
import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorNodeRef;
import caxecraft.editor.EditorTypes.EditorSection;
import caxecraft.editor.EditorTypes.EditorTreeNode;
import caxecraft.scenario.Scenario;

/**
	Builds small deterministic facts shared by editor views and automation.

	The session owns mutation and history; this stateless module only maps a
	typed command to semantic change identities and projects one typed scenario
	into a flat tree. Keeping that work here avoids teaching renderers or future
	transport adapters how CAXEMAP stores each record family.
**/
/** Return the semantic identities one accepted command may change. */
function changesFor(command:EditorCommand):Array<EditorChangeId> {
	return switch command {
		case SetTitle(_): [ChangedTitle];
		case ResizeWorld(_): [ChangedWorldShape];
		case SetPaletteEntry(code, _): [ChangedPalette(code)];
		case PaintVoxel(_, _) | EraseVoxel(_) | PaintVoxels(_, _) | EraseVoxels(_) | FillBounds(_, _): [ChangedTerrain];
		case PutFluid(fluid): [ChangedFluid(fluid.id)];
		case RemoveFluid(id): [ChangedFluid(id)];
		case StampPrefab(id, _, _, _): [ChangedObject(id)];
		case PutObject(object): [ChangedObject(object.id)];
		case RemoveObject(id): [ChangedObject(id)];
		case PutDialogue(dialogue): [ChangedDialogue(dialogue.id)];
		case RemoveDialogue(id): [ChangedDialogue(id)];
		case PutObjective(objective): [ChangedObjective(objective.id)];
		case RemoveObjective(id): [ChangedObjective(id)];
		case PutRule(rule): [ChangedRule(rule.id)];
		case RemoveRule(id): [ChangedRule(id)];
		case SetDefaultLocale(_): [ChangedLocalization];
		case PutLocale(locale): [ChangedLocale(locale.id)];
		case RemoveLocale(locale): [ChangedLocale(locale)];
		case PutMessage(locale, message): [ChangedMessage(locale, message.id)];
		case RemoveMessage(locale, message): [ChangedMessage(locale, message)];
		case RestoreLastPlayable: [ChangedDocument];
	};
}

/** Append identities once while preserving first-command order. */
function mergeChanges(target:Array<EditorChangeId>, source:Array<EditorChangeId>):Void {
	for (candidate in source) {
		var found = false;
		for (existing in target)
			if (sameChange(existing, candidate)) {
				found = true;
				break;
			}
		if (!found)
			target.push(candidate);
	}
}

/** Project every current CAXEMAP record into one deterministic flat tree. */
function buildTree(scenario:Scenario):Array<EditorTreeNode> {
	final result:Array<EditorTreeNode> = [];
	final root = ScenarioNode(scenario.id);
	result.push(node(root, null, 6));
	addSection(result, root, World, 3);
	addSection(result, root, Objects, scenario.objects.length);
	addSection(result, root, Story, 4);
	addSection(result, root, Flow, 3);
	addSection(result, root, Extensions, scenario.extensions.length);

	final world = SectionNode(World);
	addSection(result, world, Palette, scenario.world.palette.length);
	addSection(result, world, Chunks, scenario.world.chunks.length);
	addSection(result, world, Fluids, scenario.world.fluids.length);
	for (entry in scenario.world.palette)
		result.push(node(PaletteNode(entry.code), SectionNode(Palette), 0));
	for (chunk in scenario.world.chunks)
		result.push(node(ChunkNode(chunk.id), SectionNode(Chunks), 0));
	for (fluid in scenario.world.fluids)
		result.push(node(FluidNode(fluid.id), SectionNode(Fluids), 0));
	for (object in scenario.objects)
		result.push(node(ObjectNode(object.id), SectionNode(Objects), 0));

	final story = SectionNode(Story);
	addSection(result, story, Dialogues, scenario.story.dialogues.length);
	addSection(result, story, Journal, scenario.story.journal.length);
	addSection(result, story, Objectives, scenario.story.objectives.length);
	addSection(result, story, Routes, scenario.story.routes.length);
	for (dialogue in scenario.story.dialogues)
		result.push(node(DialogueNode(dialogue.id), SectionNode(Dialogues), 0));
	for (entry in scenario.story.journal)
		result.push(node(JournalNode(entry.id), SectionNode(Journal), 0));
	for (objective in scenario.story.objectives)
		result.push(node(ObjectiveNode(objective.id), SectionNode(Objectives), 0));
	for (route in scenario.story.routes)
		result.push(node(RouteNode(route.id), SectionNode(Routes), 0));

	final flow = SectionNode(Flow);
	addSection(result, flow, Variables, scenario.flow.variables.length);
	addSection(result, flow, Sequences, scenario.flow.sequences.length);
	addSection(result, flow, Rules, scenario.flow.rules.length);
	for (variable in scenario.flow.variables)
		result.push(node(VariableNode(variable.id), SectionNode(Variables), 0));
	for (sequence in scenario.flow.sequences)
		result.push(node(SequenceNode(sequence.id), SectionNode(Sequences), 0));
	for (rule in scenario.flow.rules)
		result.push(node(RuleNode(rule.id), SectionNode(Rules), 0));

	final localeCount = switch scenario.messages {
		case NoMessageCatalog: 0;
		case EmbeddedMessageCatalog(catalog): catalog.locales.length;
	};
	addSection(result, root, Localization, localeCount);
	switch scenario.messages {
		case NoMessageCatalog:
		case EmbeddedMessageCatalog(catalog):
			for (locale in catalog.locales) {
				final localeRef = LocaleNode(locale.id);
				result.push(node(localeRef, SectionNode(Localization), locale.messages.length));
				for (message in locale.messages)
					result.push(node(MessageNode(locale.id, message.id), localeRef, 0));
			}
	}
	for (extension in scenario.extensions)
		result.push(node(ExtensionNode(extension.feature, extension.id), SectionNode(Extensions), 0));
	return result;
}

/** Find one compact node row without exposing the source scenario arrays. */
function findNode(scenario:Scenario, ref:EditorNodeRef):Null<EditorTreeNode> {
	for (candidate in buildTree(scenario))
		if (sameNodeRef(candidate.ref, ref))
			return {ref: candidate.ref, parent: candidate.parent, childCount: candidate.childCount};
	return null;
}

private function addSection(target:Array<EditorTreeNode>, parent:EditorNodeRef, section:EditorSection, children:Int):Void
	target.push(node(SectionNode(section), parent, children));

private inline function node(ref:EditorNodeRef, parent:Null<EditorNodeRef>, childCount:Int):EditorTreeNode
	return {ref: ref, parent: parent, childCount: childCount};

/** Compare typed tree identities without flattening them into display text. */
@:noCompletion
function sameNodeRef(left:EditorNodeRef, right:EditorNodeRef):Bool {
	return switch left {
		case ScenarioNode(id): switch right {
				case ScenarioNode(other): id.text() == other.text();
				case _: false;
			};
		case SectionNode(section): switch right {
				case SectionNode(other): section == other;
				case _: false;
			};
		case PaletteNode(code): switch right {
				case PaletteNode(other): code == other;
				case _: false;
			};
		case ChunkNode(id): switch right {
				case ChunkNode(other): id.text() == other.text();
				case _: false;
			};
		case FluidNode(id): switch right {
				case FluidNode(other): id.text() == other.text();
				case _: false;
			};
		case ObjectNode(id): switch right {
				case ObjectNode(other): id.text() == other.text();
				case _: false;
			};
		case DialogueNode(id): switch right {
				case DialogueNode(other): id.text() == other.text();
				case _: false;
			};
		case JournalNode(id): switch right {
				case JournalNode(other): id.text() == other.text();
				case _: false;
			};
		case ObjectiveNode(id): switch right {
				case ObjectiveNode(other): id.text() == other.text();
				case _: false;
			};
		case RouteNode(id): switch right {
				case RouteNode(other): id.text() == other.text();
				case _: false;
			};
		case VariableNode(id): switch right {
				case VariableNode(other): id.text() == other.text();
				case _: false;
			};
		case SequenceNode(id): switch right {
				case SequenceNode(other): id.text() == other.text();
				case _: false;
			};
		case RuleNode(id): switch right {
				case RuleNode(other): id.text() == other.text();
				case _: false;
			};
		case LocaleNode(id): switch right {
				case LocaleNode(other): id.text() == other.text();
				case _: false;
			};
		case MessageNode(locale, message): switch right {
				case MessageNode(otherLocale, otherMessage): locale.text() == otherLocale.text() && message.text() == otherMessage.text();
				case _: false;
			};
		case ExtensionNode(feature, id): switch right {
				case ExtensionNode(otherFeature, otherId): feature.text() == otherFeature.text() && id.text() == otherId.text();
				case _: false;
			};
	};
}

private function sameChange(left:EditorChangeId, right:EditorChangeId):Bool {
	return switch left {
		case ChangedDocument:
			switch right {
				case ChangedDocument: true;
				case _: false;
			}
		case ChangedTitle:
			switch right {
				case ChangedTitle: true;
				case _: false;
			}
		case ChangedWorldShape:
			switch right {
				case ChangedWorldShape: true;
				case _: false;
			}
		case ChangedTerrain:
			switch right {
				case ChangedTerrain: true;
				case _: false;
			}
		case ChangedPalette(code):
			switch right {
				case ChangedPalette(other): code == other;
				case _: false;
			}
		case ChangedFluid(id):
			switch right {
				case ChangedFluid(other): id.text() == other.text();
				case _: false;
			}
		case ChangedObject(id):
			switch right {
				case ChangedObject(other): id.text() == other.text();
				case _: false;
			}
		case ChangedDialogue(id):
			switch right {
				case ChangedDialogue(other): id.text() == other.text();
				case _: false;
			}
		case ChangedObjective(id):
			switch right {
				case ChangedObjective(other): id.text() == other.text();
				case _: false;
			}
		case ChangedRule(id):
			switch right {
				case ChangedRule(other): id.text() == other.text();
				case _: false;
			}
		case ChangedLocalization:
			switch right {
				case ChangedLocalization: true;
				case _: false;
			}
		case ChangedLocale(id):
			switch right {
				case ChangedLocale(other): id.text() == other.text();
				case _: false;
			}
		case ChangedMessage(locale, message):
			switch right {
				case ChangedMessage(otherLocale, otherMessage): locale.text() == otherLocale.text() && message.text() == otherMessage.text();
				case _: false;
			}
	};
}
