package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.io.Bytes;
import reflaxe.c.ast.CAST;
import reflaxe.c.contract.TypedCContract.TypedCBuildFact;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.naming.CSymbolRegistry;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.semantics.CPrimitiveSemantics;
import reflaxe.c.semantics.CPrimitiveTypes.CPrimitiveHelperKind;

private typedef StandardSymbol = {
	final name:String;
	final preprocessor:Bool;
}

/** One selected compiler-owned helper after every emitted identifier is finalized. */
class CPrimitiveHelperPlan {
	public final kind:CPrimitiveHelperKind;
	public final helperId:String;
	public final cName:CIdentifier;
	public final parameterNames:Array<CIdentifier>;
	public final standardNames:Map<String, CIdentifier>;

	public function new(kind:CPrimitiveHelperKind, helperId:String, cName:CIdentifier, parameterNames:Array<CIdentifier>,
			standardNames:Map<String, CIdentifier>) {
		this.kind = kind;
		this.helperId = helperId;
		this.cName = cName;
		this.parameterNames = parameterNames.copy();
		this.standardNames = standardNames;
	}
}

/**
	Request-local selection for total primitive helpers.

	The HxcIR remains the source of truth: only named `IRIProgramLocal` operations
	are selected, dependencies are closed explicitly, and no helper becomes an
	`hxrt` feature merely because direct C would be unsafe.
 */
class CPrimitiveHelperSelection {
	final selectedById:Map<String, CPrimitiveHelperKind> = [];
	final ownersById:Map<String, Array<String>> = [];
	final helperRequests:Map<String, CSymbolRequest> = [];
	final parameterRequests:Map<String, Array<CSymbolRequest>> = [];
	final standardRequests:Map<String, CSymbolRequest> = [];

	public function new() {}

	public function collect(program:HxcIRProgram):Void {
		for (module in program.modules) {
			for (fn in module.functions) {
				for (block in fn.blocks) {
					for (instruction in block.instructions) {
						switch instruction.kind {
							case IRIOUnary(_, _, IRIProgramLocal(helperId)) | IRIOBinary(_, _, _, IRIProgramLocal(helperId)) |
								IRIOConvert(_, _, _, IRIProgramLocal(helperId), null):
								selectId(helperId, module.id);
							case _:
						}
					}
				}
			}
		}
	}

	public function register(symbols:CSymbolRegistry):Void {
		for (kind in orderedKinds()) {
			final helperId = CPrimitiveSemantics.helperId(kind);
			final readableName = helperId.split(".").slice(1);
			final request = new CSymbolRequest(CSKSpecialization, ["compiler", "primitive-helper"], CNSOrdinary("translation-unit"), CSVInternal, null, [],
				[helperId], null, readableName);
			symbols.register(request);
			helperRequests.set(helperId, request);
			final parameters:Array<CSymbolRequest> = [];
			for (index in 0...parameterCount(kind)) {
				final role = parameterRole(kind, index);
				final parameter = new CSymbolRequest(CSKLocal, ["compiler", "primitive-helper", helperId, role], CNSOrdinary(request.stableKey()),
					CSVInternal, null, [], [], index, [role]);
				symbols.register(parameter);
				parameters.push(parameter);
			}
			parameterRequests.set(helperId, parameters);
			for (standard in requiredStandardSymbols(kind)) {
				registerStandardSymbol(symbols, standard);
			}
		}
	}

	public function finalize(symbols:CSymbolRegistry):Array<CPrimitiveHelperPlan> {
		final plans:Array<CPrimitiveHelperPlan> = [];
		for (kind in orderedKinds()) {
			final helperId = CPrimitiveSemantics.helperId(kind);
			final helperRequest = helperRequests.get(helperId);
			final parameters = parameterRequests.get(helperId);
			if (helperRequest == null || parameters == null) {
				throw new CBodyEmissionError('primitive helper `$helperId` was not registered before symbol finalization');
			}
			final names:Map<String, CIdentifier> = [];
			for (standard in requiredStandardSymbols(kind)) {
				final request = standardRequests.get(standard.name);
				if (request == null) {
					throw new CBodyEmissionError('primitive helper `$helperId` lost standard symbol `${standard.name}`');
				}
				names.set(standard.name, symbols.identifierFor(request));
			}
			plans.push(new CPrimitiveHelperPlan(kind, helperId, symbols.identifierFor(helperRequest),
				parameters.map(parameter -> symbols.identifierFor(parameter)), names));
		}
		return plans;
	}

