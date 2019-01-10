/*
   srb_nav.h
   Navigatoin system for SRB
   Written by Jarod Lam
*/

#ifndef srb_nav_h
#define srb_nav_h

#include "srb.h"
#include "srb_motor.h"

class SrbNav {
  
  public:

    /*
     * Intialise navigation with stats object.
     */
    SrbNav(SrbStats *stats, SrbMotor *motors);

    /*
     * Update function called every loop.
     */
    void update();
    
  private:

    /*
     * Pointer to the stats object given at creation.
     */
    SrbStats *_stats;

    /*
     * Pointer to the motors object given at creation.
     */
    SrbMotor *_motors;

    /*
     * Navigation functions.
     */
    void _navDisabled();
    void _navAuto();
    void _navManual();

    /*
     * Get the difference between target and current heading
     */
    int _headingDiff(int goalDirection, int currentHeading);

    /*
     * Get a multiplier to scale down values within -bound and bound
     */
    float _normaliseFactor(int val1, int val2, int bound);

    /*
     * Send the SRB in a particular direction with a particular power
     */
    void _moveTo(int power, int tHeading);

};


#endif
