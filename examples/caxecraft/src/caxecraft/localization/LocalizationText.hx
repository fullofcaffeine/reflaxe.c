package caxecraft.localization;

/**
 * Text returned by a Caxecraft localization catalog.
 *
 * Catalog selection is target-neutral: callers provide a locale and typed
 * message ID, then receive text without knowing which languages exist. Haxe
 * Eval uses ordinary `String` so tools and semantic tests run without Raylib.
 * The current native C renderer instead receives `c.CString`, whose lowering
 * proves that every built-in value selects static, embedded-NUL-free literal
 * storage before raylib borrows it. This one representation seam emits no
 * runtime target test and keeps C ABI lifetime policy out of catalog data.
 */
#if c
typedef LocalizationText = c.CString;
#else
typedef LocalizationText = String;
#end