	public function buildFacts():Array<TypedCBuildFact> {
		final moduloId = CPrimitiveSemantics.helperId(CPHF64Modulo);
		if (!selectedById.exists(moduloId)) {
			return [];
		}
		final owners = ownersById.get(moduloId);
		if (owners == null || owners.length == 0) {
			throw new CBodyEmissionError("selected floating modulo helper has no source-module provenance");
		}
		return [
			{
				kind: "link",
				name: "m",
				value: null,
				valueKind: null,
				ownerModulePaths: owners.copy()
			}
		];
	}

	function selectId(helperId:String, ownerModule:String):Void {
		final kind = CPrimitiveSemantics.helperKind(helperId);
		if (kind == null) {
			throw new CBodyEmissionError('HxcIR selected unknown primitive program-local helper `$helperId`');
		}
		select(kind, ownerModule);
	}

	function select(kind:CPrimitiveHelperKind, ownerModule:String):Void {
		final helperId = CPrimitiveSemantics.helperId(kind);
		selectedById.set(helperId, kind);
		var owners = ownersById.get(helperId);
		if (owners == null) {
			owners = [];
			ownersById.set(helperId, owners);
		}
		addUnique(owners, ownerModule);
		owners.sort(compareStrings);
		for (dependency in CPrimitiveSemantics.helperDependencies(kind)) {
			select(dependency, ownerModule);
		}
	}

	function orderedKinds():Array<CPrimitiveHelperKind> {
		final kinds = [for (kind in selectedById) kind];
		kinds.sort((left, right) -> {
			final rankDifference = helperRank(left) - helperRank(right);
			return rankDifference != 0 ? rankDifference : compareStrings(CPrimitiveSemantics.helperId(left), CPrimitiveSemantics.helperId(right));
		});
		return kinds;
	}

	function registerStandardSymbol(symbols:CSymbolRegistry, standard:StandardSymbol):Void {
		if (standardRequests.exists(standard.name)) {
			return;
		}
		final request = new CSymbolRequest(standard.preprocessor ? CSKField : CSKMethod, ["c-standard-library", standard.name],
			standard.preprocessor ? CNSPreprocessor : CNSOrdinary("translation-unit"), CSVExternal, standard.name);
		symbols.register(request);
		standardRequests.set(standard.name, request);
	}

	static function helperRank(kind:CPrimitiveHelperKind):Int
		return kind == CPHU32ToI32Bits ? 0 : 1;

	static function parameterCount(kind:CPrimitiveHelperKind):Int {
		return switch kind {
			case CPHU32ToI32Bits | CPHI32Negate | CPHI32BitwiseNot | CPHF64ToI32Saturating: 1;
			case CPHI32Add | CPHI32Subtract | CPHI32Multiply | CPHI32Modulo | CPHU32Modulo | CPHI32ShiftLeft | CPHI32ShiftRight | CPHI32UnsignedShiftRight |
				CPHI32BitAnd | CPHI32BitOr | CPHI32BitXor | CPHF64Divide | CPHF64Modulo:
				2;
		};
	}

	static function parameterRole(kind:CPrimitiveHelperKind, index:Int):String {
		if (index < 0 || index >= parameterCount(kind)) {
			throw new CBodyEmissionError('primitive helper `${CPrimitiveSemantics.helperId(kind)}` has invalid parameter index `$index`');
		}
		return parameterCount(kind) == 1 ? "value" : (index == 0 ? "left" : "right");
	}

