#ifndef __MY_SHARP_SENSOR__
#define __MY_SHARP_SENSOR__

//this function should read and store a new measurement taken with sensor given by the parameter 'sensor'
#define SFRONT  0     //variables changed in accordance to powerpoint
#define SLEFT   1     //variables changed in accordance to powerpoint
#define SRIGHT  2
#define LFRONT  3
#define N 10

// Array of values corresponding to short distance measurements
float Stable[] = {
  40.0f, 32.8f, 26.5f, 22.2f, 18.7f, 16.0f,
  14.4f, 12.9f, 11.5f, 10.7f, 9.6f,  9.0f,
  8.4f, 7.9f, 7.4f, 6.9f, 6.5f, 6.0f, 5.8f,
  5.5f, 5.1f, 4.8f, 4.5f, 4.4f, 4.0f, 3.9f
  };

// Array of values corresponding to long distance measurements
float Ltable[] = {
  150.0f, 130.0f, 110.0f, 92.0f, 81.0f, 70.0f,
  64.0f, 57.0f, 53.0f, 48.0f, 45.0f, 41.0f, 39.0f, 37.0f, 
  35.0f, 32.0f, 30.0f, 28.0f, 26.0f, 24.0f, 22.5f, 20.0f
  };

// Global Array for Sensor Measurements
float distBuff[4*N];

//this function takes as input the value obtained by a short distance sensor,
//and it should return its respective value in inches
float shortToInches(int value)
{
  // Map measurement of sensor back to corresponding voltage reading, 
  // then map the voltage value to the index of the hardcoded measurements
  float anVolt = (float)value / 1023.0f * 5.0f;
  float f_index = (anVolt - 0.3f) / (2.8f - 0.3f)* 25.0f;

  // Interpolate between hardcoded value to obtain average measurement
  int lo_index = (int)f_index;
  int hi_index = lo_index + 1;
  float fraction = f_index - (float)lo_index;
  float result;

  // Check the range of the index; if out of range, return -1.0
  if((f_index <0.0f) || (f_index >25.0f))
  {
    Serial.println("analog value out of range");
    return -1.0f;
  }
  else if(fraction == 0)    // No fractional component of measurement
  {
    result = Stable[lo_index];  // Map directly to hardcoded value
  }
  else  // Interpolate and convert cm to inches
  {
    result = fraction * (Stable[hi_index] - Stable[lo_index]);
    result += Stable[lo_index];
    result /= 2.54f;    // Convert to inches
  }

  return (result);
}

//this function takes as input the value obtained by the long distance sensor,
//and it should return its respective value in inches
float longToInches(int value)
{
  // Map Measurements 
  float anVolt = (float)value / 1023.0f * 5.0f;
  float f_index = (anVolt - 0.4f) / (2.5f - 0.4f)* 21.0f;

  // Interpolate 
  int lo_index = (int)f_index;
  int hi_index = lo_index + 1;
  float fraction = f_index - (float)lo_index;
  float result;

  // Check range
  if((f_index <0.0f) || (f_index >21.0f))
  {
    Serial.println("analog value out of range");
    return -1.0f;
  }
  else if(fraction == 0)
  {
   // Serial.println("right on the money!");
    result = Ltable[lo_index];
  }
  else
  {
    result = fraction * (Ltable[hi_index] - Ltable[lo_index]);
    result += Ltable[lo_index];
    result /= 2.54f;
  }

 return (result);
}

// Function used by BubbleSort algorithm; needed to obtain median values
void swap(float* a, int i, int j)
{
    float t = a[i];
    a[i] = a[j];
    a[j] = t;
}

// Sorts Array in ascending order 
void bubbleSort(float* a, int len)
{
  bool done = false;

 while (!done)
 {
    done = true;
    for (int i = 0; i < (len - 1); ++i)
      if (a[i] > a[i + 1])
      {
        done = false;
        swap(a, i, i + 1);
      }
 }
}


//the following two functions are meant for combining (taking the mean, median or etc) multiple measurements


void takeNewMeasurement(int sensor)
{
  if((sensor < 0 ) || (sensor > 3))
     {
      Serial.print("\nError: Invalid Sensor");
     }
  else
  {
    //case for short range sensors
    if(sensor < LFRONT)
    {
      for(int i=0; i < N; i++)
      {
        int val = analogRead(sensor);
        distBuff[sensor * N + i] = shortToInches(val);
        
        delay(20);      
      }
    }
    else
      for(int i=0; i < N; i++)
      {
        int val = analogRead(sensor);
        distBuff[sensor * N + i] = longToInches(val);
        
        delay(50);      
      }
  }
}


//this function should combine the last N measurement taken with sensor 'sensor' and return the result.
//You are free to chose the value of N
float getCombinedDistance(int sensor)
{
  takeNewMeasurement(sensor);
  if(sensor == LFRONT)
  {
    //Serial.println("Finding mean");
      float sum = 0;
      
      //return average for long distance sensor
      for(int i = 0; i < N; i++)
        sum += distBuff[sensor * N + i];
        
      return (sum / (float)N);
  }
  else if((sensor < 3) && (sensor >= 0))
  {
    //Serial.println("Finding median");
    //bubbleSort and return median
    bubbleSort(&distBuff[sensor * N], N);
    return (( distBuff[sensor * N + 4] + distBuff[sensor * N + 5]) / 2.0f );
  }  
}


#endif
