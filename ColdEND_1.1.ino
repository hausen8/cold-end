/*

  ColdEND v1.1 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de/geloetetes/3/pumpen-steuerung-1.5-bauteile-set

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Based on firmware by:
  Sebastian End / END-CNC
  Daniel Seoane / SeoGeo

  Rewritten by Tilman, 2020-09-08

*/


#include "config.h"


// Mist and spit variables
int mist_flow_rate;                         // Coolant flow rate value
int spit_time;                              // Spit mode time
byte spit_stat = 0;                         // Spit mode state
int mist_pot;                               // Raw mist potentiometer value
int mist_prev;                              // Previous smoothed mist potentiometer value
int mist_smooth;                            // Smoothed mist potentiometer value
int spit_pot;                               // Raw spit potentiometer value
byte air_valve = 0;                         // Air valve state (on/off)
byte mist_valve = 0;                        // Mist valve state (on/off)
unsigned long spit_start;                   // Spit mode start time in millis
unsigned long spit_stop;                    // Spit mode stop time in millis


// Display variables
int mist_pot_val;                           // Mist value in milliliter
int mist_pot_old;                           // Previous mist value
int spit_pot_val;                           // Spit value in seconds
int spit_pot_old;                           // Previous spit value
byte mist_valve_old;                        // Previous mist valve state
byte air_valve_old;                         // Previous air valve state
byte spit_mode;                             // Spit mode state


void setup() {
  // Set output pins
  pinMode(outStep, OUTPUT);
  pinMode(outDir, OUTPUT);
  pinMode(outEna, OUTPUT);
  pinMode(outMistValve, OUTPUT);
  pinMode(outAirValve, OUTPUT);
  pinMode(outSpitLED, OUTPUT);

  // Set input pins
  pinMode(inMist, INPUT_PULLUP);
  pinMode(inFast, INPUT_PULLUP);
  pinMode(inAir, INPUT_PULLUP);

  // Set stepper pins
  digitalWrite(outDir, LOW);                // Change direction with HIGH and LOW
  digitalWrite(outEna, HIGH);               // Disable stepper

  // Initialize I2C OLED
  #ifdef SSD1306
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  #endif
  
  #ifdef SH1106
    Serial.begin(9600);
    display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  #endif
}


void loop() {
  readSpitPot();                            // Read values from spit pot
  readMistPot();                            // Read values from mist pot
  
  if (spit_time > spit_min_time) {          // Activate spit LED when spit time > 1s
    digitalWrite(outSpitLED, HIGH);
  }
  else {
    digitalWrite(outSpitLED, LOW);
  }
  
  if (digitalRead(inAir) == LOW) {          // Manually switching air valve
    openAirValve();
  }
  else {
    closeAirValve();
  }

  if (digitalRead(inFast) == LOW) {         // Switch to fast mode (overrides normal operation)
    moveStepper(fast_flow_rate);
  }
  else if (digitalRead(inMist) == LOW) {    // Open air valve, check for spit mode and switch to normal operation
    openAirValve();
    spitMode();
    moveStepper(mist_flow_rate);
  }
  else {                                    // Stops normal operation (including spit mode)
    stopStepper();
    spit_stat = 0;
  }

  #ifdef oled
    prepareDisplay();                       // Refresh display if necessary
  #endif
}


void spitMode() {
  // Spit mode, check for conditions and run for a predefined time
  if (spit_stat == 0 && spit_time > spit_min_time) {
    #ifdef oled
      air_valve = 1;
      spit_mode = 1;
      prepareDisplay();
    #endif
    
    delay(switch_debounce);
    spit_start = millis();
    spit_stop = spit_start + spit_time;
    while ((digitalRead(inMist) == LOW) && (millis() < spit_stop)) {
      moveStepper(spit_flow_rate);
    }
    spit_mode = 0;
  }
  spit_stat = 1;
}


void moveStepper(int delay) {
  // Open mist valve and run stepper at desired speed
  // Speed is defined by delays between step HIGH and step LOW
  digitalWrite(outMistValve, LOW);
  digitalWrite(outEna, LOW);
  digitalWrite(outStep, HIGH);
  delayMicroseconds(delay);
  digitalWrite(outStep, LOW);
  delayMicroseconds(delay);
  mist_valve = 1;
}


void stopStepper() {
  digitalWrite(outEna, HIGH);
  digitalWrite(outMistValve, HIGH);
  mist_valve = 0;
}


void openAirValve() {
  digitalWrite(outAirValve, LOW);
  air_valve = 1;
}


void closeAirValve() {
  digitalWrite(outAirValve, HIGH);
  air_valve = 0;
}


void readMistPot() {
  // Read mist pot, smooth the value and convert it for stepper speed and display output
  mist_prev = mist_smooth;
  mist_pot = analogRead(potMist);
  mist_smooth = smooth_filter*mist_pot + (1-smooth_filter)*mist_prev;
  mist_pot_val = map(mist_smooth, 0, 1023, max_ml_per_hour, min_ml_per_hour)/5*5;
  mist_flow_rate = map(mist_smooth, 0, 1023, mist_max_flow_rate, mist_min_flow_rate);
}


void readSpitPot() {
  // Read spit pot and convert value for spit time and display output
  spit_pot = analogRead(potSpit);
  spit_pot_val = map(spit_pot, 0, 911, 8, 0);
  spit_time = map(spit_pot, 0, 1023, spit_max_time, 0);
}


#ifdef oled
  void prepareDisplay() {
    // Check for new values by comparing them to previous values
    // Refresh OLED content only when any of the displayed values have changed
    if (mist_pot_val != mist_pot_old || spit_pot_val != spit_pot_old || mist_valve != mist_valve_old || air_valve != air_valve_old) {
      refreshDisplay();
      mist_pot_old = mist_pot_val;
      spit_pot_old = spit_pot_val;
      mist_valve_old = mist_valve;
      air_valve_old = air_valve;
    }
  }


  void refreshDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Coolant");
    display.setCursor(101, 0);
    display.print("Spit");
    display.drawLine(0, 12, 128, 12, WHITE);
    display.setFont(&FreeSans18pt7b);
    display.setCursor(0, 42);
    display.print(mist_pot_val);
    display.setFont();
    display.setTextSize(1);
    if (mist_pot_val < 10) {
      display.setCursor(22, 36);
    }
    else if (mist_pot_val < 100) {
     display.setCursor(41, 36);
    }
    else {
      display.setCursor(60, 36);
    }
    display.print("ml/h");
    display.setFont(&FreeSans18pt7b);
    display.setCursor(101, 42);
    display.print(spit_pot_val);
    display.setFont();
    display.setTextSize(1);
    display.setCursor(122, 36);
    display.print("s");
    display.drawLine(0, 48, 128, 48, WHITE);
    display.setTextColor(BLACK);
    if (mist_valve == 1) {
      display.fillRect(0, 54, 63, 10, WHITE);
      display.setCursor(2, 55);
      display.print("Coolant On");
    }
    else if (spit_mode == 1) {
      display.fillRect(0, 54, 57, 10, WHITE);
      display.setCursor(2, 55);
      display.print("Spit Mode");
    }
    if (air_valve == 1) {
      display.fillRect(89, 54, 39, 10, WHITE);
      display.setCursor(91, 55);
      display.print("Air On");
    }
    display.display();
  }
#endif





