/**
 * @file Device.cpp
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

#include "Device.h"

uint8_t Device::ID = 0; // static member variable initialization

Device::Device(bool isSensor, bool isEffector) : effector(isEffector), sensor(isSensor) {
  //if (!isSensor && !isEffector){
    //???
  //}
  ID++;
  id = ID;
}

bool Device::isEffector(){
  return effector;
}

bool Device::isSensor(){
  return sensor;
}

uint8_t Device::getID(){
  return id;
}
