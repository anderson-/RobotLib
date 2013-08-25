/**
 * @file RadioConnection.cpp
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @author Fernando Ferreira <fpf.padilha@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Anderson Antunes <anderson.utf@gmail.com>
 *                       Fernando Ferreira <fpf.padilha@gmail.com>
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

#include "RadioConnection.h"
#include "Debug.h"

#define PAYLOAD 20
#define TIMEOUT 50
#define PIPE_A 0xF0F0F0F000LL
#define PIPE_B 0xF0F0F0F100LL

RadioConnection::RadioConnection(uint8_t pin_ce, uint8_t pin_ss, uint8_t id, bool isMaster) :
	radio(pin_ce,pin_ss),
	id(id),
	master(isMaster),
    isAvailable(false) {
  //buffer = (uint8_t*)malloc(PAYLOAD*sizeof(uint8_t));
  //check(buffer);
}

bool RadioConnection::isMaster(){
  return master;
}

void RadioConnection::begin(){
  radio.begin();
  // delay de (15+1)x250us = 4 ms X 15 tentativas
  radio.setRetries(15,15);
  // define o numero de bytes enviados/recebidos (max 32)
  radio.setPayloadSize(PAYLOAD);
  // abre os canais de escrita e leitura
  uint64_t u64_id = (uint64_t)id;
  if (master){
    radio.openWritingPipe(PIPE_A | u64_id);
    radio.openReadingPipe(1,PIPE_B | u64_id);
  } else {
    radio.openWritingPipe(PIPE_B | u64_id);
    radio.openReadingPipe(1,PIPE_A | u64_id);
  }
   /* 
   * Start listening on the pipes opened for reading.
   * Be sure to call openReadingPipe() first.  Do not call write() while
   * in this mode, without first calling stopListening().  Call
   * isAvailable() to check for incoming traffic, and read() to get it.
   */
  radio.startListening();
}

uint8_t RadioConnection::available(){
  /*unsigned long started_waiting_at = millis();
  bool timeout = false;
  while (!radio.available() && !timeout){
    if (millis() - started_waiting_at > TIMEOUT){
      timeout = true;
    }
  }
  isAvailable = !timeout;
  return (timeout)? 0 : PAYLOAD;*/
  isAvailable = radio.available();
  return (isAvailable)? PAYLOAD : 0;
}

void RadioConnection::start(){
  radio.startListening();
}

void RadioConnection::stop(){
  radio.stopListening();
}

bool RadioConnection::sendMessage(const uint8_t * data, uint8_t size){
  radio.stopListening();
  bool received = radio.write(data, size);
  //Serial.println((received)? "recebida com sucesso" : "não recebida..");
  radio.startListening();
  return received;
}

uint8_t RadioConnection::receiveMessage(uint8_t * buffer, uint8_t size){
  if (isAvailable){
    bool done = false;
    uint8_t timeout = 3; // tentativas, RF24L01 tem 3 FIFOs
    while (!done){
      done = radio.read(buffer, size);
      //Serial.print("Tentativas: ");
      //Serial.println(timeout);
      delay(1);
      if (timeout > 0){
        timeout--;
      } else {
        return 0;
      }
    }
    return PAYLOAD;
  }
  return 0;
}
