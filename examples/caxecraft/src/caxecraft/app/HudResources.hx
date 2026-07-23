package caxecraft.app;

#if c
import raylib.Texture2D;

/**
	Raylib resources borrowed while drawing one heads-up display frame.

	This record contains presentation handles, not gameplay state. The outer
	application still owns loading and unloading each texture; `drawHud` may use
	the handles only during the call and checks the matching readiness flag before
	drawing. Keeping this C-only value separate is what lets `HudView` remain
	target-neutral and useful to future non-Raylib presentations.
**/
typedef HudResources = {
	/** Loaded HUD atlas handle; used only when `hudTextureReady` is true. */
	final hudTexture:Texture2D;

	/** True when the HUD atlas handle is valid for drawing. */
	final hudTextureReady:Bool;

	/** Loaded base-item atlas handle. */
	final itemTexture:Texture2D;

	/** True when the base-item atlas handle is valid for drawing. */
	final itemTextureReady:Bool;

	/** Loaded adventure-item atlas handle. */
	final adventureItemTexture:Texture2D;

	/** True when the adventure-item atlas handle is valid for drawing. */
	final adventureItemTextureReady:Bool;
}
#end
