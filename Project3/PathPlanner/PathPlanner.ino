// NEWEST CODE - DO NOT BREAK!!!!
#include "MyEncoders.h"
#include "MyServos.h"
#include "MySharpSensor.h"
#include "MyOptions.h"
#include "MyColorSensor.h"
#include "MapCells.h"

Servo LServo, RServo;        // init servos/encoders
int selectPressed = 0;

void initLCD();
void lightShow(char* s);
void setup() 
{
    Serial.begin(9600);

    initEncoders();             // initializes encoders
    LServo.attach(2);           // attach servos motors
    RServo.attach(3);
    initLCD();
    initColorSensor();          // initialize color sensor
    initializeMaze();
    initializedirTable();
    setSpeeds(0,0);
   // calibrate2();

}

//void loop()
//{
//    //rotate(25, -25, 27);        //turn around is GREAT!!!
//
//    //rotate(-22, 20, 14);          //turn left perf
//    //rotate(25, -23, 14);        //good turn right, give left wheel edge
//    delay(2000);
//    
//    resetCounts();
//    
//    
//}

void loop()
{
    if(configuring)                 //Enter options menu
    {
        updateOptionStates();
        delay(50);
        currentCell = startLocation;
    }
    else                            //Options complete, do SLAM here
    {
        if(mapping)
            updateMappingStates();
        else if(planShortestPath)
            doShortestPath();
        else
        {


            buttons = lcd.readButtons();
            if((buttons & BUTTON_SELECT) && (printState1 == false))
            {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(row1);
                lcd.setCursor(0,1);
                lcd.print(row2);
                delay(100);
                printState1 = true;
            }
            else if((buttons & BUTTON_SELECT) && (printState2 == false))
            {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(row3);
                lcd.setCursor(0,1);
                lcd.print(row4);
                delay(100);
                printState2 = true;

            }
            else if((buttons & BUTTON_SELECT) && (printState3 == false))
            {
                lcd.clear();
                lcd.setCursor(0,0);
                lightShow("THE END");
                printState3 = true;
                delay(100);
                configuring = true;
            }

        }
    }
}

void initLCD()
{
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.setBacklight(GREEN);
    delay(100);

    lightShow("Hello!");
}

void lightShow(char* s)
{
    int i = 0;
    float end = millis() + 1000.0f;
    lcd.print(s);
    while(float now = millis() < end)
    {
        lcd.setBacklight(i);
        i = (++i)%8;
        delay(75); 
    }    
    lcd.setBacklight(GREEN);
}

