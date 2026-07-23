/*
 * Handwritten C reference for Caxecraft's retained immediate terrain renderer.
 *
 * This is benchmark evidence, not game implementation. The Python orchestrator
 * generates only caxecraft_benchmark_scene.h from the validated CAXEMAP; all
 * face discovery, atlas selection, tinting, rlgl submission, timing, and edit
 * replay below are deliberately ordinary reviewed C. The same algorithm could
 * be authored in Haxe, but compiling both sides through haxe.c would make them
 * share the compiler behavior this reference is meant to calibrate. Keeping
 * the exception small makes the comparison understandable without maintaining
 * a second game or using direct C to work around a compiler limitation.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "rlgl.h"

#include "caxecraft_benchmark_scene.h"

enum
{
    WORLD_WIDTH = 32,
    WORLD_HEIGHT = 16,
    WORLD_DEPTH = 32,
    WORLD_VOLUME = WORLD_WIDTH*WORLD_HEIGHT*WORLD_DEPTH,
    WARMUP_FRAMES = 2,
    MEASURED_FRAMES = 12,
};

typedef enum VoxelFace
{
    FACE_TOP,
    FACE_BOTTOM,
    FACE_NORTH,
    FACE_SOUTH,
    FACE_EAST,
    FACE_WEST,
} VoxelFace;

typedef struct SheetCounters
{
    int visible;
    int faces;
    int draw_calls;
    uint32_t stream_hash;
} SheetCounters;

static uint8_t world_cells[WORLD_VOLUME];

static int world_index(int x, int y, int z)
{
    return x + WORLD_WIDTH*(y + WORLD_HEIGHT*z);
}

static uint8_t query_world(int x, int y, int z)
{
    if ((x < 0) || (x >= WORLD_WIDTH) ||
        (y < 0) || (y >= WORLD_HEIGHT) ||
        (z < 0) || (z >= WORLD_DEPTH))
    {
        return UINT8_C(0);
    }
    return world_cells[world_index(x, y, z)];
}

static bool is_solid(uint8_t kind)
{
    return kind != UINT8_C(0);
}

static bool belongs_to_adventure_sheet(uint8_t kind)
{
    return (kind == UINT8_C(8)) || (kind == UINT8_C(9));
}

static int square_distance(int x, int z, int player_x, int player_z)
{
    int dx = x - player_x;
    int dz = z - player_z;
    if (dx < 0) dx = -dx;
    if (dz < 0) dz = -dz;
    return (dx > dz)? dx : dz;
}

static Color face_tint(VoxelFace face, int distance)
{
    if (distance >= 22)
    {
        switch (face)
        {
            case FACE_TOP: return (Color){ 220, 235, 226, 255 };
            case FACE_SOUTH:
            case FACE_EAST: return (Color){ 197, 218, 212, 255 };
            case FACE_NORTH:
            case FACE_WEST: return (Color){ 181, 205, 204, 255 };
            case FACE_BOTTOM: return (Color){ 158, 182, 185, 255 };
        }
    }
    if (distance >= 12)
    {
        switch (face)
        {
            case FACE_TOP: return (Color){ 240, 246, 239, 255 };
            case FACE_SOUTH:
            case FACE_EAST: return (Color){ 219, 231, 222, 255 };
            case FACE_NORTH:
            case FACE_WEST: return (Color){ 199, 216, 211, 255 };
            case FACE_BOTTOM: return (Color){ 173, 190, 188, 255 };
        }
    }
    switch (face)
    {
        case FACE_TOP: return (Color){ 255, 255, 255, 255 };
        case FACE_SOUTH:
        case FACE_EAST: return (Color){ 235, 241, 230, 255 };
        case FACE_NORTH:
        case FACE_WEST: return (Color){ 211, 225, 220, 255 };
        case FACE_BOTTOM: return (Color){ 178, 191, 188, 255 };
    }
    return WHITE;
}

static void atlas_tile(uint8_t kind, VoxelFace face, int *column, int *row)
{
    *column = 2;
    *row = 0;
    switch (kind)
    {
        case 1: /* grass */
            if (face == FACE_TOP) { *column = 0; *row = 0; }
            else if (face == FACE_BOTTOM) { *column = 2; *row = 0; }
            else { *column = 1; *row = 0; }
            break;
        case 2: *column = 2; *row = 0; break; /* dirt */
        case 3: *column = 3; *row = 0; break; /* stone */
        case 4: *column = 3; *row = 3; break; /* bedrock */
        case 5: *column = 0; *row = 1; break; /* sand */
        case 6: /* wood */
            if ((face == FACE_TOP) || (face == FACE_BOTTOM))
            {
                *column = 3;
                *row = 1;
            }
            else
            {
                *column = 2;
                *row = 1;
            }
            break;
        case 7: *column = 0; *row = 2; break; /* leaves */
        case 8: /* snow */
            *column = (face == FACE_TOP)? 0 : 1;
            *row = 0;
            break;
        case 9: /* ash */
            *column = (face == FACE_TOP)? 0 : 1;
            *row = 1;
            break;
        default: break;
    }
}

