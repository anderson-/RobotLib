#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <RadioConnectionM.h>
#include <Robot.h>
#include <SerialConnection.h>

/**
 * Sketch para comunicaçao PC-Robo via radio.
 * 
 * Tudo que eh recebido pela porta serial eh por radio.
 */

class MyRobot : public Robot {
public:
  MyRobot() : radio(9,10,true), // Arduino: 9,10
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
      if (!radio.sendMessage(data,size)) {
        //serial.println("FALHA");
      }
    }
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
  //Adicionar os dispositivos aqui
  RadioConnectionM radio;
  SerialConnection serial;
};

MyRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
