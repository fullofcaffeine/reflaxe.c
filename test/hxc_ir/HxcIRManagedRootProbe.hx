import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;

/**
	Protect exact collector paths and diagnostics independently of C emission.

	A nested record stores two managed pointers and one optional pointer. Each
	pointer needs its own root. Invalid duplicate fields deliberately produce
	the same expected path twice, so completeness checks must retain multiplicity.
**/
class HxcIRManagedRootProbe {
	static final source = new HxcSourceSpan("test/hxc_ir/fixtures/ManagedRoots.hx", 1, 1, 1, 2);
	static final nestedA = "field(outer,nested)/field(inner,a)";
	static final nestedB = "field(outer,nested)/field(inner,b)";
	static final optional = "field(outer,optional)/nullable-payload/field(wrapper,pointer)";

	/** Compare complete ordered diagnostics, including reuse across functions and requests. */
	static function main():Void {
		final validator = new HxcIRValidator();
		check(validator, "nested-valid", [fn("one", IRTInstance("outer"), roots())], []);
		check(validator, "missing", [fn("one", IRTInstance("outer"), [])], missing("one", [nestedA, nestedB, optional]));
		final invalid = roots();
		invalid[0] = root("root.a", [IRMRPNullablePayload]);
		check(validator, "invalid", [fn("one", IRTInstance("outer"), invalid)], [
			diagnostic("module:roots.function:one.managedRoot:0:root.a", "managed root `root.a` has invalid path `nullable-payload` for `instance:outer`"),
			missing("one", [nestedA])[0]
		]);
		final duplicate = roots();
		duplicate.push(duplicate[0]);
		check(validator, "duplicate-root", [fn("one", IRTInstance("outer"), duplicate)], [
			diagnostic("module:roots.function:one.managedRoot:3:root.a", "duplicate managed root ID `root.a`"),
			diagnostic("module:roots.function:one.managedRoot:3:root.a", 'managed value `value` repeats root path `$nestedA`')
		]);
		check(validator, "duplicate-expected-missing", [fn("one", IRTInstance("outer"), [roots()[2]])],
			missing("one", [nestedA, nestedA]).concat([diagnostic("module:roots.type:inner.field:1:a", "duplicate aggregate field `a`")]), true);
		check(validator, "duplicate-expected-present", [fn("one", IRTInstance("outer"), [roots()[0], roots()[2]])],
			[diagnostic("module:roots.type:inner.field:1:a", "duplicate aggregate field `a`")], true);
		check(validator, "block-parameter", [fn("one", IRTInstance("outer"), [roots()[0]], true)], [
			diagnostic("module:roots.function:one.managedRoot:0:root.a", "managed block parameter `value` requires edge-owned root updates"),
			diagnostic("module:roots.function:one.managedRoots", "managed block parameter `value` requires edge-owned root updates")
		]);
		final unknown = roots();
		unknown.push({
			id: "root.unknown",
			valueId: "absent",
			projections: [],
			source: source
		});
		check(validator, "unknown-value", [fn("one", IRTInstance("outer"), unknown)], [
			diagnostic("module:roots.function:one.managedRoot:3:root.unknown", "managed root `root.unknown` names unknown value `absent`")
		]);
		final scalarRoots = [root("root.scalar.0", []), root("root.scalar.1", [])];
		check(validator, "function-local-identities", [
			fn("first", IRTInstance("outer"), roots()),
			fn("second", IRTInt(32, true), scalarRoots)
		], invalidScalarRoots("second"));
		check(validator, "reused-request-managed", [fn("one", IRTInstance("outer"), roots())], []);
		check(validator, "reused-request-scalar", [fn("one", IRTInt(32, true), scalarRoots)], invalidScalarRoots("one"));
		check(validator, "reused-request-missing", [fn("one", IRTInstance("outer"), [])], missing("one", [nestedA, nestedB, optional]));
		check(validator, "reused-request-valid", [fn("one", IRTInstance("outer"), roots())], []);
		Sys.println("HXC_IR_MANAGED_ROOT_PATHS_OK");
	}

	/** Expected messages and ordering are authored here, not copied from validator output. */
	static function check(validator:HxcIRValidator, label:String, functions:Array<HxcIRFunction>, expected:Array<String>, duplicateFields:Bool = false):Void {
		final actual = validator.validate(program(functions, duplicateFields), "portable").map(item -> item.render());
		if (actual.join("\n") != expected.join("\n"))
			throw new haxe.Exception(label + ": diagnostic array differed\nExpected:\n" + expected.join("\n") + "\nActual:\n" + actual.join("\n"));
	}

