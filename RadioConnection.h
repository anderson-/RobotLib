/**
 * @file RadioConnection.h
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

#ifndef RADIO_CONNECTION_H
#define	RADIO_CONNECTION_H

#include <stdint.h>
#include "Connection.h"


#ifdef LIBRARY_RF24
	#include <RF24.h>
#else
	#include <Mirf.h>
	#include <nRF24L01.h>
	#include <MirfHardwareSpiDriver.h>
#endif

class RadioConnection : public Connection {

public:
  RadioConnection(uint8_t pin, uint8_t SPI_SS, bool isMaster = false);
  bool isMaster();
  
  void begin();
  void start();
  void stop();
  void printDetails();
  uint8_t available();
  uint8_t receiveMessage(uint8_t * buffer, uint8_t size);
  bool sendMessage(const uint8_t * data, uint8_t size);

private:
#ifdef LIBRARY_RF24
  RF24 radio;
#else
  uint8_t pin;
  uint8_t SPI_SS;
#endif
  bool master;

};

#endif	/* RADIO_CONNECTION_H */

