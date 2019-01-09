/*
   srb_motor.h
   Motor functions for SRB
   Written by Jarod Lam
*/

#ifndef srb_motor_h
#define srb_motor_h

#include <Servo.h>

// Number of servos
#define NUM_MOTORS 2

// Pulse width min and max for motor signals
#define SRB_SERVO_MIN 1000
#define SRB_SERVO_MAX 2000

class SrbMotor {

  public:

    /*
     * Initialise the servo objects, call in setup()
     */
    void begin(int pins[]);

   /*
    * Set power for specified motor
    */
   void setPower(int motorNo, int power);

   /*
    * Stop all motors
    */
   void stopAll();

  private:

    /*
     * Servos
     */
    Servo _motors[NUM_MOTORS];

};

#endif
