
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


/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos,
 * sem serial, com funçoes complexas, e suporte a adiçao de 
 * novos dispositivos *dinamicamente*
 */
const uint8_t pin_sel[] = {4, 3, 16};

class RoboF : public GenericRobot {

public:
  RoboF() : 
  radio(7,8,ROBOT_ID,RADIO_ID,false),
  hbridge(5,6,9,10),
  irsensor(17),
  reflectance(A0, pin_sel, 200)
  {
  	if (EEPROM.read(0) == (byte)ROBOT_ID) {
  		// bussola calibrada
			EEPROM_readAnything(0,configuration);
			compass = Compass(configuration.xmin, configuration.xmax,
												configuration.ymin, configuration.ymax);
		} else {
			// sem calibracao
			compass = Compass();
		}

    addConnection(radio);  //connID = 0
    //adicionado por padrao: 
    //addDevice(clock)      //devID = 0
    addDevice(hbridge);     //devID = 1
    addDevice(compass);     //devID = 2
    addDevice(irsensor);    //devID = 3
    addDevice(reflectance); //devID = 4
    // para adicionar novo sensor
    // <ADD> <SENSOR_ID> <TAMANHO1BYTE> <PINO>
  }

private:
  RadioConnection radio;

  Compass compass;
  HBridge hbridge;
  IRProximitySensor irsensor;
  ReflectanceSensorArray reflectance;
};


RoboF robot;


void setup(){
  //adicionando funçoes... 
  robot.addAction(head);  // id = 0
  robot.addAction(turn);  // id = 1
  robot.addAction(calibrateCompass);  // id = 2
  robot.begin();
}

void loop(){
  robot.step();
}
