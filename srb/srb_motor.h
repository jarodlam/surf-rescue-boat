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

// Motor acceleration in power percentage points per second
#define MOTOR_ACCEL 100

class SrbMotor {

  public:

    /*
     * Initialise the servo objects, call in setup()
     */
    void begin(int pins[], int sides[]);

    /*
     * Update motors every tick
     */
    void update();
    
    /*
     * Set power for specified motor
     */
    void setPower(int motorNo, float power);

    /*
     * Set power for specified side of motors
     */
    void setSidePower(int side, float power);

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

    /*
     * Motor target powers
     */
    float _currentPowers[NUM_MOTORS];
    int _targetPowers[NUM_MOTORS];

    /*
     * Milliseconds since last update
     */
    unsigned long _lastMillis;

    /*
     * Directly set speed of motor
     */
    void _setMotor(int motorNo, float power);
    
};

#endif
