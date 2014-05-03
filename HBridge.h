/**
 * @file HBridge.h
 * @author Luís Fernando Guerreiro <lf1992@hotmail.com>
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Luís Fernando Guerreiro <lf1992@hotmail.com>
 *                       Anderson Antunes <anderson.utf@gmail.com>
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

#ifndef H_Bridge_H
#define	H_Bridge_H

#include <stdint.h>
#include "Device.h"

class HBridge : public Device {
public:
  HBridge(uint8_t pin1,uint8_t pin2,uint8_t pin3,uint8_t pin4);
  void begin();
  void stop();
  void reset();
  void update();
  bool isReady();
  uint8_t get(uint8_t * buffer, uint8_t size);
  void set (const uint8_t * data, uint8_t size = 1);
  // particular functions
  void setMotorState(uint8_t motor, int8_t speed);
private:
    uint8_t leftMotor1,leftMotor2,rightMotor1,rightMotor2;
    uint8_t speedLeftMotor1, speedLeftMotor2, speedRightMotor1, speedRightMotor2;


};

#endif	/* H_Bridge_H */

