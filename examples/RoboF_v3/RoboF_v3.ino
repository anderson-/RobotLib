// Descomente a linha abaixo para utilizar a biblioteca RF_24
// #define LIBRARY_RF24

#include <SPI.h>
#include <float.h>

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
#include <RadioConnection.h>
#include <Clock.h>
#include <HBridge.h>
#include <Compass.h>
#include <IRProximitySensor.h>
#include <ReflectanceSensorArray.h>
#include <LED.h>
#include <Potentiometer.h>
#include <Buzzer.h>
#include <Button.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"


#define ROBOT_ID  1
#define RADIO_ID  (98+2*ROBOT_ID)


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
const uint8_t reflectance_addr[] = {0,1,2,3,4};


class RoboF : public GenericRobot {

public:
  RoboF() : 
  radio(7,8,ROBOT_ID,RADIO_ID,false),
  hbridge(5,6,9,10),
  irsensor(A3),
  reflectance(A0, reflectance_addr, 200)
  buzzer(1),
  led(0),
  button(A0, 5),
  potentiometer(A0, 6)
  {
    if (EEPROM.read(0) == (byte)ROBOT_ID) {
      // bussola calibrada
      EEPROM_readAnything(0,configuration);
      compass = Compass(configuration.xmin, configuration.xmax,
      configuration.ymin, configuration.ymax);
    } 
    else {
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
    addDevice(buzzer);      //devID = 5
    addDevice(led);         //devID = 6
    addDevice(button);      //devID = 7
    addDevice(potentiometer);//devID = 8
    // para adicionar novo sensor
    // <ADD> <SENSOR_ID> <TAMANHO1BYTE> <PINO>
  }

private:
  RadioConnection radio;

  Buzzer buzzer;
  LED led;
  Button button;
  Potentiometer potentiometer;
  Compass compass;
  HBridge hbridge;
  IRProximitySensor irsensor;
  ReflectanceSensorArray reflectance;

};


RoboF robot;

/**
 * Funcoes internas do Robo F
 */
bool head (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length);
bool turn (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length);
bool calibrateCompass (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length);

/**
 * Funcoes basicas do Arduino
 */
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

