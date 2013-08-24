/**
 * @file RadioRobot.h
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

#ifndef RADIO_ROBOT_H
#define	RADIO_ROBOT_H

#include <stdint.h>
#include "Robot.h"

typedef enum { STOP = 1, ECHO, PRINT, GET, SET, ADD, RESET, DONE, RUN, NO_OP, FAIL, ALL = 222, FREE_RAM, SYSTEM, BEGIN, END } CMD;

typedef bool (*ActionFunc)(Device **, uint8_t, Connection &, const uint8_t *, uint8_t);

typedef struct {
  Device ** devices;
  uint8_t nDevices;
  Connection * connection;
  ActionFunc function;
} ActionParam;

class RadioRobot : public Robot {
public:
  RadioRobot() : actions(NULL), nActions(0), running(NULL), nRunning(0) {};
  void addAction (ActionFunc action);
  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection);
  void deviceReady (Device & d){};
  void think();
  virtual Device * createNew(uint8_t id, const uint8_t * data, uint8_t size) { return NULL; }
private:
  void startAction (ActionFunc action, const uint8_t * deviceList, uint8_t size, Connection & c, const uint8_t * data, uint8_t length);
  void freeParam (ActionParam ** p);
private:
  ActionFunc * actions;
  uint8_t nActions;
  ActionParam ** running;
  uint8_t nRunning;
};

#endif	/* RADIO_ROBOT_H */

