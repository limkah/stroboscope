#include <TimerOne.h>
#include <LiquidCrystal.h>

//Initialize libraries.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Pin number definitions
int pot = A0; //Input for adjustment potentiometer.
int led = 9; //Output pin for LED on/off (gate of 2N7000 for P-chan MOSFET drive).
int up_sw = 7; //Input for RPM step increase switch.
int dw_sw = 6; //Input for RPM step decrease switch.

//Program variable definitions/initializatons
long rpm_to_us = 60 * 1000000; //60Hz/RPM * 1000000us/s, conversion factor to clean up code.
int rpm = 0; //Initialize RPM variable.
int rpm_step = 200;//Set step value for up/down switches.
int rpm_offset = 1000; //Set initial offset of RPM value.
int pos_rpm_cap = 4000; //Cap for RPM values, may remove this later.
int duty = 256; //Set 25% duty cycle by default.
long per = 0; //Initialize period variable.
int lp_counter = 0;
int lp_sp = 35;

void setup() {
  pinMode(up_sw, INPUT);
  pinMode(dw_sw, INPUT);
  pinMode(led, OUTPUT);
  pinMode(pot, INPUT);
  lcd.begin(16, 2); //Initialize 2 row, 16 character LCD display.
  Timer1.initialize(10000);//Initialize timer with a random frequency by default, because why not.
}

void loop() {
  //Read switches and adjust offset accordingly. Only allowing 1 update per loop() cycle.
  lp_counter++;

  if (lp_counter == lp_sp) {
    if (digitalRead(up_sw) == HIGH) {
      rpm_offset = rpm_offset + rpm_step;
      if (rpm_offset > pos_rpm_cap) {
        rpm_offset = pos_rpm_cap; //Cap RPM adjustment values.
      }
    } else if (digitalRead(dw_sw) == HIGH) {
      rpm_offset = rpm_offset - rpm_step;
      //Shouldn't need any checks because the offset will just stop at 0; int type variable.
    }
    lp_counter = 0;
  }

  rpm = (analogRead(pot)) + rpm_offset; //Read potentiometer and update RPM

  //Ensure RPM always remains positive.
  if (rpm < 1) {
    rpm = 1;
  }

  per = (rpm_to_us / rpm); //Convert RPM to period in us.
  Timer1.setPeriod(per);//Update timer period.
  Timer1.pwm(led, duty); //Update PWM on output pin.

  //Write RPM, period, and duty cycle values to display.
  lcd.setCursor(0, 0);
  lcd.print("RPM:");
  lcd.print(rpm);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(per / 1000); //period in ms
  lcd.print("ms ");
  lcd.print("D:");
  lcd.print((int)((duty * 100) / 1024));
  lcd.print("%");
  lcd.print("     ");

}
