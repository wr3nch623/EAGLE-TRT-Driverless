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
	WindowManager window;
	window.DrawCycle();

	return 0;
}
