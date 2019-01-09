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
     * 
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

};


#endif
