#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "snake.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define TYPE_CLI 0
#define TYPE_GUI 1

#define SNAKE_KEY_NONE 0
#define SNAKE_KEY_ESCAPE 1
#define SNAKE_KEY_UP 2
#define SNAKE_KEY_RIGHT 3
#define SNAKE_KEY_DOWN 4
#define SNAKE_KEY_LEFT 5

/**
 * @brief A megjelenítés adatait tároló struct
 */
typedef struct Screen {
    Coord dim; /**< A pálya méretei, nem pixelben, hanem blokkszámban */
    int type; /**< A megjelenítés típusa: grafikus vagy konzolos */
    int block_size; /**< Grafikus megjelenítés esetén hány pixel legyen egy blokk */
    SDL_Window *window; /**< Grafikus megjelenítéshez */
    SDL_Renderer *renderer; /**< Grafikus megjelenítéshez */
    TTF_Font *font; /**< Grafikus megjelenítéshez */
    void (*game_loop)(void); /**< fölösleges - TODO kitörölni */
} Screen;

Screen *init_screen(int w, int h, int type, int block_size, void (*game_loop)(void));
void draw_map(Screen const *sc);
void draw_block(Screen const *sc, Block const *b);
void draw_snake(Screen const *sc, Snake const *s);
void erase_block(Screen const *sc, Block const *b);
void erase_snake(Screen const *sc, Snake const *s);
void draw_score(Screen const *sc, int score);
bool ask_new_game(Screen const *sc);
int next_frame(Screen const *sc, Snake *s);
void free_screen(Screen *sc);

// Game loop
// Keyboard handling

#endif
