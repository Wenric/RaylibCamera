#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "scene_manager.hpp"

struct objects {

    std::string name;
    Vector2 position;
    Vector2 size; 
    float opacity;
};

class game : public Scene {

    
objects spawn[5];
const float FPS = 60;
const float min_speed = 50, max_speed = 300;
float speed_x = 0, speed_y = 0;
float background_width = 8436, background_height = 1544;

Rectangle invis_box = { -250, -200, 500, 400 };
Vector2 mouse_pos;

enum camera_mode { free_mode, locked_mode};

std::string background_name;
Texture background;
Camera2D camera_view = { 0 };
camera_mode current_mode = free_mode;

public: 
    void Begin() override{

        SetTargetFPS(FPS);
        SetMousePosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        camera_view.zoom = 1.0f;
        camera_view.offset = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
        camera_view.target = { 0, 0};

        //Input the file to the program 
        std::ifstream settings_file("settings.txt");

        //Instantiate variables needed for: 

        std::string line; //checking the content of the current line
        int lineNumber = 1; //checking which line the loop is currently at
        int obj_count = 0; //checking which objects child is currently active

        while (std::getline(settings_file, line)) {
        
            size_t eq = line.find("=");
            std::string clean = line.substr(eq + 1);

            // Check if the current line number is the one you want to process
            
            if (lineNumber == 1) {
                // Process the line as needed for background image
                sscanf(clean.c_str(), "%s", background_name);

            }

            if (lineNumber >= 2 && lineNumber <= 6) {
                // Process the line as needed for objects
                sscanf(clean.c_str(), "%s, {%f, %f}, {%f, %f}, %f", spawn[obj_count].name, spawn[obj_count].position.x, spawn[obj_count].position.y, spawn[obj_count].size.x, spawn[obj_count].size.y, spawn[obj_count].opacity);
                obj_count++;

            }

            //if (lineNumber == 7) {
                // Process the line as needed for edge_min 

            //}

            //if (lineNumber == 8) {
                // Process the line as needed for edge_max 

            //}

            if (lineNumber == 9) {
                // Process the line as needed for camera window
                sscanf(clean.c_str(), "%f, %f", &WINDOW_WIDTH, &WINDOW_HEIGHT);
                
            }

            //if (lineNumber == 10) {
                // Process the line as needed for camera drift

            //}

            // Increment the line number
            lineNumber++;
        }

        background = LoadTexture(background_name.c_str());

    }

    void Update() override{

        float delta = GetFrameTime();

        // define edge boundaries
        float EDGE_X[2], EDGE_Y[2];
        EDGE_X[0] = -background_width/2 + WINDOW_WIDTH/2;
        EDGE_X[1] =  background_width/2 - WINDOW_WIDTH/2;
        EDGE_Y[0] = -background_height/2 + WINDOW_HEIGHT/2;
        EDGE_Y[1] = background_height/2 - WINDOW_HEIGHT/2;

        if (IsKeyDown(KEY_A)){
            GetSceneManager()->SwitchScene(1);
        }

        switch (current_mode) {
            case free_mode:
                camera_view.zoom = 1.0f;

                if (IsCursorOnScreen()) {
                    mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera_view);

                    // calc direction and distance
                    Vector2 center = { invis_box.x + invis_box.width / 2, invis_box.y + invis_box.height / 2 };
                    Vector2 direction = Vector2Normalize(Vector2Subtract(mouse_pos, center));
                    float distance_x = mouse_pos.x - center.x;
                    float distance_y = mouse_pos.y - center.y;

                    // calc speed based on distance from center and clamped to min,max speed
                    speed_x = fabsf(distance_x / (invis_box.width / 2) * max_speed);
                    speed_x = Clamp(speed_x, min_speed, max_speed);

                    speed_y = fabsf(distance_y / (invis_box.height / 2) * max_speed);
                    speed_y = Clamp(speed_y, min_speed, max_speed);

                    camera_view.target.x += direction.x * speed_x * delta;
                    camera_view.target.y += direction.y * speed_y * delta;

                }

                if (IsMouseButtonPressed(0)) {
                    mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera_view);
                    current_mode = locked_mode;
                }
                camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0], EDGE_X[1]);
                camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0], EDGE_Y[1]);
                break;
                
            case locked_mode:
                camera_view.target = mouse_pos;
                camera_view.zoom = 1.5f;
                // if (IsMouseButtonPressed(0)) {
                //     //select
                // }
                if (IsMouseButtonPressed(1)) {
                    camera_view.target = mouse_pos;
                    camera_view.zoom = 1.5f;
                    current_mode = free_mode;   
                }

                camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0] - WINDOW_WIDTH/6, EDGE_X[1] + WINDOW_WIDTH/6);
                camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0] - WINDOW_HEIGHT/6, EDGE_Y[1] + WINDOW_WIDTH/6);
                break;

            default:
                break;

        }

        //clamp to edges

        
        // invis box follows camera movement offset to center
        invis_box.x = camera_view.target.x - 250;
        invis_box.y = camera_view.target.y - 200;

    }

// read settings.txt file
// std::ifstream settings_file("settings.txt");
// std::string background_name;

    void End() override {
        
    }
    void Draw() override {
        
        BeginMode2D(camera_view);
        DrawTexture(background, -background_width/2, -background_height/2, WHITE);
        if (current_mode == free_mode) {
            DrawRectangleLinesEx(invis_box, 3, RED);
        }

        //DrawTextEx(custom_font, TextFormat("Punch Cooldown: %.1f", player.charge_cooldown), {(WINDOW_WIDTH - score_size.x) / 2 + 150, 10}, header_size, 1, BLACK);

        EndMode2D();

    }    
};