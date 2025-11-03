#include "windowmanager.h"
#include "../RayLib/raylib.h"
#include "../BicycleController/bicyclecontroller.h"
#include "../PController/pcontroller.h"

// Needed for sliders and stuff
#define RAYGUI_IMPLEMENTATION 
#include "../RayLib/raygui.h"

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>

struct Point{
    float x, y;
    int r;
    Color c;
};
#define DIM 50000

WindowManager::WindowManager(float windowWidth, float windowHeight) : windowWidth(windowWidth),
                                                            windowHeight(windowHeight)
{
    InitWindow(this->windowWidth, this->windowHeight, "EAGLE-TRT Driverless Project");
    GuiSetStyle(DEFAULT, TEXT_SIZE, this->baseFontSize); // Set font size to 32
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

}

WindowManager::WindowManager(){
    this->windowWidth = 1680;
    this->windowHeight = 1050;

    InitWindow(this->windowWidth, this->windowHeight, "EAGLE-TRT Driverless Project");
    GuiSetStyle(DEFAULT, TEXT_SIZE, this->baseFontSize); // Set font size to 32
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

WindowManager::~WindowManager(){
    //return;
    CloseWindow();
}


int WindowManager::MainMenu(){
    // Declaring scale and fontSize variable. The fontsize variable is computed based on scale
    // because the window was supposed to be dinamically resizable.
    float scale = 0, fontSize = 0, frameTime = 0;

    // TODO: Find more beautiful background color.
    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {this->windowWidth/7, this->windowHeight/6+50, 100, 50};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        frameTime = GetFrameTime();
    
        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
         * that is responsive
         */
        this->windowWidth = GetScreenWidth();
        this->windowHeight = GetScreenHeight();
        scale = this->windowWidth / 800;
        fontSize = scale * this->baseFontSize;

        firstTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10, 
                    std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // TODO: Chose a better color, white burns eyes
            ClearBackground(backgroundColor);

            DrawText("EAGLE-TRT Driverless Project", this->windowWidth/7, this->windowHeight/6, std::max(fontSize, baseFontSize), WHITE);
            //DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
            DrawRectangleRec(firstTask, LIGHTGRAY);


            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);       // Texture unloading

    //CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return 0;
}


int WindowManager::FirstTask(){

        //--------------------------------------------------------------------------------------
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0, dt = 0;
    const float baseFontSize = 32;
    bool first = true, error_msg = false;
    float x = screenWidth/2, y = screenHeight/2, delta = 0, theta = 0, velocity = 30, wheelbase = 100;
    float prevDelta = delta, prevTheta = theta, prevVelocity = velocity, prevWheelbase = wheelbase;


    //y-=wheelbase/2;
    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};
    BicycleController *bicycle = new BicycleController(GetScreenWidth()/2, GetScreenHeight()/2, delta, theta, wheelbase, velocity);
    

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Checking if the trajectory has exited the screen boundaries. In that case
        // it will create a messagebox showing an error.



        theta = bicycle->getTheta();
        delta = bicycle->getDelta();
        velocity = bicycle->getVelocity();
        wheelbase = bicycle->getWheelBase();
        x = bicycle->getX();
        y = bicycle->getY();


        dt = GetFrameTime();

        if (x < 0 || x > GetScreenWidth()){
            error_msg = true;
        }

        if (y < 0 || y > GetScreenHeight()){
            error_msg = true;
        }
        

        

    
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
    
            if(first){
                ClearBackground(backgroundColor);
                first = false;

                DrawCircle(x,y,30,RED);

            }


            //DrawPixel((int)x, (int)y, WHITE);
            DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);


            // GUI controls
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 200);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 150);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta * RAD2DEG), &delta, -0.5, 0.5);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -3.14, 3.14);

            if(error_msg){
                GuiMessageBox((Rectangle){ 300, 40, 600, 600}, "Crash detected", "Nice, crash box tested\n\n the mechanics love you already.\n\n\n\n You're fired... \n\nLike the battery is burning get out", "Get out");
            }


            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();


        if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
            bicycle->Update(delta, theta, wheelbase, velocity);
        }

        prevTheta = theta;
        prevDelta = delta;
        prevVelocity = velocity;
        prevWheelbase = wheelbase;

        bicycle->Step(dt);
        //----------------------------------------------------------------------------------
    }

    CloseWindow();    
    delete bicycle;     

    return 0;
}



int WindowManager::SecondTask(){

    //--------------------------------------------------------------------------------------
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0, error = 0, targetHeading = 0, yerror = 0;
    const float baseFontSize = 32;
    bool first = true, error_msg = false;
    float x = screenWidth/2, y = screenHeight/2, delta = 0, theta = 0, velocity = 50, wheelbase = 100;
    float prevDelta = delta, prevTheta = theta, prevVelocity = velocity, prevWheelbase = wheelbase;
    int i = 1, pos = 0;
    Point a[DIM];

    for (int t = 0; t < DIM; t++){
        a[t].x = 0;
        a[t].y = 0;
        if (t == 0){
            a[t].c = Color(RED);
            a[t].r = 30;
        }
        else {
            a[t].c = Color(WHITE);
            a[t].r = 2;
        }
    }
    
    // Random engine
    std::mt19937 rng(std::random_device{}());

    // uniform distribution random values
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    


    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};
    BicycleController *bicycle = new BicycleController(x, y, delta, theta, wheelbase, velocity);
    PController *pcont = new PController(2.5f);
    
       

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Checking if the trajectory has exited the screen boundaries. In that case
        // it will create a messagebox showing an error.
        
        if (bicycle->getX() < 0 || bicycle->getX() > GetScreenWidth()){
            bicycle->setX(0);
            x = 0;
        }

        if (DIM == pos) {
            pos = 0;
        }

        theta = bicycle->getTheta();
        delta = bicycle->getDelta();
        velocity = bicycle->getVelocity();
        wheelbase = bicycle->getWheelBase();

        prevTheta = theta;
        prevDelta = delta;
        prevVelocity = velocity;
        prevWheelbase = wheelbase;

        //if (y < 0 || y > GetScreenHeight()){
        //    error_msg = true;
        //}
        

    
        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
         * that is responsive
         */
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        scale = screenWidth / 800;
        fontSize = scale * baseFontSize;


        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // TODO: Chose a better color, white burns eyes
            ClearBackground(backgroundColor);
            if(first){
                //ClearBackground(backgroundColor);
                first = false;

                DrawCircle(x,y,30,RED);
                a[pos].x = x;
                a[pos].y = y;
                pos++;

            }

            for (int t = 0; t < pos; t++){
                DrawCircle(a[t].x, a[t].y, a[t].r, a[t].c);
            }


            //DrawPixel((int)x, (int)y, WHITE);
            DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);
            a[pos].x = bicycle->getX();
            a[pos].y = bicycle->getY();
            pos++;

            // GUI controls
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 720);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 100);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta), &delta, -10, 10);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -10, 10);

            

            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();

        
        bicycle->PStep(GetFrameTime(), pcont->computeDelta(error));
        //bicycle->Step(GetFrameTime());
        if (i%50 == 0){
            bicycle->setDelta(abs(dist(rng) * 2 - 0.5));
            i = 0;
        }
        i++;
        //targetHeading = tan(bicycle->getTheta());

        error = targetHeading - bicycle->getTheta();

        if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
            bicycle->Update(delta, theta, wheelbase, velocity);
        }
        
        std::cout << error << std::endl;
        x++;

        //----------------------------------------------------------------------------------
    }

    CloseWindow();         

    return 0;
}