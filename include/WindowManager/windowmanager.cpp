#include "windowmanager.h"
#include "../RayLib/raylib.h"

#include <stdlib.h>
#include <iostream>
#include <cmath>

int WindowManager::DrawCycle(){
     // Initialization
    //--------------------------------------------------------------------------------------
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0;
    const float baseFontSize = 32;

    // TODO: Find more beautiful background color.
    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};

    // making the window resizable and initialize it
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "EAGLE-TRT Driverless Project");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
    
        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
         * that is responsive
         */
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        scale = screenWidth / 800;
        fontSize = scale * baseFontSize;

        firstTask = {screenWidth/7, screenHeight/7 + screenHeight/10, std::max(screenWidth/8, (float)100), std::max(screenHeight/16, (float)50)};
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // TODO: Chose a better color, white burns eyes
            ClearBackground(backgroundColor);

            DrawText("EAGLE-TRT Driverless Project", screenWidth/7, screenHeight/6, std::max(fontSize, baseFontSize), WHITE);
            //DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
            DrawRectangleRec(firstTask, LIGHTGRAY);


            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);       // Texture unloading

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return 0;
}


int WindowManager::FirstTask(){

        //--------------------------------------------------------------------------------------
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0;
    const float baseFontSize = 32;

    float x = screenWidth/2, y = screenHeight/2, delta = 0.6, theta = 0.6, velocity = 100, wheelbase = 50;

    // TODO: Find more beautiful background color.
    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};

    // making the window resizable and initialize it
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "EAGLE-TRT Driverless Project");
    bool first = true;

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        x = x + (velocity * cos(theta)) * (GetFrameTime());
        y = y + (velocity * sin(theta)) * (GetFrameTime());
        theta = theta + ((velocity/wheelbase) * tan(delta)) * (GetFrameTime());
    
        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
         * that is responsive
         */
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        scale = screenWidth / 800;
        fontSize = scale * baseFontSize;

        firstTask = {screenWidth/7, screenHeight/7 + screenHeight/10, std::max(screenWidth/8, (float)100), std::max(screenHeight/16, (float)50)};
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // TODO: Chose a better color, white burns eyes
            if(first){
                ClearBackground(backgroundColor);
                first = false;

            }


            //DrawPixel((int)x, (int)y, WHITE);
            DrawCircle(x, y, 2, WHITE);

            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);       // Texture unloading

    CloseWindow();         

    return 0;
}