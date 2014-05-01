/**
 * @file Buzzer.cpp
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

#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin_out) : TimedDevice(false, true),
    pin_out(pin_out),
    timer(-1),
    numNotes(0),
    thisNote(0) {}


void Buzzer::begin() {
  pinMode(pin_out, OUTPUT);
}

void Buzzer::stop() {
  noTone(pin_out);
  digitalWrite(pin_out, LOW);
  clock.remove(timer);
  timer = -1;
}

void Buzzer::reset() {}

void Buzzer::update() {
  if (timer == 0) {
    digitalWrite(pin_out, LOW);

    if(thisNote < numNotes) {
      // para calcular a duracao da nota, pegue um segundo dividido pelo tipo da nota
      // e.g. nota em quarta = 1000 / 4, nota em oitava = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      tone(pin_out, melody[thisNote], noteDuration);

      // para distinguir entre as notas, coloque um espaco minimo entre elas
      // a duracao da nota + 30% parece funcionar bem:
      int pauseBetweenNotes = noteDuration * 1.30;
      timer = pauseBetweenNotes;

      thisNote++;
    } else {
      clock.remove(timer);
      timer = -1;
    }
  }
}

bool Buzzer::isReady() {
  return true;
}

uint8_t Buzzer::get(uint8_t * buffer, uint8_t size) {
  return 0;
}

void Buzzer::set(const uint8_t * data, uint8_t size) {
  if(data[0] == 0) { // apenas a nota
    int note = *((int *)(data+1));
    playTone(note);
  } else { // nota + duracao
    int note = *((int *)(data+1));
    long duration = *((long *)(data+3));
    playTone(note, duration);
  }
}

void Buzzer::playMelody(int numNotes, int melody[], int noteDurations[]) {
  // itera pelas notas da melodia
  for(int thisNote = 0; thisNote < numNotes; thisNote++) {

    // para calcular a duracao da nota, pegue um segundo dividido pelo tipo da nota
    // e.g. nota em quarta = 1000 / 4, nota em oitava = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(pin_out, melody[thisNote],noteDuration);

    // para distinguir entre as notas, coloque um espaco minimo entre elas
    // a duracao da nota + 30% parece funcionar bem:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // pare de tocar a nota
    noTone(pin_out);
  }
}

void Buzzer::playMelodyU(int _numNotes, int _melody[], int _noteDurations[]) {
  thisNote      = 0;
  numNotes      = _numNotes;
  melody        = _melody;
  noteDurations = _noteDurations;

  // para calcular a duracao da nota, pegue um segundo dividido pelo tipo da nota
  // e.g. nota em quarta = 1000 / 4, nota em oitava = 1000/8, etc.
  int noteDuration = 1000/noteDurations[thisNote];
  tone(pin_out, melody[thisNote], noteDuration);

  // para distinguir entre as notas, coloque um espaco minimo entre elas
  // a duracao da nota + 30% parece funcionar bem:
  int pauseBetweenNotes = noteDuration * 1.30;
  timer = pauseBetweenNotes;
  clock.add(timer);
}

void Buzzer::playTone(int note, long duration) {
  if(note) {
    if(duration) {
      tone(pin_out, note, duration);
    } else {
      tone(pin_out, note);
    }
  } else {
    if(duration) {
      timer = duration;
      clock.add(timer);
    }
    digitalWrite(pin_out, HIGH);
  }
}

