#ifndef __MY_ENCODERS__
#define __MY_ENCODERS__
#include <PinChangeInt.h>
#define Lservo 2
#define Rservo 3
#define Lencoder 10
#define Rencoder 11
volatile unsigned long Lticks;
volatile unsigned long Rticks;
volatile float LdeltaT;
volatile float RdeltaT;
volatile float timeL, timeR;

//this function sets the tick counts to 0
void resetCounts()
{
  Lticks = 0;
  Rticks = 0;
}

//this function should return the left and right tickcounts 
// since either the start of the program or the last
//call to the function resetCounts()
void getCounts(unsigned long counts[])
{
  counts[0] = Lticks;
  counts[1] = Rticks;
}


//this function should return the instantaneous speeds of the wheels
//meassured in revolutions per second.
void getSpeeds(float speeds[])
{
  unsigned long now = millis();

  LdeltaT = now - timeL;
  RdeltaT = now - timeR;
//f = 1/T = 1/(32*deltaT)
  speeds[0] = (float)Lticks/(32.0f*LdeltaT) * 1000.0f;
  speeds[1] = (float)Rticks/(32.0f*RdeltaT) * 1000.0f;

 timeL = timeR = millis();

 resetCounts();
}

void LinterruptFunction()
{
  //float now = millis();
  //LdeltaT = (now - timeL);
  //timeL = millis();
    Lticks++;
}
void RinterruptFunction()
{
  //float now = millis();
  //RdeltaT = (now - timeR);
  //timeR = millis();
    Rticks++;
}

//this function should include whatever code necessary to initialize this module
void initEncoders()
{
    timeL = millis();
    timeR = timeL;

    pinMode(Lencoder, INPUT_PULLUP);
  attachPinChangeInterrupt(Lencoder, LinterruptFunction, FALLING);

  pinMode(Rencoder, INPUT_PULLUP);
  attachPinChangeInterrupt(Rencoder, RinterruptFunction, FALLING);
}



#endif

