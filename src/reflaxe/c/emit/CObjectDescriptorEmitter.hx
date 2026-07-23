package reflaxe.c.emit;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import reflaxe.c.ast.CAST;

/**
	One reachability-selected internal object descriptor after C names are final.

	The caller owns semantic decisions: which payload needs managed storage, which
	exact trace function visits it, and whether cleanup is required. This record
	only carries those closed decisions into structural C emission. Keeping that
	boundary explicit prevents the emitter from turning every class into a runtime
	object or guessing ownership from a pointer-shaped field.
**/
class CObjectDescriptorSpec {
	public final id:String;
	public final descriptorName:CIdentifier;
	public final payload:CTypedDeclarator;
	public final traceName:Null<CIdentifier>;
	public final finalizerName:Null<CIdentifier>;

	/** True when generated functions in other translation units take its address. */
	public final externalLinkage:Bool;

	public function new(id:String, descriptorName:CIdentifier, payload:CTypedDeclarator, traceName:Null<CIdentifier>, finalizerName:Null<CIdentifier>,
			externalLinkage:Bool = false) {
		validateStableId(id);
		this.id = id;
		this.descriptorName = descriptorName;
		this.payload = payload;
		this.traceName = traceName;
		this.finalizerName = finalizerName;
		this.externalLinkage = externalLinkage;
	}

	static function validateStableId(value:String):Void {
		if (!~/^[a-z][a-z0-9]*(?:[.-][a-z0-9]+)*$/.match(value))
			throw new CObjectDescriptorEmissionError('object descriptor ID must use stable lowercase dotted/kebab spelling: `$value`');
	}
}

/** A descriptor-plan error found before any output is printed or written. */
class CObjectDescriptorEmissionError extends haxe.Exception {}

/**
	Emit the selected descriptor objects and their compile-time layout checks.

	An empty input produces no declarations. That property is deliberate: direct
	class layouts and runtime-free metal programs must not gain a descriptor merely
	because this component exists. The caller adds `hxrt/object.h` only when the
	returned declaration list is non-empty.
**/
class CObjectDescriptorEmitter {
	static final DESCRIPTOR_TYPE = new CIdentifier("hxc_type_descriptor");
	static final ABI_VERSION = new CIdentifier("HXC_TYPE_DESCRIPTOR_ABI_VERSION");
	static final TRACE_FLAG = new CIdentifier("HXC_TYPE_DESCRIPTOR_HAS_TRACE");
	static final FINALIZER_FLAG = new CIdentifier("HXC_TYPE_DESCRIPTOR_HAS_FINALIZER");
	static final ABI_FIELD = new CIdentifier("abi_version");
	static final FLAGS_FIELD = new CIdentifier("flags");
	static final SIZE_FIELD = new CIdentifier("object_size");
	static final ALIGNMENT_FIELD = new CIdentifier("object_alignment");
	static final TRACE_FIELD = new CIdentifier("trace");
	static final FINALIZER_FIELD = new CIdentifier("finalize");

	public function new() {}

	/**
			Return deterministic descriptor definitions for the complete selected set.

			IDs and final C names must both be unique. Sorting uses UTF-8 bytes, matching
			the rest of the project emitter rather than depending on discovery order or
		the host locale.
	**/
	public function declarations(input:Array<CObjectDescriptorSpec>):Array<CDecl> {
		final specs = input.copy();
		specs.sort((left, right) -> compareUtf8(left.id, right.id));
		final ids:Map<String, Bool> = [];
		final names:Map<String, String> = [];
		final result:Array<CDecl> = [];
		for (spec in specs) {
			if (ids.exists(spec.id))
				throw new CObjectDescriptorEmissionError('duplicate object descriptor ID `${spec.id}`');
			ids.set(spec.id, true);
			final priorId = names.get(spec.descriptorName.value);
			if (priorId != null)
				throw new CObjectDescriptorEmissionError('object descriptors `$priorId` and `${spec.id}` share C name `${spec.descriptorName.value}`');
			names.set(spec.descriptorName.value, spec.id);
			result.push(layoutAssertion(spec));
			result.push(descriptorDefinition(spec));
		}
		return result;
	}

	/** Declare only cross-translation-unit descriptors in a private header. */
	public function externDeclarations(input:Array<CObjectDescriptorSpec>):Array<CDecl> {
		final specs = input.copy();
		specs.sort((left, right) -> compareUtf8(left.id, right.id));
		return [
			for (spec in specs)
				if (spec.externalLinkage) DVariable({
					storage: [SExtern],
					alignments: [],
					type: new CType(TStruct(DESCRIPTOR_TYPE), [QConst]),
					declarator: DName(spec.descriptorName),
					initializer: null,
					attributes: []
				})
		];
	}

	function layoutAssertion(spec:CObjectDescriptorSpec):CDecl {
		final size = ESizeOfType(spec.payload.type, spec.payload.declarator);
		final alignment = EAlignOfType(spec.payload.type, spec.payload.declarator);
		return DStaticAssert(EBinary(Equal, EBinary(Modulo, size, alignment), zero()), 'descriptor `${spec.id}` payload size must be a multiple of alignment');
	}

	function descriptorDefinition(spec:CObjectDescriptorSpec):CDecl {
		return DVariable({
			storage: spec.externalLinkage ? [] : [SStatic],
			alignments: [],
			type: new CType(TStruct(DESCRIPTOR_TYPE), [QConst]),
			declarator: DName(spec.descriptorName),
			initializer: IList([
				field(ABI_FIELD, EIdentifier(ABI_VERSION)),
				field(FLAGS_FIELD, flags(spec)),
				field(SIZE_FIELD, ESizeOfType(spec.payload.type, spec.payload.declarator)),
				field(ALIGNMENT_FIELD, EAlignOfType(spec.payload.type, spec.payload.declarator)),
				field(TRACE_FIELD, optionalName(spec.traceName)),
				field(FINALIZER_FIELD, optionalName(spec.finalizerName))
			]),
			attributes: []
		});
	}

	static function flags(spec:CObjectDescriptorSpec):CExpr {
		final values:Array<CExpr> = [];
		if (spec.traceName != null)
			values.push(EIdentifier(TRACE_FLAG));
		if (spec.finalizerName != null)
			values.push(EIdentifier(FINALIZER_FLAG));
		if (values.length == 0)
			return zero();
		var result = values[0];
		for (index in 1...values.length)
			result = EBinary(BitOr, result, values[index]);
		return result;
	}

	static function optionalName(name:Null<CIdentifier>):CExpr
		return name == null ? ENull : EIdentifier(name);

	static function field(name:CIdentifier, value:CExpr):CInitializerItem
		return {designators: [DField(name)], value: IExpr(value)};

	static function zero():CExpr
		return EInt(CIntegerLiteral.decimal("0"));

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
#else

/** Compiler-only descriptor emitter stub for generated-program source scans. */
class CObjectDescriptorEmitter {
	public function new() {}
}
#end
