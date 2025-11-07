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
    CreateWindow();
}
// TODO: Remove this constructor.
WindowManager::WindowManager(){
    /* 
        Default resolution. Now reduced because the pc i used for coding decided to not work yesterday
        for a bit so it was reduced in the main.cpp file
    */
    this->windowWidth = 1680;
    this->windowHeight = 1050;

    

    CreateWindow();
}

WindowManager::~WindowManager(){
    //return;
    CloseWindow();
}
/* 
    Creates the RayLib window, sets the default fontsize and target fps. In origin it was planned to
    make it resizable, the code should support it but time was a problem during this project,
    since a lot has happened in 14 days.
*/
void WindowManager::CreateWindow(){
    
    InitWindow(this->windowWidth, this->windowHeight, "EAGLE-TRT Driverless Project");
    GuiSetStyle(DEFAULT, TEXT_SIZE, this->baseFontSize); // Set font size to 32
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

// Resets memorized points for drawing inside the tasks.
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

// Function that run the entire project, based on the state it renders the correct task.
int WindowManager::Run(){
    GameState state = MAIN_MENU;
    
    //remove everything i believe
    float screenWidth = 1680, screenHeight = 1050, scale = 0, fontSize = 0, dt = 0;
    const float baseFontSize = 32;
    //remove this
    bool first = true, error_msg = false, firstTaskBool = false;
    //remove this
    float x = screenWidth/2, y = screenHeight/2, delta = 0.6, theta = 0.6, velocity = 30, wheelbase = 50, error = 0, errory = 0;
    // remove this
    float prevDelta = delta, prevTheta = theta, prevVelocity = velocity, prevWheelbase = wheelbase;

    BicycleController *bicycle = new BicycleController(GetScreenWidth()/2, GetScreenHeight()/2, delta, theta, wheelbase, velocity);
    PController *pcont = new PController(2.5f); 
    PController *pcontHeadDir = new PController(2.5f, 0.1f); 
    PIDController *pidcont = new PIDController(1.8f, 0.1f, 0.7f, 1.0f);
    StanleyController *scont = new StanleyController(1.8f);

    Color backgroundColor = {69, 6, 147, 100};

    ResetPoints();


    while(!WindowShouldClose()){
        /*
            While the window is opened, draw whatever task you're in at the moment.

            TODO: close the window using also a button somewhere.
        */

        // Updating window dimentions cause resizable window will come in the future
        this->windowWidth = GetScreenWidth();
        this->windowHeight = GetScreenHeight();

        // state based window. This decides which screen to show based on user actions.
        switch (state)
        {
            case MAIN_MENU:
                // Variable reset
                if(this->occupied != 0){
                    ResetPoints();
                    bicycle->setX(this->windowWidth/2);
                    bicycle->setY(this->windowHeight/2);
                    bicycle->Update(delta, theta, wheelbase, velocity);
                    error = 0;
                    errory = 0;
                }
                state = MainMenu();
                break;

            case FIRST_TASK:
                state = FirstTask(bicycle);
                break;

            case SECOND_TASK:
                state = SecondTask(bicycle, pcont, error);
                break;

            case THIRD_TASK:
                state = ThirdTask(bicycle, pcontHeadDir, error, errory);
                break;

            case FOURTH_TASK:
                state = FourthTask(bicycle, pidcont, error, errory);
                break;

            case FIFTH_TASK:
                state = FifthTask(bicycle, scont, error, errory);
                break;
            
            
            default:
                std::cout << "You should not be here." << std::endl;
                return -1;
                break;
            }
    
    }

    return 0;
}

// Main menu function, it draws the main menu to execute the code for each task.
GameState WindowManager::MainMenu(){
    // Declaring scale and fontSize variable. The fontsize variable is computed based on scale
    // because the window was supposed to be dinamically resizable.
    float scale = 0, fontSize = 0;
    //frameTime = 0;
    bool firstTaskBool = false, secondTaskBool = false, thirdTaskBool = false, fourthTaskBool = false, fifthTaskBool = false;

    Color backgroundColor = {69, 6, 147, 100};
    Rectangle firstTask = {this->windowWidth/7, this->windowHeight/6+50, 100, 50}, 
              secondTask = {this->windowWidth/7, this->windowHeight/6+100, 100, 50},
              thirdTask = {this->windowWidth/7, this->windowHeight/6+100, 100, 50},
              fourthTask = {this->windowWidth/7, this->windowHeight/6+100, 100, 50},
              fifthTask = {this->windowWidth/7, this->windowHeight/6+100, 100, 50};;

    Vector2 mousePosition;


    //frameTime = GetFrameTime();

    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */

    scale = this->windowWidth / 800;
    fontSize = scale * this->baseFontSize;

    firstTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10, 
                std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};

    secondTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 50, 
                std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};

    thirdTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 150, 
                std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};

    fourthTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 250, 
                std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};
    fifthTask = {this->windowWidth/7, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 350, 
                std::max(this->windowWidth/8, (float)100), std::max(this->windowHeight/16, (float)50)};


    mousePosition = GetMousePosition();


    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(backgroundColor);

        // Draw text and rectangles to switch task 
        DrawText("EAGLE-TRT Driverless Project", this->windowWidth/7, this->windowHeight/6, std::max(fontSize, baseFontSize), WHITE);
        DrawRectangleRec(firstTask, LIGHTGRAY);
        DrawText("First task", this->windowWidth/7 + 20, this->windowHeight/7 + this->windowHeight/10 + 30, 20, BLACK);

        DrawRectangleRec(secondTask, LIGHTGRAY);
        DrawText("Second task", this->windowWidth/7 + 20, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 80, 20, BLACK);

        DrawRectangleRec(thirdTask, LIGHTGRAY);
        DrawText("Third task", this->windowWidth/7 + 20, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 180, 20, BLACK);

        DrawRectangleRec(fourthTask, LIGHTGRAY);
        DrawText("Fourth task", this->windowWidth/7 + 20, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 280, 20, BLACK);

        DrawRectangleRec(fifthTask, LIGHTGRAY);
        DrawText("Fifth task", this->windowWidth/7 + 20, this->windowHeight/7 + this->windowHeight/10 + this->windowHeight/16 + 380, 20, BLACK);


        // Checking collisions with boxes and the mouse. If mouse is hovering changes color to red, and if there
        // is a click it changes the boolean responsible for changing the task. 
        if(CheckCollisionPointRec(mousePosition, firstTask)){
            DrawRectangleRec(firstTask, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                firstTaskBool = true;
            }

        }
        if(CheckCollisionPointRec(mousePosition, secondTask)){
            DrawRectangleRec(secondTask, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                secondTaskBool = true;
            }

        }
        if(CheckCollisionPointRec(mousePosition, thirdTask)){
            DrawRectangleRec(thirdTask, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                thirdTaskBool = true;
            }

        }
        if(CheckCollisionPointRec(mousePosition, fourthTask)){
            DrawRectangleRec(fourthTask, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                fourthTaskBool = true;
            }

        }
        
        if(CheckCollisionPointRec(mousePosition, fifthTask)){
            DrawRectangleRec(fifthTask, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                fifthTaskBool = true;
            }

        }

    EndDrawing();
    //----------------------------------------------------------------------------------

    
    // Changes state of the application returning a state when a button is pressed.
    if(firstTaskBool){
        return FIRST_TASK;
    }
    if(secondTaskBool){
        return SECOND_TASK;
    }

    if(thirdTaskBool){
        return THIRD_TASK;
    }

    if(fourthTaskBool){
        return FOURTH_TASK;
    }

    if(fifthTaskBool){
        return FIFTH_TASK;
    }


	return MAIN_MENU;
}

