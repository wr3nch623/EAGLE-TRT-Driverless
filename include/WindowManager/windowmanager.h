
#include "../RayLib/raylib.h"                 // for Color
#include "../BicycleController/bicyclecontroller.h"  // for BicycleController
#include "../PController/pcontroller.h"


enum GameState{
    MAIN_MENU,
    FIRST_TASK,
    SECOND_TASK,
    THIRD_TASK,
    FOURTH_TASK,
    FIFTH_TASK,
    EXIT
};

struct Point{
    float x, y;
    int r;
    Color c;
};
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

        //void ResetBicycle(float delta, float theta, float wheelbase, float velocity);

        void ResetWindow();
        void ResetPoints();
        void DrawPoints();
        void SavePoint(float x, float y);

        int Run();
        GameState MainMenu();
        GameState FirstTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle);
        GameState SecondTask(float &prevDelta, float &prevTheta, float &prevVelocity, float &prevWheelbase, BicycleController *bicycle, PController *pcont, float &error);
};