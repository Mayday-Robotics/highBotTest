/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       nikkasouza                                                */
/*    Created:      6/26/2024, 1:48:50 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <iostream>

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;

motor frontRightMotor(PORT1, ratio18_1, true);
motor middleRightMotor(PORT2, ratio18_1, true);
motor backRightMotor(PORT3, ratio18_1, true);

motor frontLeftMotor(PORT4, ratio18_1, false);
motor middleLeftMotor(PORT5, ratio18_1, false);
motor backLeftMotor(PORT6, ratio18_1, false);

motor_group rightDrive(frontRightMotor, middleRightMotor, backRightMotor);
motor_group leftDrive(frontLeftMotor, middleLeftMotor, backLeftMotor);

controller primaryController;

drivetrain driveTrain = drivetrain(leftDrive, rightDrive, 12.5664, 14, 13, inches, 1);

task lcdThread;
bool lcdThreadRunning = false;

int turnVelocity = 75;
int driveVelocity = 100;

// 4 inch wheels
// 14 inch distance between wheels
// 13 inch wheel base

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  return;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // drive forward a foot, turn left 90 degrees, drive forward a foot and spin in a circle for 15s at max velocity
  driveTrain.setDriveVelocity(50, percent);
  driveTrain.setTurnVelocity(50, percent);

  driveTrain.driveFor(12, inches);
  driveTrain.turnFor(left, 90, degrees);
  driveTrain.driveFor(12, inches);

  driveTrain.setTurnVelocity(100, percent);
  driveTrain.turn(right);
  wait(15, sec);

  driveTrain.stop();

  return;
}

int displayMotorTemperature(void) {
  while (true) {
    primaryController.Screen.clearScreen();

    primaryController.Screen.setCursor(1, 1);
    primaryController.Screen.print("FL: %.1fC FR: %.1fC", frontLeftMotor.temperature(temperatureUnits::celsius), frontRightMotor.temperature(temperatureUnits::celsius));
    primaryController.Screen.setCursor(2, 1);
    primaryController.Screen.print("ML: %.1fC MR: %.1fC", middleLeftMotor.temperature(temperatureUnits::celsius), middleRightMotor.temperature(temperatureUnits::celsius));
    primaryController.Screen.setCursor(3, 1);
    primaryController.Screen.print("BL: %.1fC BR: %.1fC", backLeftMotor.temperature(temperatureUnits::celsius), backRightMotor.temperature(temperatureUnits::celsius));

    task::sleep(5000);
  }

  return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// axis 3 forward and backward
// axis 4 left and right
// axis 2 up left, down right

// up arrow, up drive velocity, down arrow down drive velocity
// left arrow, minus turn velocity, right arrow, plus turn velocity
// 5 percent increments

void usercontrol(void) {
  // User control code here, inside the loop

  // Callback example
  // primaryController.ButtonA.pressed([]() {
  //   std::cout << "A pressed" << std::endl;
  // });

  // primaryController.ButtonB.pressed([]() {
  //   std::cout << "B pressed" << std::endl;
  // });

  primaryController.ButtonX.pressed([]() {
    if (lcdThreadRunning) {
      lcdThread.stop();
      lcdThreadRunning = false;
    } else {
      lcdThread.resume();
      lcdThreadRunning = true;
    }
  });

  // primaryController.ButtonY.pressed([]() {
  //   std::cout << "Y pressed" << std::endl;
  // });

  // primaryController.ButtonL1.pressed([]() {
  //   std::cout << "L1 pressed" << std::endl;
  // });

  // primaryController.ButtonL2.pressed([]() {
  //   std::cout << "L2 pressed" << std::endl;
  // });

  // primaryController.ButtonR2.pressed([]() {
  //   std::cout << "R2 pressed" << std::endl;
  // });

  primaryController.ButtonUp.pressed([]() {
    driveVelocity += 5;
  });

  primaryController.ButtonDown.pressed([]() {
    driveVelocity -= 5;
  });

  primaryController.ButtonLeft.pressed([]() {
    turnVelocity -= 5;
  });

  primaryController.ButtonRight.pressed([]() {
    turnVelocity += 5;
  });

  while (1) {
    int axis2 = primaryController.Axis2.position();
    int axis3 = primaryController.Axis3.position();

    driveTrain.setDriveVelocity(driveVelocity, percent);
    driveTrain.setTurnVelocity(turnVelocity, percent);

    if (axis3 > 0) {
      driveTrain.drive(forward);
    } else if (axis3 < 0) {
      driveTrain.drive(reverse);
    }

    if (axis2 > 0) {
      driveTrain.turn(left);
    } else if (axis2 < 0) {
      driveTrain.turn(right);
    } 

    // make button y brake
    if (primaryController.ButtonR1.pressing()) {
      driveTrain.stop(brakeType::hold);
    }

    // TODO: What the hell is moving left and right

    wait(10, msec); 
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  lcdThread = task(displayMotorTemperature);
  lcdThread.stop();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
