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

	sprintf(message,"Setting speeds (%d, %d)", speedL, speedR);
	Serial.println(message);
	speedL = constrain(speedL, -100, 100);
	speedR = constrain(speedR, -100, 100);

	sprintf(message, "Constrained: (%d, %d)", speedL, speedR);
	Serial.println(message);

	int tmpL = map(speedL, -100, 100, 1300, 1700);
	int tmpR = map(speedR, -100, 100, 1700, 1300);
	LServo.writeMicroseconds(tmpL);
	RServo.writeMicroseconds(tmpR);

	sprintf(message, "micros: (%d, %d)", tmpL, tmpR);
	Serial.println(message);
  
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
				lesser = i-direction;
				greater = i;	

				if(direction == 1)
				{
					microsLesser = map(lesser, start, end, 0, 100);
					microsGreater = map(greater, start, end, 0, 100);
				}
				else if(direction == -1)
				{
					microsLesser = map(lesser, end, start, -100, 0);
					microsGreater = map(greater, end, start, -100, 0);
				}

				if(t == 0)
					micros = map(rps, calibratedL[lesser], calibratedL[greater], microsLesser, microsGreater);
				if(t == 1)
					micros = map(rps, calibratedR[lesser], calibratedR[greater], microsLesser, microsGreater);

				speeds[t] = (int)micros;
			}
		}
	}

  Serial.print("requested rps (");
  Serial.print(rpsLeft);
  Serial.print(", ");
  Serial.print(rpsRight);
	Serial.println(")");

	setSpeeds(speeds[0], speeds[1]);
}

//same as the function setSpeeds, but the inputs now indicate 
//the inches per second (ips) at which each wheel should spin
void setSpeedsIPS(float ipsLeft, float ipsRight)
{
	float ipr = 8.1995f;	//inches per revolution
	float tmpL, tmpR;
	tmpL = ipsLeft/ipr;
	tmpR = ipsRight/ipr;

	Serial.print("requested ips L: ");
	Serial.print(ipsLeft);
	Serial.print(",     R: ");
	Serial.println(ipsRight);
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
	for(int i=0; i< SAMPLESIZE; i++)
	{

			speed = map(i, 0, SAMPLESIZE-1, -100, 100);
			setSpeeds(speed, -speed);
			delay(2000);
			getSpeeds(speeds);
			calibratedL[i]= speeds[0];
			calibratedR[i]= speeds[1];


		Serial.print("i="); Serial.print(speed);
	    Serial.print(",         L: ");
	    Serial.print(calibratedL[i]);
	    Serial.print(",         R: ");
	    Serial.println(calibratedR[i]);

	}
	Serial.println("Calibration done.\n");
}


//this function should make the robot move with a linear speed of v 
//measured in inches per second, and an angular speed of w
//in rads per second. Positive ws should indicate a counterclockwise spin.
void setSpeedsvw(float v, float w)
{
	float vL, vR, dr, R;

	//R = radius of circle
	R = v / w;

	//dr = half the distance from wheel to wheel
	dr = 2.10f;
	vL = w * (R-dr);
	vR = w * (R+dr);

	setSpeedsIPS(vL, vR);

}



#endif 
