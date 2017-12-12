#ifndef __MY_SHARP_SENSOR__
#define __MY_SHARP_SENSOR__

//this function should read and store a new measurement taken with sensor given by the parameter 'sensor'
#define SFRONT  0
#define SLEFT   1
#define SRIGHT  2
#define LFRONT  3
#define N 30

float Stable[] = {
  40.0f, 32.8f, 26.5f, 22.2f, 18.7f, 16.0f,
  14.4f, 12.9f, 11.5f, 10.7f, 9.6f,  9.0f,
  8.4f, 7.9f, 7.4f, 6.9f, 6.5f, 6.0f, 5.8f,
  5.5f, 5.1f, 4.8f, 4.5f, 4.4f, 4.0f, 3.9f
  };
  
float Ltable[] = {
  150.0f, 130.0f, 110.0f, 92.0f, 81.0f, 70.0f,
  64.0f, 57.0f, 53.0f, 48.0f, 45.0f, 41.0f, 39.0f, 37.0f, 
  35.0f, 32.0f, 30.0f, 28.0f, 26.0f, 24.0f, 22.5f, 20.0f
  };

//float distBuff[4*N];
int distBuff[4*N];

//this function takes as input the value obtained by a short distance sensor,
//and it should return its respective value in inches
float shortToInches(int value)
{
    //Serial.print("Raw reading: ");    Serial.println(value);
    
    float anVolt = (float)value / 1023.0f * 5.0f;
    float f_index = (anVolt - 0.3f) / (2.8f - 0.3f)* 25.0f;

    //Serial.print("Analog Voltage: "); Serial.println(anVolt);
    //Serial.print("Float Index: ");    Serial.println(f_index);
    int lo_index = (int)f_index;
    int hi_index = lo_index + 1;
    float fraction = f_index - (float)lo_index;
    float result;
    
    
    if(f_index <0.0f)                       //check lower bound
        result = Stable[0];
    else if(f_index >25.0f)                 //check upper bound
        result = Stable[25];
    else if(fraction == 0)                  //Serial.println("right on the money!"); 
        result = Stable[lo_index];
    else
    {
        //Serial.print("lo index: ");       Serial.print(lo_index);
        //Serial.print(", hi index: ");     Serial.println(hi_index);
        
        result = fraction * (Stable[hi_index] - Stable[lo_index]);
        result += Stable[lo_index];
        
        //Serial.print("result: ");         Serial.println(result);
        //Serial.println();                 Serial.println();
    }

    // Convert to inches
    result /= 2.54f;
    
    return (result);
}

//this function takes as input the value obtained by the long distance sensor,
//and it should return its respective value in inches
float longToInches(int value)
{
    //Serial.print("Raw reading: ");        Serial.println(value);

    float anVolt = (float)value / 1023.0f * 5.0f;
    float f_index = (anVolt - 0.4f) / (2.5f - 0.4f)* 21.0f;
    
    //Serial.print("Analog Voltage: ");     Serial.println(anVolt);
    //Serial.print("Float Index: ");        Serial.println(f_index);
    
    int lo_index = (int)f_index;
    int hi_index = lo_index + 1;
    float fraction = f_index - (float)lo_index;
    float result;
  
    if(f_index <0.0f)                       //check lower bound
        return Ltable[0];
    else if(f_index >21.0f)                 //check upper bound
        return Ltable[21];
    else if(fraction == 0)                  //Serial.println("right on the money!");
        result = Ltable[lo_index];
    else
    {
        //Serial.print("lo index: ");       Serial.print(lo_index);
        //Serial.print(", hi index: ");     Serial.println(hi_index);
        
        result = fraction * (Ltable[hi_index] - Ltable[lo_index]);
        result += Ltable[lo_index];
        result /= 2.54f;
        
        //Serial.print("result: ");         Serial.println(result);
    }

    return (result);
}

void swap(float* a, int i, int j)
{
    float t = a[i];
    a[i] = a[j];
    a[j] = t;
}

void swap(int* a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}


void bubbleSort(float* a, int len)
{
    bool done = false;

    while (!done)
    {
        done = true;
        for (int i = 0; i < (len - 1); ++i)
        {
            if (a[i] > a[i + 1])
                {
                    done = false;
                    swap(a, i, i + 1);
                }
        }
    }
}

void intbubbleSort(int* a, int len)
{
    bool done = false;

    while (!done)
    {
        done = true;
        for (int i = 0; i < (len - 1); ++i)
        {
            if (a[i] > a[i + 1])
            {
                done = false;
                swap(a, i, i + 1);
            }
        }
    }
}

//the following two functions are meant for combining (taking the mean, median or etc) multiple measurements
int findAverage(int sensor, int* f)
{
    int sum = 0;
    for(int i = 0; i < N; i++)
        sum += f[sensor * N + i];

    return (sum / N);
}

int findMedian(int sensor, int* f)
{
    //bubbleSort(&(f[sensor * N]), N);
    intbubbleSort(&(f[sensor * N]), N);
    if ((N % 2) == 0)
      return ((f[sensor * N + (N / 2 - 1)] + f[sensor * N + (N / 2)]) / 2 );
    else
      return f[sensor * N + (N - 1) / 2];
}

void takeNewMeasurement(int sensor)
{
    if((sensor < 0 ) || (sensor > 3))
        Serial.print("\nError: Invalid Sensor");
    else
    {
        //Serial.print("Raw Values: ");
        for(int i=0; i < N; i++)
        {
            int val = analogRead(sensor);
            //Serial.print(val); 
            
            //if(i != (N-1))
            //  Serial.print(", ");
                
            distBuff[sensor * N + i] = val;
            //Serial.println(distBuff[sensor*N+i]);
            
            if(sensor == LFRONT)
                delay(50);
            else
                delay(20);      
        }
    }
}

//this function should combine the last N measurement taken with sensor 'sensor' and return the result.
//You are free to chose the value of N
float getCombinedDistance(int sensor)
{
    takeNewMeasurement(sensor);
    return shortToInches(findMedian(sensor, distBuff));    
    
}

#endif
