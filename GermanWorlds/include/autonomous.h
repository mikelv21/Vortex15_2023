#include <iostream>
#include "constants.h"
#include "robot-config.h"


using namespace vex;
extern brain Brain;

//------- Aux function definition -------//
void shoot(int volt, int discs);
void move_to_coordinate(double target_x, double target_y, double target_heading);

//--------- Main auton functions ---------//
void auton(){
  // Movement configurations
  Drivetrain.setTurnConstant(1.0);   // Recommended value 0.8 a 1.2
  Drivetrain.setTurnThreshold(1.0);  // Recommended value 2 a 5 deg
  
  Drivetrain.setDriveVelocity(60, velocityUnits::pct);
  move_to_coordinate(0, -39, 90);              // Go to the roller
  Drivetrain.setDriveVelocity(30, velocityUnits::pct);
  move_to_coordinate(0, -6, 0);               // Move to roller
  wait(1.5, sec);
  Drivetrain.setDriveVelocity(50, velocityUnits::pct);
  Intake_roller_group.spinFor(fwd, 180, deg);  // Move roller
  move_to_coordinate(0, 10, 15);               // Aim to basket
  shoot(12, 2);                                // Shoot 2 discs
  move_to_coordinate(0, 0, -15);               // Reset position
  move_to_coordinate(0, 0, -50);               // Aim to discs
  // move_to_coordinate(0, 0, -60);               // Reset position and Aim discs
  move_to_coordinate(0, 25, 180);
  Intake_roller_group.spin(reverse, 12, volt); // Activate Intake
  move_to_coordinate(0, -120, -90);            // Collect discs and aim basket
  Intake_roller_group.stop();                  // Stop Intake
  shoot(9, 3);                                 // Shoot 3 discs
  // --- END --- //
}

void skills(){
  auton();
}

void shoot(int volts, int discs){
  double temp = WAIT_UNTIL_LAUNCH*2.0;

  Flywheel.spin(fwd, volts, voltageUnits::volt);
  wait(temp, msec);
  for (int i = 0; i < discs; i++){
    Indexer.open(); 
    wait(INDEXER_WAIT*1.5, msec);
    Indexer.close();
    wait(INDEXER_WAIT*1.5, msec);
    // temp = 0;
  }
  Flywheel.stop();
}

void move_to_coordinate(double target_x, double target_y, double target_heading){
  if (target_x == 0 && target_y != 0){
    Drivetrain.driveFor(fwd, target_y, distanceUnits::cm); 
  }
  if (target_y == 0 && target_x != 0){
    double ang = 90;
    if (target_x > 0){ Drivetrain.turnToHeading(ang, rotationUnits::deg); }
    if (target_x < 0){ Drivetrain.turnToHeading(-ang, rotationUnits::deg); }
    Drivetrain.driveFor(fwd, target_x, distanceUnits::cm);
  }
  if (target_x != 0 && target_y != 0){
    double ang = atan(target_y / target_x) * 180 / M_PI;
    double hyp = sqrt(target_x * target_x + target_y * target_y);
    // 1st quadrant
    if (target_x > 0 && target_y > 0){ Drivetrain.turnToHeading(ang, rotationUnits::deg); }
    // 2nd quadrant 
    if (target_x < 0 && target_y > 0){ Drivetrain.turnToHeading(-ang, rotationUnits::deg); }
    // 3rd quadrant
    if (target_x < 0 && target_y < 0){ Drivetrain.turnToHeading(180 - ang, rotationUnits::deg); }
    // 4th quadrant
    if (target_x > 0 && target_y < 0){ Drivetrain.turnToHeading(180 + ang, rotationUnits::deg); }
    Drivetrain.driveFor(hyp, distanceUnits::cm);
  }
  if (target_heading != 0){
    Drivetrain.turnToHeading(target_heading, rotationUnits::deg);
  }
  DrivetrainInertial.resetHeading();
  Drivetrain.stop(brakeType::hold);
}

/* 
Record values
 - 45% -> 50% -> 
 - (0, -37, 90)   -> 
 - 15% -> 
 - (0, -14.2, 0)  ->  
 - 45% -> 
 - 180 deg        -> 
 - (0, 13, 15)    -> 
 - 12V, 2 discs   -> 
 - (0, 0, -15)    -> 
 - (0, 0, -45)    -> 
 - 12V ->  
 - (0, -120, -90) -> 
 - 9V, 3 discs    -> 
*/