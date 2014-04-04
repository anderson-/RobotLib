// Descomente a linha abaixo para utilizar a biblioteca RF_24
// #define LIBRARY_RF24

#include <float.h>
#include <SPI.h>

#ifdef LIBRARY_RF24
#include <RF24_config.h>
#else
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#endif

#include <Wire.h>
#include <HMC5883L.h>
#include <GenericRobot.h>
#include <SerialConnection.h>
#include <RadioConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <IRProximitySensor.h>
#include <ReflectanceSensorArray.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define ROBOT_ID  1
#define RADIO_ID  (108+2*ROBOT_ID)


struct config_t {
  byte robotNumber;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
} configuration;
