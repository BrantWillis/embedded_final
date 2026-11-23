#ifndef MUSIC_H_
#define MUSIC_H_

#define EIGHTH 8
#define QUARTER 16
#define HALF 32
#define DOTTED_HALF 48
#define WHOLE 64

typedef struct {
    char *name;
    uint32_t note_frequency;
    uint32_t delay;
} Note;

typedef struct {
    char *name; char *artist; char *genre;
    const Note *notes;
    uint32_t length;
} Song;

#endif