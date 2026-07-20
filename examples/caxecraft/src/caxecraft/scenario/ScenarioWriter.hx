package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowAction;
import caxecraft.scenario.CaxeFlow.FlowArgument;
import caxecraft.scenario.CaxeFlow.FlowComparison;
import caxecraft.scenario.CaxeFlow.FlowEvent;
import caxecraft.scenario.CaxeFlow.FlowPredicate;
import caxecraft.scenario.CaxeFlow.FlowRepeatPolicy;
import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.Scenario.ScenarioMode;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioStory.ObjectiveState;
import haxe.io.Bytes;

/** Canonical, deterministic CAXEMAP 1 serializer. */
final class ScenarioWriter {
	public static function write(scenario:Scenario):Bytes {
		final lines:Array<String> = ["CAXEMAP 1"];
		final required = scenario.requiredFeatures.copy();
		final optional = scenario.optionalFeatures.copy();
		required.sort((left, right) -> compareUtf8(left.text(), right.text()));
		optional.sort((left, right) -> compareUtf8(left.text(), right.text()));
		for (feature in required)
			lines.push('feature required ${feature.text()}');
		for (feature in optional)
			lines.push('feature optional ${feature.text()}');
		lines.push('map ${scenario.id.text()}');
		lines.push('asset-pack ${scenario.assetPack.text()}');
		lines.push('title ${text(scenario.title)}');
		lines.push('mode ${mode(scenario.mode)}');
		lines.push('world ${scenario.world.size.width} ${scenario.world.size.height} ${scenario.world.size.depth}');

		final palette = scenario.world.palette.copy();
		palette.sort((left, right) -> left.code - right.code);
		for (entry in palette)
			lines.push('palette ${entry.code} ${entry.blockType.text()}');
		final chunks = scenario.world.chunks.copy();
		chunks.sort((left, right) -> {
			if (left.origin.z != right.origin.z)
				return left.origin.z < right.origin.z ? -1 : 1;
			if (left.origin.y != right.origin.y)
				return left.origin.y < right.origin.y ? -1 : 1;
			if (left.origin.x != right.origin.x)
				return left.origin.x < right.origin.x ? -1 : 1;
			return compareUtf8(left.id.text(), right.id.text());
		});
		for (chunk in chunks) {
			lines.push('chunk ${chunk.id.text()} ${chunk.origin.x} ${chunk.origin.y} ${chunk.origin.z} ${chunk.size.width} ${chunk.size.height} ${chunk.size.depth}');
			for (run in chunk.runs)
				lines.push('  run ${run.paletteCode} ${run.count}');
			lines.push("end chunk");
		}

		final objects = scenario.objects.copy();
		objects.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (object in objects) {
			lines.push('object ${object.id.text()}');
			for (tag in object.tags)
				lines.push('  tag ${tag.text()}');
			lines.push('  placement ${placement(object.placement)}');
			lines.push("end object");
		}

		final dialogues = scenario.story.dialogues.copy();
		dialogues.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (dialogue in dialogues) {
			lines.push('dialogue ${dialogue.id.text()}');
			for (entry in dialogue.lines)
				lines.push('  line ${entry.speaker == null ? "narrator" : entry.speaker.text()} ${text(entry.text)}');
			lines.push("end dialogue");
		}

		final journal = scenario.story.journal.copy();
		journal.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (entry in journal) {
			lines.push('journal ${entry.id.text()} title ${text(entry.title)}');
			lines.push('  body ${text(entry.body)}');
			lines.push("end journal");
		}
		final objectives = scenario.story.objectives.copy();
		objectives.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (objective in objectives) {
			lines.push('objective ${objective.id.text()} ${objectiveState(objective.initialState)} title ${text(objective.title)}');
			lines.push('  body ${text(objective.body)}');
			lines.push("end objective");
		}
		final routes = scenario.story.routes.copy();
		routes.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (route in routes) {
			lines.push('route ${route.id.text()} title ${text(route.title)}');
			for (objective in route.objectives)
				lines.push('  objective ${objective.text()}');
			lines.push("end route");
		}

		final variables = scenario.flow.variables.copy();
		variables.sort((left, right) -> {
			final scopeOrder = scopeCompare(left.scope, right.scope);
			return scopeOrder == 0 ? compareUtf8(left.id.text(), right.id.text()) : scopeOrder;
		});
		for (variable in variables)
			lines.push('variable ${variable.id.text()} ${scope(variable.scope)} ${flowValue(variable.initial)}');

		final sequences = scenario.flow.sequences.copy();
		sequences.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (sequence in sequences) {
			lines.push('sequence ${sequence.id.text()}');
			for (parameter in sequence.parameters)
				lines.push('  parameter ${parameter.id.text()} ${flowValue(parameter.initial)}');
			for (entry in sequence.actions)
				appendAction(lines, "  do ", entry);
			lines.push("end sequence");
		}

		final rules = scenario.flow.rules.copy();
		rules.sort((left, right) -> compareUtf8(left.id.text(), right.id.text()));
		for (rule in rules) {
			lines.push('rule ${rule.id.text()} priority ${rule.priority} ${repeat(rule.repeat)}');
			lines.push('  when ${event(rule.event)}');
			lines.push('  if ${predicate(rule.predicate)}');
			for (entry in rule.actions)
				appendAction(lines, "  do ", entry);
			lines.push("end rule");
		}
		final extensions = scenario.extensions.copy();
		extensions.sort((left, right) -> {
			final featureOrder = compareUtf8(left.feature.text(), right.feature.text());
			return featureOrder == 0 ? compareUtf8(left.id.text(), right.id.text()) : featureOrder;
		});
		for (extension in extensions) {
			lines.push('extension ${extension.feature.text()} ${extension.id.text()}');
			lines.push('  data ${quoted(extension.data)}');
			lines.push("end extension");
		}
		lines.push("end-map");
		return Bytes.ofString(lines.join("\n") + "\n");
	}

