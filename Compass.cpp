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

Compass::Compass() : Device(false, true, 3),
	compass(),
	angleInt(0),
	error(0){
}

Compass::Compass(float xmin, float xmax, float ymin, float ymax) : Device(false, true, 3),
	compass(),
	angleInt(0),
	error(0){
	Xmin = xmin;
	Xmax = xmax;
	Ymin = ymin;
	Ymax = ymax;
}



void Compass::begin(){

    Wire.begin(); // Start the I2C interface.
    error = 0;
    //Serial.println("Setting scale to +/- 1.3 Ga");
    error = compass.SetScale(1.30); // Set the scale of the compass.

    //diz que tá dando erro na hora de escolher a escala e dar o modo de medição
    //mas aparentemente funciona normalmente

    if(error != 0) // If there is an error, print it out.
    {
        //Serial.println(compass.GetErrorText(error));
    }

    //Serial.println("Setting measurement mode to continous.");
    error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous

    if(error != 0) // If there is an error, print it out.
    {
        //Serial.println(compass.GetErrorText(error));
    }

}

void Compass::stop(){}

void Compass::reset(){}

void Compass::update(){
  // Retrive the raw values from the compass (not scaled).
  //MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();

  // Values are accessed like so:
  //int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float xcal = (scaled.XAxis-Xmin)/(Xmax-Xmin) - 0.5;
  float ycal = (scaled.YAxis-Ymin)/(Ymax-Ymin) - 0.5;
  float heading = atan2(ycal, xcal);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = -0.333061181;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;

  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;

  float angle = headingDegrees;

  angleInt = (int) angle;

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  // delay(66);
}

bool Compass::isReady(){
  return true;
}

uint8_t Compass::get(uint8_t * buffer, uint8_t size){
    memcpy(buffer,&angleInt,sizeof(int));
    return 2;
}

void Compass::set(const uint8_t * data, uint8_t size){

}

int Compass::getAngle(){
	return angleInt;
}
