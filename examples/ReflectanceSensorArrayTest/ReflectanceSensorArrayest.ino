#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <RadioRobot.h>
#include <ReflectanceSensorArray.h>

/**
 * 
 * 
 * 
 * 
 */

class RoboF : public RadioRobot {
public:
  RoboF() : serial(Serial,9600),
            //reflectance(A0, (uint8_t[]){16, 3, 4}, 200), // sensor c/ mux
            reflectance((uint8_t[]){14, 15, 16, 3, 4}), // sensor s/ mux
            t(1000)
            {
    addConnection(serial);
    addDevice(reflectance);
    clock.add(t);
  }
  
  void think(){
    if (t == 0) {
      uint8_t data[] = {0};
      if (reflectance.get(data, 1)) {
        int i;
        uint8_t value[10];
        for (i = 0; i < 5; i++) {
          value[2*i] =  0x30 + ((data[0] >> i) & 1);
          value[2*i+1] = ' ';
        }
        serial.sendMessage((const uint8_t*)"IR: ", 4);
        serial.sendMessage(value, 10);
      } else {
        serial.sendMessage((const uint8_t*)"Erro!", 5);
      }
      
      t = 1000;
    }
  }

private:
  SerialConnection serial;
  Timer t;
  
  ReflectanceSensorArray reflectance;
};

RoboF robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
