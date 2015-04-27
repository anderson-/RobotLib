/**
 * @file LED.cpp
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

#include "LED.h"
#include "Debug.h"

LED::LED(uint8_t pin) : TimedDevice(false, true, 1),
                        _pin(pin), 
                        _period(0), 
                        _timer(0),
                        _times(0),
                        _state(0),
                        _pwm(false),
                        _blink(false),
                        _increase(true) {
  
}

void LED::begin(){
  pinMode(_pin, OUTPUT);
  //digitalWrite(_pin, true);
  //delay(10);
  digitalWrite(_pin, false);
  clock.add(_timer);
}

void LED::stop(){
  
}

void LED::reset(){
  
}

void LED::update() {
    if (_pwm) {
        if (_blink) {

        } else {
            analogWrite(_pin, _state);
        }
    } else {
        if (_blink && !_timer) {
            if (_times) {
                _state = (_state)? 0 : 255;
                if (_state && _times != 255) {
                    _times--;
                }
            } else {
                _state = 0;
                _blink = false;
            }
            _timer = _period;
        }
        digitalWrite(_pin, _state);
    }
}

bool LED::isReady(){
  return true;
}

uint8_t LED::get(uint8_t * buffer, uint8_t size){
  buffer[0] = _state;
  return 1;
}

void LED::set(const uint8_t * data, uint8_t size){
  _state = data[0];
}

void LED::setState(uint8_t state){
	_state = state;
}

void LED::toggle(){
	_state = (_state)? 0 : 255;
}

uint8_t LED::getState(){
	return _state;
}

void LED::blink(uint16_t period, uint8_t times){
	_blink = true;
	_period = period;
	_times = times;
}

void LED::setPeriod(uint16_t period){
	_period = period;
}

uint16_t LED::getPeriod(){
	return _period;
}

void LED::setBlinkTimes(uint8_t times){
	_blink = true;
	_times = times;
}

uint8_t LED::getBlinkTimes(){
	return _times;
}

void LED::setPWM(bool pwm){
	_pwm = pwm;
}
