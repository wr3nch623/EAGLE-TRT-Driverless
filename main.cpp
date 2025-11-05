#include <stdio.h>
#include <iostream>

// Including Raylib stuff
#include "include/RayLib/raylib.h"
#include "include/RayLib/raymath.h"
#include "include/RayLib/rlgl.h"
#include "include/WindowManager/windowmanager.h"
//#include "include/BicycleController/bicyclecontroller.h"
//#include "include/PController/pcontroller.h"


using namespace std;

int main(){
    //WindowManager *window = new WindowManager();
	float windowWidth = 1280, windowHeight = 650;
	WindowManager* window = new WindowManager(windowWidth, windowHeight);
	window->Run();
	//window->MainMenu();
	//window->FirstTask();
	//window->SecondTask();
	//window.DrawCycle();
	//window.FirstTask();
	delete window;
	return 0;
}
