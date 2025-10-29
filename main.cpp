#include <stdio.h>
#include <iostream>

// Including Raylib stuff
#include "include/RayLib/raylib.h"
#include "include/RayLib/raymath.h"
#include "include/RayLib/rlgl.h"
#include "include/WindowManager/windowmanager.h"

using namespace std;

int main(){
    //WindowManager *window = new WindowManager();
	float windowWidth = 1680, windowHeight = 1050;
	WindowManager* window = new WindowManager(windowWidth, windowHeight);
	//window->DrawCycle();
	window->FirstTask();
	//window.DrawCycle();
	//window.FirstTask();
	delete window;
	return 0;
}