// Function that draws the path of the vehicle.
void WindowManager::DrawPoints(){

    for(int i = 0; i < this->occupied; i++){
        DrawCircle(this->points[i].x, this->points[i].y, this->points[i].r, this->points[i].c);

    }
}
// Function that saves the points that draw the path of the vehicle
void WindowManager::SavePoint(float x, float y){

    /*
        This function resets the pathing when it reaches the max dimention. 
        I did not like the fact that this could make the program segfaulting,
        even if it never happened once. But still, better safe than sorry.

        Cannot wait for this to be the reason we test the crash box and the structure integrity
        of the actual vehicle.
    */
    if (this->occupied >= DIM){
        ResetPoints();
    }

    this->points[this->occupied].x = x;
    this->points[this->occupied].y = y;

    this->occupied++;

}


GameState WindowManager::FirstTask(BicycleController *bicycle){

    float delta, theta, velocity, wheel, x, y, dt, wheelbase;
    float prevTheta, prevDelta, prevWheelbase, prevVelocity;
    bool error_msg = false, mainMenu = false;
    Rectangle mainMenuRectangle = {20, 20, 60, 40};
    Vector2 mousePosition = GetMousePosition();


    theta = bicycle->getTheta();
    delta = bicycle->getDelta();
    velocity = bicycle->getVelocity();
    wheelbase = bicycle->getWheelBase();
    x = bicycle->getX();
    y = bicycle->getY();

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    dt = GetFrameTime();

    // Checking if the trajectory has exited the screen boundaries. In that case
    // it will create a messagebox showing an error.

    if (x < 0 || x > this->windowWidth){
        error_msg = true;
    }

    if (y < 0 || y > this->windowHeight){
        error_msg = true;
    }
    
    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */
    //screenWidth = GetScreenWidth();
    //screenHeight = GetScreenHeight();
    //scale = screenWidth / 800;
    //fontSize = scale * baseFontSize;



    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(this->backgroundColor);

        DrawPoints();

        //DrawPixel((int)x, (int)y, WHITE);
        DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

        SavePoint(bicycle->getX(), bicycle->getY());
        DrawRectangleRec(mainMenuRectangle, LIGHTGRAY);
        DrawText("Back", 25, 25, 18, BLACK);

        // GUI controls
        DrawVehicleSliders(wheelbase, velocity, delta, theta);

        // Collision check for back button
        if(CheckCollisionPointRec(mousePosition, mainMenuRectangle)){
            DrawRectangleRec(mainMenuRectangle, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                mainMenu = true;
            }

        }
        
        if(error_msg){
            GuiMessageBox((Rectangle){ 300, 40, 600, 600}, "Crash detected", "Nice, crash box tested\n\n the mechanics love you already.\n\n\n\n You're fired... \n\nLike the battery is burning get out", "Get out");
        }



    EndDrawing();


    if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
        bicycle->Update(delta, theta, wheelbase, velocity);
    }

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    bicycle->Step(dt);

    if(mainMenu){
        return MAIN_MENU;
    }


    return FIRST_TASK;
}

