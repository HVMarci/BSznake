#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "snake.h"

#define TYPE_CLI 0
#define TYPE_GUI 1

typedef struct Screen {
    int w, h, type;
} Screen;

Screen *init_screen(int w, int h, int type);
void draw_map(Screen const *sc);
void draw_block(Screen const *sc, Block const *b);
void draw_snake(Screen const *sc, Snake const *s);
void free_screen(Screen *sc);

// Game loop
// Keyboard handling

#endif
