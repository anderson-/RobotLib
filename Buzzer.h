/**
 * @file Buzzer.h
 * @author Fernando Padilha <fpf.padilha@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2014 by Fernando Ferreira <fpf.padilha@gmail.com>
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

#ifndef BUZZER_H_
#define BUZZER_H_

#include <Arduino.h>

#include "Device.h"
#include "Clock.h"

class Buzzer: public TimedDevice {
public:
    Buzzer(uint8_t pin_out);
    void begin();
    void stop();
    void reset();
    void update();
    bool isReady();
    uint8_t get(uint8_t * buffer, uint8_t size);
    void set (const uint8_t * data, uint8_t size = 1);

    void playMelody(int numNotes, int melody[], int noteDurations[]);
    void playTone(int note, long duration = 0);
    bool isFinished();

private:
    uint8_t pin_out;
    Timer timer;
    int thisNote;
    int numNotes;
    int * melody;
    int * noteDurations;
};


#endif /* BUZZER_H_ */
