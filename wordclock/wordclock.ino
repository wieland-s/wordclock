#include <Time.h>
#include <TimeLib.h>
#include <FastLED.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC


/** RTC - Arduino
 *  GND -> GND 
 *  Vcc -> 5V (3.3 Volt auch möglich)
 *  SCL -> SCL (nahe Rest Knopf; Alt: A5))
 *  SDA -> SDA (neben SCL; Alt. A4)
 */

#define DATA_PIN    3
#define NUM_LEDS    40
#define LED_TYPE    WS2812B
#define COLOR_ORDER BRG
#define DEFAULT_BRIGHTNESS         240 // Value range: 0-255


// Define the array of leds
// CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;

//Array to save configuration until going online.
int myleds[NUM_LEDS];

// Time variables
int h = 1;
int m = 1;

// Values to controll with bluetooth
String bluetoothVal; 
int brightness = DEFAULT_BRIGHTNESS; 
CRGB myColor = CRGB::White; //Default: White
bool rainbow = false; 
char SEPERATOR[] = ";:,"; // To seperate entered values (used in time and rgb colors)

time_t myTime;

uint8_t gHue = 0; // rotating "base color" used for raibow
static uint8_t hue=0;




// bluetooth connection and handler of received data 
void bluetooth(){
  bluetoothVal = "";
  while(Serial.available()){
    delay(10); //wait 10ms
    char c = Serial.read();
    if (c == '#') {break;} // # is end of instruction, break up while loop
    bluetoothVal = bluetoothVal+c;
  }
  
  if(bluetoothVal.length()>0){
    Serial.println(bluetoothVal);

    // Change static CRGB Color
    if(bluetoothVal.startsWith("c")){
      if(bluetoothVal=="c red"){
        myColor = CRGB::Red; 
      } else 
      if(bluetoothVal=="c white"){
       myColor=CRGB::White;     
      } else
      if(bluetoothVal=="c blue"){
       myColor=CRGB::Blue;     
      } else 
      if(bluetoothVal=="c green"){
       myColor=CRGB::Green;    
      } else
      if(bluetoothVal=="c black"){
       myColor=CRGB::Black;   
      } 
      rainbow = false;
      bluetoothVal.remove(0,1);
      Serial.println("Turn to " + bluetoothVal);
     } else
    
    if(bluetoothVal=="off"){
     myColor=CRGB::Black;  
     rainbow = false;   
     Serial.println("Turn off");
    } else
    
    if(bluetoothVal=="on"){
     myColor=CRGB::White;   
     rainbow = false;
     if ((brightness) + 20 <= 255){
        brightness = brightness + 20;
        FastLED.setBrightness(brightness);
     } 
     Serial.println("Turn on in White");
    } else

    // Set Color with RGB Values
    if(bluetoothVal.startsWith("rgb")){
      rainbow = false;
      bluetoothVal.remove(0,4);
      int rgb [2];
      char btValChar[16];
      bluetoothVal.toCharArray(btValChar, 16);
      char *value;
      value = strtok(btValChar, SEPERATOR); 
      int i = 0; 
      while (value != NULL){
        rgb[i] = atoi(value);
        Serial.println(value);
        value =  strtok(NULL, SEPERATOR);
        i++;
      }
      myColor = CRGB(rgb[0],rgb[1],rgb[2]);
      Serial.println("Color set to CRGB " + bluetoothVal);
    } else

    // Change brightness
    if(bluetoothVal.startsWith("b")){
      bluetoothVal.remove(0,1);
      brightness = bluetoothVal.toInt();
      if(0 <= brightness && brightness <= 255){
        FastLED.setBrightness(brightness);    
        Serial.println("turn brigntness to " + bluetoothVal);
      }
    } else
    if (bluetoothVal=="lower"){
        if ((brightness - 20) >= 0){ 
          brightness = brightness -20;
          Serial.print("turn brigthness to ");
          Serial.print(brightness);
          Serial.println(" (-20)");
        }
       FastLED.setBrightness(brightness);
    } else
    if (bluetoothVal=="higher"){
       if ((brightness) + 20 <= 255){
          brightness = brightness + 20;
          Serial.print("turn brigthness to ");
          Serial.print(brightness);
          Serial.println(" (+20)");
       }
       FastLED.setBrightness(brightness);
    } else

    //set time stamp (hh:mm:ss:dd:mm:yy)
    if(bluetoothVal.startsWith("t")){
      bluetoothVal.remove(0,2);
      time_t t;
      tmElements_t tm;
      char btValChar[18];
      int timestamp[6];
      char *value;
      int i = 0; 
      bluetoothVal.toCharArray(btValChar, 16);
      value = strtok(btValChar, SEPERATOR);
      while (value != NULL){
        timestamp[i] = atoi(value);
        value = strtok(NULL, SEPERATOR);
        i++;
      }
      tm.Year = CalendarYrToTm(timestamp[5]);
      tm.Month = timestamp[4];
      tm.Day = timestamp[3];
      tm.Hour = timestamp[0];
      tm.Minute = timestamp[1];
      tm.Second = timestamp[3];
      t = makeTime(tm);
      RTC.set(t);
      setTime(t);
      Serial.println("Time set to " + bluetoothVal);
    } else
    if(bluetoothVal == "rainbow on"){
      rainbow = true; 
      Serial.println("Turn rainbow on");
    } else
    if (bluetoothVal == "rainbow off"){
      rainbow = false; 
      Serial.println("turn rainbow off");
    } else
    {
      Serial.println("Command not known");
    }
    
  }
  
}


// Start routine
void setup() {
  delay(5000); // 5 second delay for recovery
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  Serial.begin(9600); //Begin Serial for debugging purposes

  //RTC
  setSyncProvider(RTC.get); //  loop code does not require any logic to maintain time sync. The Time library will automatically monitor NTP and sync the time as necessary.
  if(timeStatus() != timeSet){
        Serial.println("Unable to sync with the RTC");
  }
    else {
        Serial.println("RTC has set the system time");
  }
  
  // Startscreen Rainbow 5 sec
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
 
  bluetooth(); // Handles bluetooth commands
  
  // for debug: 
  // Serial.print(h);
  // Serial.print(":");
  // Serial.println(m);

  // trivial checking checking which LED lights for actual time  

  // "Es ist" (it is) constant
    myleds[39] = 1;   // ES
    myleds[38] = 1;   // IST

  // Minutes
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
      if (!rainbow){
         leds[i] = myColor; 
      } else {
        leds(i,i).fill_rainbow(hue++);
      }
    }
    myleds[i] = 0;
  }

  FastLED.show(); //Display changes
  delay(500);
  
  // Clear Config for next iteration
  fill_solid(leds, NUM_LEDS, CRGB::Black); 
}
