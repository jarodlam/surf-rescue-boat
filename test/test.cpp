/*
 * test.cpp
 * Local testing of non-Arduino-specific functionality
 * Written by Jarod Lam
 */

#include <stdio.h>
#include <stdint.h>
#include "nmea.h"

NMEA nmea;

int main() {
	const char *sent = nmea.constructSentence(3, "SRBSM", "124.54", "4.3");
	printf("%s\n", sent);
	printf("%d\n", nmea.validateSentence(sent));
	printf("%d\n", nmea.validateSentence("$blah*1a"));
	return 0;
}