void WindowManager::DrawVehicleSliders(float &wheelbase, float &velocity, float &delta, float &theta){
    GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, 40, 120, 20}, "Wheelbase", TextFormat("%.2f", wheelbase), &wheelbase, 1, 200);
    GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, 80, 120, 20}, "Velocity", TextFormat("%.2f", velocity), &velocity, 1, 150);
    GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, 120, 120, 20}, "Delta", TextFormat("%.2f", delta), &delta, -30, 30);
    GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, 160, 120, 20}, "Theta", TextFormat("%.2f", theta), &theta, -3.14, 3.14);
}

// This beautiful task was not working, while before it was... Why? idk, i just spent too much time on this i must move on.
GameState WindowManager::SecondTask(BicycleController *bicycle, PController *pcont, float &error){

    float delta, theta, velocity, wheel, x, y, dt, wheelbase;
    bool error_msg = false, mainMenu = false;
    float prevTheta, prevDelta, prevWheelbase, prevVelocity;

    float heading = 0;
    float noise = 0;

    Rectangle mainMenuRectangle = {20, 20, 60, 40};
    Vector2 mousePosition = GetMousePosition();
    //Random engine
    std::mt19937 rng(std::random_device{}());

    // uniform distribution random values
    std::uniform_real_distribution<double> dist(0.0, 1.0);



    // get all the attributes of the class. These are needed later for the update of all the variables if the slider changes value.
    theta = bicycle->getTheta();
    delta = bicycle->getDelta();
    velocity = bicycle->getVelocity();
    wheelbase = bicycle->getWheelBase();
    x = bicycle->getX();
    y = bicycle->getY();


    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;


    dt = GetFrameTime();


    if (x > this->windowWidth){
        x = 0;
        bicycle->setX(0);
    }

    


    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */

    //scale = screenWidth / 800;
    //fontSize = scale * baseFontSize;


    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(this->backgroundColor);
        DrawPoints();

        //DrawPixel((int)x, (int)y, WHITE);
        DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

        SavePoint(bicycle->getX(), bicycle->getY());
        DrawRectangleRec(mainMenuRectangle, LIGHTGRAY);
        DrawText("Back", 25, 25, 18, BLACK);

        // GUI controls
        DrawVehicleSliders(wheelbase, velocity, delta, theta);

        if(CheckCollisionPointRec(mousePosition, mainMenuRectangle)){
            DrawRectangleRec(mainMenuRectangle, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                mainMenu = true;
            }

        }


    EndDrawing();

    // add some error in the steering. Probably it would have been more effective by changing the theta value.
    if (this->occupied % 100 == 0){
        noise = (dist(rng) * 5);
        bicycle->setDelta(abs(dist(rng) * 7 - 0.5));

        /* 
            This step has been the bain of my existance. Because this line that was not present
            the little heading error was never computed since it always reset the delta value at every iteration because
            theta was set to zero, so the error was set to zero and on the pstep function delta was set to zero.
            Not fun.
        */ 
        bicycle->Step(dt);
    }
    // compute error based on the heading (in this case we want to go right always, so heading is always set to 0).
    // It is set up to 
    error = heading - (bicycle->getTheta());

    // Step of the bicycle where it updates x, y, delta and theta based on the error
    bicycle->PStep(dt, pcont->computeDelta(error));

    // updates bicycle if slider changed value.
    if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
        bicycle->Update(delta, theta, wheelbase, velocity);
    }

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    if(mainMenu){
        return MAIN_MENU;
    }


    return SECOND_TASK;
}

