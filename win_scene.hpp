#include <raylib.h>
#include <iostream>
#include <string>

#include "scene_manager.hpp"

// Assuming the score variable is declared in a different file

class win : public Scene {
    
public:
    void Begin() override {}
    void Update() override {}
    void End() override {}
    void Draw() override {
        // Draw "Game Over!" in the middle-top of the screen
        DrawText("You Win(?)", WINDOW_WIDTH / 2 - MeasureText("You Win(?)", 25), WINDOW_HEIGHT / 2 - MeasureText("You Win(?)", 25), 50, RED);

        //draw text box
    }
};

