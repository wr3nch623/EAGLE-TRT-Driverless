#include "windowmanager.h"
#include "../RayLib/raylib.h"
//#include "../BicycleController/bicyclecontroller.h"
//#include "../PController/pcontroller.h"

// Needed for sliders and stuff
#define RAYGUI_IMPLEMENTATION 
#include "../RayLib/raygui.h"

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <random>

#include <chrono>
#include <thread>

/*
TODO:
Finish main menu, since that still needs to be done

Making the UI modular, since i already use sliders in two different places.


*/



WindowManager::WindowManager(float windowWidth, float windowHeight) : windowWidth(windowWidth),
                                                            windowHeight(windowHeight)
{
    ResetWindow();
}
// TODO: Remove this constructor.
WindowManager::WindowManager(){
    this->windowWidth = 1680;
    this->windowHeight = 1050;

    

    ResetWindow();
}

WindowManager::~WindowManager(){
    //return;
    CloseWindow();
}

void WindowManager::ResetWindow(){
    
    InitWindow(this->windowWidth, this->windowHeight, "EAGLE-TRT Driverless Project");
    GuiSetStyle(DEFAULT, TEXT_SIZE, this->baseFontSize); // Set font size to 32
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

//void WindowManager::ResetBicycle(float delta, float theta, float wheelbase, float velocity){
//
//    if(this->bicycle != nullptr)
//        delete this->bicycle;
//
//    this->bicycle = new BicycleController(this->windowWidth/2, this->windowHeight/2, 
//        delta, theta, wheelbase, velocity);
//}

void WindowManager::ResetPoints(){
    this->occupied = 0;

    for (int i = 0; i < DIM; i++){
        this->points[i].x = 0;
        this->points[i].y = 0;
        if (i == 0){
            this->points[0].c = Color(RED);
            this->points[0].r = 30;
        }
        else {
            this->points[i].c = Color(WHITE);
            this->points[i].r = 2;
        }
    }
    
}

int WindowManager::Run(){
    GameState state = SECOND_TASK;
    
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0, dt = 0;
    const float baseFontSize = 32;
    bool first = true, error_msg = false, firstTaskBool = false;
    float x = screenWidth/2, y = screenHeight/2, delta = 0.6, theta = 0.6, velocity = 100, wheelbase = 50, error = 0;
    float prevDelta = delta, prevTheta = theta, prevVelocity = velocity, prevWheelbase = wheelbase;

    BicycleController *bicycle = new BicycleController(GetScreenWidth()/2, GetScreenHeight()/2, delta, theta, wheelbase, velocity);
    PController *pcont = new PController(2.5f); 


    //ResetBicycle(delta, theta, wheelbase, velocity);

    //y-=wheelbase/2;
    Color backgroundColor = {69, 6, 147, 100};
    //Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};
    ResetPoints();


    while(!WindowShouldClose()){

        switch (state)
        {
        case MAIN_MENU:
            if(this->occupied != 0)
                ResetPoints();
            state = MainMenu();
            break;

        case FIRST_TASK:
            state = FirstTask(prevDelta, prevTheta, prevVelocity, prevWheelbase, bicycle);
            break;

        case SECOND_TASK:
            state = SecondTask(prevDelta, prevTheta, prevVelocity, prevWheelbase, bicycle, pcont, error);
            break;
        
        default:
            std::cout << "no" << std::endl;
            return -1;
            break;
        }
    
    }

    return 0;
}

GameState WindowManager::MainMenu(){
    // Declaring scale and fontSize variable. The fontsize variable is computed based on scale
    // because the window was supposed to be dinamically resizable.
    float scale = 0, fontSize = 0, frameTime = 0;
    bool firstTaskBool = false, secondTaskBool = false;

    // TODO: Find more beautiful background color.
    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {this->windowWidth/7, this->windowHeight/6+50, 100, 50}, 
              secondTask = {this->windowWidth/7, this->windowHeight/6+100, 100, 50};

    Vector2 mousePosition;

    // Main game loop
    //while (!WindowShouldClose())    // Detect window close button or ESC key
    //{
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

        secondTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 50, 
                    std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};



        mousePosition = GetMousePosition();


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            // TODO: Chose a better color, white burns eyes
            ClearBackground(backgroundColor);

            DrawText("EAGLE-TRT Driverless Project", this->windowWidth/7, this->windowHeight/6, std::max(fontSize, baseFontSize), WHITE);
            //DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
            DrawRectangleRec(firstTask, LIGHTGRAY);
            DrawRectangleRec(secondTask, LIGHTGRAY);

            if(CheckCollisionPointRec(mousePosition, firstTask)){
                DrawRectangleRec(firstTask, RED);

                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    //CloseWindow();
                    //ResetWindow();
                    firstTaskBool = true;
                    //break;
                    //this->FirstTask();
                }

            }
            if(CheckCollisionPointRec(mousePosition, secondTask)){
                DrawRectangleRec(secondTask, RED);

                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    //CloseWindow();
                    //ResetWindow();
                    secondTaskBool = true;
                    //break;
                    //this->FirstTask();
                }

            }
        


            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    // /}
    

    if(firstTaskBool){
        return FIRST_TASK;
    }
    if(secondTaskBool)
        return SECOND_TASK;

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);       // Texture unloading

    //CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
	return MAIN_MENU;
}

