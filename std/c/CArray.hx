package c;

/**
	Inline C array contract. `N` is a compiler-recognized type-level length witness;
	no value-level or runtime representation is implemented by the M0 seed.
**/
@:coreType
extern abstract CArray<T, N> {}
