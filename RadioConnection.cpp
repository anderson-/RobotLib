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


RadioConnection::RadioConnection(uint8_t pin_ce, uint8_t pin_ss, uint8_t slave_id, uint8_t master_id, bool isMaster) :
	slave_id(slave_id),
	master_id(master_id),
  #ifdef LIBRARY_RF24
		radio(pin_ce,pin_ss),
#else
		pin_ce(pin_ce),
		pin_ss(pin_ss),
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

  // define o canal a ser utilizado (0-127)
  radio.setChannel(master_id);

  // abre os canais de escrita e leitura
	uint64_t u64_mid = (uint64_t) master_id;
	uint64_t u64_sid = (uint64_t) slave_id;
  if (master){
    radio.openWritingPipe(pipe_a | u64_sid);
    radio.openReadingPipe(1,pipe_b | u64_mid);
  } else {
    radio.openWritingPipe(pipe_b | u64_mid);
    radio.openReadingPipe(1,pipe_a | u64_sid);
  }

  radio.startListening();
#else
  Mirf.cePin = pin_ce;
	Mirf.csnPin = pin_ss;
	Mirf.spi = &MirfHardwareSpi;
	Mirf.init();

	Mirf.payload = PAYLOAD;
	//Mirf.channel = master_id;
	Mirf.config();

	strcpy ((char *) pipe_a, "serv1");
	snprintf((char *) pipe_b, 8, "robo%hu", slave_id);

	if (master){
		Mirf.setRADDR(pipe_a);
	} else {
		Mirf.setRADDR(pipe_b);
	}
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
		Mirf.setTADDR(pipe_b);
	} else {
		Mirf.setTADDR(pipe_a);
	}

	Mirf.send((uint8_t *)data);

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
