#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <RadioRobot.h>
#include <IRProximitySensor.h>

/**
 * 
 * 
 * 
 * 
 */

class RoboF : public RadioRobot {
public:
  RoboF() : serial(Serial,9600),
            distance(A3),
            t(1000)
            {
    addConnection(serial);
    addDevice(distance);
    clock.add(t);
  }
  
  void think(){
    if (t == 0) {
      uint8_t size = sizeof(int);
      int data;
      if (distance.get((uint8_t *)&data, size)) {
        int i;
        uint8_t value[4];
        if (data >= 1000) {
          value[0] = 0x30 + (uint8_t)(data / 1000);
          data = data % 1000;
        } else {
          value[0] = 0x30;
        }
        if (data >= 100) {
          value[1] = 0x30 + (uint8_t)(data / 100);
          data = data % 100;
        } else {
          value[1] = 0x30;
        }
        if (data >= 10) {
          value[2] = 0x30 + (uint8_t)(data / 10);
          data = data % 10;
        } else {
          value[2] = 0x30;
        }
        value[3] = 0x30 + (uint8_t)(data);
        serial.sendMessage((const uint8_t*)"Distance: ", 10);
        serial.sendMessage(value, 4);
      } else {
        serial.sendMessage((const uint8_t*)"Erro!", 5);
      }
      
      t = 1000;
    }
  }

private:
  SerialConnection serial;
  Timer t;
  
  IRProximitySensor distance;
};

RoboF robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
