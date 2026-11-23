#ifndef SONG_LIB_H_
#define SONG_LIB_H_

// Sample songs for the Assessment 3 music player. You may use one of them, adapt them,
// or write your own song (at least 8 notes, at least two different durations).
//
// Each note is {name, frequency, duration}: the name as text, the frequency in Hz from
// tone_pitch.h, and the duration in milliseconds.
// Your music.h must include tone_pitch.h and provide the Note type and the duration
// constants (WHOLE = 1000 ms, HALF, QUARTER).
// If your design stores notes differently, edit the entries to match it.
// Include this file from one .c file only (it defines the arrays).

#include "music.h"
#include "tone_pitch.h"

// Mary Had a Little Lamb (4/4): 26 notes; quarter, half and whole notes.
const Note MaryHadALittleLamb[] = {
    {"E4", NOTE_E4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, HALF},
    {"D4", NOTE_D4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"D4", NOTE_D4, HALF},
    {"E4", NOTE_E4, QUARTER},
    {"G4", NOTE_G4, QUARTER},
    {"G4", NOTE_G4, HALF},
    {"E4", NOTE_E4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"C4", NOTE_C4, WHOLE}
};

// Happy Birthday (F major): 25 notes, quarter and half notes. Each "Hap-py" is two
// quarter notes (simplified from a dotted eighth and a sixteenth); the phrase ends are half notes.
const Note HappyBirthday[] = {
    // Hap-py birth-day to you
    {"C4", NOTE_C4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"F4", NOTE_F4, QUARTER},
    {"E4", NOTE_E4, HALF},
    // Hap-py birth-day to you
    {"C4", NOTE_C4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"G4", NOTE_G4, QUARTER},
    {"F4", NOTE_F4, HALF},
    // Hap-py birth-day dear [name]
    {"C4", NOTE_C4, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"C5", NOTE_C5, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"F4", NOTE_F4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    // Hap-py birth-day to you
    {"AS4", NOTE_AS4, QUARTER},
    {"AS4", NOTE_AS4, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"F4", NOTE_F4, QUARTER},
    {"G4", NOTE_G4, QUARTER},
    {"F4", NOTE_F4, HALF}
};

const Note MyFavoriteThings[] = {
    {"E4", NOTE_E4, WHOLE},
    {"B4", NOTE_B4, QUARTER},
    {"B4", NOTE_B4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"B3", NOTE_B3, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"E4", NOTE_E4, HALF},
    {"E4", NOTE_E4, QUARTER},
    {"B4", NOTE_B4, QUARTER},
    {"B4", NOTE_B4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"B3", NOTE_B3, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"E4", NOTE_E4, HALF},
    {"E4", NOTE_E4, QUARTER},
    {"B4", NOTE_B4, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"G4", NOTE_G4, QUARTER},
    {"C4", NOTE_C4, DOTTED_HALF},
    {"B3", NOTE_B3, QUARTER},
    {"C4", NOTE_C4, QUARTER},
    {"D4", NOTE_D4, QUARTER},
    {"E4", NOTE_E4, QUARTER},
    {"F#4", NOTE_FS4, QUARTER},
    {"G4", NOTE_G4, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"B4", NOTE_B4, QUARTER},
    {"A4", NOTE_A4, QUARTER},
    {"D#4", NOTE_DS4, DOTTED_HALF},
};

#endif /* SONG_LIB_H_ */
