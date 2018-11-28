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

};

#endif
