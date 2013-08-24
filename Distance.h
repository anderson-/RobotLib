/**
 * @file LED.h
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdint.h>
#include <Arduino.h>

#include "Device.h"
#include "Debug.h"

class Distance: public Device {

public:
    Distance(uint8_t pin);
    void begin();
    void stop();
    void reset();
    void update();
    bool isReady();
    uint8_t get(uint8_t * buffer, uint8_t size);
    void set (const uint8_t * data, uint8_t size = 1);

private:
    uint8_t pin;
    int distance;
};

#endif

