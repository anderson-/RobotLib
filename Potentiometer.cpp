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

Potentiometer::Potentiometer(uint8_t pin_in) : Device(false, true),
    pin_in(pin_in),
    value(0),
    pos(-1) {}

Potentiometer::Potentiometer(uint8_t pin_in, int8_t pos, const uint8_t pins_sel[]) : Device(false, true),
    pin_in(pin_in),
    value(0),
    pos(pos) {
  this->pins_sel[0] = pins_sel[0];
  this->pins_sel[1] = pins_sel[1];
  this->pins_sel[2] = pins_sel[2];
}

void Potentiometer::begin() {
  pinMode(pin_in, INPUT);
  if (pos >= 0) {
    pinMode(pins_sel[0], OUTPUT);
    pinMode(pins_sel[1], OUTPUT);
    pinMode(pins_sel[2], OUTPUT);
  }
}

void Potentiometer::stop() {}

void Potentiometer::reset() {}

void Potentiometer::update() {
  if (pos > 0) {
    digitalWrite(pins_sel[0], (pos & 1));
    digitalWrite(pins_sel[1], ((pos >> 1) & 1));
    digitalWrite(pins_sel[2], ((pos >> 2) & 1));
  }
  value = analogRead(pin_in);
}

bool Potentiometer::isReady() {
  return true;
}

uint8_t Potentiometer::get(uint8_t * buffer, uint8_t size) {

    uint8_t *temp = (uint8_t*)&value;
    uint8_t i;
    for(i = 0; i<sizeof(value);i++) {
        buffer[i] = temp[i];
    }
    return 2;

}

void Potentiometer::set(const uint8_t * data, uint8_t size) {}



