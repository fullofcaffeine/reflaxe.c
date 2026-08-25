package caxecraft.scenario;

import caxecraft.scenario.CaxeFlow.FlowScope;
import caxecraft.scenario.CaxeFlow.FlowValue;
import caxecraft.scenario.CaxeFlowRuntime.FlowPosition;
import caxecraft.scenario.CaxeFlowSnapshot.CaxeFlowStateSnapshot;
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
	final scenario:Scenario;
	final registry:ScenarioContentRegistry;
	final variables:Array<RuntimeVariable> = [];
	final objects:Array<RuntimeObject> = [];
	final inventory:Array<RuntimeInventoryStack> = [];
	final objectives:Array<RuntimeObjective> = [];
	final journal:Array<ScenarioId> = [];
	var checkpoint:Null<ScenarioId>;

	public function new(scenario:Scenario, registry:ScenarioContentRegistry) {
		this.scenario = scenario;
		this.registry = registry;
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

	/**
		Validate one position sample without changing live predicate state.

		The executor calls this before it commits the tick boundary. Duplicate IDs
		are rejected because their order would otherwise decide which position wins.
	**/
	public function validatePositions(values:Array<FlowPosition>):Null<ScenarioId> {
		if (values.length > ScenarioLimits.MAX_OBJECTS)
			return values[ScenarioLimits.MAX_OBJECTS].objectId;
		for (index in 0...values.length) {
			final value = values[index];
			if (findObject(value.objectId) == null || !positionIsInsideWorld(value.xMilli, value.yMilli, value.zMilli))
				return value.objectId;
			for (earlier in 0...index)
				if (sameScenarioId(values[earlier].objectId, value.objectId))
					return value.objectId;
		}
		return null;
	}

	/** Apply one previously validated position sample atomically. */
	public function updatePositions(values:Array<FlowPosition>):Null<ScenarioId> {
		final invalid = validatePositions(values);
		if (invalid != null)
			return invalid;
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

	public function setInventory(owner:ScenarioId, itemType:ContentId, quantity:Int):Bool {
		final maximum = registry.maximumItemQuantity(itemType);
		if (findScenarioObject(owner) == null || maximum <= 0 || quantity < 0 || quantity > maximum)
			return false;
		final entry = findInventory(owner, itemType);
		if (entry == null) {
			inventory.push(new RuntimeInventoryStack(owner, itemType, quantity));
		} else {
			entry.quantity = quantity;
		}
		return true;
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
		if (object == null || object.state == null || !objectStateIsAllowed(id, value))
			return false;
		object.state = value;
		return true;
	}

	public function objectiveState(id:ScenarioId):Null<ObjectiveState> {
		final objective = findObjective(id);
		return objective == null ? null : objective.state;
	}

	/**
		Select the first active objective in the order authored by the scenario.

		Several objectives may be active at once. Returning one deterministic ID lets
		presentation show a compact current objective without reconstructing state from
		the order of change events. Null means that no objective is currently active.
	**/
	public function activeObjectiveId():Null<ScenarioId> {
		for (objective in objectives)
			if (objective.state == Active)
				return objective.id;
		return null;
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

	/** Return a copy-owned account of every mutable predicate and action fact. */
	public function snapshot():CaxeFlowStateSnapshot
		return {
			variables: [for (entry in variables) {id: entry.id, value: entry.value}],
			objects: [
				for (entry in objects)
					{
						id: entry.id,
						active: entry.active,
						state: entry.state,
						hasPosition: entry.hasPosition,
						xMilli: entry.xMilli,
						yMilli: entry.yMilli,
						zMilli: entry.zMilli
					}
			],
			inventory: [
				for (entry in inventory)
					{owner: entry.owner, itemType: entry.itemType, quantity: entry.quantity}
			],
			objectives: [for (entry in objectives) {id: entry.id, state: entry.state}],
			journal: journal.copy(),
			checkpoint: checkpoint
		};

	/**
		Validate and atomically restore one snapshot for this exact scenario.

		Array order and semantic identities must match canonical scenario order. An
		invalid value leaves the freshly constructed state unchanged.
	**/
	public function restore(snapshot:CaxeFlowStateSnapshot):Bool {
		if (!snapshotIsValid(snapshot))
			return false;
		for (index in 0...variables.length)
			variables[index].value = snapshot.variables[index].value;
		for (index in 0...objects.length) {
			final source = snapshot.objects[index];
			final target = objects[index];
			target.active = source.active;
			target.state = source.state;
			target.hasPosition = source.hasPosition;
			target.xMilli = source.xMilli;
			target.yMilli = source.yMilli;
			target.zMilli = source.zMilli;
		}
		inventory.resize(0);
		for (entry in snapshot.inventory)
			inventory.push(new RuntimeInventoryStack(entry.owner, entry.itemType, entry.quantity));
		for (index in 0...objectives.length)
			objectives[index].state = snapshot.objectives[index].state;
		journal.resize(0);
		for (id in snapshot.journal)
			journal.push(id);
		checkpoint = snapshot.checkpoint;
		return true;
	}

	public function objectsAreNear(actor:ScenarioId, target:ScenarioId, maximumMilliBlocks:Int):Bool {
		final left = findObject(actor);
		final right = findObject(target);
		if (left == null || right == null || !left.active || !right.active || !left.hasPosition || !right.hasPosition)
			return false;
		final dx:Float = (left.xMilli : Float) - right.xMilli;
		final dy:Float = (left.yMilli : Float) - right.yMilli;
		final dz:Float = (left.zMilli : Float) - right.zMilli;
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

	/** Check all snapshot fields before `restore` mutates one runtime value. */
	function snapshotIsValid(snapshot:CaxeFlowStateSnapshot):Bool {
		if (snapshot.variables.length != variables.length
			|| snapshot.objects.length != objects.length
			|| snapshot.objectives.length != objectives.length
			|| snapshot.inventory.length > ScenarioLimits.MAX_OBJECTS
			|| snapshot.journal.length > scenario.story.journal.length)
			return false;
		for (index in 0...variables.length)
			if (!sameScenarioId(snapshot.variables[index].id, variables[index].id)
				|| !sameValueKind(snapshot.variables[index].value, variables[index].value))
				return false;
		for (index in 0...objects.length) {
			final saved = snapshot.objects[index];
			final original = scenario.objects[index];
			if (!sameScenarioId(saved.id, objects[index].id))
				return false;
			final expectsPosition = switch original.placement {
				case TriggerZone(_): false;
				case _: true;
			};
			final expectsState = switch original.placement {
				case StatefulObject(_, _, _): true;
				case _: false;
			};
			if (saved.hasPosition != expectsPosition
				|| (!saved.hasPosition && (saved.xMilli != 0 || saved.yMilli != 0 || saved.zMilli != 0))
				|| (saved.hasPosition && !positionIsInsideWorld(saved.xMilli, saved.yMilli, saved.zMilli))
				|| (saved.state != null) != expectsState || (saved.state != null && !objectStateIsAllowed(saved.id, saved.state)))
				return false;
		}
		for (index in 0...objectives.length)
			if (!sameScenarioId(snapshot.objectives[index].id, objectives[index].id))
				return false;
		for (index in 0...snapshot.inventory.length) {
			final entry = snapshot.inventory[index];
			final maximum = registry.maximumItemQuantity(entry.itemType);
			if (maximum <= 0 || entry.quantity < 0 || entry.quantity > maximum || findScenarioObject(entry.owner) == null)
				return false;
			for (earlier in 0...index)
				if (sameScenarioId(snapshot.inventory[earlier].owner, entry.owner)
					&& snapshot.inventory[earlier].itemType.text() == entry.itemType.text())
					return false;
		}
		for (index in 0...snapshot.journal.length) {
			if (!scenarioHasJournal(snapshot.journal[index]))
				return false;
			for (earlier in 0...index)
				if (sameScenarioId(snapshot.journal[earlier], snapshot.journal[index]))
					return false;
		}
		return snapshot.checkpoint == null || scenarioHasCheckpoint(snapshot.checkpoint);
	}

	/** Return the exact authored object, or null for a stale save identity. */
	function findScenarioObject(id:ScenarioId):Null<ScenarioObject> {
		for (object in scenario.objects)
			if (sameScenarioId(object.id, id))
				return object;
		return null;
	}

	/** Check one state against the content type owned by its authored object. */
	function objectStateIsAllowed(id:ScenarioId, state:ContentId):Bool {
		final object = findScenarioObject(id);
		if (object == null)
			return false;
		return switch object.placement {
			case StatefulObject(objectType, _, _): registry.statefulObjectHasState(objectType, state);
			case _: false;
		};
	}

	/** Keep runtime and restored positions inside the validated level volume. */
	function positionIsInsideWorld(xMilli:Int, yMilli:Int, zMilli:Int):Bool
		return xMilli >= 0
			&& yMilli >= 0
			&& zMilli >= 0
			&& xMilli < scenario.world.size.width * 1000
			&& yMilli < scenario.world.size.height * 1000
			&& zMilli < scenario.world.size.depth * 1000;

	/** True only for a journal entry authored by this scenario. */
	function scenarioHasJournal(id:ScenarioId):Bool {
		for (entry in scenario.story.journal)
			if (sameScenarioId(entry.id, id))
				return true;
		return false;
	}

	/** Check that a saved checkpoint still names a checkpoint placement. */
	function scenarioHasCheckpoint(id:ScenarioId):Bool {
		final object = findScenarioObject(id);
		if (object == null)
			return false;
		return switch object.placement {
			case Checkpoint(_): true;
			case _: false;
		};
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

	/** Preserve each variable's declared closed value kind across reload. */
	static function sameValueKind(left:FlowValue, right:FlowValue):Bool
		return switch [left, right] {
			case [Flag(_), Flag(_)] | [Counter(_), Counter(_)] | [State(_), State(_)]: true;
			case _: false;
		};

	static inline function sameScenarioId(left:ScenarioId, right:ScenarioId):Bool
		return left.text() == right.text();
}
