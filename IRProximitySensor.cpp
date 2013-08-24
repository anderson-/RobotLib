/* 
 * File:   IRProximitySensor.cpp
 * Author: Anderson Antunes
 *
 * Created on July 18, 2013, 4:04 PM
 */

#include "IRProximitySensor.h"
#include "Debug.h"


IRProximitySensor::IRProximitySensor(uint8_t pin_out, const uint8_t pins_sel[], uint16_t thld) : Device(true),
            pin_out(pin_out),
            thld(thld),
            value(0) {
  uint8_t i;
  this->pins_sel[0] = pins_sel[0];
  this->pins_sel[1] = pins_sel[1];
  this->pins_sel[2] = pins_sel[2];
  analog = 1;
}

IRProximitySensor::IRProximitySensor(const uint8_t pins[]) : Device(true),
            value(0) {
  uint8_t i;
  for (i = 0; i < NUM_SENSORS; i++) {
    this->pins[i] = pins[i];
  }
  analog = 0;
}

void IRProximitySensor::begin() {
  uint8_t i;
  if (analog) {
	  pinMode(pin_out, INPUT);
	  pinMode(pins_sel[0], OUTPUT);
	  pinMode(pins_sel[1], OUTPUT);
	  pinMode(pins_sel[2], OUTPUT);
  } else {
	  for (i = 0; i < NUM_SENSORS; i++) {
		pinMode(pins[i],INPUT);
	  }
  }
}

void IRProximitySensor::stop() {
  
}

void IRProximitySensor::reset() {
  value = 0;
  begin();
}

void IRProximitySensor::update(){
  uint8_t i;
  value = 0;
  for (i = 0; i < NUM_SENSORS; i++) {
	if (analog) {
		digitalWrite(pins_sel[0], (i & 1));
		digitalWrite(pins_sel[1], ((i >> 1) & 1));
		digitalWrite(pins_sel[2], ((i >> 2) & 1));
		delayMicroseconds(100);
		value |= ((analogRead(pin_out) > thld) << i);
	} else {
		value |= (digitalRead(pins[i]) << i);
	}
  }
}

bool IRProximitySensor::isReady() {
  return true;
}

uint8_t IRProximitySensor::get(uint8_t * buffer, uint8_t size) {
  if(buffer[0] == 0) {
    buffer[0] = value;
    return 1;
  } else if(buffer[0] == 1) {
	if (analog) {
		uint8_t * p = (uint8_t *) &thld;
		buffer[0] = p[0];
		buffer[1] = p[1];
		return 2;
	}
  }
  return 0;
}

void IRProximitySensor::set(const uint8_t * data, uint8_t size) {
  if (size == 2) {
    //thld = (data[1] << 8) | data[0];
    thld = *((uint16_t *) data);
  }
}
