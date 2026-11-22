package;

import reflaxe.c.ir.HxcIR.HxcIRProgram;
import reflaxe.c.ir.HxcIRValidator.ValidatedHxcIRProgram;

/** Proves only the validator can construct the validated ownership proof. */
class ValidatedProgramConstructorConsumer {
	static function forge(raw:HxcIRProgram):ValidatedHxcIRProgram {
		return new ValidatedHxcIRProgram(raw);
	}

	static function main():Void {}
}
