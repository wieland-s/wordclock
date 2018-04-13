#include <Time.h>
#include <TimeLib.h>
#include <FastLED.h>


#define DATA_PIN    3
#define NUM_LEDS    120
#define LED_TYPE    WS2812B
#define COLOR_ORDER BRG

#define BRIGHTNESS         220 // Value range: 0-255
// #define FRAMES_PER_SECOND  120

// Define the array of leds
CRGB leds[NUM_LEDS];

// Time variables
int h;
int m;
int s;

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  setTime(12,0,0,12,4,2018); //Initialize current time as Midnight/noon 12.04.2018

  Serial.begin(9600); //Begin Serial for debugging purposes
}

void loop() {
  /* Get current time */
  h=hourFormat12();    // Returns the hour of current time between 1-12
  m=minute();         // Returns the minute of current time
  s=second();        // Returns the second of current time (not used, included for completeness) 

  // for debug: 
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);

  // "Es ist" (english: it is) constant
    leds[119] = CRGB::White; //E
    leds[118] = CRGB::White; //S
    leds[116] = CRGB::White; //I
    leds[115] = CRGB::White; //S
    leds[114] = CRGB::White; //T

    

  // leds[0] = CRGB::White; FastLED.show(); delay(30);

  //Minutes
  // minutes 0-5 -> "Uhr" (o' clock)
  if ((m>=0 && m<5)){
    leds[8] = CRGB::White;
    leds[9] = CRGB::White;
    leds[10] = CRGB::White;
  }
  
  // minutes 5-10 or 55-60: Fünf (five)
  if ((m>=5 && m<10) || (m>=55 && m<60)){
    leds[112] = CRGB::White;
    leds[111] = CRGB::White;
    leds[110] = CRGB::White;
    leds[109] = CRGB::White;
  }

  //Zehn (ten)
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    leds[96] = CRGB::White;
    leds[97] = CRGB::White;
    leds[98] = CRGB::White;
    leds[99] = CRGB::White;
  }

  // Viertel (quarter)
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    leds[91] = CRGB::White;
    leds[90] = CRGB::White;
    leds[89] = CRGB::White;
    leds[88] = CRGB::White;
    leds[87] = CRGB::White;
    leds[86] = CRGB::White;
    leds[85] = CRGB::White;
  }
  
  //Zwanzig (twenty)
  if ((m>=20 && m<25) || (m>=35 && m<45)){
    leds[100] = CRGB::White;
    leds[101] = CRGB::White;
    leds[102] = CRGB::White;
    leds[103] = CRGB::White;
    leds[104] = CRGB::White;
    leds[105] = CRGB::White;
    leds[106] = CRGB::White;
  }  

/**
   Minutes between 25-30 or 35-40 - Light "TWENTY," "FIVE," "MINUTES" 
  if ((m>=25 && m<30) || (m>=35 && m<40)){
   
  }
*/
  /* Minutes between 30-35 - Light "HALF" */
  if ((m>=25 && m<35)){
    leds[71] = CRGB::White;
    leds[70] = CRGB::White;
    leds[69] = CRGB::White;
    leds[68] = CRGB::White;
  }
  
  // Minutes between 5-35 -> "Nach" (past)
  if ((m>=5) && (m<25)){
    leds[74] = CRGB::White;
    leds[75] = CRGB::White;
    leds[76] = CRGB::White;
    leds[77] = CRGB::White;;
  }
  
  // Minutes between 35-60 - Light "TO" & MODIFY CURRENT HOUR VALUE */
  if (m>=35){
    leds[78] = CRGB::White;
    leds[79] = CRGB::White;
    leds[80] = CRGB::White;
    h++; //Add 1h to current hour
    /*Set time to twelve for hour around midnight, noon */
    if (h==0){
      h=12; 
    }
    /*Corner case for 12:35-12:59 */
    if (h==13){
      h=1;
    }
}


  // Hours
  // if Hour=1 -> Light "Eins" (engl.: one)
  if (h == 1){
    leds[50] = CRGB::White;
    leds[51] = CRGB::White;
    leds[52] = CRGB::White;
  }

  if (h == 2){
    leds[48] = CRGB::White;
    leds[49] = CRGB::White;
    leds[50] = CRGB::White;
    leds[51] = CRGB::White;
  }

  if (h == 3){
    leds[46] = CRGB::White;
    leds[45] = CRGB::White;
    leds[44] = CRGB::White;
    leds[43] = CRGB::White;
  }

  if (h == 4){
    leds[31] = CRGB::White;
    leds[32] = CRGB::White;
    leds[33] = CRGB::White;
    leds[34] = CRGB::White;
  }

  if (h == 5){
    leds[40] = CRGB::White;
    leds[39] = CRGB::White;
    leds[38] = CRGB::White;
    leds[37] = CRGB::White;
  }

  if (h == 6){
    leds[1] = CRGB::White;
    leds[2] = CRGB::White;
    leds[3] = CRGB::White;
    leds[4] = CRGB::White;
    leds[5] = CRGB::White;
  }

  if (h == 7){
    leds[53] = CRGB::White;
    leds[54] = CRGB::White;
    leds[55] = CRGB::White;
    leds[56] = CRGB::White;
    leds[57] = CRGB::White;
    leds[58] = CRGB::White;
  }

  if (h == 8){
    leds[22] = CRGB::White;
    leds[21] = CRGB::White;
    leds[20] = CRGB::White;
    leds[19] = CRGB::White;
  }

  if (h == 9){
    leds[27] = CRGB::White;
    leds[28] = CRGB::White;
    leds[29] = CRGB::White;
    leds[30] = CRGB::White;
  }

  if (h == 10){
    leds[18] = CRGB::White;
    leds[17] = CRGB::White;
    leds[16] = CRGB::White;
    leds[15] = CRGB::White;
  }

  if (h == 11){
    leds[24] = CRGB::White;
    leds[25] = CRGB::White;
    leds[26] = CRGB::White;
  }

  if (h == 12){
    leds[66] = CRGB::White;
    leds[65] = CRGB::White;
    leds[64] = CRGB::White;
    leds[63] = CRGB::White;
    leds[62] = CRGB::White;
  }

  FastLED.show(); //Display changes

  // Clear Config for next iteration
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
}
