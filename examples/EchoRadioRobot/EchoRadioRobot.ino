#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>

#define ROBOT_ID  10

/**
 * Sketch para teste da comunicaçao via radio entre PC-Robo.
 * 
 * Tudo que eh recebido pelo radio eh enviado de volta.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(7,8,ROBOT_ID,true) {
    addConnection(radio);
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    connection.sendMessage(data,size);
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
  RadioConnection radio;
};

MyRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
