/*
   srb_stats.h
   Surf rescue boat control system
   Written by Jarod Lam
*/

#ifndef srb_h
#define srb_h

// Number of servos
#define NUM_MOTORS 2

// Left and right motor names
#define LEFT 0
#define RIGHT 1

// Class containing current stats of SRB
class SrbStats {
  
  public:

    /*
     * ID of the SRB, set at start of program
     */
    int ID;

    /*
     * State where 0=disabled, 1=manual, 2=auto
     */
    int state = 0;

    /*
     * Boolean GPS fix flag
     */
    int gpsFix = 0;
  
    /*
     * Current latitude and longitude in decimal degrees
     */
    float lat = 0;
    float lon = 0;

    /*
     * Current ground speed in m/s
     */
    float speed = 0;
    
    /*
     * Current compass heading in degrees from north
     */
    float heading = 0;

    /*
     * Current battery voltage
     */
    float battV = 0;

    /*
     * Forward power percentage (-100-100 manual, 0-100 auto)
     */
    int forwardPower = 0;

    /*
     * Target compass heading in degrees from north
     */
    float targetHeading = 0;

    /*
     * Target lat/lon in decimal degrees
     */
    float targetLat = 0;
    float targetLon = 0;

    /*
     * Magnetic variation (declination) in degrees
     */
    float magVar = 0;
};

#endif
