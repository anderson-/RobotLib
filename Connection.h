/**
 * @file Connection.h
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

#ifndef CONNECTION_H
#define	CONNECTION_H

#include <stdint.h>

/**
 * 
 */

class Connection {
public:
  /**
   * Inicializa a conexão.
   */
  virtual void begin() = 0;
  
  /**
   * Verifica se existe alguma mensagem disponivel.
   * 
   * @return O numero de bytes disponiveis, se possivel.
   */
  virtual uint8_t available() = 0;
  
  /**
   * Envia uma mensagem.
   * 
   * @param[in] data Um array de bytes contendo a mensagem.
   * @param size O tamanho do array \c data.
   * 
   * @return \c TRUE se a mensagem foi entregue corretamente.
   */
  virtual bool sendMessage(const uint8_t * data, uint8_t size) = 0;
  
  /**
   * Recebe uma mensagem.
   * 
   * @param[out] buffer Um array de bytes usado para armazenar a mensagem recebida.
   * @param size O tamanho do \c buffer.
   * 
   * @return O tamanho da mensagem recebida.
   */
  virtual uint8_t receiveMessage(uint8_t * buffer, uint8_t size) = 0;
private:

};

#endif	/* CONNECTION_H */

