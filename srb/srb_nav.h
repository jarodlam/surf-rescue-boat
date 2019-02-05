/*
   srb_nav.h
   Navigatoin system for SRB
   Written by Jarod Lam
*/

#ifndef srb_nav_h
#define srb_nav_h

#include <Arduino.h>
#include <math.h>
#include "srb_stats.h"
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
     * Send the SRB in a particular direction with a particular power
     */
    void _moveTo(int power, int tHeading);

    /*
     * Calculate a multiplier used to scale motor power according to heading offset
     */
    float _turnMultiplier(int dHead);

};


#endif
