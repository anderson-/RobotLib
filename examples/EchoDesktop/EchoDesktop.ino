#include <Wire.h>
#include <HMC5883L.h>
#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <RadioRobot.h>
#include <LED.h>

RadioConnection rf(9,10,false);

uint8_t data[] = {2,8,'a','n','d','e','r','s','o','n'};

void setup(){
  Serial.begin(9600);
  Serial.println("Inicializando..");
  rf.begin();
}

void loop(){
  Serial.println("Enviando..");
  rf.sendMessage(data,10);
  if (rf.available()){
    Serial.println("Recebendo..");
    uint8_t * in = rf.receiveMessage();
    if (in){
      int i;
      for (i = 0; i < 15; i++){
        Serial.print(in[i]);
        Serial.print(" ~~ ");
        Serial.println(data[i]);
      }
    } else {
      Serial.println("FAIL");
    }
  }
}

