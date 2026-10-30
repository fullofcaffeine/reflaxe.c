package;

import reflaxe.c.ir.HxcIR.HxcIRProgram;
import reflaxe.c.runtime.RuntimeRequirementAnalyzer;

/** Proves production semantic analysis cannot consume unvalidated raw HxcIR. */
class RawProgramConsumer {
	static function analyze(raw:HxcIRProgram):Void {
		new RuntimeRequirementAnalyzer().analyze(raw, []);
	}

	static function main():Void {}
}
