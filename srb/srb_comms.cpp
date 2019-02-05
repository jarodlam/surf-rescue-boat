/*
   srb_comms.cpp
   Communication functions for SRB
   Written by Jarod Lam
*/

#include <Arduino.h>
#include "srb_comms.h"
#include "nmea.h"

SrbComms::SrbComms(SrbStats *stats, HardwareSerial *port) : SrbSerial(stats, port, 9600) {
  
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

  _updateSerial();
  
}

void SrbComms::sendMessage(const char* s) {
  _serial->println(s);
}

void SrbComms::_parseBuffer() {
  parseSentence(_buffer);
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
    _readSRBJS(&nmea);
  }
  else if (strcmp(type, "SRBWP") == 0) {
    _readSRBWP(&nmea);
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
  nmea.appendFloat(_stats->lat, 6);
  
  // 5. Longitude
  nmea.appendFloat(_stats->lon, 6);
  
  // 6. Speed
  nmea.appendFloat(_stats->speed, 2);

  // 7. Heading
  nmea.appendFloat(_stats->heading, 1);

  // 8. Battery voltage
  nmea.appendFloat(_stats->battV, 2);

  // 9. Forward power
  nmea.appendInt(_stats->forwardPower);

  // 10. Target latitude
  nmea.appendFloat(_stats->targetLat, 6);
  
  // 11. Target longitude
  nmea.appendFloat(_stats->targetLon, 6);
  
  // 12. Target heading
  nmea.appendFloat(_stats->targetHeading, 1);

  // Checksum
  nmea.appendChecksum();

  // Send the message
  sendMessage(nmea.read());
}

void SrbComms::_readSRBJS(Nmea *nmea) {
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

void SrbComms::_readSRBWP(Nmea *nmea) {
  
  // Check number of fields
  if (nmea->numFields() != 3) return;
  
  // 2. SRB ID, abort if doesn't match
  int recvID = strtod(nmea->nextField(), NULL);
  if (recvID != _stats->ID) return;

  // 3. Target latitude
  _stats->targetLat = strtod(nmea->nextField(), NULL);

  // 4. Target longitude
  _stats->targetLon = strtod(nmea->nextField(), NULL);
  
  // Set state
  _stats->state = 2;
}
