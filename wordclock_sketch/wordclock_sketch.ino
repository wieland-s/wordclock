#include <Time.h>
#include <TimeLib.h>
#include <FastLED.h>


#define DATA_PIN    3
#define NUM_LEDS    40
#define LED_TYPE    WS2812B
#define COLOR_ORDER BRG

#define BRIGHTNESS         240 // Value range: 0-255
// #define FRAMES_PER_SECOND  120

// Define the array of leds
CRGB leds[NUM_LEDS];

// Time variables
int h;
int m;
int s;

uint8_t gHue = 0; // rotating "base color" used for raibow

void setup() {
  delay(8000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  setTime(12,0,0,15,4,2018); //Initialize current time as Midnight/noon 15.04.2018

  // Serial.begin(9600); //Begin Serial for debugging purposes

  // Startscrren Rainbow 5 sec
  delay(1000);
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  FastLED.show();
  delay(5000);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void loop() {
  /* Get current time */
  h=hourFormat12();    // Returns the hour of current time between 1-12
  m=minute();         // Returns the minute of current time
   
  // for debug: 
/*
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);
  */

  // "Es ist" (it is) constant
    leds[39] = CRGB::White;   // ES
    leds[38] = CRGB::White;   // IST

  //Minutes
  // minutes 0-5 -> "Uhr" (o' clock)
  if ((m>=0 && m<5)){
    leds[3] = CRGB::White;
  }
  
  // minutes 5-10 or 55-60: Fünf (five)
  if ((m>=5 && m<10) || (m>=55 && m<60) || (m>=25 && m<30) || (m>=35 && m<40)){
    leds[37] = CRGB::White;
    leds[36] = CRGB::White;
  }

  //Zehn (ten)
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    leds[32] = CRGB::White;
  }

  // Viertel (quarter)
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    leds[29] = CRGB::White;
    leds[28] = CRGB::White;
  }
  
  //Zwanzig (twenty)
  if ((m>=20 && m<25) || (m>=40 && m<45)){
    leds[34] = CRGB::White;
    leds[35] = CRGB::White;
  }  

  //Half 
   if ((m>= 25 && m<40)){
    leds[23] = CRGB::White;
  }

  // -> "Nach" (past)
  if (((m>=5) && (m<25)) || (m>=35 && m<40) ){
    leds[25] = CRGB::White;
  }
  
  // Minutes  40-60 and 25-30 -> "vor" (to)"
  if (m>=40 || (m>=25 && m<30) ){
    leds[26] = CRGB::White;
  }
  
  // Modify current hour value (because it is TO five, when the fouth hour is)
  if ((m>=25)){
    h++;
    if (h==0){
      h=12; 
    }
    if (h==13){
      h=1;
    }
  }


  // Hours
  // if Hour=1 -> Light "Eins" (engl.: one)
  if (h == 1){
    leds[17] = CRGB::White;
  }

  if (h == 2){
    leds[16] = CRGB::White;
  }

  if (h == 3){
    leds[15] = CRGB::White;
    leds[14] = CRGB::White;
  }

  if (h == 4){
    leds[11] = CRGB::White;
  }

  if (h == 5){
    leds[12] = CRGB::White;
  }

  if (h == 6){
    leds[0] = CRGB::White;
    leds[1] = CRGB::White;
  }

  if (h == 7){
    leds[18] = CRGB::White;
    leds[19] = CRGB::White;
  }

  if (h == 8){
    leds[7] = CRGB::White;
  }

  if (h == 9){
    leds[9] = CRGB::White;
  }

  if (h == 10){
    leds[5] = CRGB::White;
  }

  if (h == 11){
    leds[8] = CRGB::White;
  }

  if (h == 12){
    leds[21] = CRGB::White;
  }

  FastLED.show(); //Display changes
  delay(500);
  
  // Clear Config for next iteration
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
}
