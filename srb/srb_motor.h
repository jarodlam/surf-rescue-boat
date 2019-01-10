/*
   srb_motor.h
   Motor functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_motor_h
#define srb_motor_h

#include "srb.h"
#include <Servo.h>

// Pulse width min and max for motor signals
#define SRB_SERVO_MIN 1000
#define SRB_SERVO_MAX 2000

class SrbMotor {

  public:

    /*
     * Initialise the servo objects, call in setup()
     */
    void begin(int pins[], int sides[]);

   /*
    * Set power for specified motor
    */
   void setPower(int motorNo, int power);

   /*
    * Set power for specified side of motors
    */
   void setSidePower(int side, int power);

   /*
    * Set power for all left/right motors
    */
   void setLeft(int power);
   void setRight(int power);

   /*
    * Stop all motors
    */
   void stopAll();

   /*
    * Return number of motors
    */
   int count();

   /*
    * Return the side of the SRB the motor is on (0=left, 1=right)
    */
   int side(int motorNo);

  private:

    /*
     * Servos
     */
    Servo _motors[NUM_MOTORS];

    /*
     * Motor sides
     */
    int _motorSides[NUM_MOTORS];

};

#endif
