#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include "scene_manager.hpp"

struct objects {
    std::string name;
    Vector2 position;
    Vector2 size; 
    Color color;
    Vector2 textposition;
    bool is_selected;
};

class game : public Scene {

    
objects spawn[5];
const float FPS = 60;
float min_speed = 50, max_speed = 1000;
float speed_x = 0, speed_y = 0;
float background_width = 8436, background_height = 1544;
float EDGE_X[2], EDGE_Y[2];

Rectangle invis_box = { -250, -200, 500, 400 };
Vector2 mouse_pos;

enum camera_mode { free_mode, locked_mode};

std::string background_name;
Texture background;
Camera2D camera_view = { 0 };
camera_mode current_mode = free_mode;
int goal_number = 0;

public: 
    void Begin() override{

        SetTargetFPS(FPS);
        SetMousePosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

        camera_view.zoom = 1.0f;
        camera_view.offset = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
        camera_view.target = { 0, 0};

        std::ifstream settings_file("settings.txt");

        

        std::string line; //checking the content of the current line
        int lineNumber = 1; //checking which line the loop is currently at
        int obj_count = 0; //checking which objects child is currently active
    

        while (std::getline(settings_file, line)) {
        
            size_t eq = line.find("=");
            std::string clean = line.substr(eq + 2);

            // Check if the current line number is the one you want to process
            
            if (lineNumber == 1) {
                // Process the line as needed for background image
                background_name = clean;

            }

            if (lineNumber >= 2 && lineNumber <= 6) {
                // Process the line as needed for objects
                
                std::istringstream iss(clean);
                iss >> spawn[obj_count].name >> spawn[obj_count].position.x >> spawn[obj_count].position.y >> spawn[obj_count].size.x >> spawn[obj_count].size.y >> spawn[obj_count].textposition.x >> spawn[obj_count].textposition.y;
                spawn[obj_count].color = BLACK;
                spawn[obj_count].is_selected = false;
                obj_count++;

            }

            if (lineNumber == 7) {
                // Process the line as needed for edge_min 

                std::istringstream iss(clean);
                iss >> EDGE_X[0] >> EDGE_Y[0];

            }

            if (lineNumber == 8) {
                // Process the line as needed for edge_max 
                
                std::istringstream iss(clean);
                iss >> EDGE_X[1] >> EDGE_Y[1];
            
            }

            if (lineNumber == 9) {
                // Process the line as needed for camera window
                sscanf(clean.c_str(), "%f, %f", &WINDOW_WIDTH, &WINDOW_HEIGHT);
                
            }

            if (lineNumber == 10) {
                // Process the line as needed for camera drift
                
                std::istringstream iss(clean);
                iss >> max_speed;
            
            }

            // Increment the line number
            lineNumber++;
        }

        background = LoadTexture(background_name.c_str());

    }

    void Update() override{

        if (goal_number == 5) {
            GetSceneManager()->SwitchScene(1);
        }

        float delta = GetFrameTime();

        // define edge boundaries
    
        EDGE_X[0] = -background_width/2 + WINDOW_WIDTH/2;
        EDGE_X[1] =  background_width/2 - WINDOW_WIDTH/2;
        EDGE_Y[0] = -background_height/2 + WINDOW_HEIGHT/2;
        EDGE_Y[1] = background_height/2 - WINDOW_HEIGHT/2;

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
                camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0] - WINDOW_WIDTH/6, EDGE_X[1] + WINDOW_WIDTH/6);
                camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0] - WINDOW_HEIGHT/6, EDGE_Y[1] + WINDOW_HEIGHT/6);
                if (IsMouseButtonPressed(0)) {

                    for (int i = 0; i < 5; i++) {
                        
                        if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera_view), { spawn[i].position.x, spawn[i].position.y, spawn[i].size.x, spawn[i].size.y }) && !spawn[i].is_selected) {
                            spawn[i].is_selected = true;
                            spawn[i].color = RED;
                            goal_number++;
                        }
                    }
                }
                if (IsMouseButtonPressed(1)) {
                    camera_view.target = mouse_pos;
                    camera_view.zoom = 1.5f;
                    current_mode = free_mode;   
                }

                
                break;

            default:
                break;

        }
        
        // invis box follows camera movement offset to center
        invis_box.x = camera_view.target.x - 250;
        invis_box.y = camera_view.target.y - 200;

    }

    void End() override {
        
    }
    void Draw() override {

        BeginMode2D(camera_view);

        for (int i = 0; i < 5; i++) {
            DrawRectangleV(spawn[i].position, spawn[i].size, RED);
        };

        DrawTexture(background, -background_width/2, -background_height/2, {255, 255, 255, 255});
        if (current_mode == free_mode) {
            DrawRectangleLinesEx(invis_box, 3, RED);
        }


        EndMode2D();   

        for (int i = 0; i < 5; i++) {
            DrawText(spawn[i].name.c_str(), spawn[i].textposition.x, spawn[i].textposition.y, 30, spawn[i].color);
        }
    }
};