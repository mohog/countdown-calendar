#define SW_READ_INTERVAL    50ul        //mS between switch reads
#define FASTLED_FORCE_SOFTWARE_PINS
////////
#include "FastLED.h"
#include <Adafruit_DotStar.h>//trinket on board RGB off
Adafruit_DotStar strip = Adafruit_DotStar(1, INTERNAL_DS_DATA, INTERNAL_DS_CLK, DOTSTAR_BGR);//trinket on board RGB off

#define NUM_LEDS 28

#define DATA_PIN        2
CRGB leds[NUM_LEDS];
///////////
const byte pinSwitch = 3;

byte
    option,
    swLast;

void setup( void )
{
   ////////////////////////////
  strip.begin(); strip.setPixelColor(0, 0, 0, 0); strip.show();//trinket on board RGB off
  
//  FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  
  LEDS.clear(true);
  FastLED.setBrightness(255*.5);//brightness max times decimal 0.5 = half bright

    //////////////////////
    Serial.begin(9600);
    
    pinMode( pinSwitch, INPUT_PULLUP );
    swLast = digitalRead( pinSwitch );
    option = 0;
        
}//setup

void loop( void )
{
    static byte
        lastOption = 0xff;

    //read the switch
    ReadSwitch();

    //show how option value changes when button pressed
    if( lastOption != option )
    {
        lastOption = option;
        
        //print it to show the option value changed
        Serial.print( "Day " ); Serial.println( option );
        
    }//if
    
    //your code could act on the value of option to select which effect to play
    //...

    ////////////////////////////

    for (int i = 0; i < option; i++) {
    leds[option-3] = CRGB::Red;// ( 255, 0, 0);//Lead LED Color
    leds[option-2] = CRGB::Black;// Second LED Color
    leds[option-1] = CRGB::Yellow;//Wipe LED Color
    leds[option] = CRGB::Black;//Wipe LED Color
    //leds[option-2] = CRGB::Green;//Follow up LED Color
    FastLED.show();}
     if ( option <= 0){
     fill_solid(leds, NUM_LEDS, CRGB::Green);//Reset Fill Color
     FastLED.show();
     //LEDS.clear(true);
     }
    delay(10);
   

    ///////////////////////////
    
}//loop

void ReadSwitch( void )
{
    byte
        swNow;
    unsigned long
        timeNow;
    static unsigned long
        timeSwitch = 0;
        
    //check the time
    timeNow = millis();
    //has our switch read interval passed?
    if( (timeNow - timeSwitch) >= SW_READ_INTERVAL )
    {
        //yes; set up for the next switch read
        timeSwitch = timeNow;

        //read the switch
        swNow = digitalRead( pinSwitch );
        //is is different than the last read value?
        if( swNow != swLast )
        {
            //yes; save the new as last
            swLast = swNow;
            //if new is low it means the button was just pressed
            if( swNow == LOW )
            {
                //so bump the "option" count; in this example it is limited to 16 values (0x0 ..0xf)
                option = (option + 1);
                if (option>NUM_LEDS){
                  option=0;
                
                }
                
            }//if
            
        }//if
        
    }//if
    
}//ReadSwitch
