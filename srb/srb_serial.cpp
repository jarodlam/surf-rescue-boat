/*
   srb_serial.h
   General class for receiving data over serial
   Written by Jarod Lam
*/

#include "srb_serial.h"
#include <Arduino.h>
#include "srb.h"

SrbSerial::SrbSerial(SrbStats *stats, HardwareSerial *port, long baud) {
  _serial = port;
  _serial->begin(baud);
  _stats = stats;
  _clearBuffer();
}

void SrbSerial::_updateSerial() {
  
  while (_serial->available()) {
    
    // Read a single character from serial
    char c = _serial->read();
    
    // Clear the buffer if line feed
    if (c == '\n') {
      _clearBuffer();
    }
    
    // Parse the sentence if carriage return
    else if (c == '\r') {
      _parseBuffer();
      _clearBuffer();
    }

    // Filter out non printable characters
    else if (!isPrintable(c)) {
      _clearBuffer();
    }
    
    // Else, add character to the buffer
    else {
      if (strlen(_buffer) >= SERIAL_BUFFER_SIZE) {
        _clearBuffer();
      }
      strncat(_buffer, &c, 1);
    }
  }

  // Timeout and clear the buffer
  if (millis() - _bufferClearTime > SERIAL_BUFFER_TIMEOUT) {
    _clearBuffer();
  }
  
}

void SrbSerial::_clearBuffer() {
  memset(_buffer, 0, sizeof(*_buffer));
  _bufferClearTime = millis();
}
