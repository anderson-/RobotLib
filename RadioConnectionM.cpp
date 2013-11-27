/**
 * @file RadioConnectionM.cpp
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

#include "RadioConnectionM.h"
#include "Debug.h"

#define PAYLOAD 16
#define PIPE_A 0xF0F0F0F0E1LL
#define PIPE_B 0xF0F0F0F0D2LL

RadioConnectionM::RadioConnectionM(uint8_t pin, uint8_t SPI_SS, bool isMaster) : pin(pin),SPI_SS(SPI_SS), 
                                                                              master(isMaster), 
                                                                              isAvailable(false){
}

bool RadioConnectionM::isMaster(){
  return master;
}

void RadioConnectionM::begin(){
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
}

uint8_t RadioConnectionM::available(){
  return (!Mirf.isSending() && Mirf.dataReady());
}

void RadioConnectionM::start(){
 
}

void RadioConnectionM::stop(){
 
}

bool RadioConnectionM::sendMessage(const uint8_t * data, uint8_t size){
	
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
}

uint8_t RadioConnectionM::receiveMessage(uint8_t * buffer, uint8_t size){
	
  Mirf.getData(buffer);
	
  return PAYLOAD;
}
