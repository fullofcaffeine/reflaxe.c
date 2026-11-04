package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.CaxeFlow;
import caxecraft.scenario.CaxeFlow.FlowAction;

/**
	Connects executor-produced event identities to their authored action owners.

	Timers and campaign exits are not free-floating engine callbacks. A matching
	`Schedule` or `RequestCampaignExit` action must own each source identity. The
	validator and restored-event boundary share this traversal so they cannot
	disagree about which identities can exist.
**/
/** True when any rule or reusable sequence can schedule the named timer. */
function flowOwnsTimer(flow:CaxeFlow, timer:ScenarioId):Bool {
	for (sequence in flow.sequences)
		if (actionsOwnTimer(sequence.actions, timer))
			return true;
	for (rule in flow.rules)
		if (actionsOwnTimer(rule.actions, timer))
			return true;
	return false;
}

/** True when any rule or reusable sequence can request the named campaign exit. */
function flowOwnsCampaignExit(flow:CaxeFlow, exit:ScenarioId):Bool {
	for (sequence in flow.sequences)
		if (actionsOwnCampaignExit(sequence.actions, exit))
			return true;
	for (rule in flow.rules)
		if (actionsOwnCampaignExit(rule.actions, exit))
			return true;
	return false;
}

/** Search ordered and choice-nested actions for one timer owner. */
private function actionsOwnTimer(actions:Array<FlowAction>, timer:ScenarioId):Bool {
	for (action in actions)
		switch action {
			case Schedule(candidate, _, _, _) if (candidate.text() == timer.text()):
				return true;
			case ChooseSeeded(_, choices):
				for (choice in choices)
					if (actionsOwnTimer(choice.actions, timer))
						return true;
			case _:
		}
	return false;
}

/** Search ordered and choice-nested actions for one campaign-exit owner. */
private function actionsOwnCampaignExit(actions:Array<FlowAction>, exit:ScenarioId):Bool {
	for (action in actions)
		switch action {
			case RequestCampaignExit(candidate) if (candidate.text() == exit.text()):
				return true;
			case ChooseSeeded(_, choices):
				for (choice in choices)
					if (actionsOwnCampaignExit(choice.actions, exit))
						return true;
			case _:
		}
	return false;
}
