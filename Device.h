/**
 * @file Device.h
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

#ifndef DEVICE_H
#define	DEVICE_H

#include <stdint.h>

/**
 * 
 */

class Device {
public:
  /**
   * Construtor padrão.
   * 
   * @param isEffector Define que o dispositivo é um atuador.
   * @param isSensor Define que o dispositivo é um sensor.
   */
  Device(bool isEffector, bool isSensor = false);
  
  /**
   * @return \c TRUE se o dispositivo é um atuador.
   */
  bool isEffector();
  
  /**
   * @return \c TRUE se o dispositivo é um sensor.
   */
  bool isSensor();
  
  /**
   * @return O identificador exclusivo de cada dispositivo.
   */
  uint8_t getID();
  
  /**
   * Inicializa o dispositivo.
   * 
   * Esta função é executada apenas uma vez em Robot::begin().
   */
  virtual void begin() = 0;
  
  /**
   * Interrompe o funcionamento do dispositivo.
   */
  virtual void stop() = 0;
  
  /**
   * Redefine o estado do dispositivo.
   */
  virtual void reset() = 0;
  
  /**
   * Atualiza o estado atual do dispositivo.
   * 
   * Esta função é executada uma vez a cada Robot::step().
   */
  virtual void update() = 0;
  
  /**
   * Verifica se o dispositivo está pronto para receber
   * novos comandos ou leituras.
   * 
   * É importante para Robot definir se uma ação de maior 
   * grau de abstração foi concluída ou não.
   * 
   * @return \c FALSE se o dispositivo está ocioso.
   */
  virtual bool isReady() = 0;
  
  /**
   * 
   * @param[out] buffer Array de bytes com tamanho suficiente 
   * para armazenar a resposta.
   * @param size Tamanho do \c buffer.
   * 
   * @return O numero de bytes escritos em \c buffer.
   */
  virtual uint8_t get(uint8_t * buffer, uint8_t size) = 0;
  
  /**
   * 
   * @param[in] data Mensagem a ser processada pelo dispositivo.
   * @param size Tamanho do vetor data.
   */
  virtual void set (const uint8_t * data, uint8_t size = 1) = 0;
  
private:
  static uint8_t ID;
  uint8_t id;
  bool effector;
  bool sensor;
};

#endif	/* DEVICE_H */

