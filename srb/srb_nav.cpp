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

  // Get target heading of goal
  int tHead = atan2(dLat, dLon);

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

float SrbNav::_normaliseFactor(int val1, int val2, int bound) {

  val1 = abs(val1);
  val2 = abs(val2);
  int maxVal = max(val1, val2);

  if (maxVal > bound) {
    return (float)bound / (float)maxVal;
  } else {
    return 1;
  }
  
}

void SrbNav::_moveTo(int power, int tHeading) {

  // Calculate distance from heading
  int dHead = _headingDiff(tHeading, _stats->heading);

  // Set left and right motor power to forward power
  int powerL = power;
  int powerR = power * -1;
  
  // Add heading rotation to each of the motors
  powerL += dHead;
  powerR += dHead;

  // Normalise 
  float normFact = _normaliseFactor(powerL, powerR, 100);
  powerL *= normFact;
  powerR *= normFact;

  // Turn motors
  _motors->setSidePower(LEFT,  powerL);
  _motors->setSidePower(RIGHT, powerR);
  
}

