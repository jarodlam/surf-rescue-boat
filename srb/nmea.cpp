/*
 * nmea.cpp
 * Library for manipulating NMEA-0183 strings
 * Written by Jarod Lam
 */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

Nmea::Nmea(void) {
  memset(&_buffer, 0, sizeof(_buffer));
  _sentence[0] = '$';
}

const char *Nmea::read() {
  return _sentence;
}

void Nmea::write(const char *s) {
  memset(_sentence, 0, sizeof(_sentence));
  strcpy(_sentence, s);
}

void Nmea::begin() {
  memset(_sentence, 0, sizeof(_sentence));
  _sentence[0] = '$';
}

void Nmea::append(const char *s) {
  int fieldLen = strlen(s);
  int sentLen = strlen(_sentence);

  // Abort if the sentence will become too long
  if ((fieldLen + sentLen) > (NMEA_BUFFER_SIZE + 5)) return;

  // Add a comma if necessary
  if (_sentence[sentLen-1] != '$') {
    strcat(_sentence, ","); 
  }

  // Concatenate
  strcat(_sentence, s);
}

void Nmea::appendChecksum() {
  char cs_string[2];
  sprintf(cs_string, "%.2X", generateChecksum(_sentence));
  strcat(_sentence, "*");
  strcat(_sentence, cs_string);
}

int Nmea::validate() {
  int sentLen = strlen(_sentence);
  
  // Check if it starts with a dollar sign
  if (_sentence[0] != '$') return 0;

  // Check if it ends with an asterisk
  if (_sentence[sentLen-3] != '*') return 0;

  // Check the checksum
  char cs_recv = strtol(&_sentence[sentLen-2], NULL, 16);
  char cs_calc = generateChecksum(_sentence);
  if (cs_recv != cs_calc) return 0;

  return 1;
}

const char *Nmea::nextField() {
  int startInd = 0;
  int endInd = 0;
  int len = strlen(_sentence);

  // Copy the string into the buffer
  if (len > NMEA_BUFFER_SIZE) return 0;
  memset(&_buffer, 0, sizeof(_buffer));
  strcpy(_buffer, _sentence);

  // Check for end of sentence
  if (_sentence[0] == '*') return 0;

  for (int i = 0; i < len; i++) {
    // Check for dollar sign to start the string
    if (_buffer[i] == '$') {
      startInd = i + 1;
    }
    // Check for comma or asterisk to end the string
    if ((_buffer[i] == ',') || (_buffer[i] == '*')) {
      endInd = i;
      break;
    }
  }
  if (startInd > endInd) return 0;
  
  // Truncate the string
  strcpy(_sentence, &_buffer[endInd+1]);
  
  // Return the field
  if (_buffer[0] == ',') {
    return 0;
  } else {
    char returnVal[len];
    memset(returnVal, 0, len);
    strncpy(returnVal, &_buffer[startInd], endInd-startInd);
    strcpy(_buffer, returnVal);
    return _buffer;
  }
}

int Nmea::numFields() {
  int count = 0;
  for (int i = 0; i < strlen(_sentence); i++) {
    if (_sentence[i] == ',') {
      count++;
    }
  }
  return count + 1;
}

unsigned char Nmea::generateChecksum(const char *s) {
  unsigned char checksum = 0;
  
  for (int i = 0; i < strlen(s); i++) {
    char c = s[i];
    
    if (c == '$') continue;  // Skip the character if dollar sign
    if (c == '*') break;     // Stop the checksum if asterisk

    checksum ^= c;  // XOR to checksum
  }
  
  return checksum;
}
