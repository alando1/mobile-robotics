#define STATE_FOLLOWING_WALL 0 // State Defines
#define STATE_TURNING_LEFT 1
#define STATE_TURNING_RIGHT 2

// Function Prototypes 
float getFrontDistance();
void Print_State();
void Update_State();
void Folow_Wall();
void Turn_Left();
void Turn_Right();

// Proportional Gain Constants
const float goal_w = 5.0f;
const float goal_v = 8.0f;
const float Kp_w = 0.4f;
const float Kp_v = 0.4f;
const float bounds_w = 0.5f;
const float bounds_v = 6.0f;

// Used to calculate the error between goal and actual distance
float actualSF, actualSL, actualLF, error_w, error_v;

int state;  // current state

void Print_State()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    //lcd.print("state: ");
    if (state == STATE_FOLLOWING_WALL)
        lcd.print("WALL FOLLOW");
    else if (state == STATE_TURNING_LEFT)
        lcd.print("TURN LEFT");
    else if (state == STATE_TURNING_RIGHT)
        lcd.print("TURN RIGHT");
    else
        lcd.print("?");    
}

void Update_State()
{
    float turnThreshold = 9.0f; // Minimum distance from wall needed to turn 
    float leftDistance = getCombinedDistance(SLEFT);
    float frontDistance = getFrontDistance();//getCombinedDistance(SFRONT);
    
    switch (state)
    {
    case STATE_FOLLOWING_WALL:
      // Check if left sensor is invalid, and if there is room to turn left
      if(leftDistance == -1.0f && ((frontDistance > turnThreshold ) || (frontDistance == -1.0f)))
      {
        state = STATE_TURNING_LEFT;
        return;
      }
      // Check if robot has reached the turning threashold, and check if it can only go right
      else if((frontDistance <= turnThreshold) && (leftDistance != -1.0f))
      {
        state = STATE_TURNING_RIGHT;
        return;
      }   
      break;

    case STATE_TURNING_LEFT:      
      // Check if wall is detected by left sensor, and no obstacle is in front of it
      if ((leftDistance != -1.0f) && ((frontDistance > turnThreshold ) || (frontDistance == -1.0f)))
      {
        state = STATE_FOLLOWING_WALL;
        setSpeedsvw(6.0f, 0.0f);
        return;
      }
      // Check if robot is turning into another wall before it has detected a new wall to follow
      else if ((leftDistance == -1.0f) && (frontDistance < turnThreshold ))
      {
            state = STATE_TURNING_RIGHT;
            return;        
      }
      break;  

    case STATE_TURNING_RIGHT:
        // Check if the left sensor is valid, and if there is space to start wall following
        if((leftDistance != -1.0f) && ((frontDistance > turnThreshold ) || (frontDistance == -1.0f)))
        {
            state = STATE_FOLLOWING_WALL;
            setSpeedsvw(6.0f, 0.0f);
            return;
        }

        // Check if left distance is invalid, and if front Distance is within range
        else if((leftDistance == -1.0f) && ((frontDistance >= turnThreshold ) || (frontDistance == -1.0)))
        {
            state = STATE_TURNING_LEFT;
            return;            
        }
        break;
        
    default: 
        break;
  }
}

// Function executed when robot is in wall following state
void Follow_Wall()
{
  float actual_F = getCombinedDistance(SFRONT);
  float actual_L = getCombinedDistance(SLEFT);
  
  // Calculate Error
  error_w = actual_L - goal_w;
  
  // Scale by Kp 
  error_w *= Kp_w;
 
  // Apply Saturation Function
  error_w = constrain(error_w, -bounds_w, bounds_w);

  // Check if front sensor detects a wall
  if((actual_F > goal_v) && (actual_F != -1.0f))
   {     
        // Calculate Error   
        error_v = actual_F - goal_v;
        // Scale by Kp
        error_v *= Kp_v;
        
        // Saturation Function
        error_v = constrain(error_v, -bounds_v, bounds_v);
              
        setSpeedsvw(error_v, error_w);
   }
  // If no wall detected, go full speed
  else if(actual_F == -1.0f)
       setSpeedsvw(6.0f, error_w);
  
    }

// Function executes when state is turning left
void Turn_Left()
{
    setSpeedsvw(3.0f, 0.5f);
}
// Function executes when state is turning right
void Turn_Right()
{
    setSpeedsvw(3.0f, -0.5f);    
}


float getFrontDistance()
{
  bool L = false, S = false;
  
  // get long reading
  actualLF = getCombinedDistance(LFRONT);
  actualSF = getCombinedDistance(SFRONT);
  //delay(20);  

  //if((actualLF != -1.0f) && (actualSF != -1.0f))
    
  // if it's out of bounds, read the SHORT one
  
  if((actualLF <= 11.8) || (actualLF > 59.0f))
  {
    L = false;
    actualSF = getCombinedDistance(SFRONT);

    // if the SHORT is out of bounds, flag both
  if((actualSF < 2.54f) || (actualSF > 11.8f))
      S = false;
    else
      S = true;
  }
  else
    L = true;


/*  // get short reading
  actualSF = getCombinedDistance(SFRONT);
  //delay(20);  

  // if it's out of bounds, read the long one
  if((actualSF < 2.54f) || (actualSF > 11.8f))
  {
    S = false;
    actualLF = getCombinedDistance(LFRONT);

    // if the long is out of bounds, flag both
    if((actualLF <= 11.8) || (actualLF > 59.0f))
      L = false;
    else
      L = true;
  }
  else
    S = true;*/
    
  //delay(50);

  lcd.clear();
  lcd.setCursor(0,0);

  if (S)
  {
    lcd.print("SHORT: ");
    lcd.print(actualSF);
    return actualSF; 
  }
  else if (L)
  {
    lcd.print("LONG: ");
    lcd.print(actualLF);
    return actualLF;  
  }
  else
  {
    lcd.print("OUT OF RANGE");
    return -1.0f;    
  }

  //Serial.print("actual L: ");
  //Serial.print(actualLF);

  //Serial.print(", actual S: ");
  //Serial.println(actualSF);

}