void WindowManager::DrawPoints(){
    for(int i = 0; i < this->occupied; i++){
        DrawCircle(this->points[i].x, this->points[i].y, this->points[i].r, this->points[i].c);

    }
}

void WindowManager::SavePoint(float x, float y){
    this->points[this->occupied].x = x;
    this->points[this->occupied].y = y;

    this->occupied++;

}


GameState WindowManager::FirstTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle){

        //--------------------------------------------------------------------------------------

    //BicycleController *bicycle = new BicycleController(GetScreenWidth()/2, GetScreenHeight()/2, delta, theta, wheelbase, velocity);
    

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //---------------------------------------------------------------------------------------

    // Main game loop
    //while (!WindowShouldClose())    // Detect window close button or ESC key
    //{

        // Checking if the trajectory has exited the screen boundaries. In that case
        // it will create a messagebox showing an error.
        float delta, theta, velocity, wheel, x, y, dt, wheelbase, screenWidth, screenHeight;
        bool error_msg = false, first = true;



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
        //scale = screenWidth / 800;
        //fontSize = scale * baseFontSize;


        //firstTask = {screenWidth/7, screenHeight/7 + screenHeight/10, std::max(screenWidth/8, (float)100), std::max(screenHeight/16, (float)50)};
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(this->backgroundColor);
            //if(first){
            //    ClearBackground(this->backgroundColor);
            //    first = false;

            //    DrawCircle(x,y,30,RED);

            //}

            DrawPoints();

            //DrawPixel((int)x, (int)y, WHITE);
            DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

            SavePoint(bicycle->getX(), bicycle->getY());

            // GUI controls
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 200);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 150);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta /* RAD2DEG */), &delta, -0.5, 0.5);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -10, 10);

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
    //}

    return FIRST_TASK;
}

GameState WindowManager::SecondTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PController *pcont, float &error){

        //--------------------------------------------------------------------------------------

    //BicycleController *bicycle = new BicycleController(GetScreenWidth()/2, GetScreenHeight()/2, delta, theta, wheelbase, velocity);
    

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    //---------------------------------------------------------------------------------------

    // Main game loop
    //while (!WindowShouldClose())    // Detect window close button or ESC key
    //{

        // Checking if the trajectory has exited the screen boundaries. In that case
        // it will create a messagebox showing an error.
        float delta, theta, velocity, wheel, x, y, dt, wheelbase, screenWidth, screenHeight;
        bool error_msg = false, first = true;

       //Random engine
        std::mt19937 rng(std::random_device{}());
    
        // uniform distribution random values
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        theta = bicycle->getTheta();
        delta = bicycle->getDelta();
        velocity = bicycle->getVelocity();
        wheelbase = bicycle->getWheelBase();
        x = bicycle->getX();
        y = bicycle->getY();


        dt = GetFrameTime();


        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        if (x > screenWidth){
            x = 0;
            bicycle->setX(0);
        }

        

    
        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
         * that is responsive
         */

        //scale = screenWidth / 800;
        //fontSize = scale * baseFontSize;


        //firstTask = {screenWidth/7, screenHeight/7 + screenHeight/10, std::max(screenWidth/8, (float)100), std::max(screenHeight/16, (float)50)};
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(this->backgroundColor);
            //if(first){
            //    ClearBackground(this->backgroundColor);
            //    first = false;

            //    DrawCircle(x,y,30,RED);

            //}
            DrawText("SECONDTASK", 30, 30, 300, WHITE);
            DrawPoints();

            //DrawPixel((int)x, (int)y, WHITE);
            DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

            SavePoint(bicycle->getX(), bicycle->getY());

            // GUI controls
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 200);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 150);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta), &delta, -0.5, 0.5);
            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -10, 10);

            if(error_msg){
                GuiMessageBox((Rectangle){ 300, 40, 600, 600}, "Crash detected", "Nice, crash box tested\n\n the mechanics love you already.\n\n\n\n You're fired... \n\nLike the battery is burning get out", "Get out");
            }


            //DrawText("this IS a texture!", 360, 370, 18, GRAY);

        EndDrawing();

        float heading = 0;
        float noise = 0;

        if (this->occupied % 100 == 0){
            noise = (dist(rng) * 5);
            bicycle->setDelta(abs(dist(rng) * 7 - 0.5));
            bicycle->Step(GetFrameTime());
        }
        error = heading - (bicycle->getTheta());

        bicycle->PStep(GetFrameTime(), pcont->computeDelta(error));

        if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
            bicycle->Update(delta, theta, wheelbase, velocity);
        }

        prevTheta = theta;
        prevDelta = delta;
        prevVelocity = velocity;
        prevWheelbase = wheelbase;

        //bicycle->Step(dt);
        //----------------------------------------------------------------------------------
    //}

    return SECOND_TASK;
}


GameState WindowManager::ThirdTask(){
    
}