// NOTE: this code is based on the one for the second task, so its very similar. In essence
// the only change is in the pstep function that also accounts for keeping the trajectory. In this case keep the middle line
GameState WindowManager::ThirdTask(BicycleController *bicycle, PController *pcont, float &error, float &errory){

    // Checking if the trajectory has exited the screen boundaries. In that case
    // it will create a messagebox showing an error.
    float delta, theta, velocity, wheel, x, y, dt, wheelbase, k_p, k_y, prevK_p, prevK_y;
    bool error_msg = false, first = true, mainMenu = false;
    float prevTheta, prevDelta, prevWheelbase, prevVelocity;
   
    Rectangle mainMenuRectangle = {20, 20, 60, 40};
    Vector2 mousePosition = GetMousePosition();

    float heading = 0;
    float noise = 0;

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

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;


    // getting Proportional correction values
    k_p = pcont->getK_p();
    k_y = pcont->getK_y();

    prevK_p = k_p;
    prevK_y = k_y;


    dt = GetFrameTime();


    //screenWidth = GetScreenWidth();
    //screenHeight = GetScreenHeight();
    if (x > this->windowWidth){
        x = 0;
        bicycle->setX(0);
    }

    
    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */

    //scale = screenWidth / 800;
    //fontSize = scale * baseFontSize;


    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(this->backgroundColor);

        // Draw desired trajectory to mantain
        for (int f = 0; f < windowWidth; f++){
            DrawCircle(f, windowHeight/2, 2, RED);
        }

        DrawPoints();

        DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

        SavePoint(bicycle->getX(), bicycle->getY());
        DrawRectangleRec(mainMenuRectangle, LIGHTGRAY);
        DrawText("Back", 25, 25, 18, BLACK);

        // GUI controls
        DrawVehicleSliders(wheelbase, velocity, delta, theta);

        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 80, 120, 20}, "k_p", TextFormat("%.2f", k_p), &k_p, 0, 5);
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 40, 120, 20}, "k_y", TextFormat("%.2f", k_y), &k_y, 0, 5);


        
        if(CheckCollisionPointRec(mousePosition, mainMenuRectangle)){
            DrawRectangleRec(mainMenuRectangle, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                mainMenu = true;
            }

        }


    EndDrawing();


    if (this->occupied % 500 == 0){
        noise = (dist(rng) * 5);
        bicycle->setDelta(abs(dist(rng) * 7 - 0.5));
        bicycle->Step(GetFrameTime());
    }
    // Error computation keeping the middle line.
    error = heading - (bicycle->getTheta());
    errory = this->windowHeight/2 - bicycle->getY();


    bicycle->PStep(GetFrameTime(), pcont->computeDelta(error, errory));

    if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
        bicycle->Update(delta, theta, wheelbase, velocity);
    }

    if(prevK_p != k_p || prevK_y != k_y){
        pcont->setK_p(k_p);
        pcont->setK_y(k_y);
    }

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    if(mainMenu){
        return MAIN_MENU;
    }

    return THIRD_TASK;
}

