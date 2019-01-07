/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_comms_h
#define srb_comms_h

#define COMMS_BUFFER_SIZE 1024
#define COMMS_BUFFER_TIMEOUT 1000

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
     * Set the failsafe timeout in milliseconds.
     * Run during setup.
     */
    void setTimeout(int ms);
    
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
     * Failsafe timeout length. -1 for disabled.
     */
    int _failsafeTimeout = -1;
    unsigned long _lastRecvMillis = 0;
    
    /*
     * Functions for parsing sentences.
     */
    void readSRBJS(Nmea *nmea);
    void readSRBWP(Nmea *nmea);
};

#endif
