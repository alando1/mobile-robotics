#include <Servo.h>
#include <PinChangeInt.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#include "MyEncoders.h"
#include "MyServos.h"
#include "SShapeParams.h"

Servo LServo, RServo;
char message[80];

float v, w;
int t=0;

void setup()
{
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press Select");

  initEncoders();
  Serial.begin(9600);
	LServo.attach(2);
	RServo.attach(3);

  w = 3.14159f/(float)(PARAM_Y);
  v = PARAM_R1 * w;
  calibrate2();
  resetCounts();


}

void loop()
{   

  uint8_t buttons = lcd.readButtons();
  //Serial.println(buttons);
  if((buttons & BUTTON_SELECT))
  {
    //setSpeedsvw(4.0f, 1.0f);
    SShapeSetup();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Press Select");
    lcd.setBacklight(WHITE);
  }

  if((buttons & BUTTON_DOWN))
    setSpeeds(0,0);
     // Serial.print("speedL: ");
    //  Serial.print(speeds[0]);
    //  Serial.print("        speedR: ");
    //  Serial.println(speeds[1]);

   delay(100);   

}
