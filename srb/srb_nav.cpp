/*
   srb_nav.cpp
   Navigation system for SRB
   Written by Jarod Lam
*/

#include "srb.h"
#include "srb_motor.h"
#include "srb_nav.h"
#include <Arduino.h>
#include <math.h>

SrbNav::SrbNav(SrbStats *stats, SrbMotor *motors) {
  _stats = stats;
  _motors = motors;
}

void SrbNav::update() {

  // Check the SRB status and run corresponding function
  switch(_stats->state) {
    case 1: _navManual(); break;
    case 2: _navAuto();   break;
    default: _navDisabled();
  }
    
}

void SrbNav::_navDisabled() {
  
  _motors->stopAll();
  
}

void SrbNav::_navManual() {

  _moveTo(_stats->forwardPower, _stats->targetHeading);
  
}

void SrbNav::_navAuto() {

  // Calculate distance from goal
  float dLat = _stats->targetLat - _stats->lat;
  float dLon = _stats->targetLon - _stats->lon;

  // Get target heading of goal (convert to deg)
  int tHead = atan2(dLat, dLon) * 57.29578;

  // Move
  _moveTo(_stats->forwardPower, tHead);
  
}

int SrbNav::_headingDiff(int goalDirection, int currentHeading) {
  
  int maxHeading = max(goalDirection, currentHeading);
  int minHeading = min(goalDirection, currentHeading);
  
  int clockDiff = maxHeading - minHeading;
  int antiClockDiff = minHeading + 360 - maxHeading;
  int smallest = min(clockDiff, antiClockDiff);
  
  if (((currentHeading + smallest) % 360) == goalDirection) {
    return smallest * -1; //clockwise
  } else {
     return smallest; 
  }
  
}

void SrbNav::_moveTo(int power, int tHeading) {

  // Calculate distance from heading
  int dHead = _headingDiff(tHeading, _stats->heading);

  // Set motor speeds
  int powerL = power;
  int powerR = power;

  // Scale motors by heading rotation
  if (dHead < 0) {
    powerR *= _turnMultiplier(dHead);
  } else {
    powerL *= _turnMultiplier(dHead);
  }

  // Turn motors
  _motors->setSidePower(LEFT,  powerL);
  _motors->setSidePower(RIGHT, -powerR);
  
}

float SrbNav::_turnMultiplier(int dHead) {

  if (dHead < 0) dHead *= -1;
  dHead = constrain(dHead, 0, 180);

  // Scale according to the function y = (1/90)x + 1
  float multiplier = -(1.0 / 90.0) * dHead + 1;

  return multiplier;
  
}

