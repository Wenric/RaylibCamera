#include <raylib.h>
#include <raymath.h>

int main() {
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    const float FPS = 60;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ChuaColladoParalejas_Homework01");
    SetTargetFPS(FPS);
    float min_speed = 50;
    float max_speed = 150;
    float speed_x = 0, speed_y = 0;
    float background_width = 8436, background_height = 1544;

    Camera2D camera_view = { 0 };
    camera_view.zoom = 1.0f;
    camera_view.offset = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    camera_view.target = {0,0};

    Rectangle invis_box = { -250, -200, 500, 400 };

    SetMousePosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    Texture background = LoadTexture("back.png");

    float EDGE_X[2], EDGE_Y[2];

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        EDGE_X[0] = -background_width/2 + 400;
        EDGE_X[1] =  background_width/2 - 400;
        EDGE_Y[0] = -background_height/2 + 300;
        EDGE_Y[1] = background_height/2 - 300;

        // movement
        if (IsCursorOnScreen()) {
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera_view);

            // calc direction
            Vector2 center = { invis_box.x + invis_box.width / 2, invis_box.y + invis_box.height / 2 };
            Vector2 direction = Vector2Subtract(mouse_pos, center);
            direction = Vector2Normalize(direction);

            float distance_x = mouse_pos.x - center.x;
            float distance_y = mouse_pos.y - center.y;

            // Calculate speed based on the distance from the center
            speed_x = fabsf(distance_x / (invis_box.width / 2) * max_speed);
            speed_x = Clamp(speed_x, min_speed, max_speed);

            speed_y = fabsf(distance_y / (invis_box.height / 2) * max_speed);
            speed_y = Clamp(speed_y, min_speed, max_speed);

            camera_view.target.x += direction.x * speed_x * delta;
            camera_view.target.y += direction.y * speed_y * delta;

            camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0], EDGE_X[1]);
            camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0], EDGE_Y[1]);
        }

        // Update camera target to follow the invisible box
        invis_box.x = camera_view.target.x - 250;
        invis_box.y = camera_view.target.y - 200;

        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera_view);
        DrawTexture(background, -background_width/2, -background_height/2, WHITE);
        DrawRectangleLinesEx(invis_box, 3, RED);
        EndMode2D();
        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}
