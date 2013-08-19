/**
 * @file GenericRobot.h
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

#ifndef GENERIC_ROBOT_H
#define	GENERIC_ROBOT_H

#include <stdint.h>
#include "RadioRobot.h"

typedef enum { DEV_LED = 1, DEV_HBRIDGE, DEV_COMPASS } DEVICE;

class GenericRobot : public RadioRobot {
public:
  GenericRobot() {};
  Device * createNew(uint8_t id, const uint8_t * data, uint8_t size);
private:

};

#endif	/* GENERIC_ROBOT_H */

