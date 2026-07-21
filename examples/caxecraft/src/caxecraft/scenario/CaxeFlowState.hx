package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowRuntime.FlowPosition;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.scenario.ScenarioStory.ObjectiveState;

private final class RuntimeVariable {
	public final id:ScenarioId;
	public var value:FlowValue;

	public function new(id:ScenarioId, value:FlowValue) {
		this.id = id;
		this.value = value;
	}
}

private final class RuntimeObject {
	public final id:ScenarioId;
	public var active:Bool;
	public var state:Null<ContentId>;
	public var hasPosition:Bool;
	public var xMilli:Int;
	public var yMilli:Int;
	public var zMilli:Int;

	public function new(id:ScenarioId, active:Bool, state:Null<ContentId>) {
		this.id = id;
		this.active = active;
		this.state = state;
		this.hasPosition = false;
		this.xMilli = 0;
		this.yMilli = 0;
		this.zMilli = 0;
	}
}

private final class RuntimeInventoryStack {
	public final owner:ScenarioId;
	public final itemType:ContentId;
	public var quantity:Int;

	public function new(owner:ScenarioId, itemType:ContentId, quantity:Int) {
		this.owner = owner;
		this.itemType = itemType;
		this.quantity = quantity;
	}
}

private final class RuntimeObjective {
	public final id:ScenarioId;
	public var state:ObjectiveState;

	public function new(id:ScenarioId, state:ObjectiveState) {
		this.id = id;
		this.state = state;
	}
}

/**
	Mutable scenario facts owned by one CaxeFlow executor.

	The class is an implementation detail rather than a second public gameplay
	API. `@:noCompletion` hides it from editor suggestions; it does not provide
	runtime privacy or weaken Haxe's type checks.
**/
@:noCompletion
final class CaxeFlowState {
	final variables:Array<RuntimeVariable> = [];
	final objects:Array<RuntimeObject> = [];
	final inventory:Array<RuntimeInventoryStack> = [];
	final objectives:Array<RuntimeObjective> = [];
	final journal:Array<ScenarioId> = [];
	var checkpoint:Null<ScenarioId>;

	public function new(scenario:Scenario) {
		for (variable in scenario.flow.variables)
			switch variable.scope {
				case Local(_):
				case Map | Player | Quest:
					variables.push(new RuntimeVariable(variable.id, variable.initial));
			}
		for (source in scenario.objects) {
			final initialState:Null<ContentId> = switch source.placement {
				case StatefulObject(_, value, _): value;
				case _: null;
			}
			final object = new RuntimeObject(source.id, true, initialState);
			switch source.placement {
				case PlayerSpawn(transform) | Checkpoint(transform) | Item(_, _, transform) | Entity(_, transform) | Npc(_, _, transform) |
					Prefab(_, transform) | StatefulObject(_, _, transform):
					setObjectPosition(object, transform.xMilli, transform.yMilli, transform.zMilli);
				case TriggerZone(_):
			}
			objects.push(object);
		}
		for (objective in scenario.story.objectives)
			objectives.push(new RuntimeObjective(objective.id, objective.initialState));
		checkpoint = null;
	}

	/** Apply the complete position sample and return the first unknown object. */
	public function updatePositions(values:Array<FlowPosition>):Null<ScenarioId> {
		for (value in values) {
			final object = findObject(value.objectId);
			if (object == null)
				return value.objectId;
			setObjectPosition(object, value.xMilli, value.yMilli, value.zMilli);
		}
		return null;
	}

	public function variable(id:ScenarioId):Null<FlowValue> {
		final entry = findVariable(id);
		return entry == null ? null : entry.value;
	}

	public function setVariable(id:ScenarioId, value:FlowValue):Bool {
		final entry = findVariable(id);
		if (entry == null)
			return false;
		entry.value = value;
		return true;
	}

	public function inventoryQuantity(owner:ScenarioId, itemType:ContentId):Int {
		final entry = findInventory(owner, itemType);
		return entry == null ? 0 : entry.quantity;
	}

	public function setInventory(owner:ScenarioId, itemType:ContentId, quantity:Int):Int {
		final normalized = quantity < 0 ? 0 : quantity;
		final entry = findInventory(owner, itemType);
		if (entry == null) {
			inventory.push(new RuntimeInventoryStack(owner, itemType, normalized));
		} else {
			entry.quantity = normalized;
		}
		return normalized;
	}

	public function objectActive(id:ScenarioId):Bool {
		final object = findObject(id);
		return object != null && object.active;
	}

	public function setObjectActive(id:ScenarioId, active:Bool):Bool {
		final object = findObject(id);
		if (object == null)
			return false;
		object.active = active;
		return true;
	}

	public function objectState(id:ScenarioId):Null<ContentId> {
		final object = findObject(id);
		return object == null ? null : object.state;
	}

	public function setObjectState(id:ScenarioId, value:ContentId):Bool {
		final object = findObject(id);
		if (object == null)
			return false;
		object.state = value;
		return true;
	}

	public function objectiveState(id:ScenarioId):Null<ObjectiveState> {
		final objective = findObjective(id);
		return objective == null ? null : objective.state;
	}

	public function setObjectiveState(id:ScenarioId, value:ObjectiveState):Bool {
		final objective = findObjective(id);
		if (objective == null)
			return false;
		objective.state = value;
		return true;
	}

	public function addJournal(id:ScenarioId):Bool {
		if (containsScenarioId(journal, id))
			return false;
		journal.push(id);
		return true;
	}

	public function hasJournal(id:ScenarioId):Bool
		return containsScenarioId(journal, id);

	public function setCheckpoint(id:ScenarioId):Void
		checkpoint = id;

	public function currentCheckpoint():Null<ScenarioId>
		return checkpoint;

	public function objectsAreNear(actor:ScenarioId, target:ScenarioId, maximumMilliBlocks:Int):Bool {
		final left = findObject(actor);
		final right = findObject(target);
		if (left == null || right == null || !left.active || !right.active || !left.hasPosition || !right.hasPosition)
			return false;
		final dx:Float = left.xMilli - right.xMilli;
		final dy:Float = left.yMilli - right.yMilli;
		final dz:Float = left.zMilli - right.zMilli;
		final maximum:Float = maximumMilliBlocks;
		return dx * dx + dy * dy + dz * dz <= maximum * maximum;
	}

	function findVariable(id:ScenarioId):Null<RuntimeVariable> {
		for (entry in variables)
			if (sameScenarioId(entry.id, id))
				return entry;
		return null;
	}

	function findObject(id:ScenarioId):Null<RuntimeObject> {
		for (entry in objects)
			if (sameScenarioId(entry.id, id))
				return entry;
		return null;
	}

	function findInventory(owner:ScenarioId, itemType:ContentId):Null<RuntimeInventoryStack> {
		for (entry in inventory)
			if (sameScenarioId(entry.owner, owner) && entry.itemType.text() == itemType.text())
				return entry;
		return null;
	}

	function findObjective(id:ScenarioId):Null<RuntimeObjective> {
		for (entry in objectives)
			if (sameScenarioId(entry.id, id))
				return entry;
		return null;
	}

	static function setObjectPosition(object:RuntimeObject, xMilli:Int, yMilli:Int, zMilli:Int):Void {
		object.hasPosition = true;
		object.xMilli = xMilli;
		object.yMilli = yMilli;
		object.zMilli = zMilli;
	}

	static function containsScenarioId(values:Array<ScenarioId>, id:ScenarioId):Bool {
		for (value in values)
			if (sameScenarioId(value, id))
				return true;
		return false;
	}

	static inline function sameScenarioId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();
}