	/** Spell the stable public diagnostic envelope, including source and remediation. */
	static function diagnostic(path:String, message:String):String
		return 'HXC9000 error [profile=portable] test/hxc_ir/fixtures/ManagedRoots.hx:1:1-1:2 [$path] Invalid HxcIR: $message Remediation: Report a minimized reproducer, compiler/version manifest, and IR dump; do not bypass validation with raw C.';

	static function missing(id:String, keys:Array<String>):Array<String>
		return [
			for (key in keys)
				diagnostic('module:roots.function:$id.managedRoots', 'managed value `value` is missing exact root path `$key`')
		];

	/** Empty expected-path sets must reject every root, including repeated claims. */
	static function invalidScalarRoots(id:String):Array<String>
		return [
			diagnostic('module:roots.function:$id.managedRoot:0:root.scalar.0', "managed root `root.scalar.0` has invalid path `` for `i32`"),
			diagnostic('module:roots.function:$id.managedRoot:1:root.scalar.1', "managed root `root.scalar.1` has invalid path `` for `i32`"),
			diagnostic('module:roots.function:$id.managedRoot:1:root.scalar.1', "managed value `value` repeats root path ``")
		];

	/** Name the three independently known pointers in the nested fixture. */
	static function roots():Array<HxcIRManagedRoot>
		return [
			root("root.a", [IRMRPAggregateField("outer", "nested"), IRMRPAggregateField("inner", "a")]),
			root("root.b", [IRMRPAggregateField("outer", "nested"), IRMRPAggregateField("inner", "b")]),
			root("root.optional", [
				IRMRPAggregateField("outer", "optional"),
				IRMRPNullablePayload,
				IRMRPAggregateField("wrapper", "pointer")
			])
		];

	static function root(id:String, projections:Array<HxcIRManagedRootProjection>):HxcIRManagedRoot
		return {
			id: id,
			valueId: "value",
			projections: projections,
			source: source
		};

	/** Reuse the same value ID while each function retains its own type and root plan. */
	static function fn(id:String, type:HxcIRTypeRef, roots:Array<HxcIRManagedRoot>, blockParameter:Bool = false):HxcIRFunction {
		final parameter:HxcIRParameter = {id: "value", type: type, source: source};
		return {
			id: id,
			displayName: id,
			parameters: blockParameter ? [] : [parameter],
			borrowedClassParameterIds: [],
			borrowedClassLocalIds: [],
			managedRoots: roots,
			locals: [],
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: blockParameter ? [parameter] : [],
					instructions: [],
					terminator: {kind: IRTReturn(null, []), source: source},
					source: source
				}
			],
			cleanupRegions: [],
			source: source
		};
	}

	/** Keep malformed duplicate fields visible to both layout and root validation. */
	static function program(functions:Array<HxcIRFunction>, duplicateFields:Bool):HxcIRProgram {
		final pointer = IRTPointer(IRTInstance("object"), true);
		return {
			schemaVersion: HxcIRValidator.SCHEMA_VERSION,
			dynamicPlan: {
				types: [],
				members: [],
				callShapes: [],
				operations: []
			},
			dispatch: {layouts: [], slots: [], tables: []},
			modules: [
				{
					id: "roots",
					types: [
						{
							id: "wrapper",
							displayName: "Wrapper",
							kind: IRTKAggregate([
								{
									name: "pointer",
									type: pointer,
									mutable: false,
									source: source
								}
							]),
							source: source
						},
						{
							id: "object",
							displayName: "Object",
							kind: IRTKClass({baseInstanceId: null, fields: [], header: IRCHRuntime("gc")}),
							source: source
						},
						{
							id: "inner",
							displayName: "Inner",
							kind: IRTKAggregate([
								{
									name: "a",
									type: pointer,
									mutable: false,
									source: source
								},
								{
									name: duplicateFields ? "a" : "b",
									type: pointer,
									mutable: false,
									source: source
								}
							]),
							source: source
						},
						{
							id: "outer",
							displayName: "Outer",
							kind: IRTKAggregate([
								{
									name: "nested",
									type: IRTInstance("inner"),
									mutable: false,
									source: source
								},
								{
									name: "optional",
									type: IRTNullable(IRTInstance("wrapper"), IRNTagged),
									mutable: false,
									source: source
								}
							]),
							source: source
						}
					],
					typeInstances: [
						{
							id: "wrapper",
							declarationId: "wrapper",
							arguments: [],
							representation: IRRDirect,
							source: source
						},
						{
							id: "object",
							declarationId: "object",
							arguments: [],
							representation: IRRManaged("gc"),
							source: source
						},
						{
							id: "inner",
							declarationId: "inner",
							arguments: [],
							representation: IRRDirect,
							source: source
						},
						{
							id: "outer",
							declarationId: "outer",
							arguments: [],
							representation: IRRDirect,
							source: source
						}
					],
					globals: [],
					functions: functions,
					source: source
				}
			]
		};
	}
}
