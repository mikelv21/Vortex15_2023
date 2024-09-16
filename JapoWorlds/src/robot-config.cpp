#include "vex.h"
#include "constants.h"
using namespace vex;
using signature = vision::signature;
using code = vision::code;

//---------------------- Devices ----------------------//
brain  Brain;
controller Controller1 = controller(primary);
// Chassis
inertial DrivetrainInertial = inertial(PORT16);
motor leftMotorA  = motor(PORT1, ratio18_1, true);
motor leftMotorB  = motor(PORT2, ratio18_1, false);
motor leftMotorC  = motor(PORT3, ratio18_1, true);
motor rightMotorA = motor(PORT4, ratio18_1, false);
motor rightMotorB = motor(PORT5, ratio18_1, true);
motor rightMotorC = motor(PORT6, ratio18_1, false);
motor_group LeftDriveSmart  = motor_group(leftMotorA, leftMotorB);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 
  WHEEL_TRAVEL, TRACK_WIDTH, TRACK_BASE, mm, EXT_GEAR_RATIO);
// Flywheel
motor FlywheelMotorA = motor(PORT18, ratio18_1, true);
motor FlywheelMotorB = motor(PORT19, ratio18_1, true);
motor_group Flywheel = motor_group(FlywheelMotorA, FlywheelMotorB);
// Intake-roller
motor Intake_roller_A = motor(PORT7, ratio18_1, false);
motor Intake_roller_B = motor(PORT12, ratio18_1, true);
motor_group Intake_roller_group = motor_group(Intake_roller_A, Intake_roller_B);

// Piston Indexer
pneumatics Indexer = pneumatics(Brain.ThreeWirePort.D);
//Expansion
motor Expansor = motor(PORT11, ratio18_1, false);
 
//---------------------- User control ----------------------//

bool isFlywheelRunning = false;
bool RemoteControlCodeEnabled = true;
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

int rc_auto_loop_function_Controller1() {

  while(true) {
    if(RemoteControlCodeEnabled) {
      chassis_control();
      intake_control();
      flywheel_control();
      indexer_control();
      expansor_control();
    }
    wait(20, msec);
  }
  return 0;
}

//---------------------- Aux functions ----------------------//
void chassis_control(){
  int drivetrainLeftSideSpeed = Controller1.Axis3.position() + Controller1.Axis1.position();
  int drivetrainRightSideSpeed = Controller1.Axis3.position() - Controller1.Axis1.position();
  
  if (drivetrainLeftSideSpeed < JOYSTICK_DEADBAND && drivetrainLeftSideSpeed > -JOYSTICK_DEADBAND) {
    if (DrivetrainLNeedsToBeStopped_Controller1) {
      LeftDriveSmart.stop();
      DrivetrainLNeedsToBeStopped_Controller1 = false;
    }
  } else {
    DrivetrainLNeedsToBeStopped_Controller1 = true;
  }
  if (drivetrainRightSideSpeed < JOYSTICK_DEADBAND && drivetrainRightSideSpeed > -JOYSTICK_DEADBAND) {
    if (DrivetrainRNeedsToBeStopped_Controller1) {
      RightDriveSmart.stop();
      DrivetrainRNeedsToBeStopped_Controller1 = false;
    }
  } else {
    DrivetrainRNeedsToBeStopped_Controller1 = true;
  }
  
  if (DrivetrainLNeedsToBeStopped_Controller1) {
    LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
    LeftDriveSmart.spin(forward);
  }
  if (DrivetrainRNeedsToBeStopped_Controller1) {
    RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
    RightDriveSmart.spin(forward);
  }
}

int flywheelTask() {
  while(true) {
    if(isFlywheelRunning) {
      Flywheel.spin(directionType::fwd, CONTROL_FLYWHEEL_VEL, velocityUnits::pct);
    }
    task::sleep(5);
  }
  return 0;
}

void flywheel_control(){
  if(Controller1.ButtonL2.pressing() && isFlywheelRunning == false ) {
    isFlywheelRunning = true;
    wait(500, msec);
  } 
  else if( Controller1.ButtonL2.pressing() && isFlywheelRunning == true) {
    isFlywheelRunning = false;
    Flywheel.stop(brakeType::coast);
    wait(500, msec);
  }
}

void intake_control(){
  if      (Controller1.ButtonA.pressing()) { Intake_roller_group.spin(forward, INTAKE_VEL, percent); } 
  else if (Controller1.ButtonB.pressing()) { Intake_roller_group.spin(reverse, INTAKE_VEL, percent); } 
  else                                     { Intake_roller_group.stop(); }
}

void indexer_control(){
  if (Controller1.ButtonR2.pressing()) { Indexer.set(true); }
  else { Indexer.set(false); }
}

void expansor_control(){
  if (Controller1.ButtonX.pressing()) { Expansor.spinToPosition(EXPANSOR_DEG, rotationUnits::deg, EXPANSOR_LAUNCH_VEL, velocityUnits::pct); } 
  else                                { Expansor.stop(brakeType::hold); }
}

void vexcodeInit( void ) {

  // calibrate the drivetrain Inertial
  wait(200, msec);
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  
  wait(50, msec);
}