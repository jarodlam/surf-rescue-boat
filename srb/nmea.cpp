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

NMEA::NMEA(void) {
  memset(&_buffer, 0, sizeof(_buffer));
}

char *NMEA::constructSentence(int n, ...) {
  // Declare a va_list macro and initialize it with va_start
  va_list args;
  va_start(args, n);

  // Initialise the buffer
  memset(&_buffer, 0, sizeof(_buffer));

  // Start the sentence with a dollar sign
  memset(&_buffer, '$', 1);
  int buf_ind = 1;

  // Loop over each argument and append to the buffer
  for (int i = 0; i < n; i++) {

    // Get the next argument
    const char *val = va_arg(args, const char *);
    int val_len = strlen(val);

    // Stop parsing if the buffer will overflow
    if (buf_ind + val_len >= NMEA_BUFFER_SIZE - 5) break;

    // Copy the string into the buffer
    strcpy(&_buffer[buf_ind], val);
    buf_ind += strlen(val);

    // Add a comma if this is not the final parameter
    if (i < n-1) {
      strcpy(&_buffer[buf_ind], ",");
      buf_ind++;
    }
  }

  // Append asterisk
  strcpy(&_buffer[buf_ind], "*");
  buf_ind++;

  // Append checksum
  char cs_string[2];
  sprintf(cs_string, "%.2x", generateChecksum(_buffer));
  strcpy(&_buffer[buf_ind], cs_string);
  buf_ind += 2;
  
  return _buffer;
}

unsigned char NMEA::generateChecksum(const char *s) {
  unsigned char checksum = 0;
  
  for (int i = 0; i < strlen(s); i++) {
    char c = s[i];
    
    if (c == '$') continue;  // Skip the character if dollar sign
    if (c == '*') break;     // Stop the checksum if asterisk

    checksum ^= c;  // XOR to checksum
  }
  
  return checksum;
}

int NMEA::validateSentence(const char *s) {
  int s_len = strlen(s);
  
  // Check if it starts with a dollar sign
  if (s[0] != '$') return 0;

  // Check if it ends with an asterisk
  if (s[s_len-3] != '*') return 0;

  // Check the checksum
  char cs_recv = strtol(&s[s_len-2], NULL, 16);
  char cs_calc = generateChecksum(s);
  if (cs_recv != cs_calc) return 0;

  return 1;
}

