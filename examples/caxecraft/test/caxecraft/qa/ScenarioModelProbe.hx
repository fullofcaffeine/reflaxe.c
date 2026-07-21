package caxecraft.qa;

import caxecraft.scenario.CaxeFlow;
import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowChoice;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowRule;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowSequence;
import caxecraft.scenario.CaxeFlow.FlowParameter;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlow.FlowValueKind;
import caxecraft.scenario.CaxeFlow.FlowVariable;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.LogicalPath;
import caxecraft.scenario.LocaleId;
import caxecraft.scenario.MessageId;
import caxecraft.scenario.Scenario;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioCodecModel.ScenarioLexTokenKind;
import caxecraft.scenario.ScenarioCodecModel.ScenarioSourceSubject;
import caxecraft.scenario.ScenarioDiagnostic.PersistenceStage;
import caxecraft.scenario.ScenarioDiagnostic;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioDiagnosticKind;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioExpectedRecord;
import caxecraft.scenario.ScenarioDiagnostic.ScenarioLimitKind;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioStory;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import caxecraft.scenario.ScenarioStory.ScenarioDialogue;
import caxecraft.scenario.ScenarioStory.ScenarioJournalEntry;
import caxecraft.scenario.ScenarioStory.ScenarioObjective;
import caxecraft.scenario.ScenarioStory.ScenarioRoute;
import caxecraft.scenario.ScenarioTag;
import caxecraft.scenario.ScenarioText;
import caxecraft.scenario.ScenarioWorld;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;
import caxecraft.scenario.ScenarioWorld.VoxelChunk;
import caxecraft.scenario.ScenarioWorld.VoxelRun;

/** Eval-only construction probe for every closed CAXEMAP 1 model family. */
final class ScenarioModelProbe {
	static function main():Void {
		Sys.println("scenario-model: " + modelHash());
	}

