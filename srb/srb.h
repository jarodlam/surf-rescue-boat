/*
   srb.h
   Surf rescue boat control system
   Written by Jarod Lam
*/

#ifndef srb_h
#define srb_h

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
    int state;
  
    /*
     * Current latitude and longitude in decimal degrees
     */
    float lat;
    float lon;

    /*
     * Current ground speed in m/s
     */
    float speed;
    
    /*
     * Current compass heading in degrees from north
     */
    float heading;

    /*
     * Current battery voltage
     */
    float battV;

    /*
     * Forward power percentage (-100-100 manual, 0-100 auto)
     */
    int forwardPower;

    /*
     * Target compass heading in degrees from north
     */
    float targetHeading;

    /*
     * Target lat/lon in decimal degrees
     */
    float targetLat;
    float targetLon;
};

#endif
