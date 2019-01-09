/*
   srb_motor.cpp
   Motor functions for SRB
   Written by Jarod Lam
*/

#include "srb_motor.h"
#include <Arduino.h>
#include <Servo.h>
#include <stdlib.h>

void SrbMotor::begin(int pins[]) {
  
  // Attach motors to their pins
  for (int i = 0; i < NUM_MOTORS; i++) {
    _motors[i].attach(pins[i], 1000, 2000);
    setPower(i, 0);
  }
  
}

void SrbMotor::setPower(int motorNo, int power) {

  // Abort if motor number not in range
  if (motorNo < 0 || motorNo >= NUM_MOTORS) return;

  // Map value from power domain to angle domain
  int val = constrain(power, -100, 100);
  val = map(val, -100, 100, 0, 180);

  _motors[motorNo].write(power);
  
}

void SrbMotor::stopAll() {

  for (int i = 0; i < NUM_MOTORS; i++) {
    setPower(i, 0);
  }
  
}


