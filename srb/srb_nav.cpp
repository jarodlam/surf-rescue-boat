/*
   srb_nav.cpp
   Navigation system for SRB
   Written by Jarod Lam
*/

#include "srb.h"
#include "srb_motor.h"
#include "srb_nav.h"

SrbNav::SrbNav(SrbStats *stats, SrbMotor *motors) {
  _stats = stats;
  _motors = motors;
}

void SrbNav::update() {

  // Check the SRB status and run corresponding function
  switch(_stats->state) {
    case 1: navManual();
    case 2: navAuto();
    default: navDisabled();
  }
    
}

void SrbNav::navDisabled() {
  _motors->stopAll();
}

void SrbNav::navManual() {
  
}

void SrbNav::navAuto() {
  
}



