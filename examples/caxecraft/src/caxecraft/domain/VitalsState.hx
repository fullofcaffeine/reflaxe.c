package caxecraft.domain;

/**
	Serializable health and the short protected period after receiving damage.

	This state belongs to any character that can be hurt. Player input, enemy
	artificial intelligence, and presentation remain separate concerns.
**/
typedef VitalsState = {
	final health:Int;
	final safeTicks:Int;
}
