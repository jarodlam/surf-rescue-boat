/*
   srb_motor.cpp
   Motor functions for SRB
   Written by Jarod Lam
*/

#include "srb_motor.h"

void SrbMotor::begin(int pins[], int sides[]) {
  
  // Attach motors to their pins
  for (int i = 0; i < NUM_MOTORS; i++) {
    _motors[i].attach(pins[i], SRB_SERVO_IDLE-430, SRB_SERVO_IDLE+430);
    _motors[i].writeMicroseconds(SRB_SERVO_IDLE);
    _motorSides[i] = sides[i];
  }

  delay(500);

  _motors[0].write(90);
  _motors[1].write(90);

  //_motors[0].writeMicroseconds(SRB_SERVO_IDLE);
  //_motors[1].writeMicroseconds(SRB_SERVO_IDLE);
  
}

void SrbMotor::update() {

  // Debug
  Serial.print("L=");
  Serial.print(_currentPowers[0]);
  Serial.print(" R=");
  Serial.println(_currentPowers[1]);
  
}

void SrbMotor::setPower(int motorNo, float power) {

  // Abort if motor number not in range
  if (motorNo < 0 || motorNo >= NUM_MOTORS) return;

  // Constrain to range
  power = constrain(power, -100, 100);

  // Set motor
  _setMotor(motorNo, power);
  
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
  Serial.println(val);

  // Update stored power value and servo value
  _currentPowers[motorNo] = power;
  _motors[motorNo].write(val);

}