	static function placement(value:ObjectPlacement):String {
		return switch value {
			case PlayerSpawn(position): 'player-spawn ${transform(position)}';
			case Checkpoint(position): 'checkpoint ${transform(position)}';
			case Item(itemType, quantity, position): 'item ${itemType.text()} $quantity ${transform(position)}';
			case Entity(entityType, position): 'entity ${entityType.text()} ${transform(position)}';
			case Npc(npcType, dialogue, position): 'npc ${npcType.text()} ${dialogue.text()} ${transform(position)}';
			case Prefab(prefabType, position): 'prefab ${prefabType.text()} ${transform(position)}';
			case TriggerZone(bounds):
				'trigger-zone ${bounds.origin.x} ${bounds.origin.y} ${bounds.origin.z} ${bounds.size.width} ${bounds.size.height} ${bounds.size.depth}';
			case StatefulObject(objectType, state, position): 'stateful ${objectType.text()} ${state.text()} ${transform(position)}';
		}
	}

	static function transform(value:caxecraft.scenario.ScenarioGeometry.ScenarioTransform):String
		return '${value.xMilli} ${value.yMilli} ${value.zMilli} ${value.yawDegrees}';

	static function text(value:ScenarioText):String {
		return switch value {
			case Message(id): 'message ${id.text()}';
			case Literal(value): 'literal ${quoted(value)}';
		}
	}

	static function scope(value:FlowScope):String {
		return switch value {
			case Map: "map";
			case Player: "player";
			case Quest: "quest";
			case Local(sequence): 'local ${sequence.text()}';
		}
	}

	static function scopeCompare(left:FlowScope, right:FlowScope):Int {
		final leftRank = scopeRank(left);
		final rightRank = scopeRank(right);
		if (leftRank != rightRank)
			return leftRank - rightRank;
		return switch [left, right] {
			case [Local(leftId), Local(rightId)]: compareUtf8(leftId.text(), rightId.text());
			case _: 0;
		}
	}

	static function scopeRank(value:FlowScope):Int
		return switch value {
			case Map: 0;
			case Player: 1;
			case Quest: 2;
			case Local(_): 3;
		}

	static function flowValue(value:FlowValue):String {
		return switch value {
			case Flag(value): 'flag $value';
			case Counter(value): 'counter $value';
			case State(value): 'state ${value.text()}';
		}
	}

	static function repeat(value:FlowRepeatPolicy):String {
		return switch value {
			case Once: "once";
			case Repeat: "repeat";
			case Cooldown(ticks): 'cooldown $ticks';
		}
	}

	static function mode(value:ScenarioMode):String
		return switch value {
			case Creative: "creative";
			case Adventure: "adventure";
		}

	static function objectiveState(value:ObjectiveState):String {
		return switch value {
			case Hidden: "hidden";
			case Active: "active";
			case Complete: "complete";
			case Failed: "failed";
		}
	}

	static function comparison(value:FlowComparison):String {
		return switch value {
			case Equal: "equal";
			case NotEqual: "not-equal";
			case Less: "less";
			case LessOrEqual: "less-or-equal";
			case Greater: "greater";
			case GreaterOrEqual: "greater-or-equal";
		}
	}

	static function event(value:FlowEvent):String {
		return switch value {
			case EnterZone(id): 'enter-zone ${id.text()}';
			case LeaveZone(id): 'leave-zone ${id.text()}';
			case Interact(id): 'interact ${id.text()}';
			case BlockChanged(zone, block): 'block-changed ${zone.text()} ${block.text()}';
			case UseItem(item): 'use-item ${item.text()}';
			case EntityDefeated(entity): 'entity-defeated ${entity.text()}';
			case SignalReceived(signal): 'signal ${signal.text()}';
			case TimerExpired(timer): 'timer ${timer.text()}';
			case ObjectiveChanged(objective): 'objective-changed ${objective.text()}';
			case StateChanged(variable): 'state-changed ${variable.text()}';
		}
	}

