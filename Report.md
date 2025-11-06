# Approach
The approach used for this project was to first create the window and get the raylib files so that everything was compiled statically for ease of use, so that linking issues were not present. I know this is not really ideal, but without having a standardized enviroment, where if someone had a different linux distribution there is a potential difference for installing raylib, compile it from scratch, or something else where the reviewer have to mess with it so... Best to avoid it. Before you ask yes, it works on my machine, so if you encounter problems i have a solid case.

I've done four task out of five, without the bonus due to limited time available in this period, due to many things that had happened, starting from my degree discussion, meetings with the research group and work related to it etc. Do i expect that this would influence the reviewer? Absolutely not, i know that i would have probably developed a better solution if i had more time available, but alas everything went wrong in those two weeks from the amount of work to my laptop acting up, so i had to make the most of it anyway, also it was a good stress test for me showing that i have room for improvement.

For all of the tasks basically the same template was used, it is not the best thing but rewriting the entire code in the limited time i had was not possible, especially since the majority of the code in the windowmanager.cpp is to draw everything on the screen.

This resource proved useful to help understanding and develop [the bicycle model](https://thomasfermi.github.io/Algorithms-for-Automated-Driving/Control/BicycleModel.html) and also PID for a brief reminder.

# Encountered problems
I had some problems regarding raylib on how to design the cycle. This caused me to rewrite most of the tasks because first i did not finish the design of the main menu, causing to always having to instantiate the window in every function. This worked until i had to integrate the menu creating an headache that lasted a day in order to make it work properly, leading to the state driven menu I have right now. 

In the second task, as said in the comments inside windowmanager.cpp a line caused to never have noise in the steering angle, because of how the code was structured, so now i purposely update one time so that theta (vehicle heading) can update, calculating an error and then correcting at the next step.

# Further development
As of now the code is not yet ready to be considered done. There is still some polishing to do, implement newer task, the bonus, define an actual track where the bicycle must move, implement the bicycle drawing so that the orientation is known and its not just dots showing the trajectory etc. There are still things to work on that will be addressed in the future, i'll certainly finish this project sometime, for now its going to be as is since today, November 6th is the due date, but i had fun doing this, despite the circumstances i'm in right now.

# To the reviewer
Thanks for reviewing this code, and having to see some things that i also don't like, like the passing by reference of the previous values that could be instanced inside every function, and my beautiful code that still needs work, especially for the graphics rendering. If i pass the requirements, i would like to start as soon as i'm back in Trento (the 16th), since i should have completed most of the work i am still required to do. We can also discuss about this.

# TODO
- add buttons to get back to the main menu everywhere
- polish the code in order to strip it down to the bare minimum where it should work without having useless variables.
- implement more complex paths with the definition of a custom track.
- do fifth task
- add some other todos that i cannot think about now.



