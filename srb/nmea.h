/*
 * nmea.h
 * Library for manipulating NMEA-0183 strings
 * Written by Jarod Lam
 */

#ifndef nmea_h
#define nmea_h

#define NMEA_BUFFER_SIZE 256

class NMEA {
  public:
    NMEA();
    
    char *constructSentence(int n, ...);

    /*
     * Generate the NMEA checksum.
     * 
     * Automatically skips $ and terminates at *.
     */
    unsigned char generateChecksum(const char *s);

    /*
     * Checks if an NMEA sentence is valid.
     * 
     * Needs to:
     *   - Start with a $
     *   - End with * and checksum
     *   - Correct checksum
     */
    bool validateSentence(const char *s);

    
  private:
    /*
     * Character buffer for containing sentences
     */
    char _buffer[NMEA_BUFFER_SIZE];
};

#endif
