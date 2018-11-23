/*
 * srb-base.cpp
 * Send NMEA commands to surface rescue boat through serial
 */

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#include "nmea.h"

#define MAX_LENGTH 1024

int fd_in;
int fd_out;
char buffer[MAX_LENGTH];

/*
 * Opens a serial port given by file name.
 * Set FDs to stdin and stdout if serial_port is "std"
 */
void open_port(const char *serial_port)
{
	if (strcmp(serial_port, "std") == 0) {
		fd_in = 0;
		fd_out = 1;
		return;
	}
	int fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
			perror("Unable to open serial port");
	} else {
		fcntl(fd, F_SETFL, 0);  // Clear all file status flags
	}
	fd_in = fd;
	fd_out = fd;
}

/*
 * Remove \r and \n from char array.
 */
void remove_crlf(char *str) {
    strtok(str, "\n");
}

/*
 * Writes data to the port.
 * Returns number of bytes sent or -1 if error.
 */
int serial_write(const char *input) {
	int n = write(fd_out, input, strlen(input));
	fprintf(stderr, "sent: %s\n", input);
	return n;
}

/*
 * Reads serial input.
 * Stores input in buffer.
*/
int serial_read(char *buf) {
	int n = read(fd_in, buf, MAX_LENGTH);
	return n;
}

/*
 * Sends a NMEA sentence over serial.
 * Appends $ and *checksum to start and end of sentence.
 */
int sentence_write(const char *input) {
	Nmea nmea;
	nmea.write("$");
	char *input_stripped = (char*)input;
	remove_crlf(input_stripped);
	nmea.append(input_stripped);
	nmea.appendChecksum();
	char *sentence = (char*)nmea.read();
	strcat(sentence, "\r\n");
	int n = serial_write(sentence);
	return n;
}

/*
 * Checks for NMEA sentences from serial.
 */
int sentence_read() {
	int n = serial_read(buffer);
	if (n > 0) {
		fprintf(stderr, "received: %s\n", buffer);
	}
	return n;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		perror("Please enter serial port");
		return 1;
	}
	open_port(argv[1]);
	printf("%d, %d\n", fd_in, fd_out);
	
	while (1) {
		printf("hi\n");
		char input[MAX_LENGTH];
		int n = read(0, input, MAX_LENGTH);
		if (n > 0) {
			sentence_write(input);
		}
		sentence_read();
	}
	
	return 0;
}