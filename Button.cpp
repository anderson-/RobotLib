/**
 * @file Button.cpp
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

#include "Button.h"

Button::Button(uint8_t pin_in, int8_t pos) : Device(false, true),
    pin_in(pin_in),
    value(0),
    addr(pos) {
}

void Button::begin() {
  pinMode(pin_in, INPUT);
  mux_begin();
}

void Button::stop() {}

void Button::reset() {
  begin();
}

void Button::update() {
  value = (uint8_t) digitalReadMux(pin_in, addr);
}

bool Button::isReady() {
  return true;
}

uint8_t Button::get(uint8_t * buffer, uint8_t size) {
  buffer[0] = value;
  return 1;
}

void Button::set(const uint8_t * data, uint8_t size) {}



