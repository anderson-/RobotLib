/**
 * @file Robot.h
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

#ifndef ROBOT_H
#define	ROBOT_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>
#include "Debug.h"
#include "Device.h"
#include "Connection.h"
#include "RadioConnection.h"
#include "Clock.h"

#define BUFFER_SIZE 64

/**
 * A classe Robot representa um robô simples, seja ele autonomo ou não, 
 * o mesmo possui dispositivos (Device), que obtem valores do ambiente 
 * e realizam ações físicas, e conexões (Connection) as quais permitem 
 * que ele se comunique com computadores, microcontroladores ou outros 
 * robôs.
 */
 
class Robot {
public:
  /**
   * Construtor padrão.
   */
  Robot() : devices(NULL),
            state(NULL),
            nDevices(0), 
            connections(NULL),
            nConnections(0),
            busy(false),
            buffer(NULL) {
    addDevice(clock);
  }
  
  /**
   * Inicializa o robô.
   * 
   * Deve ser chamado em \c Setup() apenas uma vez.
   * É responsável por inicializar todas as conexões e
   * dispositivos integrados ao robô.
   * 
   */
  void begin();
  
  /**
   * Adiciona uma nova conexão.
   *
   * @param c Referência da conexão.
   */
  void addConnection(Connection & c);
  
  /**
   * Adiciona um novo dispositivo.
   *
   * @param d Referência do dispositivo.
   */
  void addDevice(Device & d);
  
  /**
   * Verifica se existe algum dispositivo ocupado.
   *
   * @return \c true se existe algum dispositivo ocupado.
   */
  bool isBusy();
  
  /**
   * Realiza o escalonamento de todas as funções principais do robô.
   * 
   * É nessa função que sao executadas Device::update(), e Robot::think()
   * e é verificado por novas mensagens por meio de Connection::available().
   */
  void step();
  
  /**
   * Realiza todas as ações pendentes e retorna.
   * 
   * Útil para programar as ações do robô em uma sequência.
   */
  void perform();
  
  /**
   * TODO
   */
  
  Clock & getClock (){return clock;}
  
  /**
   * Função executada quando uma mensagem é recebida por alguma conexão.
   * 
   * @param[in] data Um vetor de constantes de 8bits (byte) contendo a mensagem.
   * @param size O tamanho da mensagem em bytes.
   * @param connection O ID da conexão que recebeu a mensagem.
   */
  virtual void messageReceived(const uint8_t * data, uint8_t size, Connection & connection) = 0;
  
  /**
   * Função executada quando um dispositivo anteriormente ocupado torna-se
   * disponível.
   * 
   * @param d Referência ao dispositivo em questão.
   */
  
  virtual void deviceReady (Device & d) = 0;
  
  /**
   * Função responsável por tomar novas decisões com base nos sensores 
   * e atuadores do robô, em um determinado ciclo.
   * 
   * É executada uma vez a cada Robot::step().
   */
  virtual void think() = 0;
  
protected:
  
  /**
   * Obtem o tamanho da lista de conexões disponiveis.
   * 
   * @return O tamanho da lista \c (n>=0)
   */
  uint8_t getConnectionListSize();
  
  /**
   * Obtem o tamanho da lista de dispositivos disponiveis.
   * 
   * @return O tamanho da lista \c (n>=0)
   */
  uint8_t getDeviceListSize();
  
  /**
   * Obtem uma determinada conexão.
   * 
   * @param index Indice do dispositivo.
   * @return Um ponteiro para a conexão ou \c NULL
   */
  Connection * getConnection(uint8_t index);
  
  /**
   * Obtem um determinado dispositivo.
   * 
   * @param index Indice do dispositivo.
   * @return Um ponteiro para o dispositivo ou \c NULL
   */
  Device * getDevice(uint8_t index);
  
  uint8_t * buffer; /**< Buffer para o recebimento das mensagens, o mesmo é encaminhado para \c messageReceived. */
protected:
  friend class TimedDevice;
  static Clock & clock;
private:
  Device ** devices;
  bool * state;
  uint8_t nDevices;
  Connection ** connections;
  uint8_t nConnections;
  bool busy;
};

/**
 * 
 * A classe QuickRobot.
 */

class QuickRobot : public Robot {
public:
  QuickRobot(void (*think)(), void (*devReady)(Device & d), void (*msgRec)(const uint8_t*, uint8_t, Connection &)) : pthink(think), pmrec(msgRec), pdevr(devReady) {};
  
  void think(){
    if (pthink){
      pthink();
    }
  };
  
  void deviceReady (Device & d){
    if (pdevr){
      pdevr(d);
    }
  }
  
  void messageReceived(const uint8_t * data, uint8_t size, Connection & connection){
    if (pmrec){
      pmrec(data,size,connection);
    }
  }
  
private:
  void (*pthink)();
  void (*pmrec)(const uint8_t*, uint8_t, Connection &);
  void (*pdevr)(Device & d);
};

#endif	/* ROBOT_H */

/** @mainpage Robot
*
* @authors PET-ECO: ...
*
* <img src="img/modelo3d.png">
*
* <hr>
* @section Links
* <A HREF="http://dainf.ct.utfpr.edu.br/peteco/"> PET-ECO </A>
* <p> <A HREF="https://github.com/anderson-/RobotLib"> RobotLib (github) </A> </p>
* <hr>
* @section notes release.notes
* release.notes
* <hr>
* @section requirements requirements
* @verbinclude requirements
* <hr> 
* @todo [optionally include text about more work to be done]
* @todo Give each todo item its own line
*
*/

