#include <Adafruit_RGBLCDShield.h>
extern Adafruit_RGBLCDShield lcd;

// color sensor pins
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define SENSOR_OUT      8
#define SAMPLE_COUNT    7

// LCD backlight colors
#define OFF     0x0
#define RED     0x1
#define GREEN   0x2
#define YELLOW  0x3
#define BLUE    0x4
#define VIOLET  0x5
#define TEAL    0x6
#define WHITE   0x7 

//globals
int raw[3];
int currentBacklight = WHITE;
int colorDetected = 2;

//function prototypes
void initColorSensor();
void getRawValues(int values[3], int sample);
void testColorRecognition();

// Initialize color sensor
void initColorSensor()
{
    // Set pins S0-S3 to output and sensorOut to input
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SENSOR_OUT, INPUT);

    /*  S0    S1     freq scaling
     * --------------------------
     *   L     L     power down
     *   L     H     2%
     *   H     L     20%
     *   H     H     100%
     */
    // Set frequency scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);

    /*  S2    S3     channel
     * --------------------------  
     *   L     L     red
     *   L     H     blue
     *   H     L     clear (all)
     *   H     H     green
     */
    // read from clear channel
    //digitalWrite(S2, HIGH);
    //digitalWrite(S3, LOW);
}

// gets raw measurements and return average
void getRawValues(int values[3], int samples)
{
    for(int i = 0; i < 3; i++)
    {
        values[i] = 0;

        // select the channel
        switch(i)
        {
            case 0:   // Red
                digitalWrite(S2, LOW);
                digitalWrite(S3, LOW);
                break;

            case 1:   // Green
                digitalWrite(S2, HIGH);
                digitalWrite(S3, HIGH);
                break;

            case 2:   // Blue
                digitalWrite(S2, HIGH);
                digitalWrite(S3, LOW);
                break;
        }

        // Get color sensor readings 
        for(int j = 0; j < samples; j++)
        {
            int temp = pulseIn(SENSOR_OUT, HIGH);
            values[i] += temp;
        }

        values[i] /= samples; // Get average reading
    }
}

void testColorRecognition()
{
        //RED raw sensor components
        //  105 < raw[0] < 130
        //  305 < raw[1] < 330
        //  55  < raw[2] < 75
        //BLUE raw sensor components
        //  270 < raw[0] < 295
        //  180 < raw[1] < 205
        //  40  < raw[2] < 60
        //DARK-GREEN-MAT raw sensor components
        //  360 < raw[0] < 380
        //  410 < raw[1] < 435
        //  105 < raw[2] < 130     
        
    // get raw readings
    getRawValues(raw, SAMPLE_COUNT);

   if( ((90 < raw[0]) && (raw[0] < 200)) &&
        ((275 < raw[1]) && (raw[1] < 375)) &&
        ((50 < raw[2]) && (raw[2] < 105)) ) 
    {

       colorDetected = 0;
        
    }
    else if( ((250 < raw[0]) && (raw[0] < 320)) &&
        ((170 < raw[1]) && (raw[1] < 220)) &&
        ((40 < raw[2]) && (raw[2] < 80)) )
    {

        colorDetected = 1;
        
    }
    else
    {
        colorDetected = 2;
    }
    
    switch(colorDetected)
        {
            case 0: 
            {
                if(currentBacklight != RED)
                {
                    currentBacklight = RED;
                    lcd.setBacklight(RED);
                    Serial.println("RED");                
                }
                break;
            }
            case 1:
            {
                if(currentBacklight != BLUE)
                {
                    currentBacklight = BLUE;
                    lcd.setBacklight(BLUE);
                    Serial.println("BLUE");
                }
                break;
            }
            case 2:
            {
                if(currentBacklight != GREEN)
                {
                    currentBacklight = GREEN;
                    lcd.setBacklight(GREEN);
                    Serial.println("OTHER");
                }
                break;
            }
            default:
            {
                if(currentBacklight != GREEN)
                {
                    currentBacklight = GREEN;
                    lcd.setBacklight(GREEN);
                    Serial.println("default");
                }
                break;
            }
        }
  
}