	public static function modelHash():Int {
		final objectId = new ScenarioId("object.one");
		final otherId = new ScenarioId("object.two");
		final content = new ContentId("caxecraft:sample");
		final transform:ScenarioTransform = {
			xMilli: 1000,
			yMilli: 2000,
			zMilli: 3000,
			yawDegrees: 90
		};
		final bounds:VoxelBounds = {
			origin: point(0, 0, 0),
			size: size(2, 2, 2)
		};
		final placements:Array<ObjectPlacement> = [
			PlayerSpawn(transform),
			Checkpoint(transform),
			Item(content, 1, transform),
			Entity(content, transform),
			Npc(content, otherId, transform),
			Prefab(content, transform),
			TriggerZone(bounds),
			StatefulObject(content, content, transform)
		];

		final events:Array<FlowEvent> = [
			EnterZone(objectId),
			LeaveZone(objectId),
			Interact(objectId),
			BlockChanged(objectId, content),
			UseItem(content),
			EntityDefeated(objectId),
			SignalReceived(content),
			TimerExpired(objectId),
			ObjectiveChanged(objectId),
			StateChanged(objectId)
		];
		final scopes:Array<FlowScope> = [FlowScope.Map, FlowScope.Player, FlowScope.Quest, FlowScope.Local(otherId)];
		final values:Array<FlowValue> = [FlowValue.Flag(false), FlowValue.Counter(0), FlowValue.State(content)];
		final valueKinds:Array<FlowValueKind> = [FlagValue, CounterValue, StateValue];
		final comparisons:Array<FlowComparison> = [Equal, NotEqual, Less, LessOrEqual, Greater, GreaterOrEqual];
		final predicates:Array<FlowPredicate> = [
			Always,
			All([Always]),
			AnyOf([Always]),
			Not(Always),
			FlagIs(objectId, true),
			CounterCompare(objectId, FlowComparison.GreaterOrEqual, 1),
			StateIs(objectId, content),
			ObjectStateIs(objectId, content),
			InventoryHas(objectId, content, FlowComparison.Equal, 1),
			ObjectiveIs(objectId, ObjectiveState.Active),
			NearObject(objectId, otherId, 1000),
			ModeIs(ScenarioMode.Creative)
		];
		final arguments:Array<FlowArgument> = [Value(FlowValue.Flag(true)), Variable(objectId)];
		final tokenKinds:Array<ScenarioLexTokenKind> = [BareToken, QuotedText];
		final sourceSubjects:Array<ScenarioSourceSubject> = [
			Header,
			Feature(content),
			MapIdentity(objectId),
			AssetPack,
			DefaultLocale,
			Locale(new LocaleId("en")),
			LocaleMessage(new LocaleId("en"), new MessageId("sample.message")),
			MessageReference(new MessageId("sample.message")),
			Title,
			Mode,
			World,
			Palette(0),
			Chunk(objectId),
			Object(objectId),
			ObjectTag(objectId, new ScenarioTag("sample")),
			Dialogue(objectId),
			Journal(objectId),
			Objective(objectId),
			Route(objectId),
			Variable(objectId),
			Sequence(otherId),
			SequenceParameter(otherId, objectId),
			SequenceAction(otherId, 0),
			Rule(objectId),
			RuleEvent(objectId),
			RulePredicate(objectId),
			RuleAction(objectId, 0),
			Extension(content, objectId)
		];
		final repeatPolicies:Array<FlowRepeatPolicy> = [Once, Repeat, Cooldown(1)];
		final choice:FlowChoice = {weight: 1, actions: [EmitSignal(content)]};
		final actions:Array<FlowAction> = [
			ShowDialogue(objectId),
			AddJournal(objectId),
			SetFlag(objectId, true),
			SetCounter(objectId, 1),
			AddCounter(objectId, 2),
			SetState(objectId, content),
			GiveItem(objectId, content, 1),
			TakeItem(objectId, content, 1),
			Spawn(objectId),
			Despawn(objectId),
			SetObjectState(objectId, content),
			SetCheckpoint(objectId),
			SetObjective(objectId, ObjectiveState.Complete),
			PlayEffect(content, objectId),
			EmitSignal(content),
			Schedule(otherId, 1, otherId, arguments),
			CallSequence(otherId, arguments),
			ChooseSeeded(objectId, [choice])
		];

		final variables:Array<FlowVariable> = [
			{id: objectId, scope: FlowScope.Map, initial: FlowValue.Flag(false)},
			{id: otherId, scope: FlowScope.Player, initial: FlowValue.Counter(0)},
			{id: new ScenarioId("quest.state"), scope: FlowScope.Quest, initial: FlowValue.State(content)},
			{id: new ScenarioId("local.flag"), scope: FlowScope.Local(otherId), initial: FlowValue.Flag(false)}
		];
		final parameter:FlowParameter = {id: objectId, initial: FlowValue.Counter(0)};
		final sequence:FlowSequence = {id: otherId, parameters: [parameter], actions: actions};
		final rule:FlowRule = {
			id: objectId,
			priority: 100,
			repeat: FlowRepeatPolicy.Once,
			event: events[0],
			predicate: predicates[0],
			actions: actions
		};
		final flow:CaxeFlow = {variables: variables, sequences: [sequence], rules: [rule]};

		final palette:Array<BlockPaletteEntry> = [{code: 0, blockType: new ContentId("caxecraft:air")}];
		final runs:Array<VoxelRun> = [{paletteCode: 0, count: 8}];
		final chunk:VoxelChunk = {
			id: objectId,
			origin: point(0, 0, 0),
			size: size(2, 2, 2),
			runs: runs
		};
		final world:ScenarioWorld = {size: size(2, 2, 2), palette: palette, chunks: [chunk]};
		final objects:Array<ScenarioObject> = [{id: objectId, tags: [new ScenarioTag("sample")], placement: placements[0]}];
		final dialogue:ScenarioDialogue = {
			id: objectId,
			lines: [{speaker: null, text: ScenarioText.Message(new MessageId("sample.message"))}]
		};
		final journal:ScenarioJournalEntry = {
			id: objectId,
			title: ScenarioText.Literal("Title"),
			body: ScenarioText.Literal("Body")
		};
		final objective:ScenarioObjective = {
			id: objectId,
			title: ScenarioText.Literal("Title"),
			body: ScenarioText.Literal("Body"),
			initialState: ObjectiveState.Active
		};
		final route:ScenarioRoute = {id: objectId, title: ScenarioText.Literal("Route"), objectives: [objectId]};
		final story:ScenarioStory = {
			dialogues: [dialogue],
			journal: [journal],
			objectives: [objective],
			routes: [route]
		};
		final scenario:Scenario = {
			formatVersion: 1,
			requiredFeatures: [new ContentId("caxecraft:core")],
			optionalFeatures: [],
			id: objectId,
			assetPack: new LogicalPath("packs/caxecraft/base"),
			messages: NoMessageCatalog,
			title: ScenarioText.Message(new MessageId("sample.title")),
			mode: ScenarioMode.Creative,
			world: world,
			objects: objects,
			story: story,
			flow: flow,
			extensions: [
				{
					feature: new ContentId("caxecraft:sample"),
					id: new ScenarioId("extension.sample"),
					data: "sample"
				}
			]
		};

		final diagnostics:Array<ScenarioDiagnosticKind> = [
			MalformedUtf8(1),
			UnknownVersion(2),
			UnknownRequiredFeature(content),
			InvalidToken,
			InvalidEscape,
			UnexpectedRecord("sample"),
			MissingRecord(FormatHeader),
			IntegerOutOfRange,
			LimitExceeded(FileBytes, 1),
			InvalidRunTotal(objectId, 1, 2),
			DuplicateContentId(content),
			DuplicatePaletteCode(1),
			DuplicateId(objectId),
			DuplicateTag(objectId, new ScenarioTag("sample")),
			DuplicateLocale(new LocaleId("en")),
			DuplicateMessage(new LocaleId("en"), new MessageId("sample.message")),
			UnknownDefaultLocale(new LocaleId("fr")),
			UnresolvedMessage(new MessageId("sample.message")),
			MissingTranslation(new LocaleId("es-mx"), new MessageId("sample.message")),
			UnknownTranslation(new LocaleId("es-mx"), new MessageId("sample.extra")),
			UnresolvedReference(objectId),
			UnresolvedContent(content),
			ImpossiblePlacement(objectId),
			InvalidRule(objectId),
			RuleCycle(objectId),
			InvalidExtension(objectId),
			EventBudgetExhausted(1),
			PersistenceFailed(PersistenceStage.ReplaceDestination)
		];
		final expectedRecords:Array<ScenarioExpectedRecord> = [
			FormatHeader,
			EndMapRecord,
			MapRecord,
			AssetPackRecord,
			DefaultLocaleRecord,
			TitleRecord,
			ModeRecord,
			WorldRecord,
			EndChunkRecord,
			EndObjectRecord,
			ObjectPlacementRecord,
			EndDialogueRecord,
			EndLocaleRecord,
			JournalBodyRecord,
			EndJournalRecord,
			ObjectiveBodyRecord,
			EndObjectiveRecord,
			EndRouteRecord,
			EndSequenceRecord,
			EndRuleRecord,
			ChoiceRecord,
			EndChoiceRecord,
			ExtensionDataRecord,
			EndExtensionRecord,
			CoreFeatureRecord,
			AirPaletteRecord,
			CompleteChunkCoverage,
			SinglePlayerSpawn
		];
		final limitKinds:Array<ScenarioLimitKind> = [
			FileBytes,
			LogicalRecords,
			TextScalars,
			WorldWidth,
			WorldHeight,
			WorldDepth,
			WorldCells,
			PaletteEntries,
			Objects,
			ObjectTags,
			Dialogues,
			DialogueLines,
			Locales,
			MessagesPerLocale,
			Objectives,
			Routes,
			Sequences,
			Variables,
			Rules,
			RuleActions,
			SequenceCallDepth
		];
		final persistenceStages:Array<PersistenceStage> = [
			CreateTemporary,
			WriteTemporary,
			FlushTemporary,
			ReplaceDestination,
			CleanupTemporary
		];
		final objectiveStates:Array<ObjectiveState> = [Hidden, Active, Complete, Failed];
		final modes:Array<ScenarioMode> = [Creative, Adventure];
		final texts:Array<ScenarioText> = [Message(new MessageId("sample.message")), Literal("sample")];
		final diagnostic:ScenarioDiagnostic = {
			coordinate: {line: 1, column: 1, record: 0},
			kind: diagnostics[0]
		};

		var hash = scenario.formatVersion + diagnostic.coordinate.line;
		for (placement in placements)
			hash = mix(hash, placementCode(placement));
		for (event in events)
			hash = mix(hash, eventCode(event));
		for (scope in scopes)
			hash = mix(hash, scopeCode(scope));
		for (value in values)
			hash = mix(hash, valueCode(value));
		for (kind in valueKinds)
			hash = mix(hash, valueKindCode(kind));
		for (comparison in comparisons)
			hash = mix(hash, comparisonCode(comparison));
		for (predicate in predicates)
			hash = mix(hash, predicateCode(predicate));
		for (action in actions)
			hash = mix(hash, actionCode(action));
		for (argument in arguments)
			hash = mix(hash, argumentCode(argument));
		for (kind in tokenKinds)
			hash = mix(hash, tokenKindCode(kind));
		for (subject in sourceSubjects)
			hash = mix(hash, sourceSubjectCode(subject));
		for (repeat in repeatPolicies)
			hash = mix(hash, repeatCode(repeat));
		for (state in objectiveStates)
			hash = mix(hash, objectiveStateCode(state));
		for (mode in modes)
			hash = mix(hash, modeCode(mode));
		for (text in texts)
			hash = mix(hash, textCode(text));
		for (diagnostic in diagnostics)
			hash = mix(hash, diagnosticCode(diagnostic));
		for (expected in expectedRecords)
			hash = mix(hash, expectedRecordCode(expected));
		for (limit in limitKinds)
			hash = mix(hash, limitKindCode(limit));
		for (stage in persistenceStages)
			hash = mix(hash, persistenceCode(stage));
		return hash;
	}

