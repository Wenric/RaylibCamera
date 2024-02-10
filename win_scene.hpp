#include <raylib.h>
#include <iostream>
#include <string>

#include "scene_manager.hpp"

// Assuming the score variable is declared in a different file

class win : public Scene {
    
public:
    void Begin() override {
        custom_font = LoadFontEx("Geist.otf", 64, 0, 0.5);
        SetTextureFilter(custom_font.texture, TEXTURE_FILTER_BILINEAR);
    }
    void Update() override {}
    void End() override {}
    void Draw() override {
        // Draw "Game Over!" in the middle-top of the screen
        DrawTextEx(custom_font, "Faggots", {(WINDOW_WIDTH - MeasureText("Faggots", body_size)) / 2, 100}, body_size, 1, RED);

        //draw text box
    }
};

