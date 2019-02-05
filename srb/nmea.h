/*
   nmea.h
   Library for manipulating NMEA-0183 strings
   Written by Jarod Lam
*/

#ifndef nmea_h
#define nmea_h

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define NMEA_BUFFER_SIZE 1024

class Nmea {
  public:
    Nmea();

    /*
       Returns the sentence.
    */
    const char *read();

    /*
       Clear and set the sentence.
    */
    void write(const char *s);

    /*
       Initialise the sentence with a dollar sign.
    */
    void begin();

    /*
       Append a field to the sentence.
    */
    void append(const char *s);
    void appendInt(int d);
    void appendFloat(float d, int places);

    /*
       Calculate the checksum and append to the sentence.
    */
    void appendChecksum();

    /*
       Checks if an NMEA sentence is valid.

       Needs to:
         - Start with a $
         - End with * and checksum
         - Correct checksum
    */
    int validate();

    /*
       Returns the next argument of the sentence.

       Removes the argument from the string once read.
       Returns NULL if end of the string is reached.
    */
    const char *nextField();

    /*
       Returns the number of fields in the sentence.
    */
    int numFields();

  private:
    /*
       Character buffer working space
    */
    char _buffer[NMEA_BUFFER_SIZE];

    /*
       Where the sentence is stored
    */
    char _sentence[NMEA_BUFFER_SIZE];

    /*
       Generate the NMEA checksum.

       Automatically skips $ and terminates at *.
    */
    unsigned char generateChecksum(const char *s);

    /*
     * Powers of 10 for float to string conversion
     */
    const long _POW10[11] = {1, 10, 100, 1000, 10000, 100000, 1000000,
                             10000000, 100000000, 1000000000, 10000000000};
};

#endif
