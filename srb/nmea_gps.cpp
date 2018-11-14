/*
   nmea_gps.h
   Receive and parse GPS NMEA strings
   Written by Jarod Lam
*/

#include "nmea_gps.h"
#include "nmea.h"
#include <Arduino.h>
#include <Time.h>

NmeaGps::NmeaGps(Stream *port) {
  _serial = port;
  clear_buffer();
}

void NmeaGps::update() {
  while (_serial->available()) {
    char c = _serial->read();

    // Clear the buffer if line feed
    if (c == '\n') {
      clear_buffer();
    }
    // Parse the sentence if carriage return
    else if (c == '\r') {
      parse_buffer();
      clear_buffer();
    }
    // Add character to the buffer
    else {
      if (strlen(_buffer) >= GPS_BUFFER_SIZE) {
        clear_buffer();
      }
      strncat(_buffer, &c, 1);
    }
  }

  // Timeout and clear the buffer
  if (millis() - _bufferClearTime > GPS_TIMEOUT) {
    clear_buffer();
  }
}

void NmeaGps::clear_buffer() {
  memset(&_buffer, 0, sizeof(_buffer));
  _bufferClearTime = millis();
}

void NmeaGps::parse_buffer() {
  // Create a new Nmea object and put the sentence in it
  Nmea nmea;
  nmea.write(_buffer);
  
  // Abort if the sentence is invalid
  if (!nmea.validate()) {
    Serial.println("Invalid sentence");
    return;
  }

  // 1. Type, abort if not GPRMC
  if (strcmp(nmea.nextField(), "GPRMC") != 0) return;

  // 2. Time, discard this
  nmea.nextField();

  // 3. Status, abort if inactive
  if (strcmp(nmea.nextField(), "A") == 0) {
    _status = 1;
  } else {
    _status = 0;
    return;
  }

  // 4-5. Latitude
  _lat = degToDec(nmea.nextField(), 2);
  _lat *= (strcmp(nmea.nextField(), "N") == 0) ? 1 : -1;

  // 6-7. Longitude
  _lon = degToDec(nmea.nextField(), 3);
  _lon *= (strcmp(nmea.nextField(), "E") == 0) ? 1 : -1;

  // 8. Speed (in knots)
  _speed = knotsToMps(nmea.nextField());

  // Discard the other fields:
  // 9. Track angle
  // 10. Date
  // 11. Magnetic variation
}

float NmeaGps::degToDec(const char *s, int degLen) {
  // Convert degrees portion
  char degStr[degLen];
  strncpy(degStr, s, degLen);
  float deg = strtod(degStr, NULL);
  
  // Convert minutes portion
  float mins = strtod(&s[degLen], NULL);
  
  // Add the two
  return deg + (mins/60);
}

float NmeaGps::knotsToMps(const char *s) {
  float knots = strtod(s, NULL);
  return knots / 1.944;
}

float NmeaGps::lat() {return _lat;}
float NmeaGps::lon() {return _lon;}
float NmeaGps::speed() {return _speed;}
int NmeaGps::status() {return _status;}

