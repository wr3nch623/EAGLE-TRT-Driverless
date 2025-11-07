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
	float windowWidth = 1680, windowHeight = 1050;
	WindowManager* window = new WindowManager(windowWidth, windowHeight);
	window->Run();

	delete window;
	return 0;
}
