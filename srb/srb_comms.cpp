/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#include "srb_comms.h"
#include "nmea.h"
#include <Arduino.h>

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
  nmea.appendInt(1);
  
  // 3. Latitude
  nmea.appendFloat(stats.lat, 5);
  
  // 4. Longitude
  nmea.appendFloat(stats.lon, 5);
  
  // 5. Speed
  nmea.appendFloat(stats.speed, 1);

  // 6. Heading
  nmea.appendFloat(stats.heading, 1);

  // 7. Battery voltage
  nmea.appendFloat(stats.battV, 2);

  // Checksum
  nmea.appendChecksum();

  // Send the message
  Serial.println(nmea.read());
  sendMessage(nmea.read());
}

void SrbComms::sendMessage(const char* s) {
  _serial->println(s);
}

void SrbComms::clearBuffer(char *buffer) {
  memset(buffer, 0, sizeof(*buffer));
}

