# cold-end
Firmware for ColdEND controller supporting SSD1306 and SH1106 OLED displays

Installation of SSD1306 or SH1106 OLED display:

1) Import https://github.com/adafruit/Adafruit-GFX-Library and either
   https://github.com/adafruit/Adafruit_SSD1306 or https://github.com/wonho-maker/Adafruit_SH1106
   depending on which OLED display you plan to use. So far, only 128x64 px are supported.

2) Uncomment (define) the appropriate OLED display in config.h. Note that only one display
   at one time is allowed so dont' forget to comment the other one.

3) Flash your ColdEND, vent the system, make sure that the mist knob is fully turned clockwise
   for maximum lubrication and turn the spit knob fully counterclockwise to deactivate spit mode.
   Meter your lubrication output over 15 minutes and multiply the value by 4 to get the maximum
   quantity in milliliter per hour.

4) Edit "max_ml_per_hour" in config.h to the correct values and reflash your ColdEND.


**ATTENTION:** Up from v1.3, https://github.com/PaulStoffregen/TimerOne library is required!

**NOTE:** If your stepper speed is too high and you have already set your microsteps to maximum,
decrease "max_flow_rate" which defines the stepper timing. After changing the value, you will
need to repeat steps 3 and 4 to calibrate the displayed quantity.

Value | frequency | rpm
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
    50 | 10000 Hz | 187,500 rpm

**NOTE:** If spit mode at times does not start, it is most likely that your mist switch is bouncing.
In this case, increase "switch_debounce" slightly.

**HISTORY:**

- v1.4: Minor fixes

- v1.3: New motor control based on hardware timer, fast display refresh rate, countdown in spit mode
        and animated pump icon added by Talla83.de

- v1.2: Choice between exponential and linear flow control and simplified calibration process.
        Added screen refresh delay to minimize loop interrupts by screen refreshes.
        Single-digits for coolant flow (exponential flow control < 10 ml/h: decimal digits).

- v1.1: Moved all configurable parameters to config.h and added "min_ml_per_hour" for a more accurate display value.

- v1.0: Initial rewritten firmware, supporting I2C SSD1306 and SH1106 OLED displays with 128x64 px.
