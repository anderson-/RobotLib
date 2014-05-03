
#define MELODY_MARIO_FERNANDO 
//#define MELODY_MARIO_DIPTO 
//#define MELODY_UNDERWORLD_DIPTO


#if defined(MELODY_MARIO_FERNANDO)
/* 
  Mario Main Theme
  by: Fernando Padilha
*/
char * melody_name = "Mario Main Theme - by Fernando Padilha";
// notes in the melody:
int melody[] = { NOTE_E5, NOTE_E5, NOTE_E5, 0, NOTE_C5, NOTE_E5, NOTE_G5,
                 0, 0, NOTE_G4, 0, 0, NOTE_C5, 0, 
                 NOTE_G4, 0, NOTE_E4, 0, NOTE_A4, NOTE_B4, 0,
                 NOTE_AS4, NOTE_A4, 0, 0, NOTE_G4, NOTE_E5, 0,
                 NOTE_G5, NOTE_A5, 0, NOTE_F5, NOTE_G5, 0, NOTE_E5, 
                 0, NOTE_C5, NOTE_D5, NOTE_B4, 0, 0, 0, 
                 NOTE_C5, 0, NOTE_G4, 0, NOTE_E4, 0, NOTE_A4,
                 NOTE_B4, 0, NOTE_AS4, NOTE_A4, 0, 0, NOTE_G4,
                 NOTE_E5, 0, NOTE_G5, NOTE_A5, 0, NOTE_F5, NOTE_G5,
                 0, NOTE_E5, 0, NOTE_C5, NOTE_D5, NOTE_B4, 0, 
                 NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, 0, NOTE_E5, 0,
                 NOTE_GS4, NOTE_A4, NOTE_C5, 0, NOTE_A4, NOTE_C5, NOTE_D5
               };
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8,8,4,8,
                        8,8,8,8,4,8,8,
                        8,8,8,4,8,8,8,
                        8,8,4,8,8,8,8,
                        8,4,8,8,8,8,8,
                        4,8,8,8,8,8,8,
                        8,8,8,8,8,8,4,
                        8,8,8,8,8,4,8,
                        8,8,8,8,4,8,8,
                        8,8,8,4,8,8,8,
                        8,8,4,8,8,8,8,
                        8,4,8,8,8,8,8 };

#elif defined(MELODY_MARIO_DIPTO)
/*
  Arduino Mario Bros Tunes
  With Piezo Buzzer and PWM
  by: Dipto Pratyaksa
  last updated: 31/3/13
*/
char * melody_name = "Mario main theme - by Dipto Pratyaksa";
//Mario main theme melody
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0, 

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6, 
  0, 0, NOTE_E6, 0, 
  0, NOTE_A6, 0, NOTE_B6, 
  0, NOTE_AS6, NOTE_A6, 0, 

  NOTE_G6, NOTE_E7, NOTE_G7, 
  NOTE_A7, 0, NOTE_F7, NOTE_G7, 
  0, NOTE_E7, 0,NOTE_C7, 
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int noteDurations[] = {
  12, 12, 12, 12, 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12, 

  12, 12, 12, 12,
  12, 12, 12, 12, 
  12, 12, 12, 12, 
  12, 12, 12, 12, 

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

#elif defined(MELODY_UNDERWORLD_DIPTO)

char * melody_name = "Underworld melody - by Dipto Pratyaksa";
// Underworld melody
int melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, 
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4, 
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4,NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};
//Underwolrd tempo
int noteDurations[] = {
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  3,
  12, 12, 12, 12, 
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18,18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

#endif