	static function requiredStandardSymbols(kind:CPrimitiveHelperKind):Array<StandardSymbol> {
		return switch kind {
			case CPHU32ToI32Bits:
				[preprocessor("INT32_MIN"), preprocessor("UINT32_C")];
			case CPHI32Modulo:
				[preprocessor("INT32_MIN")];
			case CPHI32ShiftRight:
				[preprocessor("UINT32_C")];
			case CPHF64Divide:
				[preprocessor("INFINITY"), preprocessor("NAN"), preprocessor("signbit")];
			case CPHF64Modulo:
				[preprocessor("NAN"), ordinary("fmod")];
			case CPHF64ToI32Saturating:
				[preprocessor("INT32_MAX"), preprocessor("INT32_MIN")];
			case CPHI32Add | CPHI32Subtract | CPHI32Multiply | CPHI32Negate | CPHU32Modulo | CPHI32ShiftLeft | CPHI32UnsignedShiftRight | CPHI32BitAnd |
				CPHI32BitOr | CPHI32BitXor | CPHI32BitwiseNot:
				[];
		};
	}

	static function preprocessor(name:String):StandardSymbol
		return {name: name, preprocessor: true};

	static function ordinary(name:String):StandardSymbol
		return {name: name, preprocessor: false};

	static function addUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function compareStrings(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}

/** Structural strict-C11 definitions for the selected helper closure. */
class CPrimitiveHelperEmitter {
	final byId:Map<String, CPrimitiveHelperPlan> = [];

	public function new(plans:Array<CPrimitiveHelperPlan>) {
		for (plan in plans) {
			byId.set(plan.helperId, plan);
		}
	}

	public function requiredHeaders(plans:Array<CPrimitiveHelperPlan>):Array<String> {
		final headers:Array<String> = [];
		for (plan in plans) {
			switch plan.kind {
				case CPHF64Divide | CPHF64Modulo:
					addUnique(headers, "math.h");
				case _:
					addUnique(headers, "stdint.h");
			}
		}
		headers.sort(compareStrings);
		return headers;
	}

	public function definitions(plans:Array<CPrimitiveHelperPlan>):Array<CDecl> {
		final declarations:Array<CDecl> = [];
		for (plan in plans) {
			declarations.push(DFunction({
				storage: [SStatic],
				functionSpecifiers: [FInline],
				returnType: resultType(plan.kind),
				declarator: DFunction(DName(plan.cName), FPPrototype(parameters(plan), false)),
				body: body(plan),
				attributes: []
			}));
		}
		return declarations;
	}

	function parameters(plan:CPrimitiveHelperPlan):Array<CParam> {
		final types = parameterTypes(plan.kind);
		if (types.length != plan.parameterNames.length) {
			throw new CBodyEmissionError('primitive helper `${plan.helperId}` has an incomplete finalized parameter set');
		}
		return [
			for (index in 0...types.length)
				{type: types[index], declarator: DName(plan.parameterNames[index]), attributes: []}
		];
	}

