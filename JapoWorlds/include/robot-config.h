using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern smartdrive Drivetrain;
extern motor_group Flywheel;
extern motor Intake_roller_A;
extern motor Intake_roller_B;
extern motor_group Intake_roller_group;
extern pneumatics Indexer;
extern motor Expansor;
extern motor FlywheelMotorA;
extern motor FlywheelMotorB;
extern inertial DrivetrainInertial;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
extern int rc_auto_loop_function_Controller1();
void vexcodeInit( void );
void chassis_control();
void intake_control();
void indexer_control();
void expansor_control();
void flywheel_control();
int flywheelTask();