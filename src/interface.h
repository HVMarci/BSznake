#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "snake.h"
#include "SDL.h"

#define TYPE_CLI 0
#define TYPE_GUI 1

#define SNAKE_KEY_NONE 0
#define SNAKE_KEY_ESCAPE 1
#define SNAKE_KEY_UP 2
#define SNAKE_KEY_RIGHT 3
#define SNAKE_KEY_DOWN 4
#define SNAKE_KEY_LEFT 5

typedef struct Screen {
    int w, h, type;
    int block_size;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void (*game_loop)(void);
} Screen;

Screen *init_screen(int w, int h, int type, int block_size, void (*game_loop)(void));
void draw_map(Screen const *sc);
void draw_block(Screen const *sc, Block const *b);
void draw_snake(Screen const *sc, Snake const *s);
void erase_block(Screen const *sc, Block const *b);
void erase_snake(Screen const *sc, Snake const *s);
int next_frame(Screen const *sc, Snake *s);
void free_screen(Screen *sc);

// Game loop
// Keyboard handling

#endif
