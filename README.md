# cold-end
Firmware for ColdEND controller supporting various displays

Installation and configuration:

1) Download and import the following libraries depending on your displays:

   **SSD1306**
   https://github.com/adafruit/Adafruit-GFX-Library and
   https://github.com/adafruit/Adafruit_SSD1306

   **SH1106**
   https://github.com/adafruit/Adafruit-GFX-Library and
   https://github.com/wonho-maker/Adafruit_SH1106

   **HT16K33**
   https://github.com/adafruit/Adafruit_LED_Backpack

2) Uncomment (define) the appropriate display in config.h. Note that only one type of
   display at one time is allowed so dont' forget to comment the other ones.
   For 7 segment LED displays, don't forget to jumper different I2C addresses for both
   displays and enter these values in config.h ("LED1_ADD" and "LED2_ADD").

3) Uncomment (define) "momentary_switch" if you want to use monentary switches.
   Required library: https://github.com/thomasfredericks/Bounce2
   For normal switches leave "momentary_switch" commented.

4) Flash your ColdEND controller, vent the system and try the coolant potentiometer range.
   If your coolant output is too high and you have already set your microsteps to maximum,
   decrease "max_flow_rate" and reflash the ColdEND controller. The table below shows you
   how values and stepper RPM correlate.

5) Make sure that the coolant knob is fully turned clockwise for maximum lubrication and turn
   the spit knob fully counterclockwise to deactivate spit mode. Meter your lubrication output
   over 10 minutes and multiply the value by 6 to get the maximum quantity in milliliter per hour.

6) Edit "max_ml_per_hour" in config.h to the correct value and reflash your ColdEND.


Value | Frequency | RPM
------|-----------|----
500000 |     1 Hz |  0,0185 rpm
250000 |     2 Hz |  0,0370 rpm
100000 |     5 Hz |  0,093 rpm
 50000 |    10 Hz |  0,187 rpm
 25000 |    20 Hz |  0,375 rpm
 12500 |    40 Hz |  0,750 rpm
  6250 |    80 Hz |  1,500 rpm
  5000 |   100 Hz |  1,875 rpm
  3125 |   160 Hz |  3,000 rpm
  1500 |   333 Hz |  6,243 rpm
  1000 |   500 Hz |  9,250 rpm
   500 |  1000 Hz | 18,500 rpm
   250 |  2000 Hz | 37,000 rpm
   125 |  4000 Hz | 74,000 rpm
   100 |  5000 Hz | 93,750 rpm


**ATTENTION:** ColdEND PUMPBoard PRO requires firmware v1.7 or higher! Dont' forget to uncomment the Board.

**ATTENTION:** Up from v1.3, https://github.com/PaulStoffregen/TimerOne library is required!

**NOTE:** If momentary switches are defined, https://github.com/thomasfredericks/Bounce2 library is required.

**NOTE:** If spit mode at times does not start or any of your switches seem to work not properly,
it is most likely that they are bouncing. In this case, increase "switch_debounce" slightly.


**HISTORY:**

- v1.7: Added support for 2x HT16K33 7 segment LED displays. Ready for new ColdEND PUMPBoard PRO

- v1.6: Fixed spit mode can't be interrupted by switch

- v1.5: Added choice between normal switches and momentary switches.
        When coolant is set below 1.1 ml/h, pump will now be stopped and coolant valve closed
        which makes air button obsolet. Fixed bouncing relais when powering on.

- v1.4: Minor fixes

- v1.3: New motor control based on hardware timer, fast display refresh rate, countdown in spit mode
        and animated pump icon added by Talla83.de

- v1.2: Choice between exponential and linear flow control and simplified calibration process.
        Added screen refresh delay to minimize loop interrupts by screen refreshes.
        Single-digits for coolant flow (exponential flow control < 10 ml/h: decimal digits).

- v1.1: Moved all configurable parameters to config.h and added "min_ml_per_hour" for a more accurate display value.

- v1.0: Initial rewritten firmware, supporting I2C SSD1306 and SH1106 OLED displays with 128x64 px.
