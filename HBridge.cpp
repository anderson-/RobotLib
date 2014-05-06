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

#include "Debug.h"
#include "HBridge.h"


void ISR_leftEncoder(void);
void ISR_rightEncoder(void);

volatile uint16_t leftEncCounter = 0;
volatile uint16_t rightEncCounter = 0;
volatile long tStartLE, tEndLE, tStartRE, tEndRE;


HBridge::HBridge(uint8_t pin1,uint8_t pin2,uint8_t pin3,uint8_t pin4) :
  Device(true, false),
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
      speedLeftMotor1  = (uint8_t)speed;
      speedLeftMotor2  = 0;
    } else {
      speedLeftMotor1  = 0;
      speedLeftMotor2  = (255-(uint8_t)speed);
    }
	} else {
	  if (speed >= 0){
	    speedRightMotor1 = (uint8_t)speed;
	    speedRightMotor2 = 0;
    } else {
      speedRightMotor1 = 0;
      speedRightMotor2 = (255-(uint8_t)speed);
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
  uint8_t i;
  uint16_t totalDt = 0;
  float speedLM = 0, speedRM = 0;
  float errorLM = 0, errorRM = 0;
  int16_t pidLM = 0, pidRM = 0;
  long currTime;

  currTime = millis();
  dt[currIndex] = (uint16_t)(currTime - lastUpdate);
  lastUpdate = currTime;
  currIndex++;

  if(currIndex == SPEED_MEAN_WINDOW)
    currIndex = 0;

  for(i=0; i<SPEED_MEAN_WINDOW; i++) {
    totalDt += dt[i];
  }

  speedLM = (float)(leftEncCounter - lastLeftEncCounter) * 1000.0 / totalDt; // rps
  speedLM *= RPS2SPEED; //  mm/s
  lastLeftEncCounter = leftEncCounter;
  speedRM = (float)(rightEncCounter - lastRightEncCounter) * 1000.0 / totalDt; // rps
  speedRM *= RPS2SPEED; // mm/s
  lastRightEncCounter = rightEncCounter;

  Serial.print("\nlEC = ");
  Serial.print(leftEncCounter);
  Serial.print("; rEC = ");
  Serial.print(rightEncCounter);
  Serial.print("; sLM = ");
  Serial.print(speedLM);
  Serial.print("; sRM = ");
  Serial.println(speedRM);

  errorLM = (float)(speedLeftMotor1 + speedLeftMotor2) - speedLM;
  errorRM = (float)(speedRightMotor1 + speedRightMotor2) - speedRM;

  errorSumLM += errorLM;
  errorSumRM += errorRM;

  if(errorSumLM > 511)
    errorSumLM = 511;
  else if(errorSumLM < -511)
    errorSumLM = -511;

  if(errorSumRM > 511)
    errorSumRM = 511;
  else if(errorSumRM < -511)
    errorSumRM = -511;

  Serial.print("eLM = ");
  Serial.print(errorLM);
  Serial.print("; eRM = ");
  Serial.print(errorRM);
  Serial.print("; eSLM = ");
  Serial.print(errorSumLM);
  Serial.print("; eSRM = ");
  Serial.println(errorSumRM);

  pidLM = (int16_t)(errorLM*SPEED_KP + errorSumLM*SPEED_KI);
  pidRM = (int16_t)(errorRM*SPEED_KP + errorSumRM*SPEED_KI);
  //pidLM = (speedLeftMotor1 + speedLeftMotor2);
  //pidRM = (speedRightMotor1 + speedRightMotor2);

  if(pidLM > 255)
    pidLM = 255;
  else if(pidLM < 0)
    pidLM = 0;

  if(pidRM > 255)
    pidRM = 255;
  else if(pidRM < 0)
    pidRM = 0;

  Serial.print("pidLM = ");
  Serial.print(pidLM);
  Serial.print("; pidRM = ");
  Serial.println(pidRM);

  if(speedLeftMotor1 > 0) {
    analogWrite(leftMotor1, pidLM);
    digitalWrite(leftMotor2, LOW);
  } else {
    digitalWrite(leftMotor1, LOW);
    analogWrite(leftMotor2, pidLM);
  }

  if(speedRightMotor1 > 0) {
    analogWrite(rightMotor1, pidRM);
    digitalWrite(rightMotor2, LOW);
  } else {
    digitalWrite(rightMotor1, LOW);
    analogWrite(rightMotor2, pidRM);
  }


  /*analogWrite(leftMotor1,  speedLeftMotor1);
  analogWrite(leftMotor2,  speedLeftMotor2);
  analogWrite(rightMotor1, speedRightMotor1);
  analogWrite(rightMotor2, speedRightMotor2);*/
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

void HBridge::attachEncoder(uint8_t enc1, uint8_t enc2) {
  attachInterrupt(enc1, ISR_leftEncoder, RISING);
  attachInterrupt(enc2, ISR_rightEncoder, RISING);
  leftEncCounter = 0;
  rightEncCounter = 0;
  lastLeftEncCounter = 0;
  lastRightEncCounter= 0;
  lastUpdate = millis();
  errorSumLM = 0;
  errorSumRM = 0;
  currIndex = 0;
  tStartLE = millis();
  tStartRE = tStartLE;
  tEndLE = tStartLE;
  tEndRE = tStartLE;
  memset(dt, 0, SPEED_MEAN_WINDOW);
}

void ISR_leftEncoder(void) {
  tEndLE = millis();
  if (tEndLE - tStartLE > 200) {
    leftEncCounter++;
    tStartLE = tEndLE;
  }
}

void ISR_rightEncoder(void) {
  tEndRE = millis();
  if (tEndRE - tStartRE > 200) {
    rightEncCounter++;
    tStartRE = tEndRE;
  }
}

