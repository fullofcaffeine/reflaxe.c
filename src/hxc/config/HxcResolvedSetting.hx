package hxc.config;

/** A typed effective value paired with the exact layer that selected it. */
class HxcResolvedSetting<T> {
	public final value:T;
	public final origin:HxcConfigOrigin;

	public function new(value:T, origin:HxcConfigOrigin) {
		this.value = value;
		this.origin = origin;
	}
}
