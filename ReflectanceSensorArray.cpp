/**
 * @file IRProximitySensor.cpp
 * @author Fernando Ferreira <fpf.padilha@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Fernando Ferreira <fpf.padilha@gmail.com>
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

#include "ReflectanceSensorArray.h"
#include "Debug.h"

ReflectanceSensorArray::ReflectanceSensorArray(uint8_t pin_in, const uint8_t addr[], uint16_t thld) : Device(true),
            pin_in(pin_in),
            thld(thld),
            value(0) {
  memcpy(pins, addr, NUM_SENSORS);
  muxed = 1;
}

ReflectanceSensorArray::ReflectanceSensorArray(const uint8_t _pins[], uint16_t thld) : Device(true),
            pin_in(0),
            thld(thld),
            value(0) {
  memcpy(pins, _pins, NUM_SENSORS);
  muxed = 0;
}

void ReflectanceSensorArray::begin() {
  uint8_t i;
  if (muxed) {
	  pinMode(pin_in, INPUT);
	  mux_begin();
  } else {
	  for (i = 0; i < NUM_SENSORS; i++) {
	    pinMode(pins[i],INPUT);
	  }
  }
}

void ReflectanceSensorArray::stop() {
  
}

void ReflectanceSensorArray::reset() {
  value = 0;
  begin();
}

void ReflectanceSensorArray::update(){
  uint8_t i;
  int pinValue;
  value = 0;
  for (i = 0; i < NUM_SENSORS; i++) {
    pinValue = (muxed) ? analogReadMux(pin_in, pins[i]) : analogRead(pins[i]);
    value |= ((pinValue > thld) << i);
  }
}

bool ReflectanceSensorArray::isReady() {
  return true;
}

uint8_t ReflectanceSensorArray::get(uint8_t * buffer, uint8_t size) {
  if(size > 0) {
    buffer[0] = value;
    return 1;
  }
}

void ReflectanceSensorArray::set(const uint8_t * data, uint8_t size) {
  if (size == 2) {
    //thld = (data[1] << 8) | data[0];
    thld = *((uint16_t *) data);
  }
}
