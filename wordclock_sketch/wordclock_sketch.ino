#include <Time.h>
#include <TimeLib.h>
#include <FastLED.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC


#define DATA_PIN    3
#define NUM_LEDS    40
#define LED_TYPE    WS2812B
#define COLOR_ORDER BRG

#define BRIGHTNESS         240 // Value range: 0-255
// #define FRAMES_PER_SECOND  120

// Define the array of leds
CRGB leds[NUM_LEDS];

//Array to save configuration until it goes online.
int myleds[NUM_LEDS];


// Time variables
int h = 1;
int m = 1;

time_t myTime;

uint8_t gHue = 0; // rotating "base color" used for raibow

void setup() {
  delay(5000); // 5 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // setTime(12,0,0,15,4,2018); //Initialize current time as Midnight/noon 15.04.2018

  Serial.begin(9600); //Begin Serial for debugging purposes

  setSyncProvider(RTC.get);
  if(timeStatus() != timeSet){
        Serial.println("Unable to sync with the RTC");
  }
    else {
        Serial.println("RTC has set the system time");
  }
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
  

  
  //Set default CRGB color
  CRGB myColor = CRGB::White; 
  
  // for debug: 

  Serial.print(h);
  Serial.print(":");
  Serial.println(m);
 
  

  // "Es ist" (it is) constant
    myleds[39] = 1;   // ES
    myleds[38] = 1;   // IST

  //Minutes
  // minutes 0-5 -> "Uhr" (o' clock)
  if ((m>=0 && m<5)){
    myleds[3] = 1;
  }
  
  // minutes 5-10 or 55-60: Fünf (five)
  if ((m>=5 && m<10) || (m>=55 && m<60) || (m>=25 && m<30) || (m>=35 && m<40)){
    myleds[37] = 1;
    myleds[36] = 1;
  }

  //Zehn (ten)
  if ((m>=10 && m<15) || (m>=50 && m<55)){
    myleds[32] = 1;
  }

  // Viertel (quarter)
  if ((m>=15 && m<20) || (m>=45 && m<50)){
    myleds[29] = 1;
    myleds[28] = 1;
  }
  
  //Zwanzig (twenty)
  if ((m>=20 && m<25) || (m>=40 && m<45)){
    myleds[34] = 1;
    myleds[35] = 1;
  }  

  //Half 
   if ((m>= 25 && m<40)){
    myleds[23] = 1;
  }

  // -> "Nach" (past)
  if (((m>=5) && (m<25)) || (m>=35 && m<40) ){
    myleds[25] = 1;
  }
  
  // Minutes  40-60 and 25-30 -> "vor" (to)"
  if (m>=40 || (m>=25 && m<30) ){
    myleds[26] = 1;
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
    myleds[17] = 1;
  }

  if (h == 2){
    myleds[16] = 1;
  }

  if (h == 3){
    myleds[15] = 1;
    myleds[14] = 1;
  }

  if (h == 4){
    myleds[11] = 1;
  }

  if (h == 5){
    myleds[12] = 1;
  }

  if (h == 6){
    myleds[0] = 1;
    myleds[1] = 1;
  }

  if (h == 7){
    myleds[18] = 1;
    myleds[19] = 1;
  }

  if (h == 8){
    myleds[7] = 1;
  }

  if (h == 9){
    myleds[9] = 1;
  }

  if (h == 10){
    myleds[5] = 1;
  }

  if (h == 11){
    myleds[8] = 1;
  }

  if (h == 12){
    myleds[21] = 1;
  }

  // set myleds to the LED stripe
  for (int i=0; i < NUM_LEDS; i++){
    if (myleds[i]){
      leds[i] = myColor; 
    }
  }

  FastLED.show(); //Display changes
  delay(500);
  
  // Clear Config for next iteration
  for (int i = 0; i < NUM_LEDS; i++){
    myleds[i] = 0;
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black); 
  
}
