/*
   srb.ino
   Surf rescue boat control system
   Written by Jarod Lam
*/

#include <avr/wdt.h>
#include "srb_stats.h"
#include "srb_motor.h"
#include "srb_nav.h"
#include "srb_comms.h"
#include "srb_gps.h"
#include "srb_imu.h"

#define LOOP_DELAY 100
#define USE_WATCHDOG

unsigned long prevMillis = 0;
int motorPins[]  = {2, 3};
int motorSides[] = {LEFT, RIGHT};

SrbStats stats;
SrbMotor motors;
SrbNav   nav(&stats, &motors);
SrbComms comms(&stats, &Serial1);
SrbGps   gps(&stats, &Serial2);
SrbImu   imu(&stats, &Serial3);

void setup() {
  
  // Start watchdog
  #ifdef USE_WATCHDOG
  wdt_enable(WDTO_2S);
  #endif
  
  // Start USB serial
  Serial.begin(9600);
  Serial.println("Initialising");
  
  // Initialise motors
  motors.begin(motorPins, motorSides);

  // Set comms failsafe timeout
  comms.setTimeout(30000);
  
  // Initialise stats
  stats.ID = 0;
  stats.lat = -27.477855;
  stats.lon = 153.029478;

}

void loop() {
  
  // Reset watchdog timer
  #ifdef USE_WATCHDOG
  wdt_reset();
  #endif
  
  // Call update functions for everything!
  gps.update();      // Check GPS serial
  comms.update();    // Check XBee serial
  imu.update();      // Check IMU serial
  nav.update();      // Calculate nav based on new position/target
  motors.update();   // Accelerate motors to speeds set by nav
  
  // Non-blocking loop delay
  if (millis() >= prevMillis+LOOP_DELAY) {
    prevMillis = millis();

    // Send status message
    comms.sendSRBSM();

  }

}
