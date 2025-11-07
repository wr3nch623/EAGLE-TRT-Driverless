# Approach
The approach used for this project was to first create the window and get the raylib files so that everything was compiled statically for ease of use, so that linking issues were not present. I know this is not really ideal, but without having a standardized enviroment, where if someone had a different linux distribution there is a potential difference for installing raylib, compile it from scratch, or something else where the reviewer have to mess with it so... Best to avoid it. Before you ask yes, it works on my machine, so if you encounter problems i have a solid case.

I've done all five tasks with different degrees of success, without the bonus due to limited time available in this period, due to many things that had happened, starting from my degree discussion, meetings with the research group and work related to it, kernel panics caused by some issue that is not in the logs of the computer etc.

For all of the tasks basically the same template was used, it is not the best thing but rewriting the entire code in the limited time i had was not possible, especially since the majority of the code in the windowmanager.cpp is to draw everything on the screen.

This resource proved useful to help understanding and develop [the bicycle model](https://thomasfermi.github.io/Algorithms-for-Automated-Driving/Control/BicycleModel.html) and also PID for a brief reminder.

The P and PID controller were tuned by doing some test changing the values inside the definition of the class. 

If there are questions feel free to ask.

## First task
Simple drawing of the vehicle path. You can tune the parameters of the vehicle and if you crash you have to restart the application. Some say that the developer is lazy to not permit the person to close the message box since the app is still running in the background, i say its your fault you've crashed soooo... I should implement it.

## Second task
Simple Proportional control for heading control only, it will keep going right but not aligning itself to a point specifically.

## Third task
Basically is the second task while it keeps aligned in the middle of the screen.

## Fourth task
Third task with full PID controller while trying to follow a sinusoidal path that is broken, meaning that the rightmost point is higher than the leftmost point creating a jump and making the vehicle control freak out, showing how badly it corrects the trajectory.

## Fifth task
Fourth task using a Stanley controller as control. This controller tracks better because not only does it not rely on the frametime, which can cause issued (as seen during debugging where i could not figure out at the beginning why the vehicle was teleporting around, now it seemed obvious), but, for the lookahead hack it follows the path more smoothly, because before the difference in the y axis told us how much the vehicle is far instead of the track line, while the x where we want to end up is the same for vehicle and track. This means that it heavily corrects for the y, over corrects goes to the actual track point, overshoot and then overcorrects again and the cycle goes on. This way looking ahead of 100 pixels (since we are comparing the x and y positions that are in pixels) it smooths the direction that the vehicle should take, so its more accurate following the desired trajectory, even at the jump when the vehicle is put back on x = 0. This is a total hack, based on actual real life implementation where in creating the path the vehicle looks ahead in order to turn at the right time and with the right angle of the wheels.

# Encountered problems
I had some problems regarding raylib on how to design the cycle. This caused me to rewrite most of the tasks because first i did not finish the design of the main menu, causing to always having to instantiate the window in every function. This worked until i had to integrate the menu creating an headache that lasted a day in order to make it work properly, leading to the state driven menu I have right now. 

In the second task, as said in the comments inside windowmanager.cpp a line caused to never have noise in the steering angle, because of how the code was structured, so now i purposely update one time so that theta (vehicle heading) can update, calculating an error and then correcting at the next step.

There was a little problem in my implementation of the bicycle kinematics that did not limit the value of theta, which potentially could go to infinite, clashing with the GUI sliderbars, so at some point the vehicle was going straight. Now is limited between -PI and PI.

# Further development
As of now the code is better than yesterday, so its something. There is still some polishing to do, implement the bonus tasks, define an actual track where the bicycle must move, implement the bicycle drawing so that the orientation is known and its not just dots showing the trajectory etc. There are still things to work on that will be addressed in the future, i'll certainly finish this project sometime, for now its going to be as is since today, November 7th is the due date, but i had fun doing this, despite the circumstances.


# TODO
- ~add buttons to get back to the main menu everywhere~
- ~add slider to tune pid controller~
- ~test code for memory leaks~
- Fix memory leaks
- Finish polishing the code, is decent but could be better
- implement more complex paths with the definition of a custom track.
- ~do fifth task~
- add some other todos that i cannot think about now.



