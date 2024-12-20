#include "raylib.h"
#include "raymath.h"
#include "render.h"
#include "world.h"

static Vector2 get_input_axis()
{
    Vector2 input = {0};

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_Z))
        input.y -= 1;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        input.y += 1;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        input.x -= 1;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        input.x += 1;
    return input;
}

static void drag_camera(Camera2D *camera)
{
    static bool dragging = false;
    static Vector2 drag_start = {0};
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), *camera);
    Vector2 diff = {0};

    if (!IsMouseButtonDown(MOUSE_MIDDLE_BUTTON) && !IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        dragging = false;
        return;
    }
    if (!dragging) {
        dragging = true;
        drag_start = mouse;
    }
    diff = Vector2Scale(Vector2Subtract(drag_start, mouse), camera->zoom);
    camera->offset = Vector2Subtract(camera->offset, diff);
}

static void update_zoom(Camera2D *camera)
{
    float wheel = GetMouseWheelMove();
    Vector2 mouse_world_pos = {0};
    float scale_factor = 0;

    if (wheel == 0)
        return;
    mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), *camera);
    camera->offset = GetMousePosition();
    camera->target = mouse_world_pos;
    scale_factor = 1.0f + (CAMERA_ZOOM_STEP * fabsf(wheel));
    if (wheel < 0)
        scale_factor = 1.0f / scale_factor;
    camera->zoom = Clamp(camera->zoom * scale_factor, CAMERA_MIN_ZOOM, CAMERA_MAX_ZOOM);
}

void renderer_center_camera(renderer_t *renderer, world_t *world)
{
    Vector2 center = {0};
    Vector2 center_px = {0};
    Vector2 screen = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    center = (Vector2){(float)world->size / 2, (float)world->size / 2};
    center_px = Vector2Scale(center, (float)renderer->settings.tile_size_px);
    renderer->camera.target = center_px;
    renderer->camera.offset = screen;
}

void renderer_update_camera(Camera2D *camera)
{
    Vector2 velocity = Vector2Scale(Vector2Scale(get_input_axis(), CAMERA_MOVE_SPEED), GetFrameTime() / camera->zoom);

    if (velocity.x != 0 || velocity.y != 0) {
        camera->target = Vector2Add(camera->target, velocity);
    } else {
        drag_camera(camera);
    }
    update_zoom(camera);
}
