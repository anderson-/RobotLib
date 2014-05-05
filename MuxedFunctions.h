/*
 * MuxedFunctions.h
 *
 *  Created on: May 5, 2014
 *      Author: fernando
 */

#ifndef MUXEDFUNCTIONS_H_
#define MUXEDFUNCTIONS_H_

#include "Arduino.h"

#define MUX_S1  4
#define MUX_S2 15
#define MUX_S3 16

void mux_begin();
int analogReadMux(uint8_t pin, uint8_t addr);
int digitalReadMux(uint8_t pin, uint8_t addr);

#endif /* MUXEDFUNCTIONS_H_ */