	static function point(x:Int, y:Int, z:Int):VoxelPoint
		return {x: x, y: y, z: z};

	static function size(width:Int, height:Int, depth:Int):VoxelSize
		return {width: width, height: height, depth: depth};

	static function mix(hash:Int, value:Int):Int
		return (hash ^ value) * 16777619;

	static function placementCode(value:ObjectPlacement):Int
		return switch (value) {
			case PlayerSpawn(_): 1;
			case Checkpoint(_): 2;
			case Item(_, _, _): 3;
			case Entity(_, _): 4;
			case Npc(_, _, _): 5;
			case Prefab(_, _): 6;
			case TriggerZone(_): 7;
			case StatefulObject(_, _, _): 8;
		};

	static function eventCode(value:FlowEvent):Int
		return switch (value) {
			case EnterZone(_): 1;
			case LeaveZone(_): 2;
			case Interact(_): 3;
			case BlockChanged(_, _): 4;
			case UseItem(_): 5;
			case EntityDefeated(_): 6;
			case SignalReceived(_): 7;
			case TimerExpired(_): 8;
			case ObjectiveChanged(_): 9;
			case StateChanged(_): 10;
		};

	static function scopeCode(value:FlowScope):Int
		return switch (value) {
			case Map: 1;
			case Player: 2;
			case Quest: 3;
			case Local(_): 4;
		};

