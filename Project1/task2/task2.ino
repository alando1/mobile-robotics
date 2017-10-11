#include <Servo.h>
#include <PinChangeInt.h>
#include "MyEncoders.h"
#include "MyServos.h"

extern float calibratedL[], calibratedR[];
Servo LServo, RServo;
char message[80];

void setup()
{
  initEncoders();
  Serial.begin(9600);
  LServo.attach(2);
  RServo.attach(3);

  
  calibrate();
  resetCounts();

  for(int i = 0; i<SAMPLESIZE; i++)
  {   
    Serial.print(calibratedL[i]);
    Serial.print("    ");
    Serial.println(calibratedR[i]);    
  }

}

void loop()
{   
   delay(1000);   
}

