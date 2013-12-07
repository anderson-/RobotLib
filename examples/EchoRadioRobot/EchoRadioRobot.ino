
// Descomente a linha abaixo para utilizar a biblioteca RF_24
// #define LIBRARY_RF24

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
#include <RadioConnection.h>
#include <Robot.h>

#define ROBOT_ID  1
#define RADIO_ID  120

/**
 * Sketch para teste da comunicaçao via radio entre PC-Robo.
 * 
 * Tudo que eh recebido pelo radio eh enviado de volta.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(7,8,ROBOT_ID,RADIO_ID,false) {
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
