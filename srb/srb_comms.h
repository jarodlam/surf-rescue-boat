/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_comms_h
#define srb_comms_h

#define COMMS_BUFFER_SIZE 1024

#include <Arduino.h>
#include "srb.h"
#include "Nmea.h"

class SrbComms {
  public:

    /*
     * Initialise NmeaGps with the XBee serial port.
     */
    SrbComms(Stream *port, SrbStats *stats);

    /*
     * Update the values from serial.
     */
    void update();

    /*
     * Send a message over serial
     */
    void sendMessage(const char* s);

    /*
     * General function for parsing SRB sentences.
     */
    void parseSentence(char *s);

    /*
     * Send SRBSM message
     */
    void sendSRBSM();
    
  private:
    /*
     * Pointer to the XBee serial port object.
     */
    Stream *_serial;

    /*
     * Pointer to the stats object given at creation.
     */
    SrbStats *_stats;
    
    /*
     * Buffers for storing I/O data.
     */
    char _buffer[COMMS_BUFFER_SIZE];
    unsigned long _bufferClearTime;
    void clearBuffer();

    /*
     * Functions for parsing sentences.
     */
    void readSRBJS(Nmea *nmea);
};

#endif
