#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <HBridge.h>
#include <Compass.h>
#include <RadioRobot.h>

#define ROBOT_ID  10


/**
 * Sketch para ser usado no RoboF, com radio, dispositivos basicos
 * e mais serial para debug da comunicaçao.
 */

class RoboF : public RadioRobot {
public:
  RoboF() : radio(7,8,ROBOT_ID,false),
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

RoboF robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
