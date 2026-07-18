/*
 * Implementation of native-seed-only feature `runtime-abi`.
 *
 * Only independent ABI smoke consumers call this pure version query. Selective
 * generated-program packaging never requests it. It owns no memory, failure,
 * platform, or thread behavior; base.h supplies the internal packed constant.
 */
#include "hxrt/abi.h"

uint32_t hxc_runtime_abi_version(void) {
  return HXC_RUNTIME_ABI_VERSION;
}
