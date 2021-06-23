/*

  ColdEND v1.8 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de/geloetetes/3/pumpen-steuerung-1.5-bauteile-set

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International
  License

  Based on firmware by:
  Sebastian End / END-CNC
  Daniel Seoane / SeoGeo

  Rewritten by Tilman, 2020-09-08
  New motor control, fast screen refresh rate and pump icon added by Talla83.de,
  2020-09-21

  Last edited by Tilman: 2020-10-20

*/

// ------------------------ CONFIGURATION START
// -------------------------------------------------------------------

// Board
#define PROBOARD // Uncomment for ColdEND PUMPBoard PRO

// Display
// #define SSD1306                             // Uncomment for OLED with
// SSD1306 controller
#define SH1106 // Uncomment for OLED with SH1106 controller
// #define HT16K33                             // Uncomment for 7-Segment LED
// Display with HT16K33 Controller #define LCD16X2 // Uncomment for 16x2 LCD or
// VFD with I2C Controller #define LCD16X4                             //
// Uncomment for 16x4 LCD or VFD with I2C Controller

// Mist and spit
#define max_flow_rate                                                          \
  125UL // Maximum coolant flow rate (see table in README.md)
#define fast_flow_rate                                                         \
  80 // Fast mode flow rate value 80= 115rpm /125=74rpm / 250=37rpm
#define spit_flow_rate                                                         \
  80 // Spit mode flow rate value 80= 115rpm /125=74rpm / 250=37rpm
#define spit_min_time 1000 // Spit mode minimum time in milliseconds
#define spit_max_time 8000 // Spit mode maximum time in milliseconds
#define min_ml_per_hour 1  // Minimum milliliter per hour
#define max_ml_per_hour                                                        \
  250                // Maximum milliliter per hour (needs to be metered before)
#define drain_system // Drain system with reversed fast mode when coolant is set
                     // to 0

// Operator control
// #define linear_control                      // Choose between linear and
// exponential flow control
#define smooth_filter 0.05 // Exponential filter to smooth potentiometer values
#define screen_delay                                                           \
  100 // Minimum delay between two screen refreshes in milliseconds
#define switch_debounce 30 // Delay time in milliseconds to debounce switch
// #define momentary_switch                    // Choose between normal switches
// and momentary switches #define ext_mist_ctrl                       //
// Uncomment to use Air switch for external mist control Attention:
// ext_mist_ctrl requires momentary switches!

// ------------------------ CONFIGURATION END
// ---------------------------------------------------------------------

// Pinout
#ifdef PROBOARD
#define potMist A0     // Mist potentiometer pin
#define potSpit A1     // Spit potentiometer pin
#define outDir 2       // Direction pin
#define outStep 3      // Step pin
#define outEna 4       // Enable pin
#define inFast 7       // Fast switch
#define inMist 6       // Mist switch
#define inAir 5        // Air switch or external mist switch
#define outAirValve 8  // Air valve pin
#define outMistValve 9 // Mist valve pin
#define outSpitLED 12  // Spit LED pin
#else
#define potMist A0      // Mist potentiometer pin
#define potSpit A1      // Spit potentiometer pin
#define outDir 2        // Direction pin
#define outStep 3       // Step pin
#define outEna 4        // Enable pin
#define inFast 7        // Fast switch
#define inMist 8        // Mist switch
#define inAir 9         // Air switch or external mist switch
#define outAirValve 10  // Air valve pin
#define outMistValve 11 // Mist valve pin
#define outSpitLED 12   // Spit LED pin
#endif

#ifdef SSD1306
#define OLED
#define OLED_ADD 0x3C // I2C address of OLED display
#include <Adafruit_GFX.h> // Required library: https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h> // Required library: https://github.com/adafruit/Adafruit_SSD1306
#include <Fonts/FreeSans18pt7b.h>
#include <Wire.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#endif

#ifdef SH1106
#define OLED
#define OLED_ADD 0x3C // I2C address of OLED display
#include <Adafruit_GFX.h> // Required library: https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SH1106.h> // Required library: https://github.com/wonho-maker/Adafruit_SH1106
#include <Fonts/FreeSans18pt7b.h>
#include <Wire.h>
Adafruit_SH1106 display(-1);
#endif

#ifdef HT16K33
#define LED
#define LED1_ADD                                                               \
  0x74 // I2C address of first (mist) LED display (needs to be jumpered!)
#define LED2_ADD                                                               \
  0x70 // I2C address of second (spit) LED display (needs to be jumpered!)
#define BRIGHTNESS 6 // Brightness of LED displays
#include <Adafruit_LEDBackpack.h> // Required library: https://github.com/adafruit/Adafruit_LED_Backpack
#include <Wire.h>
Adafruit_7segment mistDisplay = Adafruit_7segment();
Adafruit_7segment spitDisplay = Adafruit_7segment();
#endif

#if defined LCD16X2 || defined LCD16X4
#define LCD
#define LCD_ADD 0x27           // I2C address of LCD controller.
#include <LiquidCrystal_I2C.h> // Required library: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <Wire.h>
#ifdef LCD16X2
LiquidCrystal_I2C lcd(LCD_ADD, 16, 2);
#else
LiquidCrystal_I2C lcd(LCD_ADD, 16, 4);
#endif
#endif

#ifdef ext_mist_ctrl
#define momentary_switch
#endif

#ifdef momentary_switch
#include <Bounce2.h> // Required library: https://github.com/thomasfredericks/Bounce2
Bounce btnMist = Bounce(inMist, switch_debounce);
Bounce btnAir = Bounce(inAir, switch_debounce);
#endif
