#include <Servo.h>
#include <PinChangeInt.h>
#include "MyEncoders.h"
#include "MyServos.h"
#include "ForwardParams.h"

Servo LServo, RServo;
void setup()
{
    Serial.begin(9600);
    initEncoders();
    lcd.begin(16, 2);

    LServo.attach(2);
    RServo.attach(3);

    lcd.setCursor(0,0);
    lcd.print("Calibrating...");
    calibrate();
    resetCounts();

    lcd.clear();
    lcd.print("Press Select.");


}

void loop()
{
    uint8_t buttons = lcd.readButtons();

    if (buttons & BUTTON_SELECT)
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.setBacklight(0x1);
        lcd.print("Select Pressed.");
        setForward();

    }
               
    delay(60);
}
