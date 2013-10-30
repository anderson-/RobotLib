#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <SerialConnection.h>

/**
 * Sketch para teste da comunicaçao via radio entre PC-Robo.
 * 
 * Tudo que eh recebido pelo radio eh enviado de volta.
 */

 #define SERIAL_DEBUG 0

class MyRobot : public Robot {
public:
  MyRobot() : radio(7,8,true),
              serial(Serial,57600) {
    addConnection(radio);
    addConnection(serial);
  }

  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){

    #if SERIAL_DEBUG
      int i;
      long ti, tt;
      Serial.print("received [");
      for (i = 0; i < size; i++) {
       Serial.print(data[i], DEC);
       Serial.print(' ');
      }
      Serial.print("]...");
      delay(5);
      ti = millis();
    #else
      delay(5);
    #endif
    boolean ok = connection.sendMessage(data,size);
    #if SERIAL_DEBUG
      tt = millis() - ti;
      Serial.print("sended ");
      Serial.print(tt, DEC);
      Serial.print("ms...");
      ti = millis();
      if (ok)
        Serial.println("ok.");
      else
        Serial.println("fail.");
    #endif
  }
  
  void deviceReady (Device & d){
    
  }
  
  void think(){
    
  }
private:
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
