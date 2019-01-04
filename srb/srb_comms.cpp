/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#ifndef nmea_gps_h
#define nmea_gps_h

#include "srb_comms.h"
#include "nmea.h"
#include <Arduino.h>

#define COMMS_TIMEOUT 1000

SrbComms::SrbComms(Stream *port, SrbStats *stats) {
  _serial = port;
  _stats = stats;
  clearBuffer();
}

void SrbComms::update() {
  //Serial.println(Serial1.available());
  while (_serial->available()) {

    // Read a single character from serial
    char c = _serial->read();

    // Clear the buffer if line feed
    if (c == '\n') {
      clearBuffer();
    }
    
    // Parse the sentence if carriage return
    else if (c == '\r') {
      parseSentence(_buffer);
      clearBuffer();
    }
    
    // Else, add character to the buffer
    else {
      if (strlen(_buffer) >= COMMS_BUFFER_SIZE) {
        clearBuffer();
      }
      strncat(_buffer, &c, 1);
    }
  }

  // Timeout and clear the buffer
  if (millis() - _bufferClearTime > COMMS_TIMEOUT) {
    clearBuffer();
  }
}

void SrbComms::sendMessage(const char* s) {
  _serial->println(s);
}

void SrbComms::parseSentence(char *s) {
  // Create a new Nmea object to parse the sentence
  Nmea nmea;
  nmea.write(s);

  // Check for valid sentence
  if (!nmea.validate()) {
    Serial.print("Invalid sentence received: ");
    Serial.println(s);
  }

  // 1. Sentence type
  const char *type = nmea.nextField();

  if (strcmp(type, "SRBJS") == 0) {
    readSRBJS(&nmea);
  }
  else {
    Serial.print("Sentence type '");
    Serial.print(type);
    Serial.println("' not recognised.");
  }
}

void SrbComms::sendSRBSM() {
  // Create NMEA object
  Nmea nmea;
  nmea.begin();

  // 1. Sentence type
  nmea.append("SRBSM");

  // 2. SRB ID
  nmea.appendInt(_stats->ID);
  
  // 3. Latitude
  nmea.appendFloat(_stats->lat, 5);
  
  // 4. Longitude
  nmea.appendFloat(_stats->lon, 5);
  
  // 5. Speed
  nmea.appendFloat(_stats->speed, 1);

  // 6. Heading
  nmea.appendFloat(_stats->heading, 1);

  // 7. Battery voltage
  nmea.appendFloat(_stats->battV, 2);

  // 8. Forward power
  nmea.appendInt(_stats->forwardPower);

  // 9. Target heading
  nmea.appendFloat(_stats->targetHeading, 1);

  // Checksum
  nmea.appendChecksum();

  // Send the message
  sendMessage(nmea.read());
}

void SrbComms::readSRBJS(Nmea *nmea) {
  // Check number of fields
  if (nmea->numFields() != 3) return;
  
  // 2. SRB ID, abort if doesn't match
  int recvID = strtod(nmea->nextField(), NULL);
  if (recvID != _stats->ID) return;

  // 3. Forward power
  _stats->forwardPower = strtod(nmea->nextField(), NULL);

  // 4. Heading
  _stats->targetHeading = strtod(nmea->nextField(), NULL);
}

void SrbComms::clearBuffer() {
  memset(_buffer, 0, sizeof(*_buffer));
  _bufferClearTime = millis();
}

#endif
