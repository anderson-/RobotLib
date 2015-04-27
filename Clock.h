/**
 * @file Clock.h
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Anderson Antunes <anderson.utf@gmail.com>
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

#ifndef CLOCK_H
#define	CLOCK_H

#include <stdint.h>
#include <Arduino.h>
#include "Debug.h"
#include "Device.h"

typedef int16_t Timer; // ~ 1m

/**
 * 
 */

class Clock : public Device {
public:
  Clock() : Device(false,true,0), 
            timers(NULL), 
            nTimers(0),
            dt(0),
            accuracy(0),
            lastTime(0),
            wait(false) {
    
  };
  
  void begin();
  void stop();
  void reset();
  void update();
  bool isReady();
  uint8_t get(uint8_t * buffer, uint8_t size);
  void set (const uint8_t * data, uint8_t size);
  
  /**
   * Adiciona um novo timer.
   * 
   * O relógio global decrementa o valor de Clock::getLastDeltaT()
   * para cada Clock::update(). 
   * 
   * @param timer 
   */
  void add(Timer & timer);
  
  /**
   * Remove um timer.
   *
   * @param timer
   */
  void remove(Timer & timer);
  
  /**
   * 
   */
  float getAccuracy();
  
  /**
   * 
   *  @return O tempo gasto durante o ultimo Robot::step().
   */
  uint16_t getLastDeltaT();
private:
  Timer ** timers;
  uint8_t nTimers;
  uint16_t dt;
  float accuracy;
  uint32_t lastTime;
  uint32_t wait;
};

class TimedDevice : public Device {
public:
  TimedDevice(bool isEffector, bool isSensor, uint8_t staticID) : Device(isEffector, isSensor, staticID){}
protected:
  static Clock & clock;
};

#endif	/* CLOCK_H */
