//This program lights up a strip of individually addressable LEDs. 
//A switch determines wheather the number of LEDs that light-up is 
//defined by data from a json file or from a force sensor.
//Link to full project and tutorial at www.batjo.eu/cookbook/light-bars

//import libraries
#include "FastLED.h"
#include <ArduinoJson.h>
#include <SPI.h>

// Define number of LEDs. 
// Change the number 100 to match the number of LEDs in your bars
#define NUM_LEDS_PER_STRIP 100

// Define pins connected to pressure sensor, one for each of the bars (in this case 2 bars)
int fsrPin1 = A0;     
int fsrPin2 = A1;
int switchstate1 = 0;
int switchstate2 = 0;

// Create an array with 100 LEDs for each of the bars (in this case 2 bars)
CRGB whiteleds1[NUM_LEDS_PER_STRIP];
CRGB whiteleds2[NUM_LEDS_PER_STRIP];

// Setup code, runs once
void setup() {
  
  Serial.begin(9600);
  delay(2000);

  //For each LED strip, declare model of LEDs and the Arduino board pin they are attached to
  FastLED.addLeds<WS2812B, 5>(whiteleds1, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 6>(whiteleds2, NUM_LEDS_PER_STRIP);
}

// Main code loop to light LEDs
void loop() {
  
   // Define the variables that will store the input from the force sensors,
  // one for each bar (in this case 2 bars)
  int fsrReading1;  
  int fsnumber1;
  int fsrReading2;  
  int fsnumber2;
  
  // Define pin connected to the switches (see physical circuit), one for
  // each bar (in this case 2 bars)
  switchstate1 = digitalRead(2); 
  switchstate2 = digitalRead(3); 

  //Store the input from the force sensor into the variables declared above,
  // one for each bar (in this case 2 bars)
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);
    
  // Map pressure sensor's values according with the quantity of LEDs: max pressure corresponds to the total LEDs
  fsnumber1 = map (fsrReading1, 0, 1000 ,3, 100);
  fsnumber2 = map (fsrReading2, 0, 1000 ,3, 100);
   
  // Save and parse JSON.
  StaticJsonBuffer<200> jsonBuffer;
  // copy-paste your JSON text instead of 
  // "{\"bar1\":20,\"bar2\":30,\"bar3\":85}"

char json[] = 
  "{\"bar1\":85,\"bar2\":30,\"bar3\":10}"; 
  JsonObject& root = jsonBuffer.parseObject(json);
  
  // Test if parsing succeeded
  if (!root.success()) {
    return;
  }
    
  // Fetch data values to light up bars, by navigating JSON
  int bar1 = root["bar1"];
  int bar2 = root["bar2"];
    
  // Print values to check:
  // by navigating to tools > serial monitor you can see the printed output. 
  // You can remove this line before uploading the code to the board 
  Serial.println(bar1);
    
  // Configure LEDs to light up according to the state of the switch,for each bar.
  // (in this case 2 bars) 
  // If switch is off (LOW), show values from JSON
  int ledson1;
  int ledson2;   
  
// Define behaviour of first bar
  if (switchstate1 == LOW) { 
    ledson1 = bar1;
    
    // Tell LEDs to light up according to value of the variable ledson
    FastLED.clear();
    for(int led1 = 0; led1 < ledson1; led1++)
      // Set the color of the LEDs. The LEDs can also be programmed with custom rgb
      // codes, but this might require some testing to get a result with expected colors. 
      // Check documentation if you wish to learn more.
      whiteleds1[led1] = CRGB::Green; 
      FastLED.show();
      // Wait half a second before starting the loop again (dalay takes time in milliseconds as input). 
      delay(500);
  }
  
  // if switch is not LOW, show values from sensor (fsnumber)  
  else {
    ledson1 = fsnumber1;
    // Catch the event that the switch is on but no pressure is applied to the sensor 
    // (threshold: a sensor reading lower than 10). Define LEDs behaviour for this case.
    // ( first LED blinking with a red light).   
    if (ledson1 < 10) { 
      whiteleds1[0] = CRGB::Red;
      FastLED.show();
      // Wait half a second before continuing 
      delay(500);
      whiteleds1[0] = CRGB::Black; // Black indicates light off.
      FastLED.show();
      // Wait half a second before starting the loop again
      delay(500);
    }
      
    else {
      // Tell LEDs to light up according to value of the variable ledson
      FastLED.clear();
      for(int led1 = 0; led1 < ledson1; led1++)
        // Set the color of the LEDs
        whiteleds1[led1] = CRGB::Blue; 
        FastLED.show();
      // Wait 10 seconds before starting loop again. This gives time for the user to look at his result before 
      // Arduino runs the loop again and the LEDs change 
      // (because there pressure applied has changed or the position of the switch haschanged). 
      // You can change this value to a higher or lower one if you deem appropriate,
      delay(10000);
    }
  }

// Define behaviour of second bar
  if (switchstate2 == LOW) { 
    ledson2 = bar2;
    
    // Tell LEDs to light up according to value of the variable ledson
    FastLED.clear();
    for(int led2 = 0; led2 < ledson2; led2++)
      // Set the color of the LEDs. The LEDs can also be programmed with custom rgb
      // codes, but this might require some testing to get a result with expected colors. 
      // Check documentation if you wish to learn more.
      whiteleds2[led2] = CRGB::Green; 
      FastLED.show();
      // Wait half a second before starting the loop again (dalay takes time in milliseconds as input). 
      delay(500);
  }
  
  // if switch is not LOW, show values from sensor (fsnumber)  
  else {
    ledson2 = fsnumber2;
    // Catch the event that the switch is on but no pressure is applied to the sensor 
    // (threshold: a sensor reading lower than 10). Define LEDs behaviour for this case.
    // ( first LED blinking with a red light).   
    if (ledson2 < 10) { 
      whiteleds2[0] = CRGB::Red;
      FastLED.show();
      // Wait half a second before continuing 
      delay(500);
      whiteleds2[0] = CRGB::Black; // Black indicates light off.
      FastLED.show();
      // Wait half a second before starting the loop again
      delay(500);
    }
      
    else {
      // Tell LEDs to light up according to value of the variable ledson
      FastLED.clear();
      for(int led2 = 0; led2 < ledson2; led2++)
        // Set the color of the LEDs
        whiteleds2[led2] = CRGB::Blue; 
        FastLED.show();
      // Wait 10 seconds before starting loop again. This gives time for the user to look at his result before 
      // Arduino runs the loop again and the LEDs change 
      // (because there pressure applied has changed or the position of the switch haschanged). 
      // You can change this value to a higher or lower one if you deem appropriate,
      delay(10000);
    }
  }

// Eventually repeat the previous IF loop for all the bars
  
}