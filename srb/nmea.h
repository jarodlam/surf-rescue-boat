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

    /*
     * Construct a NMEA sentence.
     * 
     * Accepts a variatic number of char strings,
     * outputs formatted string including checksum.
     */
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
    int validateSentence(const char *s);

    /*
     * Returns the next argument of a sentence.
     * 
     * Removes the argument from the string once read.
     * Returns NULL if end of the string is reached.
     */
    const char *nextField(char *s);

    /*
     * Returns the number of fields in a sentence.
     */
    int numFields(char *s);
     
  private:
    /*
     * Character buffer for containing sentences
     */
    char _buffer[NMEA_BUFFER_SIZE];
};

#endif
