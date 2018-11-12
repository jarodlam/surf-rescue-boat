/*
 * srb.ino
 * Surf rescue boat control system
 * Written by Jarod Lam
 */

#include "nmea.h"

NMEA nmea;

void setup() {
  Serial.begin(9600);
}

void loop() {
  char *sent = nmea.constructSentence(3, "SRBSM", "124.54", "4.3");
  Serial.println(nmea.constructSentence(3, "SRBSM", "124.54", "4.3"));
}
