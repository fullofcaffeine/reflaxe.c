package caxecraft.domain;

#if c
import c.IntConvert;
#end

/**
	Provides the read-only questions that gameplay and presentation ask a world.

	Keeping these operations separate from `World`'s editing API makes the
	authority boundary visible in Haxe: a `WorldView` can be queried, but it
	cannot be passed to terrain-writing functions. The C branch only widens one
	`uint8_t` value to Haxe `Int`; every higher-level rule is shared with Eval.
**/
/** Read one validated storage position as a Haxe `Int`. */
function storageCodeAt(view:WorldView, index:Int):Int {
	#if c
	return IntConvert.exact(view[index]);
	#else
	return view[index];
	#end
}

/** Return the block kind at a coordinate, treating out-of-bounds as air. */
function query(view:WorldView, coord:BlockCoord):BlockKind {
	final index = World.indexOf(coord);
	if (index < 0)
		return Air;
	return World.kindFromCode(storageCodeAt(view, index));
}

/** Return the highest solid block in one column, or `-1` outside empty ground. */
function surfaceY(view:WorldView, x:Int, z:Int):Int {
	if (x < 0 || x >= World.WIDTH || z < 0 || z >= World.DEPTH)
		return -1;
	var y = World.HEIGHT - 1;
	while (y >= 0) {
		if (World.isSolid(query(view, World.coord(x, y, z))))
			return y;
		y--;
	}
	return -1;
}

/** Build the stable whole-world hash used by telemetry and regression checks. */
function stateHash(view:WorldView):Int {
	var hash = -2128831035;
	var index = 0;
	while (index < World.VOLUME) {
		hash = (hash ^ storageCodeAt(view, index)) * 16777619;
		index++;
	}
	return hash;
}
