#include <Wire.h>
#include <HMC5883L.h>
#include <SPI.h>
#include <RF24_config.h>
#include <Robot.h>
#include <RadioRobot.h>
#include <LED.h>

RadioConnection rf(9,53,false);
RadioRobot robo;
LED led(13);

void setup(){
  Serial.begin(9600);
  robo.addDevice(led);
  robo.addConnection(rf);
  robo.begin();
}

void loop(){
  robo.step();
}