	static function valueCode(value:FlowValue):Int
		return switch (value) {
			case Flag(_): 1;
			case Counter(_): 2;
			case State(_): 3;
		};

	static function valueKindCode(value:FlowValueKind):Int
		return switch (value) {
			case FlagValue: 1;
			case CounterValue: 2;
			case StateValue: 3;
		};

	static function comparisonCode(value:FlowComparison):Int
		return switch (value) {
			case Equal: 1;
			case NotEqual: 2;
			case Less: 3;
			case LessOrEqual: 4;
			case Greater: 5;
			case GreaterOrEqual: 6;
		};

	static function predicateCode(value:FlowPredicate):Int
		return switch (value) {
			case Always: 1;
			case All(_): 2;
			case AnyOf(_): 3;
			case Not(_): 4;
			case FlagIs(_, _): 5;
			case CounterCompare(_, _, _): 6;
			case StateIs(_, _): 7;
			case ObjectStateIs(_, _): 8;
			case InventoryHas(_, _, _, _): 9;
			case ObjectiveIs(_, _): 10;
			case NearObject(_, _, _): 11;
			case ModeIs(_): 12;
		};

	static function actionCode(value:FlowAction):Int
		return switch (value) {
			case ShowDialogue(_): 1;
			case AddJournal(_): 2;
			case SetFlag(_, _): 3;
			case SetCounter(_, _): 4;
			case AddCounter(_, _): 5;
			case SetState(_, _): 6;
			case GiveItem(_, _, _): 7;
			case TakeItem(_, _, _): 8;
			case Spawn(_): 9;
			case Despawn(_): 10;
			case SetObjectState(_, _): 11;
			case SetCheckpoint(_): 12;
			case SetObjective(_, _): 13;
			case PlayEffect(_, _): 14;
			case EmitSignal(_): 15;
			case Schedule(_, _, _, _): 16;
			case CallSequence(_, _): 17;
			case ChooseSeeded(_, _): 18;
		};

	static function argumentCode(value:FlowArgument):Int
		return switch value {
			case Value(_): 1;
			case Variable(_): 2;
		};

	static function tokenKindCode(value:ScenarioLexTokenKind):Int
		return switch value {
			case BareToken: 1;
			case QuotedText: 2;
		};

	static function sourceSubjectCode(value:ScenarioSourceSubject):Int
		return switch value {
			case Header: 1;
			case Feature(_): 2;
			case MapIdentity(_): 3;
			case AssetPack: 4;
			case DefaultLocale: 25;
			case Locale(_): 26;
			case LocaleMessage(_, _): 27;
			case MessageReference(_): 28;
			case Title: 5;
			case Mode: 6;
			case World: 7;
			case Palette(_): 8;
			case Chunk(_): 9;
			case Object(_): 10;
			case ObjectTag(_, _): 11;
			case Dialogue(_): 12;
			case Journal(_): 13;
			case Objective(_): 14;
			case Route(_): 15;
			case Variable(_): 16;
			case Sequence(_): 17;
			case SequenceParameter(_, _): 18;
			case SequenceAction(_, _): 19;
			case Rule(_): 20;
			case RuleEvent(_): 21;
			case RulePredicate(_): 22;
			case RuleAction(_, _): 23;
			case Extension(_, _): 24;
		};

