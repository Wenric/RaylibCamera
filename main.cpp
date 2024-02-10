#include <raylib.h>
#include <C:\raylib\raylib\src\raymath.h>

#include "scene_manager.hpp"
#include "game.cpp"
#include "win_scene.hpp"

int main() {
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Game");

    SceneManager scene_manager;
    
    // register game scene
    game game_scene;
    game_scene.SetSceneManager(&scene_manager);
    scene_manager.RegisterScene(&game_scene, 0);

    win win_scene;
    win_scene.SetSceneManager(&scene_manager);
    scene_manager.RegisterScene(&win_scene, 1);

    scene_manager.SwitchScene(0);

    while(!WindowShouldClose()) {
        Scene* active_scene = scene_manager.GetActiveScene();

        BeginDrawing();
        ClearBackground(WHITE);

        if (active_scene != nullptr) {
            active_scene->Update();
            active_scene->Draw();
        }

        EndDrawing();
    }

    Scene* active_scene = scene_manager.GetActiveScene();
    if (active_scene != nullptr) {
        active_scene->End();
    }

    ResourceManager::GetInstance()->UnloadAllTextures();
    CloseWindow();
    return 0;
}