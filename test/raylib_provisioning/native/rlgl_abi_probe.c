/*
 * Generated ABI probe for the precise raylib 6.0 rlgl quad-batch slice.
 * Authority: scripts/raylib/rlgl_binding.py and dbc56a87da87d973a9c5baa4e7438a9d20121d28.
 */
#include <stdio.h>
#include "rlgl.h"

_Static_assert(RL_QUADS == 7, "RL_QUADS value drifted");

static void hxc_check_raylib_rlgl_signatures(void);

static void
hxc_check_raylib_rlgl_signatures(void)
{
    void (*const hxc_signature_rlBegin)(int) = &rlBegin;
    void (*const hxc_signature_rlColor4ub)(unsigned char, unsigned char, unsigned char, unsigned char) = &rlColor4ub;
    void (*const hxc_signature_rlDrawRenderBatchActive)(void) = &rlDrawRenderBatchActive;
    void (*const hxc_signature_rlEnd)(void) = &rlEnd;
    void (*const hxc_signature_rlNormal3f)(float, float, float) = &rlNormal3f;
    void (*const hxc_signature_rlSetTexture)(unsigned int) = &rlSetTexture;
    void (*const hxc_signature_rlTexCoord2f)(float, float) = &rlTexCoord2f;
    void (*const hxc_signature_rlVertex3f)(float, float, float) = &rlVertex3f;

    (void)hxc_signature_rlBegin;
    (void)hxc_signature_rlColor4ub;
    (void)hxc_signature_rlDrawRenderBatchActive;
    (void)hxc_signature_rlEnd;
    (void)hxc_signature_rlNormal3f;
    (void)hxc_signature_rlSetTexture;
    (void)hxc_signature_rlTexCoord2f;
    (void)hxc_signature_rlVertex3f;
}

int
main(void)
{
    hxc_check_raylib_rlgl_signatures();
    (void)puts("raylib-rlgl-abi: OK");
    return 0;
}
