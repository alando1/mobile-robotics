#include <Servo.h>
#include <PinChangeInt.h>
#include "MyEncoders.h"
#include "MyServos.h"

Servo LServo, RServo;
unsigned long time;
char message[80];

void setup()
{
  initEncoders();
  Serial.begin(9600);
	LServo.attach(2);
	RServo.attach(3);

  
  //calibrate();
  resetCounts();

  setSpeeds(0,100);
  delay(1000);
  time = millis();
  sprintf(message, "start time: %lu", time);
  Serial.println(message);

}

void loop()
{   
    float speeds[2];
    unsigned long now = millis();
    if(now < time + (unsigned long)(2000))
    {
      getSpeeds(speeds);
      Serial.print(speeds[0]);
      Serial.print("    ");
      Serial.println(speeds[1]);  
      delay(30);
    }
    else
     delay(1000);   

}
