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
  nmea.begin();
  Serial.println(nmea.read());
  nmea.append("hello");
  Serial.println(nmea.read());
  nmea.append("how are you");
  Serial.println(nmea.read());
  nmea.appendChecksum();
  Serial.println(nmea.read());
  Serial.println(nmea.validate());
  Serial.println(nmea.numFields());
  const char *arg = nmea.nextField();
  printf("%s\n", arg);
  Serial.println(nmea.read());
  delay(10000);
}
