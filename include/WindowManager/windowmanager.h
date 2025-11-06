
#include "../RayLib/raylib.h"                 // for Color
#include "../BicycleController/bicyclecontroller.h"  // for BicycleController
//#include "../PController/pcontroller.h"
#include "../PIDController/pidcontroller.h"


enum GameState{
    MAIN_MENU,
    FIRST_TASK,
    SECOND_TASK,
    THIRD_TASK,
    FOURTH_TASK,
    FIFTH_TASK,
    EXIT
};

// Array of points that serve for drawing the path taken by the vehicle
struct Point{
    float x, y;
    int r;
    Color c;
};
// Dimention for said points struct. This is not the best because as of now
// the program will crash if it goes over.
#define DIM 50000



class WindowManager{
    private:
        float windowWidth, windowHeight;
        const float baseFontSize = 32;
        //BicycleController *bicycle = nullptr;
        const Color backgroundColor = {69, 6, 147, 100};
        Point points[DIM];
        int occupied;



    public:


        WindowManager();
        ~WindowManager();
        WindowManager(float, float);

        
        void CreateWindow();
        void ResetPoints();
        void DrawPoints();
        void SavePoint(float x, float y);

        int Run();
        GameState MainMenu();
        GameState FirstTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle);
        GameState SecondTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PController *pcont, float &error);
        GameState ThirdTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PController *pcont, float &error, float &errory);
        GameState FourthTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PIDController *pcont, float &error, float &errory);

    };