#include <PinChangeInt.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#include <Servo.h>
#include "MyEncoders.h"
#include "MyServos.h"
#include "MySharpSensor.h"
#include "MyFollowFuncs.h"

  Servo LServo, RServo;


void setup()
{
  lcd.begin(16,2);          //setup LCD
  lcd.clear();
  lcd.setCursor(0,0);
  initEncoders();           //initialize encoders
  Serial.begin(9600);
  LServo.attach(2);         //attach servos motors
  RServo.attach(3);

  //LServo.writeMicroseconds(1700);
  //RServo.writeMicroseconds(1300);

  calibrate2();                     //use precompiled calibrated data, so there is no need to calibrate
  resetCounts();

  // Set initial state to wall following
  state = STATE_FOLLOWING_WALL;
}

void loop()
{
  Update_State();   // Fetch current state

  Print_State();

  // Call function according to current state
  switch(state)
  {
   case STATE_FOLLOWING_WALL:       Follow_Wall();  break;
   case STATE_TURNING_LEFT:         Turn_Left();    break;
   case STATE_TURNING_RIGHT:        Turn_Right();   break;
   default:  break;
   
  }
  
  delay(10);
  
}


