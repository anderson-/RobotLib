
#ifndef ROBOF_H
#define ROBOF_H


#include <SPI.h>
#include <Wire.h>
#include <RF24_config.h>
#include <HMC5883L.h>
#include "Robot.h"
#include "SerialConnection.h"
#include "HBridge.h"
#include "Compass.h"
#include "IRProximitySensor.h"
#include "ReflectanceSensorArray.h"
#include "GenericRobot.h"


#define PIN_MTR_L1          5
#define PIN_MTR_L2          6
#define PIN_MTR_R1          9
#define PIN_MTR_R2          10

#define PIN_RADIO_CE        7
#define PIN_RADIO_CSN       8
#define RADIO_MASTER        true
#define RADIO_SLAVE         false

#define PIN_IRP           A3

#define PIN_REFLET_IN       A0
#define PINS_REFLET_SEL     {16, 3, 4}
#define REFLET_THLD         400


/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de
 * novos dispositivos *dinamicamente*
 */

class RoboF : public GenericRobot {
public:
  RoboF();

private:
   RadioConnection radio;
   Timer t;
   HBridge hbridge;
   Compass compass;
   IRProximitySensor irpsensor;
   ReflectanceSensorArray reflectance;
};

#endif  /* ROBOF_H */

