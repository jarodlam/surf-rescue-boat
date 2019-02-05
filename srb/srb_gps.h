/*
   nmea_gps.h
   Receive and parse GPS NMEA strings
   Written by Jarod Lam
*/

#ifndef nmea_gps_h
#define nmea_gps_h

#include <Arduino.h>
#include <Time.h>
#include "nmea.h"
#include "srb_stats.h"
#include "srb_serial.h"

class SrbGps : public SrbSerial {
  public:

    /*
     * Initialise SrbGps with the GPS serial port.
     */
    SrbGps(SrbStats *stats, HardwareSerial *port);

    /*
     * Update the values from serial.
     */
    void update();
    
  private:
    
    /*
     * Parse the sentence in the buffer.
     */
    void _parseBuffer();

    /*
     * Convert NMEA lat/lon string to decimal degrees.
     * 
     * "degLen" is number of degrees digits (2 for lat, 3 for lon).
     */
    float _degToDec(const char *s, int degLen);

    /*
     * Convert knots string to m/s.
     */
    float _knotsToMps(const char *s);

    /*
     * Milliseconds since the buffer was last cleared.
     */
    unsigned long _bufferClearTime;
};

#endif
