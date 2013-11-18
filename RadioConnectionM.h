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

#ifndef RADIO_CONNECTIONM_H
#define	RADIO_CONNECTIONM_H

#include <stdint.h>
#include "Connection.h"
#include "../Mirf/Mirf.h"
#include "../Mirf/nRF24L01.h"
#include "../Mirf/MirfHardwareSpiDriver.h"

class RadioConnectionM : public Connection {
public:
  RadioConnectionM(uint8_t pin, uint8_t SPI_SS, bool isMaster = false);
  bool isMaster();
  
  void begin();
  void start();
  void stop();
  uint8_t available();
  bool sendMessage(const uint8_t * data, uint8_t size);
  uint8_t receiveMessage(uint8_t * buffer, uint8_t size);
private:
  uint8_t pin;
  uint8_t SPI_SS;
  bool master;
  bool isAvailable;
};

#endif	/* RADIO_CONNECTIONM_H */

