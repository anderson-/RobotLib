
#include <SPI.h>
#include <RF24_config.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <Robot.h>
#include <SerialConnection.h>
#include <printf.h>

/**
 * Sketch para comunicaçao PC-Robo via radio.
 * 
 * Tudo que eh recebido pela porta serial eh por radio.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(9,10,false), // Arduino: 9,10
                                // MEGA2560: 9,53
                                // ROBOF: 7,8
              serial(Serial,57600) {
    addConnection(radio);
    addConnection(serial);
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    if (&connection == &radio){
      // recebe de radio e envia para serial
      //delay(5);
      serial.sendMessage(data,size);
    } else {
      // recebe de serial e envia para radio
      //serial.sendMessage(data,size);
      if (!radio.sendMessage(data,size)) {
        uint8_t resp[] = {11, data[0]};
        serial.sendMessage(resp, size);
      } else {
        uint8_t resp[] = {3, 'o', 'k'};
        serial.sendMessage(resp, size);
      }
    }
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }

  void printDetails() {
    radio.printDetails();
  }
private:
  //Adicionar os dispositivos aqui
  RadioConnection radio;
  SerialConnection serial;
};

MyRobot robot;

void setup(){
  robot.begin();
  //printf_begin();
  //robot.printDetails();
}

void loop(){
  robot.step();
}
