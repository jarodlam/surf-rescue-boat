/*
   srb.ino
   Surf rescue boat control system
   Written by Jarod Lam
*/

#include "nmea.h"
#include "nmea_gps.h"

NmeaGps gps(&Serial);

void setup() {
  Serial.begin(9600);
}

void loop() {
  gps.update();
  Serial.print(gps.lat());
  Serial.print(' ');
  Serial.println(gps.lon());
  delay(100);
}
