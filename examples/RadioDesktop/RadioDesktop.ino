
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
#include <SerialConnection.h>
#include <Robot.h>

#define ROBOT_ID  1
#define RADIO_ID  (108+2*ROBOT_ID)

/**
 * Sketch para comunicaçao PC-Robo via radio.
 * 
 * Tudo que eh recebido pela porta serial eh por radio.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(9,10,ROBOT_ID,RADIO_ID,true),  // Arduino: 9,10
                                          // MEGA2560: 9,53
                                          // ROBOF: 7,8
              serial(Serial,57600) {
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
