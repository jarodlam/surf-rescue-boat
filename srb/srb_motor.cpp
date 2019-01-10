/*
   srb_motor.cpp
   Motor functions for SRB
   Written by Jarod Lam
*/

#include "srb_motor.h"
#include "srb.h"
#include <Arduino.h>
#include <Servo.h>
#include <stdlib.h>

void SrbMotor::begin(int pins[], int sides[]) {
  
  // Attach motors to their pins
  for (int i = 0; i < NUM_MOTORS; i++) {
    _motors[i].attach(pins[i], SRB_SERVO_MIN, SRB_SERVO_MAX);
    setPower(i, 0);
    _motorSides[i] = sides[i];
  }
  
}

void SrbMotor::update() {

  // Get time since last update 
  unsigned long dt = millis() - _lastMillis;
  _lastMillis = millis();

  // Calculate amount to add to power
  float a = (float)dt / 1000.0 * MOTOR_ACCEL;

  // Check each motor if they need accelerating
  for (int i = 0; i < NUM_MOTORS; i++) {
Serial.print(_motors[i].read());
Serial.print(" ");
    float dp = _targetPowers[i] - _currentPowers[i];
//Serial.print(dp);
//Serial.print(" ");
    
    if (dp == 0) {
      continue;
    }
    else if (abs(dp) < a) {
      _setMotor(i, _targetPowers[i]);
    }
    else {
      int sign = (dp > 0) - (dp < 0);
      float newPower = _currentPowers[i] + (sign*a);
      _setMotor(i, newPower);
    }
    
  }Serial.println();
  
}

void SrbMotor::setPower(int motorNo, float power) {

  // Abort if motor number not in range
  if (motorNo < 0 || motorNo >= NUM_MOTORS) return;

  // Constrain to range
  power = constrain(power, -100, 100);

  // Update stored power value
  _targetPowers[motorNo] = power;
  
}

void SrbMotor::setSidePower(int side, float power) {

  for (int i = 0; i < NUM_MOTORS; i++) {
    if (_motorSides[i] == side) {
      setPower(i, power);
    }
  }
  
}

void SrbMotor::stopAll() {

  for (int i = 0; i < NUM_MOTORS; i++) {
    setPower(i, 0);
  }
  
}

int SrbMotor::count() {

  return NUM_MOTORS;
  
}

int SrbMotor::side(int motorNo) {

  return _motorSides[motorNo];
  
}

void SrbMotor::_setMotor(int motorNo, float power) {
  
  // Abort if motor number not in range
  if (motorNo < 0 || motorNo >= NUM_MOTORS) return;

  // Map value from power domain to angle domain
  int val = map(power, -100, 100, 0, 180);

  // Update stored power value and servo value
  _currentPowers[motorNo] = power;
  _motors[motorNo].write(val);

}
