#define PARAM_R1  6  //First radius in inches
#define PARAM_R2  10  //Second radius in inches
#define PARAM_Y   10  //Y seconds


#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#include "MyEncoders.h"

unsigned long ticksL, ticksR;

float r_wheel = 1.305f;
float r_axle  = 2.1f;

float r_outer, r_inner;
float s_outer, s_inner;

void SShapeSetup()
{
  r_outer = (float)PARAM_R1 + r_axle;
  r_inner = (float)PARAM_R1 - r_axle;

  s_outer = (3.14159f)*(r_outer);
  s_inner = (3.14159f)*(r_inner);

  ticksL = (unsigned long)((32.0f * s_outer)/(2.0f * 3.14159f * r_wheel)); 
  ticksR = (unsigned long)((32.0f * s_inner)/(2.0f * 3.14159f * r_wheel));

 float V = (float)(PARAM_R1 * 3.14159f)/(float)(PARAM_Y);
 float w = V / (float)PARAM_R1;
  
  lcd.setBacklight(RED);
  lcd.clear();
  lcd.setCursor(0,0);    
  lcd.print("V: ");
  lcd.print(V);
  lcd.print(", W: ");
  lcd.print(w);
  lcd.setCursor(0,1);
  lcd.print("Ticks: (");
  Serial.print("Max Ticks: (");
  Serial.print(ticksL);
  Serial.print(", ");
  Serial.println(ticksR);

  unsigned long counts[2];
  
  resetCounts();
  setSpeedsvw(V, -w);
  getCounts(counts);

  while( ! ((counts[0] > ticksL) && (counts[1] > ticksR)) )
  {
    getCounts(counts); 

    lcd.setCursor(8,1);
    lcd.print(counts[0]);
    lcd.print(", ");
    lcd.print(counts[1]);

    //delay(10);
  }
  setSpeeds(0, 0);
  uint8_t buttons = lcd.readButtons();

  while (!(buttons & BUTTON_SELECT))
  	buttons = lcd.readButtons();

  ticksL = (unsigned long)((32.0f * s_inner)/(2.0f * 3.14159f * r_wheel)); 
  ticksR = (unsigned long)((32.0f * s_outer)/(2.0f * 3.14159f * r_wheel));

  V = (float)(PARAM_R1 * 3.14159f)/(float)(PARAM_Y);
  w = V / (float)PARAM_R1;

  lcd.clear();
  lcd.setCursor(0,0);    
  lcd.print("V: ");
  lcd.print(V);
  lcd.print(", W: ");
  lcd.print(w);
  lcd.setCursor(0,1);
  lcd.print("Ticks: (");
  Serial.print("Max Ticks: (");
  Serial.print(ticksL);
  Serial.print(", ");
  Serial.println(ticksR);

  resetCounts();
  setSpeedsvw(V, w);
  getCounts(counts);

  while( ! ((counts[0] > ticksL) && (counts[1] > ticksR)) )
  {
    getCounts(counts); 

    lcd.setCursor(8,1);
    lcd.print(counts[0]);
    lcd.print(", ");
    lcd.print(counts[1]);

    //delay(10);
  }

  setSpeeds(0, 0);

  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.setBacklight(WHITE);
  //lcd.print("Select Again.");
  
}



