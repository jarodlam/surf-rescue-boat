/*
   nmea_gps.cpp
   Receive and parse GPS NMEA strings
   Written by Jarod Lam
*/

#include "srb_gps.h"
#include "srb.h"
#include "nmea.h"
#include <Arduino.h>
#include <Time.h>

SrbGps::SrbGps(SrbStats *stats, HardwareSerial *port) : SrbSerial::SrbSerial(stats, port) {

  _serial->begin(57600);

  // Configure to send RMC commands only
  _serial->print("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
  // Configure to send every 200ms
  _serial->print("$PMTK220,200*2C\r\n");
  
}

void SrbGps::update() {

  _updateSerial();
  
}

void SrbGps::_parseBuffer() {
  
  // Create a new Nmea object and put the sentence in it
  Nmea nmea;
  nmea.write(_buffer);
  
  // Abort if the sentence is invalid
  if (!nmea.validate()) {
    Serial.println("Invalid GPS sentence");
    return;
  }

  // 1. Type, abort if not GPRMC
  if (strcmp(nmea.nextField(), "GPRMC") != 0) return;

  // 2. Time, discard this
  nmea.nextField();

  // 3. Status, abort if inactive
  if (strcmp(nmea.nextField(), "A") == 0) {
    _stats->gpsFix = 1;
  } else {
    _stats->gpsFix = 0;
    return;
  }

  // 4-5. Latitude
  _stats->lat  = _degToDec(nmea.nextField(), 2);
  _stats->lat *= (strcmp(nmea.nextField(), "N") == 0) ? 1 : -1;

  // 6-7. Longitude
  _stats->lon  = _degToDec(nmea.nextField(), 3);
  _stats->lon *= (strcmp(nmea.nextField(), "E") == 0) ? 1 : -1;

  // 8. Speed (in knots)
  _stats->speed = _knotsToMps(nmea.nextField());

  Serial.print(_stats->lat, 6);
  Serial.print(",");
  Serial.print(_stats->lon, 6);
  Serial.println("");

  // Discard the other fields:
  // 9. Track angle
  // 10. Date
  // 11. Magnetic variation
  
}

float SrbGps::_degToDec(const char *s, int degLen) {
  
  // Convert degrees portion
  char degStr[degLen];
  strncpy(degStr, s, degLen);
  float deg = strtod(degStr, NULL);
  
  // Convert minutes portion
  float mins = strtod(&s[degLen], NULL);

  // Minutes to decmial using integer maths
  long minsInt = mins * 10000;
  minsInt = minsInt * 100 / 60;
  mins = minsInt / 1000000.0;
  
  // Add the two
  return deg + mins;
  
}

float SrbGps::_knotsToMps(const char *s) {
  
  float knots = strtod(s, NULL);
  return knots / 1.944;
  
}