static void vertex(float u, float v, float x, float y, float z)
{
    rlTexCoord2f(u, v);
    rlVertex3f(x, y, z);
}

static uint32_t mix_face_hash(
    uint32_t hash,
    uint8_t kind,
    VoxelFace face,
    int x,
    int y,
    int z)
{
    const uint32_t values[] = {
        (uint32_t)kind,
        (uint32_t)face,
        (uint32_t)x,
        (uint32_t)y,
        (uint32_t)z,
    };
    for (size_t index = 0; index < sizeof(values)/sizeof(values[0]); ++index)
    {
        hash ^= values[index];
        hash *= UINT32_C(16777619);
    }
    return hash;
}

static uint32_t emit_face(
    uint32_t hash,
    uint8_t kind,
    VoxelFace face,
    int x,
    int y,
    int z,
    float half_pixel,
    int distance)
{
    int column = 0;
    int row = 0;
    atlas_tile(kind, face, &column, &row);
    const float u0 = (float)column/4.0f + half_pixel;
    const float u1 = (float)(column + 1)/4.0f - half_pixel;
    const float v0 = (float)row/4.0f + half_pixel;
    const float v1 = (float)(row + 1)/4.0f - half_pixel;
    const Color tint = face_tint(face, distance);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    switch (face)
    {
        case FACE_TOP:
            rlNormal3f(0.0f, 1.0f, 0.0f);
            vertex(u0, v0, (float)x, (float)(y + 1), (float)z);
            vertex(u0, v1, (float)x, (float)(y + 1), (float)(z + 1));
            vertex(u1, v1, (float)(x + 1), (float)(y + 1), (float)(z + 1));
            vertex(u1, v0, (float)(x + 1), (float)(y + 1), (float)z);
            break;
        case FACE_BOTTOM:
            rlNormal3f(0.0f, -1.0f, 0.0f);
            vertex(u0, v0, (float)x, (float)y, (float)z);
            vertex(u1, v0, (float)(x + 1), (float)y, (float)z);
            vertex(u1, v1, (float)(x + 1), (float)y, (float)(z + 1));
            vertex(u0, v1, (float)x, (float)y, (float)(z + 1));
            break;
        case FACE_NORTH:
            rlNormal3f(0.0f, 0.0f, -1.0f);
            vertex(u0, v1, (float)x, (float)y, (float)z);
            vertex(u0, v0, (float)x, (float)(y + 1), (float)z);
            vertex(u1, v0, (float)(x + 1), (float)(y + 1), (float)z);
            vertex(u1, v1, (float)(x + 1), (float)y, (float)z);
            break;
        case FACE_SOUTH:
            rlNormal3f(0.0f, 0.0f, 1.0f);
            vertex(u0, v1, (float)x, (float)y, (float)(z + 1));
            vertex(u1, v1, (float)(x + 1), (float)y, (float)(z + 1));
            vertex(u1, v0, (float)(x + 1), (float)(y + 1), (float)(z + 1));
            vertex(u0, v0, (float)x, (float)(y + 1), (float)(z + 1));
            break;
        case FACE_EAST:
            rlNormal3f(1.0f, 0.0f, 0.0f);
            vertex(u0, v1, (float)(x + 1), (float)y, (float)z);
            vertex(u0, v0, (float)(x + 1), (float)(y + 1), (float)z);
            vertex(u1, v0, (float)(x + 1), (float)(y + 1), (float)(z + 1));
            vertex(u1, v1, (float)(x + 1), (float)y, (float)(z + 1));
            break;
        case FACE_WEST:
            rlNormal3f(-1.0f, 0.0f, 0.0f);
            vertex(u0, v1, (float)x, (float)y, (float)z);
            vertex(u1, v1, (float)x, (float)y, (float)(z + 1));
            vertex(u1, v0, (float)x, (float)(y + 1), (float)(z + 1));
            vertex(u0, v0, (float)x, (float)(y + 1), (float)z);
            break;
    }
    return mix_face_hash(hash, kind, face, x, y, z);
}

static void emit_if_open(
    SheetCounters *counters,
    uint8_t kind,
    VoxelFace face,
    int x,
    int y,
    int z,
    int neighbor_x,
    int neighbor_y,
    int neighbor_z,
    float half_pixel,
    int distance,
    bool *visible)
{
    if (is_solid(query_world(neighbor_x, neighbor_y, neighbor_z))) return;
    counters->stream_hash = emit_face(
        counters->stream_hash,
        kind,
        face,
        x,
        y,
        z,
        half_pixel,
        distance);
    ++counters->faces;
    *visible = true;
}

