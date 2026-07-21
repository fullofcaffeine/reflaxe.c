package caxecraft.scenario;

/**
	Language tag used by one scenario message catalog.

	CAXEMAP stores a lowercase, locale-independent spelling such as `en` or
	`es-mx`. The display layer may map that stable ID to an operating-system
	locale such as `es-MX`; saved gameplay state never stores translated text.
**/
abstract LocaleId(String) {
	public inline function new(value:String)
		this = value;

	public inline function text():String
		return this;
}
