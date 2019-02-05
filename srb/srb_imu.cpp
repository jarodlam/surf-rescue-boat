/*
   srb_imu.cpp
   IMU receiver for Sparkfun SEN-10736 using Razor AHRS firmware
   Written by Jarod Lam
*/

#include "srb_imu.h"

SrbImu::SrbImu(SrbStats *stats, HardwareSerial *port) : SrbSerial(stats, port, 57600) {
  
}

void SrbImu::update() {

  _updateSerial();
  
}

void SrbImu::_parseBuffer() {

  // Check if first five letters are "#YPR="
  char s[10];
  strncpy(s, _buffer, 5);
  if (strcmp(s, "#YPR=") != 0) {
    /*Serial.print("Invalid message from IMU: ");
    Serial.println(_buffer);*/
    return;
  }

  // Find the first comma position
  int endInd = 0;
  for (int i = 5; i < strlen(_buffer); i++) {
    if (_buffer[i] == ',') {
      endInd = i;
      break;
    }
  }

  // Make sure string length is within reasonable bounds
  int numLen = endInd - 5;
  if (numLen >= 10 || numLen <= 0) {
    Serial.print("Unable to parse number from IMU: ");
    Serial.println(_buffer);
    return;
  }

  // Extract the number from the string
  memset(s, 0, sizeof(s));
  strncpy(s, &_buffer[5], numLen);

  // Decode the number
  float heading = strtod(s, NULL);

  // Wrap around negative values
  if (heading < 0) {
    heading += 360;
  }
  heading = constrain(heading, 0, 360);

  // Add magnetic variation (declination)
  //heading += _stats->magVar;

  // Update stats value
  _stats->heading = heading;
  
}

