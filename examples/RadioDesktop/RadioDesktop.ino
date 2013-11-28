
#define LIBRARY_RF24 0
#define LIBRARY_MIRF 1

#include <SPI.h>

#if LIBRARY_RF24
  #include <RF24_config.h>
  #include <RadioConnection.h>
#elif LIBRARY_MIRF
  #include <Mirf.h>
  #include <nRF24L01.h>
  #include <MirfHardwareSpiDriver.h>
  #include <RadioConnectionM.h>
#endif

#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>
#include <Robot.h>

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
      radio.sendMessage(data,size);
    }
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
  //Adicionar os dispositivos aqui
#if LIBRARY_RF24
  RadioConnection radio;
#elif LIBRARY_MIRF
  RadioConnectionM radio;
#endif
  SerialConnection serial;
};

MyRobot robot;

void setup(){
  robot.begin();
}

void loop(){
  robot.step();
}