	function body(plan:CPrimitiveHelperPlan):CStmt {
		final left = EIdentifier(plan.parameterNames[0]);
		final right = plan.parameterNames.length == 2 ? EIdentifier(plan.parameterNames[1]) : null;
		return switch plan.kind {
			case CPHU32ToI32Bits:
				SBlock([
					SIf(EBinary(LessEqual, left, uint32Constant(plan, "2147483647")), SReturn(castInt32(left)), null),
					SReturn(EBinary(Add, standard(plan, "INT32_MIN"), castInt32(EBinary(Subtract, left, uint32Constant(plan, "2147483648")))))
				]);
			case CPHI32Add:
				SBlock([SReturn(fromU32(widenedUInt32Binary(Add, left, requireRight(plan, right))))]);
			case CPHI32Subtract:
				SBlock([SReturn(fromU32(widenedUInt32Binary(Subtract, left, requireRight(plan, right))))]);
			case CPHI32Multiply:
				SBlock([SReturn(fromU32(widenedUInt32Binary(Multiply, left, requireRight(plan, right))))]);
			case CPHI32Negate:
				SBlock([SReturn(fromU32(widenedUInt32Binary(Subtract, intConstant("0"), left)))]);
			case CPHI32Modulo:
				final divisor = requireRight(plan, right);
				final exceptional = EBinary(LogicalOr, EBinary(Equal, divisor, intConstant("0")),
					EParen(EBinary(LogicalAnd, EBinary(Equal, left, standard(plan, "INT32_MIN")), EBinary(Equal, divisor, EUnary(Minus, intConstant("1"))))));
				SBlock([
					SIf(exceptional, SReturn(intConstant("0")), null),
					SReturn(EBinary(Modulo, left, divisor))
				]);
			case CPHU32Modulo:
				final divisor = requireRight(plan, right);
				SBlock([
					SIf(EBinary(Equal, divisor, intConstant("0")), SReturn(intConstant("0")), null),
					SReturn(EBinary(Modulo, left, divisor))
				]);
			case CPHI32ShiftLeft:
				SBlock([SReturn(fromU32(widenedUInt32Shift(ShiftLeft, left, requireRight(plan, right))))]);
			case CPHI32ShiftRight:
				final bits = castUInt32(left);
				final amount = maskedShiftCount(requireRight(plan, right));
				final positive = castUInt32(EBinary(ShiftRight, castUInt64(bits), amount));
				final inverted = castUInt32(EUnary(BitwiseNot, castUInt64(bits)));
				final shiftedInverted = castUInt32(EBinary(ShiftRight, castUInt64(inverted), amount));
				final negative = castUInt32(EUnary(BitwiseNot, castUInt64(shiftedInverted)));
				final signClear = EBinary(Equal, EBinary(BitAnd, bits, uint32Constant(plan, "2147483648")), intConstant("0"));
				SBlock([SReturn(fromU32(EConditional(signClear, positive, negative)))]);
			case CPHI32UnsignedShiftRight:
				SBlock([
					SReturn(fromU32(widenedUInt32Shift(ShiftRight, left, requireRight(plan, right))))
				]);
			case CPHI32BitAnd:
				SBlock([SReturn(fromU32(widenedUInt32Binary(BitAnd, left, requireRight(plan, right))))]);
			case CPHI32BitOr:
				SBlock([SReturn(fromU32(widenedUInt32Binary(BitOr, left, requireRight(plan, right))))]);
			case CPHI32BitXor:
				SBlock([SReturn(fromU32(widenedUInt32Binary(BitXor, left, requireRight(plan, right))))]);
			case CPHI32BitwiseNot:
				SBlock([SReturn(fromU32(castUInt32(EUnary(BitwiseNot, castUInt64(castUInt32(left))))))]);
			case CPHF64Divide:
				final divisor = requireRight(plan, right);
				final zero = floatZero();
				final nanInput = EBinary(LogicalOr, EBinary(Equal, left, zero), EBinary(NotEqual, left, left));
				final negative = EBinary(NotEqual, ECall(standard(plan, "signbit"), [left]), ECall(standard(plan, "signbit"), [divisor]));
				SBlock([
					SIf(EBinary(NotEqual, divisor, zero), SReturn(EBinary(Divide, left, divisor)), null),
					SIf(nanInput, SReturn(standard(plan, "NAN")), null),
					SReturn(EConditional(negative, EUnary(Minus, standard(plan, "INFINITY")), standard(plan, "INFINITY")))
				]);
			case CPHF64Modulo:
				final divisor = requireRight(plan, right);
				SBlock([
					SIf(EBinary(Equal, divisor, floatZero()), SReturn(standard(plan, "NAN")), null),
					SReturn(ECall(standard(plan, "fmod"), [left, divisor]))
				]);
			case CPHF64ToI32Saturating:
				SBlock([
					SIf(EBinary(NotEqual, left, left), SReturn(intConstant("0")), null),
					SIf(EBinary(GreaterEqual, left, positiveI32Limit()), SReturn(standard(plan, "INT32_MAX")), null),
					SIf(EBinary(LessEqual, left, negativeI32Limit()), SReturn(standard(plan, "INT32_MIN")), null),
					SReturn(castInt32(left))
				]);
		};
	}

