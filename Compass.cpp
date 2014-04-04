/**
 * @file Compass.cpp
 * @author Diego Lee <diegolee7@gmail.com>
 * @author Anderson Antunes <anderson.utf@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Diego Lee <diegolee7@gmail.com>
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

#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L.h>
#include "Compass.h"

Compass::Compass() {
	Compass(0, 1.0, 0, 1.0);
}

Compass::Compass(float xmin, float xmax, float ymin, float ymax) :
	TimedDevice(false, true),
	compass(),
	angleInt(0),
	newValue(false),
	timer(0)
{
  Xmin = xmin;
  Xmax = xmax;
  Ymin = ymin;
  Ymax = ymax;
}

void Compass::begin() {

    Wire.begin(); // Start the I2C interface.
    //compass.SetScale(1.3); // Set the scale of the compass.
    compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
    clock.add(timer);
    timer = 66; // 15 Hz

}

void Compass::stop(){}

void Compass::reset(){}

void Compass::update() {

  // Retrive the raw values from the compass (not scaled).
  //MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float xcal = (scaled.XAxis-Xmin)/(Xmax-Xmin) - 0.5;
  float ycal = (scaled.YAxis-Ymin)/(Ymax-Ymin) - 0.5;
  float heading = atan2(ycal, xcal);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // CURITIBA-PR is: -19̣̣º 5' W, which is -19.083 Degrees, or (which we need) −0,333061181 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  const float declinationAngle = -0.333061181;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  angleInt = (int) (heading * RAD_TO_DEG);

  newValue = true;
  timer = 66;

}

bool Compass::isReady(){
	if (!timer) {
		return true;
	}	else {
		return false;
	}
}

bool Compass::available() {
	return newValue;
}

uint8_t Compass::get(uint8_t * buffer, uint8_t size){
    memcpy(buffer,&angleInt,sizeof(int));
    return 2;
}

void Compass::set(const uint8_t * data, uint8_t size){
}

void Compass::calibrate(float xmin, float xmax, float ymin, float ymax) {
		Xmin = xmin;
		Xmax = xmax;
		Ymin = ymin;
		Ymax = ymax;
}

int Compass::getAngle() {
	newValue = false;
	return angleInt;
}

HMC5883L * Compass::getCompass() {
	return &compass;
}
