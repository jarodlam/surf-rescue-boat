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
	/*
	const char *sent = nmea.constructSentence(3, "SRBSM", "124.54", "4.3");
	printf("%s\n", sent);
	printf("%d\n", nmea.validateSentence(sent));
	printf("%d\n", nmea.validateSentence("3$blah*1a"));
	*/
	
	char gpsSent[] = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
	int n = nmea.numFields(gpsSent);
	printf("%d\n", n);
	for (int i = 0; i < n; i++) {
		const char *arg = nmea.nextField(gpsSent);
		printf("%s\n", arg);
	}
	
	return 0;
}