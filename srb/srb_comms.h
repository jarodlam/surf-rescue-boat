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

class SrbComms {
  public:

    /*
     * Initialise NmeaGps with the XBee serial port.
     */
    SrbComms(Stream *port);

    /*
     * Update the values from serial.
     */
    void update();

    /*
     * Send a message over serial
     */
    void sendMessage(const char* s);

    /*
     * Send SRBSM message
     */
    void sendSRBSM(SrbStats stats);
    
  private:
    /*
     * Pointer to the XBee serial port object.
     */
    Stream *_serial;

    /*
     * Buffers for storing I/O data.
     */
    char _inBuf[COMMS_BUFFER_SIZE];
    char _outBuf[COMMS_BUFFER_SIZE];
    void clearBuffer(char *buffer);
    
    /*
     * Parse the sentence in the buffer.
     */
    void parse_buffer();
};

#endif
