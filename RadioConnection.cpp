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

#define PAYLOAD 10
#define PIPE_A 0xF0F0F0F0E1LL
#define PIPE_B 0xF0F0F0F0D2LL

RadioConnection::RadioConnection(uint8_t pin, uint8_t SPI_SS, bool isMaster) : radio(pin,SPI_SS), 
                                                                              master(isMaster), 
                                                                              isAvailable(false){
}

bool RadioConnection::isMaster(){
  return master;
}

void RadioConnection::begin(){
  radio.begin();
  // delay de 15x250us X 15 tentativas = 3.75 ms
  radio.setRetries(15,15);
  // define o numero de bytes enviados/recebidos (max 32)
  radio.setPayloadSize(PAYLOAD);
  // abre os canais de escrita e leitura
  if (master){
    radio.openWritingPipe(PIPE_A);
    radio.openReadingPipe(1,PIPE_B);
  } else {
    radio.openWritingPipe(PIPE_B);
    radio.openReadingPipe(1,PIPE_A);
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
  return radio.available();
}

void RadioConnection::start(){
  radio.startListening();
}

void RadioConnection::stop(){
  radio.stopListening();
}

bool RadioConnection::sendMessage(const uint8_t * data, uint8_t size){
  radio.stopListening();
  bool sent = false;
  int i = 0;
  while (!sent && i < 5) {
    sent = radio.write(data, size);
    i++;
  }
  /*while (!sent) {
    sent = radio.write(data, size);
  }*/
  //Serial.println((received)? "recebida com sucesso" : "não recebida..");
  radio.startListening();
  return sent;
}

uint8_t RadioConnection::receiveMessage(uint8_t * buffer, uint8_t size){
  bool done = false;
  uint8_t timeout = 5; //evita loop infinito
  while (true){
    done = radio.read(buffer, size);
    if (done){ // normalmente entra de primeira
      break;
    }
    if (timeout == 0){
      return 0;
    }
    timeout--;
    delay(1);
  }
  return PAYLOAD;
}
