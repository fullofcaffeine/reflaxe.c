package caxecraft.pilot;

/** What evidence the game adapter should preserve after one completed frame. */
enum PilotCheckpointKind {
	ObserveState;
	CaptureScreenshot;
}

/**
 * A stable evidence request attached to one exact rendered frame.
 *
 * `label` is a logical result name. It is never a host path, so the native
 * runner remains responsible for choosing and reporting an isolated output.
 */
final class PilotCheckpoint {
	public final label:String;
	public final kind:PilotCheckpointKind;

	public function new(label:String, kind:PilotCheckpointKind) {
		this.label = label;
		this.kind = kind;
	}
}
