#include <SPI.h>
#include <RF24_config.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <Robot.h>
#include <SerialConnection.h>

#define ROBOT_ID  10

/**
 * Sketch para comunicaçao PC-Robo via radio.
 * 
 * Tudo que eh recebido pela porta serial eh por radio.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(9,10,ROBOT_ID,true),  // Arduino: 9,10
                                          // MEGA2560: 9,53
                                          // ROBOF: 7,8
              serial(Serial,9600) {
    addConnection(radio);
    addConnection(serial);
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    if (&connection == &radio){
      // recebe de radio e envia para serial
      serial.sendMessage(data,size);
    } else {
      // recebe de serial e envia para radio
      radio.sendMessage(data,size);
    }
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
  //Adicionar os dispositivos aqui
  RadioConnection radio;
  SerialConnection serial;
};

MyRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
