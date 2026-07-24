enum Mode {
	Off;
	On;
}

enum Option<T> {
	None;
	Some(value:T);
}

enum Chain<T> {
	End(value:T);
	Link(value:T, next:Chain<T>);
}

enum Choices {
	NoChoices;
	ChoiceValues(values:Array<Int>);
}

/** One FlowRule-shaped value used to prove composed record ownership. */
typedef Rule = {
	final chain:Chain<Int>;
	final choices:Choices;
	final actions:Array<Int>;
}

/** A tagged enum whose active payload owns a managed closed record. */
enum RuleEnvelope {
	MissingRule;
	WrappedRule(rule:Rule);
}

class EnumFixture {
	static function identity(value:Int):Int {
		return value;
	}

	static function identityMode(value:Mode):Mode {
		return value;
	}

	static function modeEquality():Bool {
		// Calls on both sides keep operand order visible in HxcIR. Enum equality
		// deliberately reuses the same left-to-right sequencing as other binaries.
		final same = identityMode(On) == identityMode(On);
		final different = identityMode(Off) != identityMode(On);
		return same && different;
	}

	static function modeValue(value:Mode):Int {
		return switch value {
			case Off: 0;
			case On: 1;
		};
	}

	static function optionValue(value:Option<Int>):Int {
		return switch value {
			case None: 0;
			case Some(payload): payload;
		};
	}

	static function applyOption(value:Int, constructor:Int->Option<Int>):Option<Int>
		return constructor(value);

	static function constructorValue():Int
		return optionValue(applyOption(9, Some));

	static function guardedValue(value:Option<Int>):Int {
		return switch value {
			case Some(payload) if (payload > 4): payload;
			case Some(payload): payload + 1;
			case None: -1;
		};
	}

	static function boolOptionValue(value:Option<Bool>):Int {
		return switch value {
			case None: -1;
			case Some(payload): payload ? 1 : 0;
		};
	}

	static function recursiveLocal():Int {
		var tail:Chain<Int> = End(2);
		var head:Chain<Int> = Link(1, tail);
		return switch head {
			case End(value): value;
			case Link(value, next):
				switch next {
					case End(last): value + last;
					case Link(_, _): 0;
				};
		};
	}

	static function makeRule(chain:Chain<Int>, choices:Choices, actions:Array<Int>):Rule
		return {chain: chain, choices: choices, actions: actions};

	static function copyRule(value:Rule):Rule
		return value;

	static function wrapRule(value:Rule):RuleEnvelope
		return WrappedRule(value);

	static function copyEnvelope(value:RuleEnvelope):RuleEnvelope
		return value;

	static function chainValue(value:Chain<Int>):Int
		return switch value {
			case End(item): item;
			case Link(item, next): item + tailValue(next);
		};

	static function tailValue(value:Chain<Int>):Int
		return switch value {
			case End(item): item;
			case Link(_, _): 0;
		};

	static function choiceValue(value:Choices):Int
		return switch value {
			case NoChoices: 0;
			case ChoiceValues(items): items[0];
		};

	static function ruleValue(value:Rule):Int
		return chainValue(value.chain) + choiceValue(value.choices) + value.actions[0];

	static function envelopeValue(value:RuleEnvelope):Int
		return switch value {
			case MissingRule: 0;
			case WrappedRule(rule): ruleValue(rule);
		};

	static function optionalRuleValue(value:Option<Rule>):Int
		return switch value {
			case None: 0;
			case Some(rule): ruleValue(rule);
		};

	/**
		Build an Array from one fresh managed record and one borrowed record.

		The literal must give each slot its own managed-field ownership. The fresh
		record already owns its fields; `borrowed` remains owned by its caller and
		therefore needs an independent retained copy inside the Array.
	**/
	static function ruleLiteralValue(chain:Chain<Int>, choices:Choices, actions:Array<Int>, borrowed:Rule):Int {
		final rules:Array<Rule> = [{chain: chain, choices: choices, actions: actions}, borrowed];
		// Array destruction still visits both managed records; the length keeps
		// this fixture focused on construction ownership rather than the separate
		// borrowed-managed-element call boundary.
		return rules.length + ruleValue(borrowed);
	}

	static function main():Void {
		var mode = On;
		var present:Option<Int> = Some(identity(7));
		var absent:Option<Int> = None;
		var truth:Option<Bool> = Some(true);
		var choices = [3];
		var actions = [4];
		var rule = makeRule(Link(1, End(2)), ChoiceValues(choices), actions);
		var copiedRule = copyRule(rule);
		var envelope = wrapRule(copiedRule);
		var copiedEnvelope = copyEnvelope(envelope);
		var optionalRule:Option<Rule> = Some(copiedRule);
		var rules:Array<Rule> = [];
		rules.push(copiedRule);
		var envelopes:Array<RuleEnvelope> = [];
		envelopes.push(copiedEnvelope);
		while (!(modeValue(mode) == 1
			&& modeEquality()
			&& optionValue(present) == 7
			&& optionValue(absent) == 0
			&& constructorValue() == 9
			&& guardedValue(present) == 7
			&& boolOptionValue(truth) == 1
			&& recursiveLocal() == 3
			&& ruleValue(copiedRule) == 10
			&& envelopeValue(copiedEnvelope) == 10
			&& optionalRuleValue(optionalRule) == 10
			&& ruleLiteralValue(Link(1, End(2)), ChoiceValues(choices), actions, copiedRule) == 12
			&& envelopes.length == 1
			&& rules.length == 1)) {}
	}
}
