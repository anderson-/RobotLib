/**
 * @file GenericRobot.cpp
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

#include "GenericRobot.h"
#include "LED.h"
#include "HBridge.h"
#include "Compass.h"
#include "ReflectanceSensorArray.h"
#include "IRProximitySensor.h"

Device * GenericRobot::createNew(uint8_t id, const uint8_t * data, uint8_t size){
	switch (id){
		case DEV_LED:
			{
				if (size == 1){
					return new LED(data[0]);
				}
				break;
			}
		case DEV_HBRIDGE:
			{
				if (size == 4){
					return new HBridge(data[0], data[1], data[2], data[3]);
				}
				break;
			}
		case DEV_COMPASS:
			{
				return new Compass();
			}
		case DEV_REFLECT:
			{
				if (size == 6){
					uint8_t analog_pin = data[0];
					const uint8_t pins_sel [3] = {data[1], data[2], data[3]};
					uint16_t thld = *(uint16_t*)data[4];
					return new ReflectanceSensorArray(analog_pin, pins_sel, thld);
				}
				break;
			}
		case DEV_PROX:
			{
				if (size == 1){
					return new IRProximitySensor(data[0]);
				}
				break;
			}
	}
	return NULL;
}
