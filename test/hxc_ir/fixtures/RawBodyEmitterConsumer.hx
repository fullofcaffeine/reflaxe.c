package;

import reflaxe.c.lowering.CBodyEmitter;

/** Proves a production HxcIR producer cannot reach raw C body emission. */
class RawBodyEmitterConsumer {
	/** Attempt to capture the raw emitter method without executing it. */
	static function main():Void {
		final emitter = new CBodyEmitter();
		final emit = emitter.emitBody;
	}
}
