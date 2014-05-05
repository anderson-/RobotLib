/**
 * @file Potentiometer.cpp
 * @author Fernando Padilha <fpf.padilha@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2014 by Fernando Ferreira <fpf.padilha@gmail.com>
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

#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin_in, int8_t addr) : Device(false, true),
    pin_in(pin_in),
    value(0),
    addr(addr) {
}

void Potentiometer::begin() {
  pinMode(pin_in, INPUT);
  mux_begin();
}

void Potentiometer::stop() {}

void Potentiometer::reset() {
  begin();
}

void Potentiometer::update() {
  value = analogReadMux(pin_in, addr);
}

bool Potentiometer::isReady() {
  return true;
}

uint8_t Potentiometer::get(uint8_t * buffer, uint8_t size) {
    if(size>1) {
      uint8_t *temp = (uint8_t*)&value;
      buffer[0] = temp[0];
      buffer[1] = temp[1];
      return 2;
    } else {
      return 0;
    }
}

void Potentiometer::set(const uint8_t * data, uint8_t size) {}



