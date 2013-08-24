#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <RadioRobot.h>

/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos
 * e mais serial para debug da comunicaçao.
 */

class RoboF : public RadioRobot {
public:
  RoboF() : radio(7,8,false),
            serial(Serial,9600),
            hbridge(5,6,9,10),
            compass()
            {
    addConnection(radio);  //connID = 0
    addConnection(serial); //connID = 1
    //adicionado por padrao: 
    //addDevice(clock)     //devID = 0
    addDevice(hbridge);    //devID = 1
    addDevice(compass);    //devID = 2
  }
  
private:
  RadioConnection radio;
  SerialConnection serial;
  
  HBridge hbridge;
  Compass compass;
};

bool rotate (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
  static HBridge * hbridge = NULL;
  static Compass * compass = NULL;
  static int angle = 0;
  static uint8_t error = 0;
  if (data != NULL){ //inicializa a funçao
    hbridge = (HBridge *) deviceList[0]; // posiçao 0!
    compass = (Compass *) deviceList[1]; // posiçao 1!
    angle = *(int *)data;
    error = data[3];
  }
  if (hbridge && compass){
    if (compass->getAngle() > angle-error && compass->getAngle() < angle+error){
      hbridge->setMotorState(1,0);
      hbridge->setMotorState(0,0);
      return true; //termina
    } else if (compass->getAngle() > angle){
      hbridge->setMotorState(1,-60);
      hbridge->setMotorState(0,60);
      return false; //repete
    } else if (compass->getAngle() < angle){
      hbridge->setMotorState(1,60);
      hbridge->setMotorState(0,-60);
      return false; //repete
    }
  }
  return true; //termina
}

bool rotate2 (Device ** deviceList, uint8_t deviceListSize, Connection & c, const uint8_t * data, uint8_t length){
  return true;
}

RoboF robot;

void setup(){
  robot.addAction(rotate);
  robot.begin();
}

void loop(){
  robot.step();
}
