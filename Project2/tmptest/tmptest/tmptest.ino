#include <PinChangeInt.h>
#include <Servo.h>
#include "MyEncoders.h"
#include "MyServos.h"

  Servo LServo, RServo;

void setup()
{
  Serial.begin(9600);
  LServo.attach(2);
  RServo.attach(3);
  initEncoders();
  calibrate2();

  setSpeedsIPS(2.0f, 2.0f);
  
}

void loop()
{
  float counts[2];
  getSpeeds(counts);
  Serial.print(counts[0]);
  Serial.print(", ");
  Serial.println(counts[1]);
  delay(250);
}
