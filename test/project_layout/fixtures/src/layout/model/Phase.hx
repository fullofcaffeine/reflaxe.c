package layout.model;

/**
	A type-only native enum used across generated modules.

	Strict ISO C11 cannot forward-declare this representation. The split and
	package emitters therefore define it once in their shared private type header
	instead of creating hard module-header edges that can form false cycles.
**/
enum Phase {
	Ready;
}