	static function predicate(value:FlowPredicate):String {
		return switch value {
			case Always: "(always)";
			case All(children): '(all ${[for (child in children) predicate(child)].join(" ")})';
			case AnyOf(children): '(any ${[for (child in children) predicate(child)].join(" ")})';
			case Not(child): '(not ${predicate(child)})';
			case FlagIs(variable, expected): '(flag ${variable.text()} $expected)';
			case CounterCompare(variable, operation, expected): '(counter ${variable.text()} ${comparison(operation)} $expected)';
			case StateIs(variable, expected): '(state ${variable.text()} ${expected.text()})';
			case ObjectStateIs(objectId, expected): '(object-state ${objectId.text()} ${expected.text()})';
			case InventoryHas(owner, item, operation, quantity):
				'(inventory-has ${owner.text()} ${item.text()} ${comparison(operation)} $quantity)';
			case ObjectiveIs(objective, expected): '(objective ${objective.text()} ${objectiveState(expected)})';
			case NearObject(actor, objectId, maximum): '(near ${actor.text()} ${objectId.text()} $maximum)';
			case ModeIs(value): '(mode ${mode(value)})';
		}
	}

	static function appendAction(lines:Array<String>, prefix:String, value:FlowAction):Void {
		switch value {
			case ChooseSeeded(seed, choices):
				lines.push(prefix + 'choose ${seed.text()} ${choices.length}');
				for (choice in choices) {
					lines.push('  choice weight ${choice.weight}');
					for (entry in choice.actions) {
						switch entry {
							case ChooseSeeded(_, _): throw "CAXEMAP 1 does not allow a choose action inside another choose action";
							case _: lines.push("    do " + action(entry));
						}
					}
					lines.push("  end choice");
				}
			case _:
				lines.push(prefix + action(value));
		}
	}

	static function action(value:FlowAction):String {
		return switch value {
			case ShowDialogue(id): 'dialogue ${id.text()}';
			case AddJournal(id): 'journal ${id.text()}';
			case SetFlag(id, value): 'set-flag ${id.text()} $value';
			case SetCounter(id, value): 'set-counter ${id.text()} $value';
			case AddCounter(id, value): 'add-counter ${id.text()} $value';
			case SetState(id, value): 'set-state ${id.text()} ${value.text()}';
			case GiveItem(owner, item, quantity): 'give-item ${owner.text()} ${item.text()} $quantity';
			case TakeItem(owner, item, quantity): 'take-item ${owner.text()} ${item.text()} $quantity';
			case Spawn(id): 'spawn ${id.text()}';
			case Despawn(id): 'despawn ${id.text()}';
			case SetObjectState(id, value): 'set-object-state ${id.text()} ${value.text()}';
			case SetCheckpoint(id): 'checkpoint ${id.text()}';
			case SetObjective(id, value): 'objective ${id.text()} ${objectiveState(value)}';
			case PlayEffect(effect, null): 'effect ${effect.text()}';
			case PlayEffect(effect, objectId): 'effect ${effect.text()} at ${objectId.text()}';
			case EmitSignal(signal): 'signal ${signal.text()}';
			case Schedule(timer, ticks, sequence, arguments):
				'schedule ${timer.text()} $ticks ${sequence.text()}${arguments.length == 0 ? "" : " " + [for (argument in arguments) argumentText(argument)].join(" ")}';
			case CallSequence(sequence, arguments):
				'call ${sequence.text()}${arguments.length == 0 ? "" : " " + [for (argument in arguments) argumentText(argument)].join(" ")}';
			case ChooseSeeded(_, _): throw "choose is emitted as a block";
		}
	}

	static function argumentText(value:FlowArgument):String {
		return switch value {
			case Value(value): 'value ${flowValue(value)}';
			case Variable(id): 'variable ${id.text()}';
		}
	}

	static function quoted(value:String):String {
		final output = new StringBuf();
		output.add('"');
		for (index in 0...value.length) {
			final code = value.charCodeAt(index);
			switch code {
				case 34:
					output.add('\\"');
				case 92:
					output.add("\\\\");
				case 10:
					output.add("\\n");
				case 13:
					output.add("\\r");
				case 9:
					output.add("\\t");
				case value if (value < 32 || value == 127):
					output.add('\\u{${hex(value)}}');
				case _:
					output.addChar(code);
			}
		}
		output.add('"');
		return output.toString();
	}

	static function hex(value:Int):String {
		final digits = "0123456789ABCDEF";
		var remaining = value;
		var result = "";
		do {
			result = digits.charAt(remaining & 15) + result;
			remaining = remaining >>> 4;
		} while (remaining != 0);
		return result;
	}

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final shared = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...shared) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}
