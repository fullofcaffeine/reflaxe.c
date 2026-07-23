package reflaxe.c.ir;

import reflaxe.c.ir.HxcIR;

/**
	Find every exact collector pointer stored inside one immutable HxcIR value.

	A direct managed class or Array contributes one empty path. Direct records,
	tagged enums, and tagged optionals contribute one path per embedded managed
	reference. The result is finite because ordinary by-value layouts must already
	be finite; recursive enum payloads use a pointer and are deliberately not
	followed here. A recursive owned graph that also contains collector references
	needs its own recursive trace owner and remains rejected by enum preparation.

	Both root planning and validation use this class. That prevents the validator
	from accepting a path that the planner would never produce, or vice versa.
**/
class HxcIRManagedRootPaths {
	final declarations:Map<String, HxcIRTypeDeclaration> = [];
	final instances:Map<String, HxcIRTypeInstance> = [];

	public function new(program:HxcIRProgram) {
		for (module in program.modules) {
			for (declaration in module.types)
				declarations.set(declaration.id, declaration);
			for (instance in module.typeInstances)
				instances.set(instance.id, instance);
		}
	}

	/** Return the stable, declaration-ordered managed paths for `type`. */
	public function collect(type:HxcIRTypeRef):Array<Array<HxcIRManagedRootProjection>> {
		final result:Array<Array<HxcIRManagedRootProjection>> = [];
		append(type, [], [], result);
		return result;
	}

	function append(type:HxcIRTypeRef, path:Array<HxcIRManagedRootProjection>, active:Map<String, Bool>, result:Array<Array<HxcIRManagedRootProjection>>):Void {
		if (isExactManagedPointer(type)) {
			result.push(path);
			return;
		}
		switch type {
			case IRTNullable(payload, IRNTagged):
				final nested = path.copy();
				nested.push(IRMRPNullablePayload);
				append(payload, nested, active, result);
			case IRTInstance(instanceId):
				if (active.exists(instanceId))
					return;
				final instance = instances.get(instanceId);
				if (instance == null)
					return;
				final declaration = declarations.get(instance.declarationId);
				if (declaration == null)
					return;
				final nestedActive = copySet(active);
				nestedActive.set(instanceId, true);
				switch declaration.kind {
					case IRTKAggregate(fields):
						for (field in fields) {
							final nested = path.copy();
							nested.push(IRMRPAggregateField(instanceId, field.name));
							append(field.type, nested, nestedActive, result);
						}
					case IRTKTaggedUnion(cases) if (instance.representation == IRRTagged):
						for (tagCase in cases)
							for (index => payload in tagCase.payload) {
								final nested = path.copy();
								nested.push(IRMRPTagPayload(instanceId, tagCase.name, index));
								append(payload.type, nested, nestedActive, result);
							}
					case IRTKPrimitive | IRTKTaggedUnion(_) | IRTKClass(_) | IRTKReference | IRTKFunction | IRTKExtern:
				}
			case _:
		}
	}

	function isExactManagedPointer(type:HxcIRTypeRef):Bool {
		final instanceId = switch type {
			case IRTPointer(IRTInstance(id), _): id;
			case IRTInstance(id): id;
			case _: return false;
		};
		final instance = instances.get(instanceId);
		return instance != null && switch instance.representation {
			case IRRManaged("gc"): true;
			case _: false;
		};
	}

	static function copySet(source:Map<String, Bool>):Map<String, Bool> {
		final result:Map<String, Bool> = [];
		for (key in source.keys())
			result.set(key, true);
		return result;
	}

	/** Canonical text used for duplicate checks, dumps, and focused assertions. */
	public static function key(projections:Array<HxcIRManagedRootProjection>):String
		return projections.map(projectionKey).join("/");

	public static function projectionKey(projection:HxcIRManagedRootProjection):String
		return switch projection {
			case IRMRPAggregateField(instanceId, fieldName): 'field($instanceId,$fieldName)';
			case IRMRPTagPayload(instanceId, tagName, payloadIndex): 'tag($instanceId,$tagName,$payloadIndex)';
			case IRMRPNullablePayload: "nullable-payload";
		};
}
