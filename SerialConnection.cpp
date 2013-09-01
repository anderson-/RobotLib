/**
 * @file SerialConnection.cpp
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Anderson Antunes <anderson.utf@gmail.com>
 * 
 * RobotLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RobotLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with RobotLib.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "SerialConnection.h"
#include "Debug.h"

SerialConnection::SerialConnection(HardwareSerial & theSerial, uint32_t rate) :
    serial(theSerial),
    rate(rate){
  
}

void SerialConnection::begin(){
  serial.begin(rate);
}

#define TIMEOUT 10

uint8_t SerialConnection::available(){
//   unsigned long started_waiting_at = millis();
//   bool timeout = false;
//   while (!serial.available() && !timeout){
//     if (millis() - started_waiting_at > TIMEOUT){
//       timeout = true;
//     }
//   }
  return serial.available();
}

void SerialConnection::println(const char * data) {
  serial.println(data);
}

bool SerialConnection::sendMessage(const uint8_t * data, uint8_t size){
  serial.write(data,size);
  serial.println();
  return true;
}

uint8_t SerialConnection::receiveMessage(uint8_t * buffer, uint8_t size){
  uint8_t i = 0;
  uint8_t msgLenght = 0;
  uint32_t time = millis();
  while (i < size){ //TODO adicionar timeout
    if (serial.available() > 0){
      if (i == 0)
        msgLenght = serial.read();
      else
        buffer[i-1] = serial.read();
      time = millis();
      i++;
    } else if ( (millis() - time >= TIMEOUT) ){
      break;
    } else {
      delay(1);
    }
    if (i > msgLenght)
      break;
  }
  return (i-1);
}

HardwareSerial & SerialConnection::getPort(){
  return serial;
}
