package caxecraft.scenario;

/**
	Hard CAXEMAP 1 limits.

	These bounds make editor behavior, validation work, memory use, and hostile
	input handling predictable. They are format limits, not hidden C-target
	restrictions, and therefore apply identically to Eval and native builds.
**/
final class ScenarioLimits {
	public static inline final MAX_FILE_BYTES:Int = 16777216;
	public static inline final MAX_RECORDS:Int = 131072;
	public static inline final MAX_TEXT_SCALARS:Int = 8192;
	public static inline final MAX_WORLD_WIDTH:Int = 128;
	public static inline final MAX_WORLD_HEIGHT:Int = 64;
	public static inline final MAX_WORLD_DEPTH:Int = 128;
	public static inline final MAX_WORLD_CELLS:Int = 1048576;
	public static inline final MAX_CHUNK_EDGE:Int = 32;
	public static inline final MAX_PALETTE_ENTRIES:Int = 256;
	public static inline final MAX_OBJECTS:Int = 4096;
	public static inline final MAX_TAGS_PER_OBJECT:Int = 16;
	public static inline final MAX_DIALOGUES:Int = 1024;
	public static inline final MAX_DIALOGUE_LINES:Int = 128;
	public static inline final MAX_OBJECTIVES:Int = 512;
	public static inline final MAX_ROUTES:Int = 64;
	public static inline final MAX_VARIABLES:Int = 1024;
	public static inline final MAX_RULES:Int = 2048;
	public static inline final MAX_SEQUENCES:Int = 512;
	public static inline final MAX_ACTIONS_PER_RULE:Int = 64;
	public static inline final MAX_PREDICATE_DEPTH:Int = 16;
	public static inline final MAX_DEFERRED_EVENTS:Int = 4096;
	public static inline final MAX_SEQUENCE_CALL_DEPTH:Int = 16;
}
