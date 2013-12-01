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

#define PAYLOAD 16
#define PIPE_A 0xF0F0F0F0E1LL
#define PIPE_B 0xF0F0F0F0D2LL

RadioConnection::RadioConnection(uint8_t pin, uint8_t SPI_SS, bool isMaster) :
#ifdef LIBRARY_RF24
		radio(pin,SPI_SS),
#else
		pin(pin),
		SPI_SS(SPI_SS),
#endif
    master(isMaster) {
}

bool RadioConnection::isMaster(){
  return master;
}

void RadioConnection::printDetails() {
#ifdef LIBRARY_RF24
  radio.printDetails();
#else
#endif
}

void RadioConnection::begin() {
#ifdef LIBRARY_RF24
  radio.begin();
  // delay de (3+1)x250us = 1ms X 15 tentativas = 15 ms
  radio.setRetries(3,15);
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
#else
  Mirf.cePin = pin;
	Mirf.csnPin = SPI_SS;

	Mirf.spi = &MirfHardwareSpi;
	Mirf.init();

	if (master){
		Mirf.setRADDR((byte *)"pipe1");
	} else {
		Mirf.setRADDR((byte *)"pipe2");
	}

	Mirf.payload = PAYLOAD;

	Mirf.config();
#endif
}

uint8_t RadioConnection::available() {
#ifdef LIBRARY_RF24
  return radio.available();
#else
  return (!Mirf.isSending() && Mirf.dataReady());
#endif
}

void RadioConnection::start() {
#ifdef LIBRARY_RF24
  radio.startListening();
#else
#endif
}

void RadioConnection::stop() {
#ifdef LIBRARY_RF24
  radio.stopListening();
#else
#endif
}

bool RadioConnection::sendMessage(const uint8_t * data, uint8_t size) {
#ifdef LIBRARY_RF24
  radio.stopListening();
  bool sent = false;
  int tries = 0;
  while (!sent && tries < 5) {
    sent = radio.write(data, size);
    if(!sent) {
      delay(5);
      tries++;
    }
  }
  radio.startListening();
  return sent;
#else
  if (size < PAYLOAD){
		((uint8_t *)data)[size] = 0; //TODO
	}

	if (master){
		Mirf.setTADDR((byte *)"pipe2");
	} else {
		Mirf.setTADDR((byte *)"pipe1");
	}

	Mirf.send((uint8_t *)data);

	//while (Mirf.getStatus() & (1 << MAX_RT)){
		////Envia novamente..
		//Mirf.send((uint8_t *)data);
	//}

	while(Mirf.isSending()){
	 //Wait.
	}

	return Mirf.error;
#endif
}

uint8_t RadioConnection::receiveMessage(uint8_t * buffer, uint8_t size) {
#ifdef LIBRARY_RF24
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
#else
	Mirf.getData(buffer);

	return PAYLOAD;
#endif
}
