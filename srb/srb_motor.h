/*
   srb_motor.h
   Motor functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_motor_h
#define srb_motor_h

#include <Servo.h>

class SrbMotor {

  public:

    /*
     * Initialise the servo objects
     */
    SrbMotor();

  private:

    /*
     * Servos
     */
    Servo _motor1;
    Servo _motor2;

}

#endif