	function fromU32(value:CExpr):CExpr {
		final helper = byId.get(CPrimitiveSemantics.helperId(CPHU32ToI32Bits));
		if (helper == null) {
			throw new CBodyEmissionError("signed primitive helper lost its u32-to-i32 reconstruction dependency");
		}
		return ECall(EIdentifier(helper.cName), [value]);
	}

	static function parameterTypes(kind:CPrimitiveHelperKind):Array<CType> {
		return switch kind {
			case CPHU32ToI32Bits: [uint32Type()];
			case CPHU32Modulo: [uint32Type(), uint32Type()];
			case CPHF64Divide | CPHF64Modulo: [doubleType(), doubleType()];
			case CPHF64ToI32Saturating: [doubleType()];
			case CPHI32Negate | CPHI32BitwiseNot: [int32Type()];
			case CPHI32Add | CPHI32Subtract | CPHI32Multiply | CPHI32Modulo | CPHI32ShiftLeft | CPHI32ShiftRight | CPHI32UnsignedShiftRight | CPHI32BitAnd |
				CPHI32BitOr | CPHI32BitXor:
				[int32Type(), int32Type()];
		};
	}

	static function resultType(kind:CPrimitiveHelperKind):CType {
		return switch kind {
			case CPHU32Modulo: uint32Type();
			case CPHF64Divide | CPHF64Modulo: doubleType();
			case CPHF64ToI32Saturating: int32Type();
			case _: int32Type();
		};
	}

	static function maskedShiftCount(value:CExpr):CExpr
		return EBinary(BitAnd, castUInt32(value), castUInt32(intConstant("31")));

	static function widenedUInt32Binary(operation:CBinaryOp, left:CExpr, right:CExpr):CExpr
		return castUInt32(EBinary(operation, castUInt64(castUInt32(left)), castUInt64(castUInt32(right))));

	static function widenedUInt32Shift(operation:CBinaryOp, value:CExpr, count:CExpr):CExpr
		return castUInt32(EBinary(operation, castUInt64(castUInt32(value)), maskedShiftCount(count)));

	static function castInt32(value:CExpr):CExpr
		return ECast(int32Type(), DName(null), value);

	static function castUInt32(value:CExpr):CExpr
		return ECast(uint32Type(), DName(null), value);

	static function castUInt64(value:CExpr):CExpr
		return ECast(uint64Type(), DName(null), value);

	static function uint32Constant(plan:CPrimitiveHelperPlan, value:String):CExpr
		return ECall(standard(plan, "UINT32_C"), [intConstant(value)]);

	static function intConstant(value:String):CExpr
		return EInt(CIntegerLiteral.decimal(value));

	static function floatZero():CExpr
		return EFloat(new CFloatLiteral(FRDecimal("0", "0", null)));

	static function positiveI32Limit():CExpr
		return EFloat(new CFloatLiteral(FRDecimal("2147483647", "0", null)));

	static function negativeI32Limit():CExpr
		return EUnary(Minus, EFloat(new CFloatLiteral(FRDecimal("2147483648", "0", null))));

	static function int32Type():CType
		return new CType(TInt(32, true));

	static function uint32Type():CType
		return new CType(TInt(32, false));

	static function uint64Type():CType
		return new CType(TInt(64, false));

	static function doubleType():CType
		return new CType(TDouble);

	static function standard(plan:CPrimitiveHelperPlan, name:String):CExpr {
		final identifier = plan.standardNames.get(name);
		if (identifier == null) {
			throw new CBodyEmissionError('primitive helper `${plan.helperId}` has no finalized standard symbol `$name`');
		}
		return EIdentifier(identifier);
	}

	static function requireRight(plan:CPrimitiveHelperPlan, value:Null<CExpr>):CExpr {
		if (value == null) {
			throw new CBodyEmissionError('binary primitive helper `${plan.helperId}` lost its right parameter');
		}
		return value;
	}

	static function addUnique(values:Array<String>, value:String):Void {
		if (values.indexOf(value) == -1) {
			values.push(value);
		}
	}

	static function compareStrings(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final limit = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...limit) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}
#else
class CPrimitiveHelperSelection {
	public function new() {}
}

class CPrimitiveHelperEmitter {
	public function new() {}
}
#end
