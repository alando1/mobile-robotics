#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "MyEncoders.h"

#define PARAM_X  60   //X inches
#define PARAM_Y  10   //Y seconds

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setForward()
{
    float speedIPS;

	speedIPS = (float)PARAM_X / (float)PARAM_Y;

	Serial.print("speedIPS: ");
	Serial.println(speedIPS);

	// if the speed is less than the maximum servo speed
    if(speedIPS <= 6.5f) 
    {
        resetCounts();
  		setSpeedsIPS(speedIPS, speedIPS);

	  	unsigned long maxTicks = (unsigned long)((PARAM_X / 8.2f) * 32);
        unsigned long tickCounts[2];
        getCounts(tickCounts);

        while((tickCounts[0] <= maxTicks) && (tickCounts[1] <= maxTicks))
  		    getCounts(tickCounts);
  
  		setSpeeds(0, 0);
  
 	}
    else
        lcd.print("Invalid Request");
}