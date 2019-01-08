/*
   srb_comms.cpp
   Communication functions for SRB
   Written by Jarod Lam
*/

#include <Arduino.h>
#include "srb_comms.h"
#include "nmea.h"

SrbComms::SrbComms(Stream *port, SrbStats *stats) {
  _serial = port;
  _stats = stats;
  clearBuffer();
}

void SrbComms::setTimeout(int ms) {
  _failsafeTimeout = ms;
}

void SrbComms::update() {

  // Check for failsafe timeout
  if (_failsafeTimeout > 0 &&
      _lastRecvMillis + _failsafeTimeout < millis()) {
    _stats->state = 0;
  }
  
  while (_serial->available()) {
    
    // Update last received time
    _lastRecvMillis = millis();
    
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

    // Filter out non printable characters
    else if (!isPrintable(c)) {
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
  if (millis() - _bufferClearTime > COMMS_BUFFER_TIMEOUT) {
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
    return;
  }

  // 1. Sentence type
  const char *type = nmea.nextField();

  if (strcmp(type, "SRBJS") == 0) {
    readSRBJS(&nmea);
  }
  else if (strcmp(type, "SRBWP") == 0) {
    readSRBWP(&nmea);
  }
  else {
    Serial.print("Sentence type '");
    Serial.print(type);
    Serial.println("' not recognised.");
    return;
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

  // 3. State
  nmea.appendFloat(_stats->state, 0);
  
  // 4. Latitude
  nmea.appendFloat(_stats->lat, 5);
  
  // 5. Longitude
  nmea.appendFloat(_stats->lon, 5);
  
  // 6. Speed
  nmea.appendFloat(_stats->speed, 1);

  // 7. Heading
  nmea.appendFloat(_stats->heading, 1);

  // 8. Battery voltage
  nmea.appendFloat(_stats->battV, 2);

  // 9. Forward power
  nmea.appendInt(_stats->forwardPower);

  // 10. Target latitude
  nmea.appendFloat(_stats->targetLat, 5);
  
  // 11. Target longitude
  nmea.appendFloat(_stats->targetLon, 5);
  
  // 12. Target heading
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

  // Set state
  _stats->state = 1;
}

void SrbComms::readSRBWP(Nmea *nmea) {
  
  // Check number of fields
  if (nmea->numFields() != 5) return;
  
  // 2. SRB ID, abort if doesn't match
  int recvID = strtod(nmea->nextField(), NULL);
  if (recvID != _stats->ID) return;

  // 3. Target latitude
  _stats->targetLat = strtod(nmea->nextField(), NULL);

  // 4. Target longitude
  _stats->targetLon = strtod(nmea->nextField(), NULL);
  
  // 5. Target heading
  _stats->targetHeading = strtod(nmea->nextField(), NULL);

  // 6. Power
  _stats->forwardPower = strtod(nmea->nextField(), NULL);
  
  // Set state
  _stats->state = 2;
}

void SrbComms::clearBuffer() {
  memset(_buffer, 0, sizeof(*_buffer));
  _bufferClearTime = millis();
}

