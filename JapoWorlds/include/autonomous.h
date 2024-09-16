#include <iostream>
#include "robot-config.h"
#include "constants.h"

using namespace vex;
extern brain Brain;

//------- Aux function definition -------//
void go_for_roller(double dist_for_roller,  double vel_for_chassis, double turn_for_roller, double vel_for_roller);
void shot_two_disks(double dist_for_shoot, double vel_for_shoot, double heading_setpoint, double vel_for_heading);
void go_for_three_stack();
void reset_turnH(int d, int vel);
void shot_three_disks();
void go_for_three_square();
//--------- Main auton function ---------//
void auton(){
  go_for_roller(DISTANCE_FOR_ROLLER, VEL_FOR_CHASSIS_ROL, TURN_FOR_ROLLER, VEL_FOR_ROLLER);
  shot_two_disks(DIST_FOR_TWO_DISKS, VEL_CHASSIS_TWO_D, HEADING_SETPOINT, VEL_FOR_HEADING);
  go_for_three_stack();
  shot_three_disks();
  //go_for_three_square();
}


void go_for_roller(double dist_for_roller, double vel_for_chassis, double turn_for_roller, double vel_for_roller){
  Drivetrain.driveFor(directionType::rev, dist_for_roller, distanceUnits::cm, vel_for_chassis, velocityUnits::pct); 
  Intake_roller_group.spinFor(forward, turn_for_roller, rotationUnits::deg, vel_for_roller, velocityUnits::pct);
  Flywheel.setVelocity(FIRST_DISK_VEL, velocityUnits::pct);
  Flywheel.spin(forward);
}

void shot_two_disks(double dist_for_shoot, double vel_for_chassis, double heading_setpoint, double vel_for_heading){
  // Go to throw discs
  Drivetrain.driveFor(directionType::fwd, dist_for_shoot, distanceUnits::cm, vel_for_chassis, velocityUnits::pct);  
  // Turn to the basket
  Drivetrain.turnToHeading(heading_setpoint, rotationUnits::deg, vel_for_heading, velocityUnits::pct);              
  //1st disk
  
  wait(INDEXER_WAIT, msec);
  Indexer.open(); 
  wait(INDEXER_WAIT, msec);
  Indexer.close();
  //2nd disk
  wait(INDEXER_WAIT, msec);
  Flywheel.setVelocity(SECOND_DISK_VEL, velocityUnits::pct);
  Indexer.open(); 
  wait(INDEXER_WAIT, msec);
  Indexer.close();
  Flywheel.stop(brakeType::coast);
}

void go_for_three_stack(){
  //Aim to stack
  reset_turnH(-120, 34); 
  //Go for stck
  Drivetrain.driveFor(directionType::rev, 95, distanceUnits::cm, 100, velocityUnits::pct);
  Drivetrain.stop(hold);
  Drivetrain.driveFor(directionType::fwd, 12, distanceUnits::cm, 100, velocityUnits::pct);
  // Activate intake to collect the discs
  Intake_roller_group.spin(reverse, 85, velocityUnits::pct);  
  // Collecting the discs
  Drivetrain.driveFor(directionType::rev, 70, distanceUnits::cm, 20, velocityUnits::pct);  
}

void shot_three_disks(){
  Flywheel.setVelocity(THIRD_DISK_VEL, velocityUnits::pct);
  Flywheel.spin(forward);
  reset_turnH(138, 25);   // Aim for the basket
  Intake_roller_group.stop();  

  //1st disk
  wait(WAIT_UNTIL_LAUNCH/2.0, msec);
  Indexer.open(); 
  wait(INDEXER_WAIT, msec);
  Indexer.close();
  //2nd disk
  wait(INDEXER_WAIT, msec);
  Flywheel.setVelocity(FOURHT_DISK_VEL, velocityUnits::pct);
  Indexer.open(); 
  wait(INDEXER_WAIT, msec);
  Indexer.close();
  //3rd disk
  wait(INDEXER_WAIT, msec);
  Flywheel.setVelocity(FIFTH_DISK_VEL, velocityUnits::pct);
  Indexer.open(); 
  wait(INDEXER_WAIT, msec);
  Indexer.close();
  Flywheel.stop(brakeType::coast);
}

void go_for_three_square(){
  //Aim to stack
  reset_turnH(-15, 30); 
  //Go for stck
  Drivetrain.driveFor(directionType::rev, 60, distanceUnits::cm, 100, velocityUnits::pct);
  Drivetrain.stop(hold);
}


void reset_turnH(int d, int vel){
  DrivetrainInertial.calibrate();
  Drivetrain.turnToHeading(d, deg, vel, velocityUnits::pct);
}
