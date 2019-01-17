/*
   srb_comms.h
   Communication functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_comms_h
#define srb_comms_h

#include <Arduino.h>
#include "srb.h"
#include "Nmea.h"
#include "srb_serial.h"

class SrbComms : public SrbSerial {
  public:

    /*
     * Initialise SrbGps with the XBee serial port.
     */
    SrbComms(SrbStats *stats, HardwareSerial *port);

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
     * Parse contents of buffer.
     */
    void _parseBuffer();
    
    /*
     * Failsafe timeout length. -1 for disabled.
     */
    int _failsafeTimeout = -1;
    unsigned long _lastRecvMillis = 0;
    
    /*
     * Functions for parsing sentences.
     */
    void _readSRBJS(Nmea *nmea);
    void _readSRBWP(Nmea *nmea);
};

#endif
