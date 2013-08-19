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
 * Sketch para ser usado no RoboF, com dispositivos basicos
 * e mais serial para debug da comunicaçao. (sem radio)
 */

class RoboF : public RadioRobot {
public:
  RoboF() : serial(Serial,9600),
            hbridge(5,6,9,10),
            compass()
            {
    addConnection(serial);
    addDevice(hbridge);
    addDevice(compass);
  }

private:
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
