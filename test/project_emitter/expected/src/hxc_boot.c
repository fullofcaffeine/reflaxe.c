#include "hxc/emitter_fixture.h"
#include <stdio.h>

int main(void) {
    if (hxc_api_fixture_Api_value() != 42) {
        return 1;
    }
    (void)puts("project-emitter: OK");
    return 0;
}
