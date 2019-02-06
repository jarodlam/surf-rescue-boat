#!/usr/bin/env python3

import serial, sys, threading
import tkinter as tk
import tkinter.ttk as ttk
from tkinter import BOTH, END, LEFT, RIGHT, DISABLED, NORMAL, N, S, E, W, X, Y
from serial.tools.list_ports import *
from datetime import *
from nmea import *

# Tkinter
class Application(ttk.Frame):
    
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        
        self.master.protocol("WM_DELETE_WINDOW", self.exitHandler)
        
        self.logFileOpen()
        self.setupGUI()
        self.setupThread()
    
    # Set up GUI
    def setupGUI(self):
        self.pack(fill=BOTH, expand=True)
        self.master.title("SRB base")
        
        self.master.bind('<Return>', self.sendSentence)
        
        # Log box
        textScrollFrame = ttk.Frame(self)
        textScrollFrame.pack(fill=BOTH, expand=True)
        textScrollFrame.grid_columnconfigure(0,weight=1)
        textScrollFrame.grid_columnconfigure(0,weight=1)
        textScrollFrame.grid_rowconfigure(0,weight=1)
        
        scrollbar = ttk.Scrollbar(textScrollFrame)
        scrollbar.grid(column=1,row=0,sticky=N+S)
        self.textBox = tk.Text(textScrollFrame, yscrollcommand=scrollbar.set)
        self.textBox.grid(column=0,row=0,sticky=N+S+E+W)
        self.textBox.config(state=DISABLED)
        self.textBox.tag_configure("error", foreground="red")
        scrollbar.config(command=self.textBox.yview)
        
        # Command entry
        self.textEntry = ttk.Entry(self)
        self.textEntry.pack(fill=X)
        
        # Bottom controls
        controls = ttk.Frame(self)
        controls.pack(fill=X,ipady=3)
        
        self.autoscroll = tk.IntVar()
        scrollCheck = ttk.Checkbutton(controls, text="Autoscroll", variable=self.autoscroll)
        scrollCheck.pack(side=LEFT)
        self.autoscroll.set(1)

        serialPorts = [item.device for item in comports()]
        self.selectedPort = tk.StringVar()
        self.connectButton = ttk.Button(controls, text="Connect", command=self.openPort)
        self.connectButton.pack(side=RIGHT)
        portMenu = ttk.OptionMenu(controls, self.selectedPort, *serialPorts)
        portMenu.pack(side=RIGHT,fill=X, expand=True)
    
    # Threads
    def setupThread(self):
        self.ser = serial.Serial()
        self.serialThread = threading.Thread(target=self.readSerial)
        self.serialThread.daemon = True
    
    # Logging to Tkinter widget 
    def log(self, text, error=False):
        # Get current time string
        timeStr = datetime.now().strftime("%H:%M:%S.%f")
        timeStr = timeStr[:-3]  # Truncate microseconds
        
        # Console
        print("[%s] %s" % (timeStr, text))
        
        # Log file
        self.logFile.write(timeStr + "," + text + "\n")
        self.logFile.flush()
        
        # GUI window
        self.textBox.config(state=NORMAL)
        if error:
            self.textBox.insert(END, "[%s] %s\n" % (timeStr, text), "error")
        else:
            self.textBox.insert(END, "[%s] %s\n" % (timeStr, text))
        if self.autoscroll.get():
            self.textBox.yview_moveto(1)
        self.textBox.config(state=DISABLED)
    
    # Open a serial port
    def openPort(self):
        port = self.selectedPort.get()
        try:
            self.ser = serial.Serial(port, 9600, timeout=1)
            self.serialThread.start()
            self.log("Serial port opened at %s" % self.ser.name)
        except Exception as err:
            self.log("Error opening port %s: %s" % (port, str(err)), error=True)
            self.closePort()
    
    # Close a serial port
    def closePort(self):
        self.ser.close()
    
    # Serial input 
    def readSerial(self):
        while True:
            if self.ser.isOpen():
                self.connectButton.config(state=NORMAL)
                line = self.ser.readline()
                if len(line) > 0:
                    self.parseSentence(line)
            else:
                self.connectButton.config(state=DISABLED)
    
    # Sentence parsing
    def parseSentence(self, sent):
        try:
            # Create a Nmea object and validate
            s = Nmea()
            s.sentence = sent.decode().rstrip('\r\n')
            if s.validate():
                self.log("[RECV] %s" % s.sentence)
            else:
                self.log("[RECV] %s" % s.sentence, error=True)
        except Exception as err:
            self.log("Error parsing sentence: %s" % str(err), error=True)
    
    # Send a sentence and add checksum
    def sendSentence(self, *args):
        if not self.ser.isOpen(): return

        # Convert to valid NMEA sentence
        text = self.textEntry.get()
        s = Nmea()
        s.append(text)
        s.appendChecksum()
        s.sentence = s.sentence + "\r\n"
        
        # Send over serial
        try:
            self.ser.write(s.sentence.encode())
            self.textEntry.delete(0, 'end')
            self.log("[SEND] %s" % s.sentence.rstrip('\r\n'))
        except Exception as err:
            self.log("Error sending sentence: " + str(err), error=True)
        
    # Exit handler
    def exitHandler(self):
        self.closePort()
        self.master.destroy()
        self.logFileClose()
        
    # Open log file
    def logFileOpen(self):
        now = datetime.now()
        fileName = now.strftime("srb_%Y-%m-%d_%H-%M-%S.log")
        self.logFile = open(fileName, "w+")
    
    # Close log file
    def logFileClose(self):
        self.logFile.close()
    
# Main loop
if __name__ == "__main__":
    root = tk.Tk()
    app = Application(root)
    root.mainloop()