static SheetCounters draw_sheet(
    Texture2D texture,
    bool adventure,
    int player_x,
    int player_z)
{
    SheetCounters counters = { 0, 0, 0, UINT32_C(2166136261) };
    const float half_pixel = 0.5f/(float)texture.width;
    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);
    for (int z = 0; z < WORLD_DEPTH; ++z)
    {
        for (int y = 0; y < WORLD_HEIGHT; ++y)
        {
            for (int x = 0; x < WORLD_WIDTH; ++x)
            {
                const uint8_t kind = query_world(x, y, z);
                if (!is_solid(kind) || (belongs_to_adventure_sheet(kind) != adventure))
                {
                    continue;
                }
                bool visible = false;
                const int distance = square_distance(x, z, player_x, player_z);
                emit_if_open(&counters, kind, FACE_TOP, x, y, z, x, y + 1, z, half_pixel, distance, &visible);
                emit_if_open(&counters, kind, FACE_BOTTOM, x, y, z, x, y - 1, z, half_pixel, distance, &visible);
                emit_if_open(&counters, kind, FACE_NORTH, x, y, z, x, y, z - 1, half_pixel, distance, &visible);
                emit_if_open(&counters, kind, FACE_SOUTH, x, y, z, x, y, z + 1, half_pixel, distance, &visible);
                emit_if_open(&counters, kind, FACE_EAST, x, y, z, x + 1, y, z, half_pixel, distance, &visible);
                emit_if_open(&counters, kind, FACE_WEST, x, y, z, x - 1, y, z, half_pixel, distance, &visible);
                if (visible) ++counters.visible;
            }
        }
    }
    rlEnd();
    rlSetTexture(0);
    counters.draw_calls = (counters.faces > 0)? 1 : 0;
    return counters;
}

int main(void)
{
    if (sizeof(CAXECRAFT_BENCHMARK_SCENE_CELLS) != sizeof(world_cells))
    {
        (void)fprintf(stderr, "caxecraft-c-baseline: scene size mismatch\n");
        return 1;
    }
    (void)memcpy(world_cells, CAXECRAFT_BENCHMARK_SCENE_CELLS, sizeof(world_cells));
    SetTraceLogLevel(LOG_NONE);
    InitWindow(1280, 720, "Caxecraft handwritten C renderer baseline");
    const Texture2D base = LoadTexture("assets/atlases/terrain.png");
    const Texture2D adventure = LoadTexture("assets/atlases/adventure-terrain.png");
    if (!IsTextureValid(base) || !IsTextureValid(adventure))
    {
        (void)fprintf(stderr, "caxecraft-c-baseline: terrain atlas load failed\n");
        if (IsTextureValid(base)) UnloadTexture(base);
        if (IsTextureValid(adventure)) UnloadTexture(adventure);
        CloseWindow();
        return 1;
    }

    const Camera3D camera = {
        .position = { 16.519f, 7.549f, 15.900f },
        .target = { 16.519f, 6.807f, 15.230f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 70.0f,
        .projection = CAMERA_PERSPECTIVE,
    };
    double measured_seconds = 0.0;
    SheetCounters final_base = { 0 };
    SheetCounters final_adventure = { 0 };
    for (int frame = 0; frame < WARMUP_FRAMES + MEASURED_FRAMES; ++frame)
    {
        if (frame == 6) world_cells[world_index(16, 4, 13)] = UINT8_C(0);
        if (frame == 8) world_cells[world_index(16, 4, 13)] = UINT8_C(2);
        BeginDrawing();
        ClearBackground((Color){ 126, 193, 204, 255 });
        BeginMode3D(camera);
        const double started = GetTime();
        final_base = draw_sheet(base, false, 16, 15);
        final_adventure = draw_sheet(adventure, true, 16, 15);
        const double finished = GetTime();
        if (frame >= WARMUP_FRAMES) measured_seconds += finished - started;
        EndMode3D();
        EndDrawing();
    }

    UnloadTexture(adventure);
    UnloadTexture(base);
    CloseWindow();
    const int faces = final_base.faces + final_adventure.faces;
    const int visible = final_base.visible + final_adventure.visible;
    const int draw_calls = final_base.draw_calls + final_adventure.draw_calls;
    const uint32_t stream_hash = final_base.stream_hash ^ final_adventure.stream_hash;
    if ((faces != 2744) || (visible != 2430) || (draw_calls != 2) || (measured_seconds <= 0.0))
    {
        (void)fprintf(stderr, "caxecraft-c-baseline: final terrain contract drifted\n");
        return 1;
    }
    (void)printf(
        "caxecraft-c-baseline: terrainMicroseconds=%.0f measuredFrames=%d faces=%d visible=%d drawCalls=%d streamHash=%08x\n",
        measured_seconds*1000000.0,
        MEASURED_FRAMES,
        faces,
        visible,
        draw_calls,
        stream_hash);
    return 0;
}
