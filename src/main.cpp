/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       nikkasouza                                                */
/*    Created:      6/26/2024, 1:48:50 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;
brain Brain;

motor frontRightMotor(PORT1, ratio18_1, false);
motor middleRightMotor(PORT2, ratio18_1, false);
motor backRightMotor(PORT3, ratio18_1, false);

motor frontLeftMotor(PORT4, ratio18_1, true);
motor middleLeftMotor(PORT5, ratio18_1, true);
motor backLeftMotor(PORT6, ratio18_1, true);

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
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
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
  while (1) {
    driveTrain.setDriveVelocity(abs(primaryController.Axis3.position()), percent);
    driveTrain.setTurnVelocity(abs(primaryController.Axis4.position()), percent);

    if (primaryController.Axis3.position() > 0) {
      driveTrain.drive(forward);
    } else if (primaryController.Axis3.position() < 0) {
      driveTrain.drive(reverse);
    }

    if (primaryController.Axis2.position() > 0) {
      driveTrain.turn(right);
    } else if (primaryController.Axis2.position() < 0) {
      driveTrain.turn(left);
    } 

    if (primaryController.Axis2.position() == 0 && primaryController.Axis3.position() == 0) {
      driveTrain.stop();
    }

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
