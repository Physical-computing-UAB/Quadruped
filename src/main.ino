#include <CRobot.h>

const int DELAY_TIME = 100; // Time to think 

// Create instance of the robot.
CRobot Robot = CRobot();

void setup() {
  Serial.begin(9600);
  
  // 1.- Initialize the peripherials of the robot.
  Robot.init();
}

void loop() // The robot will be always running once it has been initialized
{
  // 2.- Handle the events of the robot.
  Robot.handleEvents();

  // 3.- Update the values of the peripherials and moving of the parts.
  Robot.update();

  // 4.- Move parts.
  Robot.move();
  
  delay(DELAY_TIME); // Time for the robot to think
}
