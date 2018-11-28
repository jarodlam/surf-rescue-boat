/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#include "srb_comms.h"
#include "nmea.h"
#include <Arduino.h>
#include <Time.h>

SrbComms::SrbComms(Stream *port) {
  _serial = port;
  clearBuffer(_inBuf);
  clearBuffer(_outBuf);
}

void SrbComms::sendSRBSM(SrbStats stats) {
  // Create NMEA object
  Nmea nmea;
  nmea.begin();

  // 1. Sentence type
  nmea.append("SRBSM");

  // 2. SRB ID
  nmea.append(1);
  
  // 3. Latitude
  nmea.append(stats.lat, 5);
  
  // 4. Longitude
  //nmea.appendf("%.5f", stats.lon);
  
  // 5. Speed
  //nmea.appendf("%.1f", stats.speed);

  // 6. Heading
  //nmea.appendf("%.1f", stats.heading);

  // 7. Battery voltage
  //nmea.appendf("%.2f", stats.battV);

  // Checksum
  nmea.appendChecksum();

  // Send the message
  Serial.println(nmea.read());
}

void SrbComms::clearBuffer(char *buffer) {
  memset(buffer, 0, sizeof(*buffer));
}

