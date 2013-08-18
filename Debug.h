/**
 * @file Debug.h
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
 * @section DESCRIPTION
 *
 * Conjunto de funções para debug.
 */

#ifndef DEBUG_H
#define	DEBUG_H

#include <Arduino.h>

/**
 * Desativa o robô enviando uma sequência predefinida para o 
 * LED de debug em um loop infinito.
 */
void kill();

/**
 * Verifica de o ponteiro \c p é válido, caso contrário executa \c kill.
 * 
 * @param[in] p Um ponteiro qualquer.
 */
void * check (void * p);

/**
 * Pisca o LED de debug \c n vezes por \c pulse_time ms.
 * 
 * @param n Número de vezes que o LED deverá piscar.
 * @param pulse_time Tempo em ms de cada pulso do LED.
 */
void blink (int n, int pulse_time);

/**
 * Retorna a quantidade de memoria RAM disponível.
 * 
 * @return o número de bytes disponíveis.
 */
int freeRam ();

#endif	/* DEBUG_H */