// NOTE: Code similar to third task, with the difference that here the correction is done by a PID controller, the rest is
// basically the same.
GameState WindowManager::FourthTask(BicycleController *bicycle, PIDController *pcont, float &error, float &errory){

    // Checking if the trajectory has exited the screen boundaries. In that case
    // it will create a messagebox showing an error.
    float delta, theta, velocity, wheel, x, y, dt, wheelbase;
    float prevTheta, prevDelta, prevWheelbase, prevVelocity;
    float k_p, k_y, k_i, k_d;
    float prevK_p, prevK_y, prevK_i, prevK_d;

    bool error_msg = false, first = true, mainMenu = false;
    Rectangle mainMenuRectangle = {20, 20, 60, 40};
    Vector2 mousePosition = GetMousePosition();

    float heading = 0;
    float noise = 0;

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

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    k_p = pcont->getK_p();
    k_y = pcont->getK_y();
    k_i = pcont->getK_i();
    k_d = pcont->getK_d();

    prevK_p = k_p;
    prevK_y = k_y;
    prevK_i = k_i;
    prevK_d = k_d;

    dt = GetFrameTime();

    if (x > this->windowWidth){
        x = 0;
        bicycle->setX(0);
    }

    


    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */

    //scale = screenWidth / 800;
    //fontSize = scale * baseFontSize;



    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(this->backgroundColor);

        for (float f = 0; f < this->windowWidth; f+=0.1){
            //DrawCircle(f, windowHeight/2, 2, RED);
            DrawCircle(f, this->windowHeight/2 + (sin(f / 100) * 50), 2, RED);
        }



        DrawPoints();

        DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

        SavePoint(bicycle->getX(), bicycle->getY());
        DrawRectangleRec(mainMenuRectangle, LIGHTGRAY);
        DrawText("Back", 25, 25, 18, BLACK);

        // GUI controls
        DrawVehicleSliders(wheelbase, velocity, delta, theta);

        // P sliders
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 160, 120, 20}, "k_p", TextFormat("%.2f", k_p), &k_p, 0, 5);
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 120, 120, 20}, "k_y", TextFormat("%.2f", k_y), &k_y, 0, 5);
        // ID sliders
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 80, 120, 20}, "k_i", TextFormat("%.2f", k_i), &k_i, 0, 5);
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 40, 120, 20}, "k_d", TextFormat("%.2f", k_d), &k_d, 0, 5);


        if(CheckCollisionPointRec(mousePosition, mainMenuRectangle)){
            DrawRectangleRec(mainMenuRectangle, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                mainMenu = true;
            }

        }


    EndDrawing();



    if (this->occupied % 500 == 0){
        noise = (dist(rng) * 5);
        bicycle->setDelta(abs(dist(rng) * 7 - 0.5));
        bicycle->Step(dt);
    }
    // compute the error, calls pcont that here is the PIDController and correct for it.
    //error = heading - (bicycle->getTheta());
    error = atan((100.0f / 30.0f) * cos(bicycle->getX() / 30.0f)) - bicycle->getTheta();
    //errory = screenHeight/2 - bicycle->getY();
    errory = this->windowHeight/2 + (sin(bicycle->getX() /100) * 50) - bicycle->getY();


    bicycle->PStep(dt, pcont->computeDelta(error, errory, dt));

    if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
        bicycle->Update(delta, theta, wheelbase, velocity);
    }

    if (prevK_p != k_p || prevK_i != k_i || prevK_y != k_y || prevK_d != k_d){
        pcont->Update(k_p, k_y, k_i, k_d);
    }

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    if(mainMenu){
        return MAIN_MENU;
    }


    return FOURTH_TASK;
}


