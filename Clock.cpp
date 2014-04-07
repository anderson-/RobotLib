/**
 * @file Clock.cpp
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

#include "Clock.h"
#include "Robot.h"

Clock & TimedDevice::clock = Robot::clock;

void Clock::begin(){
  reset();
}

void Clock::stop(){
  reset();
}

void Clock::reset(){
  dt = 0;
  lastTime = millis();
  wait = 1;
}

void Clock::update(){
  dt = (millis() - lastTime);
  lastTime = millis();
  accuracy = accuracy*.9f + dt*.1f;
  uint8_t i;
  for (i = 0; i < nTimers; i++){
    if (timers[i] && *timers[i]){
      if (*timers[i] <= dt){
        *timers[i] = 0;  
	  } else {
		  *timers[i] -= dt;  
	  }
    }
  }
}

bool Clock::isReady(){
  return wait;
}

uint8_t Clock::get(uint8_t * buffer, uint8_t size){
  float f = getAccuracy();
  memcpy(buffer, &f, sizeof(float));
  return sizeof(float);
}

void Clock::set (const uint8_t * data, uint8_t size){
  //TODO
}

void Clock::add(Timer & timer){
  uint8_t i;
  for (i = 0; i < nTimers; i++){
    if (!timers[i]){
      timers[i] = &timer;
      return;
    }
  }
  nTimers++;
  timers = (Timer**) check(realloc(timers,nTimers*sizeof(Timer*)));
  timers[nTimers-1] = &timer;
}

void Clock::remove(Timer & timer){
  uint8_t i;
  for (i = 0; i < nTimers; i++){
    if (timers[i] == &timer){
      timers[i] = NULL;
      return;
    }
  }
}

float Clock::getAccuracy(){
  return accuracy;
}

uint16_t Clock::getLastDeltaT(){
  return dt;
}
