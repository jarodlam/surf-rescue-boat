/*
   nmea_gps.h
   Receive and parse GPS NMEA strings
   Written by Jarod Lam
*/

#ifndef nmea_gps_h
#define nmea_gps_h

#include <Arduino.h>

#define GPS_BUFFER_SIZE 256

class NmeaGps {
  public:

    /*
     * Initialise NmeaGps with the GPS serial port.
     */
    NmeaGps(Stream *port);

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
    Stream *_serial;

    /*
     * Buffer for storing incoming data.
     */
    char _buffer[GPS_BUFFER_SIZE];
    void clear_buffer();
    
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
    void parse_buffer();

    /*
     * Convert NMEA lat/lon string to decimal degrees.
     * 
     * "degLen" is number of degrees digits (2 for lat, 3 for lon).
     */
    float degToDec(const char *s, int degLen);

    /*
     * Convert knots string to m/s.
     */
    float knotsToMps(const char *s);
};

#endif
