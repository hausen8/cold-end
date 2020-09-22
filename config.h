/*

  ColdEND v1.2 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de/geloetetes/3/pumpen-steuerung-1.5-bauteile-set

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Based on firmware by:
  Sebastian End / END-CNC
  Daniel Seoane / SeoGeo

  Rewritten by Tilman, 2020-09-08
  Rewritten by Talla83.de, 2020-09-21

  Last edited: 2020-09-21

*/


// Pinout
#define potMist A0                          // Mist potentiometer pin
#define potSpit A1                          // Spit potentiometer pin
#define outDir 2                            // Direction pin
#define outStep 3                           // Step pin
#define outEna 4                            // Enable pin
#define inFast 7                            // Fast switch
#define inMist 8                            // Mist switch
#define inAir 9                             // Air switch
#define outAirValve 10                      // Air valve pin
#define outMistValve 11                     // Mist valve pin
#define outSpitLED 12                       // Spit LED pin


// New motor control !!!                    // added Talla83.de
#include <TimerOne.h>
// New motor control !!!                    // added Talla83.de

// Mist and spit

// max_flow_rate (max value = 1900 / min value = 500) 
// value 1900 = 0,40 to 61 rpm (1/16 microstepping)
// value 1000 = 0,48 to 74 rpm (1/16 microstepping)
// value  500 = 0,96 to 148 rpm (1/16 microstepping)

#define max_flow_rate  500UL                  // Maximum coolant flow rate (see table above)
#define fast_flow_rate 80                   // Fast mode flow rate value 80= 115rpm /125=74rpm / 250=37rpm
#define spit_flow_rate 80                   // Spit mode flow rate value 80= 115rpm /125=74rpm / 250=37rpm
#define spit_min_time 1000                  // Spit mode minimum time in milliseconds
#define spit_max_time 8000                  // Spit mode maximum time in milliseconds
#define min_ml_per_hour 1                   // Minimum milliliter per hour
#define max_ml_per_hour 150                 // Maximum milliliter per hour (needs to be metered before)


// Operator control
#define switch_debounce 30                  // Delay time in milliseconds to debounce switch
//#define linear_control                      // Choose between linear and exponential flow control
#define smooth_filter 0.05                  // Exponential filter to smooth potentiometer values
#define screen_delay 100                    // Minimum delay between two screen refreshes in milliseconds


// Display
#define SSD1306                             // Uncomment for OLED with SSD1306 controller
//#define SH1106                              // Uncomment for OLED with SH1106 controller

#define SCREEN_WIDTH 128                    // OLED display width in pixels
#define SCREEN_HEIGHT 64                    // OLED display height in pixels
#define OLED_RESET     4                    // Reset pin # (or -1 if sharing Arduino reset pin)

#ifdef SSD1306
  #define oled
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Fonts/FreeSans18pt7b.h>
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

#ifdef SH1106
  #define oled
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SH1106.h>
  #include <Fonts/FreeSans18pt7b.h>
  Adafruit_SH1106 display(OLED_RESET);
#endif
