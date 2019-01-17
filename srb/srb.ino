/*
   srb.ino
   Surf rescue boat control system
   Written by Jarod Lam
*/

#include <avr/wdt.h>
#include "srb.h"
#include "srb_comms.h"
#include "srb_motor.h"
#include "srb_nav.h"
#include "srb_gps.h"

#define USE_WATCHDOG
#define LOOP_DELAY 100

unsigned long prevMillis = 0;
int motorPins[]  = {2, 3};
int motorSides[] = {LEFT, RIGHT};

SrbStats stats;
SrbMotor motors;
SrbComms comms(&stats, &Serial1);
SrbGps gps(&stats, &Serial2);
SrbNav nav(&stats, &motors);

void setup() {
  
  // Start watchdog
  #ifdef USE_WATCHDOG
  wdt_enable(WDTO_2S);
  #endif
  
  // Start USB serial
  Serial.begin(9600);
  
  // Initialise motors
  motors.begin(motorPins, motorSides);

  // Set comms failsafe timeout
  comms.setTimeout(-1);
  
  // Initialise stats
  stats.ID = 0;
  stats.state = 0;
  stats.lat = 43.4534324;
  stats.lon = 150.3432493;
  stats.speed = 3.4749;
  stats.heading = 174.3;
  stats.battV = 11.434;
  stats.forwardPower = 0;
  stats.targetHeading = 0;
  
}

void loop() {
  
  // Reset watchdog timer
  #ifdef USE_WATCHDOG
  wdt_reset();
  #endif
  
  // Call update functions for everything!
  gps.update();      // Check GPS serial
  comms.update();    // Check XBee serial
  nav.update();      // Calculate nav based on new position/target
  motors.update();   // Accelerate motors to speeds set by nav

  // Non-blocking loop delay
  if (millis() >= prevMillis+LOOP_DELAY) {
    prevMillis = millis();

    // Send status message
    comms.sendSRBSM();

  }

}
