// Tests the sensors as described in section B.1.1.2

#include "MySharpSensor.h"


void setup()
{
  Serial.begin(9600);

  Serial.println("starting sensor sampling...");
  for (int i = 0; i < 5; ++i)
  {
    Serial.println(getCombinedDistance(LFRONT));
    delay(1000);
  }
  Serial.println("done");

}

void loop()
{
  /*Print raw data measurements*/
  //Serial.println(shortToInches(analogRead(SFRONT)));
  //Serial.println(longToInches(analogRead(LFRONT)));

  /*Print combined data measurements*/
  //Serial.println(getCombinedDistance(SFRONT));
  //Serial.println(getCombinedDistance(LFRONT));
  
  delay(500);
}

