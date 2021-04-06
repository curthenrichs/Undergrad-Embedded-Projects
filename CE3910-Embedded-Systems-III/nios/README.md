# CE 3910 Embedded Systems III - Camera - Hand tracking application

The design laboratory project for the Embedded III course is an embedded computer vision system using
a digital camera attached to a DE0 development board with a NIOS II softcore processor image. The
target object is a black object on a white background that will move greater than one inch per second
across the cameras field of view. The system tracks the target with a two axis, servo actuated turret to
position the center of the target into the center of the camera’s field of view. Additionally, the camera
image is displayed onto a VGA monitor so the user has feedback onto what the camera detects. The
purpose of this design project is to act as a capstone project for the previous embedded courses. Also
this project exposes new hardware devices that are common in embedded systems such as I2C and
graphics buffers. Commands entered by the user makeup the functionality of the system and were used
to mark progress milestones for the project. Commands supported follow either instant processing
format such as read and writing from memory or they follow a polling loop pattern. When a user
commands the system to track the object the command will continue running until user presses the exit
button. All code was written in embedded C for the NIOS processor and made use of a data structure to
simplify command creation. Testing of the system shows that all requirements are met for the tracking
application. Future work could be performed to tune the pixel filtering but may not increase
performance from the user’s perspective.

See the [final report here](./CE3910_Henrichs_Final_Report.pdf).


## Code
Please refer to `software/Final_Build` for the code submitted in the course.


This is project is for archival and reference purposes. Code is not maintained.
