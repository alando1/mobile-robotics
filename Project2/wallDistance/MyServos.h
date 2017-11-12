#ifndef __MY_SERVOS__
#define __MY_SERVOS__

#define SAMPLESIZE 41
//NOTE: you are allowed to modify this file as long as the functions
//implement the requested functionality
extern Servo LServo, RServo;
float calibratedL[SAMPLESIZE], calibratedR[SAMPLESIZE];

//this function should set the left and right speeds of the wheel
//when both inputs are positive the robot should move forward
//when both inputs are negative the robot should move backward
//microsLeft should encode the left wheel velocity and viceversa
//NOTE: REQUIRES -100 TO 100 RANGE FOR INPUTS
void setSpeeds(int speedL, int speedR)
{
  char message[80];

  //sprintf(message,"Setting speeds (%d, %d)", speedL, speedR);
  //Serial.println(message);
  speedL = constrain(speedL, -100, 100);        //constrain speeds to range
  speedR = constrain(speedR, -100, 100);

 // sprintf(message, "Constrained: (%d, %d)", speedL, speedR);
  //Serial.println(message);

  int tmpL = map(speedL, -100, 100, 1300, 1700);    //interpolate between data
  int tmpR = map(speedR, -100, 100, 1700, 1300);    //reverse right wheel
  LServo.writeMicroseconds(tmpL);
  RServo.writeMicroseconds(tmpR);

  //sprintf(message, "micros: (%d, %d)", tmpL, tmpR);
  //Serial.println(message);
  
}

//same as the function setSpeeds, but the inputs now indicate 
//the revolutions per second (rps) at which each wheel should spin
void setSpeedsRPS(float rpsLeft, float rpsRight)
{
  float microsLesser, microsGreater, micros, rps;
  int speeds[2];
  int greater, lesser, direction, start, end;

  for(int t=0; t<2; t++)
  {
    if(t==0)
      rps = rpsLeft;
    if(t==1)
      rps = rpsRight;
    //edge case for wheel not moving.
    if(rps == 0.0f)
    {
      micros = 0.0f;
      speeds[t] = (int)micros;
    }
    else 
    {
      //wheel moves forward, filling 2nd half of calibrated array 
      if(rps > 0.0f)
      {
        direction = 1;
        start = (SAMPLESIZE-1)/2;
        end = SAMPLESIZE-1;
      }
      else
      {
        //wheel moves reverse, filling 1st half of calibrated array
        direction = -1;
        start = (SAMPLESIZE-1)/2;
        end = 0;
        rps *= -1.0f;
      }

      int i= start; 
      //increment i until rps is not greater or bound is reached
      if(t == 0)
        while(rps > calibratedL[i] && i >= 0 && i < SAMPLESIZE)
        {
          if(direction == 1)
            i++;
          else
            --i;
        }
      if(t == 1)
        while(rps > calibratedR[i] && i >= 0 && i < SAMPLESIZE)
        {
          if(direction == 1)
            i++;
          else
            --i;
        }

      //edge cases for array bounds
      if(i == SAMPLESIZE)
        micros = 100.0f;
      else if(i < 0)
        micros = -100.0f;
      else
      {
        lesser = i-direction;       //lesser bound is i minus direction int(1 or -1)
        greater = i;                //greater bound is i

        if(direction == 1)          //1 = positive range
        {
          microsLesser = map(lesser, start, end, 0, 100);
          microsGreater = map(greater, start, end, 0, 100);
        }
        else if(direction == -1)    //-1 = negative range
        {
          microsLesser = map(lesser, end, start, -100, 0);
          microsGreater = map(greater, end, start, -100, 0);
        }

        if(t == 0)      //t=0, left wheel calibrated value returned
          micros = map(rps, calibratedL[lesser], calibratedL[greater], microsLesser, microsGreater);
        if(t == 1)      //t=1, right wheel calibrated value returned
          micros = map(rps, calibratedR[lesser], calibratedR[greater], microsLesser, microsGreater);

        speeds[t] = (int)micros;
      }
    }
  }

  //Serial.print("requested rps (");      //print to serial monitor rps data for left and right wheel
  //Serial.print(rpsLeft);
  //Serial.print(", ");
  //Serial.print(rpsRight);
  //Serial.println(")");

  setSpeeds(speeds[0], speeds[1]);      //send calibrated data as input for setSpeeds
}

//same as the function setSpeeds, but the inputs now indicate 
//the inches per second (ips) at which each wheel should spin
void setSpeedsIPS(float ipsLeft, float ipsRight)
{
  float ipr = 2.0f * 3.141f * 1.305f;     //inches per revolution
  float tmpL, tmpR;
  tmpL = ipsLeft/ipr;                     //rps of left wheel
  tmpR = ipsRight/ipr;                    //rps of right wheel

  //Serial.print("requested ips L: ");      //print to serial requested ips
  //Serial.print(ipsLeft);
  //Serial.print(",     R: ");
  //Serial.println(ipsRight);
  setSpeedsRPS(tmpL, tmpR);               
}


