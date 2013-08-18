/**
 * @file LED.h
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

#ifndef LED_H
#define	LED_H

#include <stdint.h>
#include "Clock.h"

class LED : public TimedDevice {
public:
  LED(uint8_t pin);
  void begin();
  void stop();
  void reset();
  void update();
  bool isReady();
  uint8_t get(uint8_t * buffer, uint8_t size);
  void set (const uint8_t * data, uint8_t size = 1);
public:
  void setState(uint8_t state);
  void toggle();
  uint8_t getState();
  void blink(uint16_t period, uint8_t times);
  void setPeriod(uint16_t period);
  uint16_t getPeriod();
  void setBlinkTimes(uint8_t times);
  uint8_t getBlinkTimes();
  void setPWM(bool pwm);

private:
  uint8_t _pin;
  uint16_t _period;
  Timer _timer;
  uint8_t _times;
  uint8_t _state;
  bool _pwm;
  bool _blink;
  bool _increase;
};

#endif	/* LED_H */

