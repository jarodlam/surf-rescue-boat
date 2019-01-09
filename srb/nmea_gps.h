/*
   nmea_gps.h
   Receive and parse GPS NMEA strings
   Written by Jarod Lam
*/

#ifndef nmea_gps_h
#define nmea_gps_h

#include <Arduino.h>
#include "srb.h"

#define GPS_BUFFER_SIZE 256
#define GPS_TIMEOUT 1000

class NmeaGps {
  public:

    /*
     * Initialise NmeaGps with the GPS serial port.
     */
    NmeaGps(SrbStats *stats, HardwareSerial *port);

    /*
     * Update the values from serial.
     */
    void update();

    /*
     * Getters
     */
    float lat();
    float lon();
    float speed();
    int status();
    
  private:
    /*
     * Pointer to the GPS serial port object.
     */
    HardwareSerial *_serial;

    /*
     * Pointer to the stats object given at creation.
     */
    SrbStats *_stats;

    /*
     * Buffer for storing incoming data.
     */
    char _buffer[GPS_BUFFER_SIZE];
    void _clear_buffer();
    
    /*
     * GPS location in decimal degrees.
     * 
     * lat: +N, -S
     * lon: +E, -W
     */
    float _lat;
    float _lon;

    /*
     * Status, 1 if active, 0 if void.
     */
    int _status;

    /*
     * Speed in m/s
     */
    float _speed;
    
    /*
     * Parse the sentence in the buffer.
     */
    void _parse_buffer();

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
