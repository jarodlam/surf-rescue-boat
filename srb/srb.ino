/*
   srb.ino
   Surf rescue boat control system
   Written by Jarod Lam
*/

#include "srb.h"
#include "srb_comms.h"
#include "nmea.h"
#include "nmea_gps.h"

SrbStats stats;                   // Stats object
SrbComms comms(&Serial1, &stats); // Comms object
NmeaGps gps(&Serial);             // GPS object

void setup() {
  // Start serial
  Serial.begin(9600);
  Serial1.begin(9600);

  // Initialise stats
  stats.ID = 0;
  stats.lat = 43.4534324;
  stats.lon = 150.3432493;
  stats.speed = 3.4749;
  stats.heading = 174.3;
  stats.battV = 11.434;
  stats.forwardPower = 0;
  stats.targetHeading = 0;
}

void loop() {
  gps.update();
  comms.update();
  
  comms.sendSRBSM();
  
  delay(100);
}