//GameState WindowManager::SecondTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PController *pcont, float &error){
//
//    //--------------------------------------------------------------------------------------
//    //float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0, error = 0, targetHeading = 0, yerror = 0;
//    //const float baseFontSize = 32;
//    ///bool first = true, error_msg = false;
//    //float x = screenWidth/2, y = screenHeight/2, delta = 0, theta = 0, velocity = 50, wheelbase = 100;
//    //float prevDelta = delta, prevTheta = theta, prevVelocity = velocity, prevWheelbase = wheelbase;
//    //int i = 1, pos = 0;
//    //Point a[DIM];
//    
//
//    float delta, theta, velocity, wheel, x, y, dt, wheelbase, screenWidth, screenHeight;
//        bool error_msg = false, first = true;
//    
//    // Random engine
//    std::mt19937 rng(std::random_device{}());
//
//    // uniform distribution random values
//    std::uniform_real_distribution<double> dist(0.0, 1.0);
//
//    //Color backgroundColor = {69, 6, 147, 100};
//    //Rectangle firstTask = {screenWidth/7, screenHeight/6+50, 100, 50};
//    //BicycleController *bicycle = new BicycleController(x, y, delta, theta, wheelbase, velocity);
//    //PController *pcont = new PController(2.5f);
//    
//       
//
//    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
//
//    //---------------------------------------------------------------------------------------
//
//    // Main game loop
//    //while (!WindowShouldClose())    // Detect window close button or ESC key
//    //{
//
//        // Checking if the trajectory has exited the screen boundaries. In that case
//        // it will create a messagebox showing an error.
//        
//        if (bicycle->getX() < 0 || bicycle->getX() > GetScreenWidth()){
//            bicycle->setX(0);
//            x = 0;
//        }
//
//
//        theta = bicycle->getTheta();
//        delta = bicycle->getDelta();
//        velocity = bicycle->getVelocity();
//        wheelbase = bicycle->getWheelBase();
//        x = bicycle->getX();
//        y = bicycle->getY();
//
//        prevTheta = theta;
//        prevDelta = delta;
//        prevVelocity = velocity;
//        prevWheelbase = wheelbase;
//
//        //if (y < 0 || y > GetScreenHeight()){
//        //    error_msg = true;
//        //}
//        
//
//    
//        /* Getting current screen dimentions, calculate scaling based on width in order to have a window
//         * that is responsive
//         */
//        screenWidth = GetScreenWidth();
//        screenHeight = GetScreenHeight();
//        //scale = screenWidth / 800;
//        //fontSize = scale * baseFontSize;
//
//
//        // Update
//        //----------------------------------------------------------------------------------
//        // TODO: Update your variables here
//        //----------------------------------------------------------------------------------
//
//        // Draw
//        //----------------------------------------------------------------------------------
//        BeginDrawing();
//            // TODO: Chose a better color, white burns eyes
//            ClearBackground(backgroundColor);
//            //if(first){
//                //ClearBackground(backgroundColor);
//            //    first = false;
//
//            //    DrawCircle(x,y,30,RED);
//
//            //}
//
//            //Insert drawcircle function of the class
//
//            //for (int t = 0; t < pos; t++){
//            //    DrawCircle(a[t].x, a[t].y, a[t].r, a[t].c);
//            //}
//
//            DrawPoints();
//
//
//            //DrawPixel((int)x, (int)y, WHITE);
//            DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);
//
//            SavePoint(bicycle->getX(), bicycle->getY());
//
//            //a[pos].x = bicycle->getX();
//            //a[pos].y = bicycle->getY();
//            //pos++;
//
//            // GUI controls
//            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 200);
//            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 150);
//            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta), &delta, -0.5, 0.5);
//            GuiSliderBar((Rectangle){screenWidth - screenWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -3.14, 3.14);
//
//
//            //DrawText("this IS a texture!", 360, 370, 18, GRAY);
//
//        EndDrawing();
//        std::cout << error << std::endl;
//        std::cout << this->occupied << std::endl;
//
//
//        float targetHeading = 0;
//
//        
//        //bicycle->Step(GetFrameTime());
//        if (this->occupied%1 == 0){
//            //float r = dist(rng) * 2 - 0.5;
//            bicycle->setDelta(abs(dist(rng) * 10 - 0.5));
//            //bicycle->setDelta(r);
//            //bicycle->setTheta(r);
//            //bicycle->Step(GetFrameTime());
//            std::cout << "delta : " << bicycle->getDelta() << std::endl;
//        }
//
//        error = targetHeading - bicycle->getTheta();
//
//
//        //bicycle->PStep(GetFrameTime(), pcont->computeDelta(error));
//        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//                
//
//        if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
//            bicycle->Update(delta, theta, wheelbase, velocity);
//        }
//        
//        //std::cout << error << std::endl;
//        x++;
//
//        bicycle->PStep(GetFrameTime(), pcont->computeDelta(error));
//
//
//        //----------------------------------------------------------------------------------
//    //}
//
//    //CloseWindow();         
//
//    //return 0;
//
//    return SECOND_TASK;
//}
//