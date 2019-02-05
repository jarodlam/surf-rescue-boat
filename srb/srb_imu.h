/*
   srb_imu.h
   IMU receiver for Sparkfun SEN-10736 using Razor AHRS firmware 
   Written by Jarod Lam
*/

#ifndef srb_imu_h
#define srb_imu_h

#define IMU_BUFFER_SIZE 1024
#define IMU_BUFFER_TIMEOUT 1000

#include <Arduino.h>
#include "srb_serial.h"

class SrbImu : public SrbSerial{

  public:

    /*
     * Initialise SrbImu with the IMU serial port.
     */
    SrbImu(SrbStats *stats, HardwareSerial *port);

    /*
     * Update the values from serial.
     */
    void update();

  private:

    /*
     * Parse the sentence in the buffer.
     */
    void _parseBuffer();
  
};

#endif
