// LCD backlight colors
#define OFF     0x0
#define RED     0x1
#define GREEN   0x2
#define YELLOW  0x3
#define BLUE    0x4
#define VIOLET  0x5
#define TEAL    0x6
#define WHITE   0x7 

#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

enum OptionState 
{
    START_LOCATION,
    END_LOCATION,
    ORIENTATION,
    PATH_PLANNING,
    EXIT    
};

bool configuring = true;
bool planShortestPath = false;
OptionState currentState = START_LOCATION;
int startLocation = 0, endLocation = 15, orientation = NORTH;
uint8_t buttons;

void startLocationOptions();
void endLocationOptions();
void orientationOptions();
void pathPlanning();
void exitOptions();

void updateOptionStates()
{
    buttons = lcd.readButtons();
    switch(currentState)
    {
        case START_LOCATION:    startLocationOptions();     break;
        case END_LOCATION:      endLocationOptions();       break;
        case ORIENTATION:       orientationOptions();       break;
        case PATH_PLANNING:     pathPlanning();             break;
        case EXIT:              exitOptions();              break;
        default:    break;
    };
}

void startLocationOptions()
{
    if(buttons)
    {
        if(buttons & BUTTON_UP) 
        {
            startLocation = (++startLocation) % 16;
            delay(100);
        }
        else if (buttons & BUTTON_DOWN) 
        {
            startLocation--;
            
            if(startLocation == -1)
                startLocation = 15;
            delay(100);
        }
        
        if (buttons & BUTTON_LEFT)
        {
            currentState = EXIT;
            delay(100);
        }
        else if (buttons & BUTTON_RIGHT)
        {
            currentState = END_LOCATION;
            delay(100);
        }

        if(buttons & BUTTON_SELECT)
        {
            configuring = false;
            delay(100);
        }
    }

    if (currentState == START_LOCATION)
    {
        lcd.setCursor(0,0);
        lcd.print("Starting Locat.");
        lcd.setCursor(0,1);
        if (startLocation < 10)
        {
            lcd.print(" ");
            lcd.print(startLocation);
        }
        else
            lcd.print(startLocation);
    }
    else
        lcd.clear();
}

void endLocationOptions()
{
    if(buttons)
    {
        if(buttons & BUTTON_UP) 
        {
            endLocation = (++endLocation) % 16;
            delay(100);
        }
        else if (buttons & BUTTON_DOWN) 
        {
            endLocation--;
            
            if(endLocation == -1)
                endLocation = 15;
            delay(100);
        }
        
        if (buttons & BUTTON_LEFT)
        { 
            currentState = START_LOCATION;
            delay(100);
        }
        else if (buttons & BUTTON_RIGHT)
        {
            currentState = ORIENTATION;
            delay(100);            
        }
        
        if(buttons & BUTTON_SELECT)
        {
            configuring = false;
            delay(100);
        }
    }


    if (currentState == END_LOCATION)
    {
        lcd.setCursor(0,0);
        lcd.print("Ending Locat.");
        lcd.setCursor(0,1);
        if (endLocation < 10)
        {
            lcd.print(" ");
            lcd.print(endLocation);
        }
        else
            lcd.print(endLocation);
    }
    else
        lcd.clear();
}
void orientationOptions()
{
    if(buttons)
    {
        if(buttons & BUTTON_UP) 
        {
            orientation = (++orientation) % 4;
            delay(100);            
        }
        else if(buttons & BUTTON_DOWN)
        {
            --orientation;
            if(orientation == -1)
                orientation = 3;
            delay(100);
        }

        if(buttons & BUTTON_LEFT)
        {
            currentState = END_LOCATION;
            delay(100);
        }
        else if(buttons & BUTTON_RIGHT)
        {
            currentState = PATH_PLANNING;
            delay(100);
        }

        if(buttons & BUTTON_SELECT)
        {
            configuring = false;
            delay(100);
        }

    }
    if (currentState == ORIENTATION)
    {
        //lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Orientation");
        lcd.setCursor(0,1);
        
        switch(orientation)
        {
            case WEST:  lcd.print("West "); break;
            case NORTH: lcd.print("North"); break;
            case EAST:  lcd.print("East "); break;
            case SOUTH: lcd.print("South"); break;
            default:    lcd.print("?!!??"); break;
        };
    }
    else
        lcd.clear();
}
void exitOptions()
{
    if(buttons)
    {
        if(buttons & BUTTON_LEFT)
        {
            currentState = PATH_PLANNING;
            delay(100);
        }
        else if(buttons & BUTTON_RIGHT)
        {
            currentState = START_LOCATION;
            delay(100);
        }

        if(buttons & BUTTON_SELECT)
        {
            configuring = false;
            delay(100);
        }
            
    }
    if (currentState == EXIT)
    {
        lcd.setCursor(0,0);
        lcd.print("Exit");
    }
    else
        lcd.clear();
}

void pathPlanning()
{
    if(buttons)
    {
        if(buttons & BUTTON_LEFT)
        {
            currentState = ORIENTATION;
            delay(100);
        }
        else if(buttons & BUTTON_RIGHT)
        {
            currentState = EXIT;
            delay(100);
        }

        if(buttons & BUTTON_SELECT)
        {
            configuring = false;
            planShortestPath = true;
            delay(100);
        }
            
    }
    if (currentState == PATH_PLANNING)
    {
        lcd.setCursor(0,0);
        lcd.print("Path Planning");
    }
    else
        lcd.clear();

}

