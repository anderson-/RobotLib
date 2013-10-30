/**
 * @file Robot.cpp
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

#include "Robot.h"

Clock _gClock;// = Clock();
Clock & Robot::clock = _gClock;

void Robot::addConnection(Connection & c){
  connections = (Connection**)check(realloc(connections,(nConnections+1)*sizeof(Connection*)));
  connections[nConnections] = &c;
  nConnections++;
}

uint8_t Robot::getConnectionListSize(){
  return nConnections;
}

Connection * Robot::getConnection(uint8_t index){
  if (index < nConnections){
    return connections[index];
  } else {
    return NULL;
  }
}

void Robot::addDevice(Device & d){
  devices = (Device**)check(realloc(devices,(nDevices+1)*sizeof(Device*)));
  state = (bool*)check(realloc(state,(nDevices+1)*sizeof(bool)));
  devices[nDevices] = &d;
  state[nDevices] = true;
  nDevices++;
}

uint8_t Robot::getDeviceListSize(){
  return nDevices;
}

Device * Robot::getDevice(uint8_t index){
  if (index < nDevices){
    return devices[index];
  } else {
    return NULL;
  }
}

bool Robot::isBusy(){
  return busy;
}

void Robot::step(){
  // atualiza sensores/atuadores
  uint8_t i;
  bool s;
  for (i = 0; i < nDevices; i++){
    devices[i]->update();
	s = devices[i]->isReady();
	if (s){ 
		if (s != state[i]){
		  deviceReady(*devices[i]);
		  state[i] = s;
		}
	} else {
		busy = true;
	}
  }
  // pensa
  think();
  // recebeu alguma mensagem?
  for (i = 0; i < nConnections; i++){
    /* antigamente...
    uint8_t payload = connections[i]->available();
    if (payload){
      uint8_t size = connections[i]->receiveMessage(buffer, (payload >= BUFFER_SIZE)? BUFFER_SIZE-1 : payload);
      messageReceived(buffer, size, i);
    }
    */
    while (connections[i]->available()){
      uint8_t size = connections[i]->receiveMessage(buffer, BUFFER_SIZE);
      if (size > 0)
        messageReceived(buffer, size, *connections[i]);
    }
  }
}

void Robot::perform(){
  // espere terminar a ação atual
  while(isBusy()){
    step();
  }
}

void Robot::begin(){
  // inicializa as conexoes
  uint8_t i;
  for (i = 0; i < nConnections; i++){
    connections[i]->begin();
  }
  // inicializa os dispositivos
  for (i = 0; i < nDevices; i++){
    devices[i]->begin();
  }
  // nenhum dispositivo pode estar ocupado antes de ser usado
  if (isBusy()){
    kill();
  }
  
  buffer = (uint8_t*)malloc(BUFFER_SIZE*sizeof(uint8_t));
  check(buffer);
}

