/*
   srb.ino
   Surf rescue boat control system
   Written by Jarod Lam
*/

#include <avr/wdt.h>
#include "srb.h"
#include "srb_comms.h"
#include "nmea.h"
#include "nmea_gps.h"

#define USE_WATCHDOG

#define LOOP_DELAY 100

unsigned long prevMillis = 0;

SrbStats stats;
SrbComms comms(&Serial1, &stats);
NmeaGps gps(&Serial);

void setup() {
  
  // Start serial
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // Start watchdog
  #ifdef USE_WATCHDOG
  wdt_enable(WDTO_2S);
  #endif

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
  
  // Update serial from GPS and XBee
  gps.update();
  comms.update();

  // Non-blocking loop delay
  if (millis() >= prevMillis+LOOP_DELAY) {
    prevMillis = millis();

    // Send status message
    comms.sendSRBSM();

  }

}
