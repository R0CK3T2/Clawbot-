#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.



// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       Clawbot+.cpp                                              */
/*    Author:       RocketDeveloper                                           */
/*    Created:      4/24/25                                                   */
/*    Description:  TSA Clawbot Code V2! The more Pure Version!               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

//Define Controller and Brain
controller Control = controller();
brain Cortex = brain();

//Define Motors -- Change [Port, Reversed]

  //--Drivetrain/Chassis--
motor FrontLeftWheel = motor(PORT19, ratio18_1);
motor FrontRightWheel = motor(PORT9, ratio18_1, true);
motor BackRightWheel = motor(PORT10, ratio18_1, true);
motor BackLeftWheel = motor(PORT20, ratio18_1);

  //--Arm/Claw/Winch--
motor ArmMotor1 = motor(PORT18, ratio18_1);
motor ArmMotor2 = motor(PORT8,ratio18_1, true);
motor ArmMotor3 = motor(PORT17, ratio18_1);
motor ArmMotor4 = motor(PORT7, ratio18_1, true);
motor29 ClawMotor = motor29(Cortex.ThreeWirePort.A);


//Define Motor Groups - Move Simultaneously
motor_group LeftDrive = motor_group(FrontLeftWheel, BackLeftWheel);
motor_group RightDrive = motor_group(FrontRightWheel, BackRightWheel);
motor_group ArmMotors = motor_group(ArmMotor1, ArmMotor2, ArmMotor3, ArmMotor4);

//Speedmode [3 Def Values: P: (Presision; Axis/4), N: (Normal; Axis/2), S: (Speedrun; Axis/1) - Percentage Units]
char SpeedMode = 'N';

//Functions
void TaskKill(){ //Stop Robot program
  Brain.programStop();
}

void WriteScreen(){
  //Line 1: Speed Mode
  if (SpeedMode == 'N'){
    Control.Screen.setCursor(1,1); 
    Control.Screen.clearLine(1);
    Control.Screen.print("Normal");
  }else if (SpeedMode == 'S'){ 
    Control.Screen.setCursor(1,1); 
    Control.Screen.clearLine(1);
    Control.Screen.print("Overdrive");
  }else if (SpeedMode == 'P'){ 
    Control.Screen.setCursor(1,1); 
    Control.Screen.clearLine(1);
    Control.Screen.print("Prescision");
  }
}

void Normal(){
 SpeedMode = 'N';
 Control.rumble(rumbleShort);
 WriteScreen();
}
void Speedrun(){
 SpeedMode = 'S';
 Control.rumble(rumbleShort);
 WriteScreen();
}
void Prescision(){ 
 SpeedMode = 'P';
 Control.rumble(rumbleShort);
 WriteScreen();
}


//Main Functionality
int main() {
  // Initializing Robot Configuration
  vexcodeInit();
  //Button Functionality

  WriteScreen();
  Control.ButtonA.pressed(Speedrun);
  Control.ButtonX.pressed(Normal);
  Control.ButtonY.pressed(Prescision); 
  Control.ButtonUp.pressed(TaskKill);


  // Joystick Functionality (Axis 3(UD) and 4 (LR) on Left Joystick)
  //Left Drive = (Axis 3 + Axis 4)/SpeedDivisor
  //Right Drive = (Axis 3 - Axis 4)/SpeedDivisor

  while (1==1) {
    wait(5, msec); // Buffer - Don't waste Resources


   
    //Drivetrain Functionality
    if (SpeedMode == 'N'){
      //Normal Speed - Joystick Input is cut in half
      RightDrive.setVelocity((Control.Axis3.position() - Control.Axis4.position())/2,percent);
      LeftDrive.setVelocity((Control.Axis3.position() + Control.Axis4.position())/2,percent);
      LeftDrive.spin(forward);
      RightDrive.spin(forward);
    } else if (SpeedMode == 'P') {
      //Prescision Speed - Joystick Input is cut in fourths
      LeftDrive.setVelocity((Control.Axis3.position() + Control.Axis4.position())/4,percent);
      RightDrive.setVelocity((Control.Axis3.position() - Control.Axis4.position())/4,percent);
      LeftDrive.spin(forward);
      RightDrive.spin(forward);
    } else if (SpeedMode == 'S'){
      //Overdrive Mode - Joystick Input is not cut (Maximum Speed Differential)
      LeftDrive.setVelocity((Control.Axis3.position() + Control.Axis4.position()),percent);
      RightDrive.setVelocity((Control.Axis3.position() - Control.Axis4.position()),percent);
      LeftDrive.spin(forward);
      RightDrive.spin(forward);
    }else{ 
      LeftDrive.setStopping(coast);
      RightDrive.setStopping(coast);
      RightDrive.stop(); 
      LeftDrive.stop();
    } 

    //Arm Functionality
    if (Control.Axis2.position() != 0){
      ArmMotors.setVelocity(Control.Axis2.position(), percent);
      ArmMotors.spin(forward);
    }else{
      ArmMotors.setStopping(brake);
      ArmMotors.stop();
    }

    //Claw Functionality
    if (Control.ButtonL1.pressing() == true){
      ClawMotor.setVelocity(100,percent); 
      ClawMotor.spin(forward);
    }else if (Control.ButtonR1.pressing() == true){
      ClawMotor.setVelocity(100,percent); 
      ClawMotor.spin(reverse);
    }else{   
      ClawMotor.stop();
    }
  }
}