GameState WindowManager::FifthTask(BicycleController *bicycle, StanleyController *scont, float &error, float &errory){

    // Checking if the trajectory has exited the screen boundaries. In that case
    // it will create a messagebox showing an error.
    float delta, theta, velocity, wheel, x, y, dt, wheelbase;
    float prevTheta, prevDelta, prevWheelbase, prevVelocity;
    float k;
    float prevK;

    bool error_msg = false, first = true, mainMenu = false;
    Rectangle mainMenuRectangle = {20, 20, 60, 40};
    Vector2 mousePosition = GetMousePosition();

    float heading = 0;
    float noise = 0;

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

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    k = scont->getK();

    prevK = k;

    dt = GetFrameTime();

    if (x > this->windowWidth){
        x = 0;
        bicycle->setX(0);
    }

    


    /* Getting current screen dimentions, calculate scaling based on width in order to have a window
        * that is responsive
        */

    //scale = screenWidth / 800;
    //fontSize = scale * baseFontSize;



    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
        ClearBackground(this->backgroundColor);

        for (float f = 0; f < this->windowWidth; f+=0.1){
            //DrawCircle(f, windowHeight/2, 2, RED);
            DrawCircle(f, this->windowHeight/2 + (sin(f / 100) * 50), 2, RED);
        }



        DrawPoints();

        DrawCircle(bicycle->getX(), bicycle->getY(), 2, WHITE);

        SavePoint(bicycle->getX(), bicycle->getY());
        DrawRectangleRec(mainMenuRectangle, LIGHTGRAY);
        DrawText("Back", 25, 25, 18, BLACK);

        // GUI controls
        DrawVehicleSliders(wheelbase, velocity, delta, theta);

        // P sliders
        GuiSliderBar((Rectangle){this->windowWidth - this->windowWidth/5, this->windowHeight - 160, 120, 20}, "k", TextFormat("%.2f", k), &k, 0, 5);

        if(CheckCollisionPointRec(mousePosition, mainMenuRectangle)){
            DrawRectangleRec(mainMenuRectangle, RED);

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                mainMenu = true;
            }

        }


    EndDrawing();


    // noise that is set into the angle of the wheel. Still not the best thing
    // but it will do for now. Probably an update could be to put it
    // in the heading of the vehicle.
    if (this->occupied % 500 == 0){
        noise = (dist(rng) * 5);
        bicycle->setDelta(abs(dist(rng) * 7 - 0.5));
        bicycle->Step(dt);
    }
    // compute the error, calls scont that here is the PIDController and correct for it.
    //error = heading - (bicycle->getTheta());

    /* 
        Compute angle using atan2 function which returns the angle from point A to point B
        given the coordinates doing a simple subtraction. The idea is to smooth the angle using
        x = 100 as a lookahead in order to make the path more accutate. Which is something
        that i have not done in other parts of the project. Why do this? Because i got the idea
        somewhere while researching pure pursuit and stanley where they looked ahead in order to make
        a smoother turning, especially since we want to be in a point where x is the same for the wehicle
        and the path (so the difference is equal to zero), while the y changes. If we get x=0 the problem 
        is that it will try to correct heavily going up or down, so it will always overshoot the path that
        we want to follow. Yes, this is an hack, yes i'm proud of it. No, i wont change the other task to
        make it more accurate. This is why atan2(y difference, x look ahead to make it smoother).
    */
    error = atan2(this->windowHeight/2 + (sin(x / 100) * 50) - y, 100) - bicycle->getTheta();
    //errory = screenHeight/2 - bicycle->getY();
    errory = windowHeight/2 + (sin(bicycle->getX() /100) * 50) - bicycle->getY();


    bicycle->PStep(dt, scont->computeDelta(error, errory, velocity));

    if (prevDelta != delta || prevTheta != theta || prevVelocity != velocity || prevWheelbase != wheelbase){
        bicycle->Update(delta, theta, wheelbase, velocity);
    }

    if (prevK != k){
        scont->setK(k);
    }

    prevTheta = theta;
    prevDelta = delta;
    prevVelocity = velocity;
    prevWheelbase = wheelbase;

    if(mainMenu){
        return MAIN_MENU;
    }


    return FIFTH_TASK;
}
