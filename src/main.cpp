// COMP-10184 – Mohawk College
// Alarm System
// I, Julia Norrish, 000839030 certify that this material is my original work.  No other person's work has been used without due acknowledgement.
// 
// Make the LED blink for 10 seconds when motion is detected. If the button is pressed while the LED is blinking, it is disabled and
// will not do anything when motion is detected until the button is pressed again. If the button is not pressed while the LED is
// blinking, the LED will stay on solid.
// 
// @author Julia Norrish
// @id 000839030

#include <Arduino.h>
// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6

// define all possible alarm states. 
#define ALARM_DISABLED  0 
#define ALARM_ENABLE    1 
#define ALARM_COUNTDOWN 2 
#define ALARM_ACTIVE    3 

// define the delay for the blinking LED 
#define BLINK_DELAY 125
 
// the state the alarm is in
int iAlarmState; 
// the state the button is in
int ibuttonState;
// the state of the button from the last loop
int ioldButtonState;
// whether or not motion is detected from the PIR sensor
bool bPIR;

// *************************************************************
// This function checks the state of the alarm. It switches between states depending on
// if it detects motion and if the button is pressed.
void checkAlarmState() {
  switch(iAlarmState) {
    case 0:
      // turn the LED off
      digitalWrite(LED_BUILTIN, 1);

      // check if the button is pressed
      if (ibuttonState != ioldButtonState){
        ioldButtonState = ibuttonState;
  
        // if the button is pressed, change the alarm state to 1 to wait for motion
        if (ibuttonState == 1){
          iAlarmState = 1;
        }
    }
      break;
    case 1:
      // read the PIR sensor
      bPIR = digitalRead(PIN_PIR);

      // if motion is detected, set the alarm state to 2 to start the countdown
      if (bPIR) {
        iAlarmState = 2;
      }
      break;
    case 2:
      // blink the LED 40 times
      for (int i = 0; i <= 40; i ++){
        // read the button
        ibuttonState = digitalRead(PIN_BUTTON);
        // check if the button has been pressed
        if (ibuttonState != ioldButtonState) {
          // save the old state
          ioldButtonState = ibuttonState;

          // if the button has been pressed, turn the LED off and set the alarm state to 0
          if (ibuttonState == 1){
            digitalWrite(LED_BUILTIN, 1);
            iAlarmState = 0;
            break;
          }
        }
        // turn the LED on and off with a short delay to make it blink
        digitalWrite(LED_BUILTIN, LOW);
        delay(BLINK_DELAY);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(BLINK_DELAY);

        // if the LED has blinked for 10 seconds, set the alarm state to 3
        if (i == 40){
          iAlarmState = 3;
        }
      }                                           
      break;
    case 3:
      // turn the LED on
      digitalWrite(LED_BUILTIN, 0);
      break;
  }
}



// *************************************************************
void setup() {
 // configure the USB serial monitor
 Serial.begin(115200);
 // configure the LED output
 pinMode(LED_BUILTIN, OUTPUT);
 // PIR sensor is an INPUT
 pinMode(PIN_PIR, INPUT);
 // Button is an INPUT
 pinMode(PIN_BUTTON, INPUT_PULLUP);
}
// *************************************************************
void loop() {
  // read the button
  ibuttonState = digitalRead(PIN_BUTTON);

  checkAlarmState();  
}

