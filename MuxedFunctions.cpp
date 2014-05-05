/*
 * MuxedFunctions.cpp
 *
 *  Created on: May 5, 2014
 *      Author: fernando
 */

#include "MuxedFunctions.h"

void mux_begin() {
  pinMode(MUX_S1,OUTPUT);
  pinMode(MUX_S2,OUTPUT);
  pinMode(MUX_S3,OUTPUT);
}

int analogReadMux(uint8_t pin, uint8_t addr) {
  digitalWrite(MUX_S1, (addr&1));
  digitalWrite(MUX_S2, ((addr>>1)&1));
  digitalWrite(MUX_S3, ((addr>>2)&1));
  return analogRead(pin);
}

int digitalReadMux(uint8_t pin, uint8_t addr) {
  digitalWrite(MUX_S1, (addr&1));
  digitalWrite(MUX_S2, ((addr>>1)&1));
  digitalWrite(MUX_S3, ((addr>>2)&1));
  return digitalRead(pin);
}
