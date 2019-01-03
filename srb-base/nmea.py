#!/usr/bin/env python3

#import re

class Nmea:
    
    def __init__(self):
        self.sentence = "$"
    
    # Append string to end of sentence.
    def append(self, s):
        if len(self.sentence) > 0:
            if self.sentence[-1] != '$':
                self.sentence = self.sentence + ','
        
        self.sentence = self.sentence + s
    
    # Generate checksum for the sentence.
    def generateChecksum(self):
        checksum = 0
        
        for c in self.sentence.encode():
            if c == '$'.encode()[0]: continue
            if c == '*'.encode()[0]: break
            checksum ^= c
        
        checksumString = "%02X" % checksum
        return checksumString
        
    # Append checksum to the sentence.
    def appendChecksum(self):
        checksum = self.generateChecksum()
        self.sentence = self.sentence + '*' + checksum
    
    # Check if the sentence is valid.
    def validate(self):
        if self.sentence[0]  != '$': return False
        if self.sentence[-3] != '*': return False
        
        cs_recv = self.sentence[-2:]
        cs_calc = self.generateChecksum()
        if (cs_recv != cs_calc): return False
        
        return True
        
    # Return arguments as a list.
    def listify(self):
        sentenceStripped = self.sentence[self.sentence.find("$")+1:self.sentence.find("*")]
        listified = sentenceStripped.split(',')
        return listified