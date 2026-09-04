package caxecraft.pilot;

/** What evidence the game adapter should preserve after one completed frame. */
enum PilotCheckpointKind {
	ObserveState;
	CaptureScreenshot;
}

/** Closed screenshot identity carried without exposing a host filename. */
enum PilotScreenshot {
	Launch;
	Movement;
	Pause;
	Combat;
	Recovery;
	FullInventoryMining;
	Resize;
	AquaticGear;
	SmoothMotion;
	EditorTerrainPrompt;
	EditorTestPlay;
	EditorText;
	EditorAssets;
	EditorEnvironment;
	EditorReturn;
	CampaignTravel;
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
	public final screenshot:Null<PilotScreenshot>;

	public function new(label:String, kind:PilotCheckpointKind, ?screenshot:PilotScreenshot) {
		this.label = label;
		this.kind = kind;
		this.screenshot = screenshot;
	}
}
