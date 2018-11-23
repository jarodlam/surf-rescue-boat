#!/usr/bin/env python3

import serial, sys

# Open the port
try:
    ser = serial.Serial(sys.argv[1], 9600, timeout=1)
except:
    raise Exception('Please run the program with a valid serial port.')

print("Serial port opened at %s" % ser.name)

# Decode NMEA strings

while True:
    ser.write(b"hello")
    line = ser.readline()
    print("%s" % line)

ser.close()