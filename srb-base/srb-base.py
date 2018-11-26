#!/usr/bin/env python3

import serial, sys, threading
from nmea import *

# Open the port
try:
    ser = serial.Serial(sys.argv[1], 9600, timeout=1)
except:
    raise Exception('Please run the program with a valid serial port.')

print("Serial port opened at %s" % ser.name)
   
# Serial input 
def readSerial():
    while True:
        ser.write(b"hello")
        line = ser.readline()
        if len(line) > 0:
            parseSentence(line)

def parseSentence(sent):
    # Create a Nmea object and validate
    s = Nmea()
    s.sentence = sent.decode().rstrip('\r\n')
    if s.validate():
        print("Valid sentence: %s" % s.sentence)
    else:
        print("Invalid sentence: %s" % s.sentence)

# Thread
serialThread = threading.Thread(target=readSerial)
serialThread.daemon = True
serialThread.start()

while True:
    pass

ser.close()