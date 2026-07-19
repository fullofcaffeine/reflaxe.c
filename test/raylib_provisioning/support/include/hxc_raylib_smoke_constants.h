#ifndef HXC_RAYLIB_SMOKE_CONSTANTS_H_INCLUDED
#define HXC_RAYLIB_SMOKE_CONSTANTS_H_INCLUDED

#include <stdint.h>
#include <raylib.h>

#define HXC_RAYLIB_SMOKE_SIZE INT32_C(16)
#define HXC_RAYLIB_SMOKE_CENTER INT32_C(8)
#define HXC_RAYLIB_SMOKE_COLOR UINT32_C(0x102030ff)
#define HXC_RAYLIB_SMOKE_KEY_NULL INT32_C(0)
#define HXC_RAYLIB_SMOKE_MOUSE_LEFT INT32_C(0)
#define HXC_RAYLIB_SMOKE_LOG_NONE INT32_C(7)

static inline Camera
raylib_smoke_fixture_camera(void)
{
    const Camera camera = {
        .position = { .x = 0.0F, .y = 1.0F, .z = 2.0F },
        .target = { .x = 0.0F, .y = 0.0F, .z = 0.0F },
        .up = { .x = 0.0F, .y = 1.0F, .z = 0.0F },
        .fovy = 60.0F,
        .projection = CAMERA_PERSPECTIVE
    };
    return camera;
}

static inline Vector2
raylib_smoke_fixture_screen_position(void)
{
    const Vector2 position = { .x = 8.0F, .y = 8.0F };
    return position;
}

#endif /* HXC_RAYLIB_SMOKE_CONSTANTS_H_INCLUDED */
