#include <PinChangeInt.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#include <Servo.h>
#include "MyEncoders.h"
#include "MyServos.h"
#include "MySharpSensor.h"

  Servo LServo, RServo;
  float actual, error;
  const float goal = 5.0f;
  const float Kp = 0.9f;
  unsigned long startTime;

void setup()
{
  lcd.begin(16,2);          //initialize lcd
  initEncoders();           //initialize encoders
  Serial.begin(9600);
  LServo.attach(2);         //attach servos motors
  RServo.attach(3);

  calibrate2();            //use precompiled calibrated data, so there is no need to calibrate
  resetCounts();
  
  startTime = millis();

}

void loop()
{
  
  // Read Sensor 
  actual = getCombinedDistance(SFRONT);
   //Serial.print("D: ");
   //Serial.print(actual);
  // Calculate Error
  error = actual - goal;
  Serial.print("e: ");
  Serial.print(error);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Goal: ");
  lcd.print(goal);
  lcd.setCursor(0,1);
  lcd.print("error: ");
  lcd.print(error);
  // Scale by Kp 
  error *= Kp;
 
  // Apply Saturation Function
  if(error > 6.0f)
    error = 6.0f;
  else if(error < -6.0f)
    error = -6.0f;


  // Print time elapsed  
  unsigned long elapsedTimei = millis() - startTime;
  float elapsedTime = (float)elapsedTimei / 1000.0f;
   //Serial.print(" , ips: ");
  //Serial.println(error);
  Serial.print(", t: ");
  Serial.println(elapsedTime);
  setSpeedsIPS(error, error);
  

  delay(50);
  
}

