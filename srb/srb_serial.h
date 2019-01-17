/*
   srb_serial.h
   General class for receiving data over serial
   Written by Jarod Lam
*/

#ifndef srb_serial_h
#define srb_serial_h

#define SERIAL_BUFFER_SIZE 1024
#define SERIAL_BUFFER_TIMEOUT 1000

#include <Arduino.h>
#include "srb.h"

class SrbSerial {

  public:

    /*
     * Initialise SrbImu with the IMU serial port.
     */
    explicit SrbSerial(SrbStats *stats, HardwareSerial *port);

    /*
     * Update the values from serial.
     */
    void update();

  protected:

    /*
     * Pointer to the XBee serial port object.
     */
    HardwareSerial *_serial;

    /*
     * Pointer to the stats object given at creation.
     */
    SrbStats *_stats;

    /*
     * Buffers for storing I/O data.
     */
    char _buffer[SERIAL_BUFFER_SIZE];
    unsigned long _bufferClearTime;
    void _clearBuffer();

    /*
     * Setup SrbSerial with stats and serial objects.
     */
    void _setup(SrbStats *stats, HardwareSerial *port);

    /*
     * Update buffer with new serial info.
     */
    void _updateSerial();

    /*
     * Parse the contents of the buffer after a carriage return.
     */
    virtual void _parseBuffer() {};
  
};

#endif