//this function is meant for doing whatever necessary for the 
//functions 'setSpeedsIPS' and 'setSpeedsRPS' to work properly
void calibrate()
{

  int speed;
  float speeds[2];

  getSpeeds(speeds);          

  Serial.println("Calibrating ...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");

  for(int i=0; i< SAMPLESIZE; i++)                  //iterate to create calibrated array
  {

      speed = map(i, 0, SAMPLESIZE-1, -100, 100);   //change range of value i
      setSpeeds(speed, -speed);                     //set speeds of left and right

      lcd.setCursor(0, 1);                          //print speed to lcd
      lcd.print("speed: ");
      lcd.print(speed);
      lcd.print("   ");

      delay(2000);                                  //delay 2 seconds each iteration
      getSpeeds(speeds);                            //getSpeeds updates deltaT for both wheels
      calibratedL[i]= speeds[0];                    //store left and right speeds in respective arrays
      calibratedR[i]= speeds[1];

      Serial.print("i="); Serial.print(speed);      //print to serial monitor corresponding calibrated values
      Serial.print(",         L: ");
      Serial.print(calibratedL[i]);
      Serial.print(",         R: ");
      Serial.println(calibratedR[i]);

  }

  setSpeeds(0,0);                                   //upon completion stop motors.
  Serial.println("Calibration done.\n");
}

void calibrate2()                                   //static array very useful for storing once calibrated data is compiled
{
	Serial.println("Loading calibration values from static code...");
	calibratedL[0] = 0.70f;
	calibratedR[0] = 0.70f;
	calibratedL[1] = 0.72f;
	calibratedR[1] = 0.72f;
	calibratedL[2] = 0.72f;
	calibratedR[2] = 0.73f;
	calibratedL[3] = 0.70f;
	calibratedR[3] = 0.72f;
	calibratedL[4] = 0.72f;
	calibratedR[4] = 0.73f;
	calibratedL[5] = 0.72f;
	calibratedR[5] = 0.73f;
	calibratedL[6] = 0.72f;
	calibratedR[6] = 0.72f;
	calibratedL[7] = 0.70f;
	calibratedR[7] = 0.73f;
	calibratedL[8] = 0.70f;
	calibratedR[8] = 0.72f;
	calibratedL[9] = 0.69f;
	calibratedR[9] = 0.70f;
	calibratedL[10] = 0.69f;
	calibratedR[10] = 0.70f;
	calibratedL[11] = 0.66f;
	calibratedR[11] = 0.69f;
	calibratedL[12] = 0.64f;
	calibratedR[12] = 0.66f;
	calibratedL[13] = 0.61f;
	calibratedR[13] = 0.62f;
	calibratedL[14] = 0.56f;
	calibratedR[14] = 0.58f;
	calibratedL[15] = 0.48f;
	calibratedR[15] = 0.50f;
	calibratedL[16] = 0.37f;
	calibratedR[16] = 0.41f;
	calibratedL[17] = 0.30f;
	calibratedR[17] = 0.31f;
	calibratedL[18] = 0.17f;
	calibratedR[18] = 0.19f;
	calibratedL[19] = 0.08f;
	calibratedR[19] = 0.09f;
	calibratedL[20] = 0.00f;
	calibratedR[20] = 0.00f;
	calibratedL[21] = 0.03f;
	calibratedR[21] = 0.03f;
	calibratedL[22] = 0.12f;
	calibratedR[22] = 0.12f;
	calibratedL[23] = 0.25f;
	calibratedR[23] = 0.27f;
	calibratedL[24] = 0.33f;
	calibratedR[24] = 0.36f;
	calibratedL[25] = 0.44f;
	calibratedR[25] = 0.45f;
	calibratedL[26] = 0.52f;
	calibratedR[26] = 0.53f;
	calibratedL[27] = 0.58f;
	calibratedR[27] = 0.59f;
	calibratedL[28] = 0.61f;
	calibratedR[28] = 0.62f;
	calibratedL[29] = 0.66f;
	calibratedR[29] = 0.67f;
	calibratedL[30] = 0.67f;
	calibratedR[30] = 0.67f;
	calibratedL[31] = 0.70f;
	calibratedR[31] = 0.70f;
	calibratedL[32] = 0.70f;
	calibratedR[32] = 0.72f;
	calibratedL[33] = 0.70f;
	calibratedR[33] = 0.72f;
	calibratedL[34] = 0.72f;
	calibratedR[34] = 0.72f;
	calibratedL[35] = 0.73f;
	calibratedR[35] = 0.73f;
	calibratedL[36] = 0.72f;
	calibratedR[36] = 0.73f;
	calibratedL[37] = 0.73f;
	calibratedR[37] = 0.72f;
	calibratedL[38] = 0.72f;
	calibratedR[38] = 0.73f;
	calibratedL[39] = 0.73f;
	calibratedR[39] = 0.73f;
	calibratedL[40] = 0.72f;
	calibratedR[40] = 0.73f;
}


//this function should make the robot move with a linear speed of v 
//measured in inches per second, and an angular speed of w
//in rads per second. Positive ws should indicate a counterclockwise spin.
void setSpeedsvw(float v, float w)
{
  float vL, vR, dr, R;

  //R = radius of circle
  

  //dr = half the distance from wheel to wheel
  dr = 2.10f;

  if (w < 0)          //if w < 0, vL becomes the outer wheel radial velocity and vR the inner wheel radial velocity
  {
  	R = v / -w;
  	vL = -w * (R+dr);
    vR = -w * (R-dr);
  }
  else if (w > 0)     //if w > 0, vR becomes the outer wheel radial velocity and vL the inner wheel radial velocity
  {
  	R = v / w;
  	vL = w * (R-dr);
  	vR = w * (R+dr);
  }
  else                //edge case for 0
  {
  	vL = 0.0f;
  	vR = 0.0f;
  }

  if(v >= 6.5f)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Invalid Request");
    lcd.setCursor(0,1);
    lcd.print("v: ");
    lcd.print(v);
  }
  else
  {
    Serial.print("vL: ");       //print to serial monitor vL and vR
    Serial.print(vL);
    Serial.print(", vR: ");
    Serial.println(vR);
    setSpeedsIPS(vL, vR);
 
  }
}



#endif 

