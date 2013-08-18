/**
 * @file Debug.cpp
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
 * @section DESCRIPTION
 *
 * Conjunto de funções para debug.
 */

#include "Debug.h"

#define LED 3
#define MAX_DELAY_ERROR 80
#define MIN_DELAY_ERROR 20

void kill(){
  bool increase = true;
  bool led_on = true;
  int pulse_time = MIN_DELAY_ERROR;
  pinMode(LED,OUTPUT);
  for (;;){
    digitalWrite(LED,led_on);
    delay(pulse_time);
    led_on = !led_on;
    if (increase){
      if (pulse_time < MAX_DELAY_ERROR){
	pulse_time++;
      } else {
	increase = false;
      }
    } else {
      if (pulse_time > MIN_DELAY_ERROR){
	pulse_time--;
      } else {
	increase = true;
      }
    }
  }
}

void * check (void * p){
  if (!p) kill();
  return p;
}

void blink (int n, int pulse_time){
  bool led_on = true;
  for (;n >=0; n--){
    digitalWrite(LED,led_on);
    delay(pulse_time);
    led_on = !led_on;
  }
  digitalWrite(LED,0);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
