/**
 * @file HBridge.cpp
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

#include "HBridge.h"
#include "Debug.h"

HBridge::HBridge(uint8_t pin1,uint8_t pin2,uint8_t pin3,uint8_t pin4) : Device(true, false),
                                                                        leftMotor1(pin1),
                                                                        leftMotor2(pin2),
                                                                        rightMotor1(pin3),
                                                                        rightMotor2(pin4),
                                                                        speedLeftMotor1(0),
                                                                        speedLeftMotor2(0),
                                                                        speedRightMotor1(0),
                                                                        speedRightMotor2(0) {
}

void HBridge::setMotorState(uint8_t motor, int8_t speed){ //speed de -128 até 127
	if (motor == 1){
		if (speed >= 0){
      speedLeftMotor1  = (uint8_t)speed*2;
      speedLeftMotor2  = 0;
    } else {
      speedLeftMotor1  = 0;
      speedLeftMotor2  = (uint8_t)speed*2;
    }
	} else {
	  if (speed >= 0){
	    speedRightMotor1 = (uint8_t)speed*2;
	    speedRightMotor2 = 0;
    } else {
      speedRightMotor1 = 0;
      speedRightMotor2 = (uint8_t)speed*2;
    }
	}
}

void HBridge::stop(){
	begin();
}

void HBridge::reset(){
	begin();
}

void HBridge::begin(){
  pinMode( leftMotor1, OUTPUT);
  pinMode( leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  speedLeftMotor1  = 0;
  speedLeftMotor2  = 0;
  speedRightMotor1 = 0;
  speedRightMotor2 = 0;
}

void HBridge::update(){
  analogWrite( leftMotor1, speedLeftMotor1 );
  analogWrite( leftMotor2, speedLeftMotor2 );
  analogWrite(rightMotor1, speedRightMotor1);
  analogWrite(rightMotor2, speedRightMotor2);
}

bool HBridge::isReady(){
return true;
}

uint8_t HBridge::get(uint8_t * buffer, uint8_t size){
  buffer[0] = max( speedLeftMotor1, speedLeftMotor2 );
  buffer[1] = max(speedRightMotor1, speedRightMotor2);
  return 2;
}

void HBridge::set(const uint8_t * data, uint8_t size){
	setMotorState(data[0], data[1]);
}
