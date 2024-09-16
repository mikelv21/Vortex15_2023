/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\nerij                                            */
/*    Created:      Wed Apr 19 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 2, 4, 5, 16  
// Flywheel             motor_group   18, 19          
// IntakeRoller         motor         7               
// Indexer              digital_out   D               
// Motor11              motor         11              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "autonomous.h"
#include "robot-config.h"

using namespace vex;
competition Competition; 

void display_info_dinamic(){
  std::cout << "FWU: " << FlywheelMotorA.temperature(celsius) << std::endl;
  std::cout << "FWD: " << FlywheelMotorB.temperature(celsius)   << std::endl;
  std::cout << "Heading: " << DrivetrainInertial.heading()  << std::endl;
}

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
  auton();
}

void usercontrol(void) {
  rc_auto_loop_function_Controller1();
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    display_info_dinamic();
    wait(100, msec);
  }
}
