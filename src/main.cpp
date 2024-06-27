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
// axis 2 up right, down left

void usercontrol(void) {
  // User control code here, inside the loop

  // Callback example
  primaryController.ButtonA.pressed([]() {
    std::cout << "A pressed" << std::endl;
  });

  while (1) {
    int axis2 = primaryController.Axis2.position();
    int axis3 = primaryController.Axis3.position();
    int axis4 = primaryController.Axis4.position();

    driveTrain.setDriveVelocity(abs(axis3), percent);
    driveTrain.setTurnVelocity(abs(axis2), percent);

    if (axis3 > 0) {
      driveTrain.drive(forward);
    } else if (axis3 < 0) {
      driveTrain.drive(reverse);
    }

    if (axis2 > 0) {
      driveTrain.turn(right);
    } else if (axis2 < 0) {
      driveTrain.turn(left);
    } 

    if (axis2== 0 
      && axis3 == 0
      && axis3 == 0){
      driveTrain.stop();
    }



    // TODO: Ask if wants continuous push or once callbacks
    if (primaryController.ButtonR1.pressing()) {
      std::cout << "R1 pressed" << std::endl;
    }

    if (primaryController.ButtonR2.pressing()) {
      std::cout << "R2 pressed" << std::endl;
    }

    if (primaryController.ButtonL1.pressing()) {
      std::cout << "L1 pressed" << std::endl;
    }

    if (primaryController.ButtonL2.pressing()) {
      std::cout << "L2 pressed" << std::endl;
    }

    if (primaryController.ButtonUp.pressing()) {
      std::cout << "Up pressed" << std::endl;
    }

    if (primaryController.ButtonDown.pressing()) {
      std::cout << "Down pressed" << std::endl;
    }

    if (primaryController.ButtonLeft.pressing()) {
      std::cout << "Left pressed" << std::endl;
    }

    if (primaryController.ButtonRight.pressing()) {
      std::cout << "Right pressed" << std::endl;
    }

    if (primaryController.ButtonB.pressing()) {
      std::cout << "B pressed" << std::endl;
    }

    if (primaryController.ButtonX.pressing()) {
      std::cout << "X pressed" << std::endl;
    }

    if (primaryController.ButtonY.pressing()) {
      std::cout << "Y pressed" << std::endl;
    }

    // TODO: What the hell is moving left and right

    wait(20, msec); 
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

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