	static function repeatCode(value:FlowRepeatPolicy):Int
		return switch (value) {
			case Once: 1;
			case Repeat: 2;
			case Cooldown(_): 3;
		};

	static function objectiveStateCode(value:ObjectiveState):Int
		return switch (value) {
			case Hidden: 1;
			case Active: 2;
			case Complete: 3;
			case Failed: 4;
		};

	static function modeCode(value:ScenarioMode):Int
		return switch (value) {
			case Creative: 1;
			case Adventure: 2;
		};

	static function textCode(value:ScenarioText):Int
		return switch (value) {
			case Message(_): 1;
			case Literal(_): 2;
		};

	static function diagnosticCode(value:ScenarioDiagnosticKind):Int
		return switch (value) {
			case MalformedUtf8(_): 1;
			case UnknownVersion(_): 2;
			case UnknownRequiredFeature(_): 3;
			case InvalidToken: 4;
			case InvalidEscape: 5;
			case UnexpectedRecord(_): 6;
			case MissingRecord(_): 7;
			case IntegerOutOfRange: 8;
			case LimitExceeded(_, _): 9;
			case InvalidRunTotal(_, _, _): 10;
			case DuplicateContentId(_): 11;
			case DuplicatePaletteCode(_): 12;
			case DuplicateId(_): 13;
			case DuplicateTag(_, _): 14;
			case DuplicateLocale(_): 23;
			case DuplicateMessage(_, _): 24;
			case UnknownDefaultLocale(_): 25;
			case UnresolvedMessage(_): 26;
			case MissingTranslation(_, _): 27;
			case UnknownTranslation(_, _): 28;
			case UnresolvedReference(_): 15;
			case UnresolvedContent(_): 16;
			case ImpossiblePlacement(_): 17;
			case InvalidRule(_): 18;
			case RuleCycle(_): 19;
			case InvalidExtension(_): 20;
			case EventBudgetExhausted(_): 21;
			case PersistenceFailed(_): 22;
		};

	static function expectedRecordCode(value:ScenarioExpectedRecord):Int
		return switch value {
			case FormatHeader: 1;
			case EndMapRecord: 2;
			case MapRecord: 3;
			case AssetPackRecord: 4;
			case DefaultLocaleRecord: 27;
			case TitleRecord: 5;
			case ModeRecord: 6;
			case WorldRecord: 7;
			case EndChunkRecord: 8;
			case EndObjectRecord: 9;
			case ObjectPlacementRecord: 10;
			case EndDialogueRecord: 11;
			case EndLocaleRecord: 28;
			case JournalBodyRecord: 12;
			case EndJournalRecord: 13;
			case ObjectiveBodyRecord: 14;
			case EndObjectiveRecord: 15;
			case EndRouteRecord: 16;
			case EndSequenceRecord: 17;
			case EndRuleRecord: 18;
			case ChoiceRecord: 19;
			case EndChoiceRecord: 20;
			case ExtensionDataRecord: 21;
			case EndExtensionRecord: 22;
			case CoreFeatureRecord: 23;
			case AirPaletteRecord: 24;
			case CompleteChunkCoverage: 25;
			case SinglePlayerSpawn: 26;
		};

	static function limitKindCode(value:ScenarioLimitKind):Int
		return switch value {
			case FileBytes: 1;
			case LogicalRecords: 2;
			case TextScalars: 3;
			case WorldWidth: 4;
			case WorldHeight: 5;
			case WorldDepth: 6;
			case WorldCells: 7;
			case PaletteEntries: 8;
			case Objects: 9;
			case ObjectTags: 10;
			case Dialogues: 11;
			case DialogueLines: 12;
			case Locales: 20;
			case MessagesPerLocale: 21;
			case Objectives: 13;
			case Routes: 14;
			case Sequences: 15;
			case Variables: 16;
			case Rules: 17;
			case RuleActions: 18;
			case SequenceCallDepth: 19;
		};

	static function persistenceCode(value:PersistenceStage):Int
		return switch (value) {
			case CreateTemporary: 1;
			case WriteTemporary: 2;
			case FlushTemporary: 3;
			case ReplaceDestination: 4;
			case CleanupTemporary: 5;
		};
}
