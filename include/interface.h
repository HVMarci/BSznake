/** @file interface.h
 * @brief A megjelenítésért felelős kódok.
 * 
 * Eltakarja a különbségeket a CLI és GUI módok között.
 */

#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "snake.h"
#include "leaderboard.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define TYPE_CLI 0
#define TYPE_GUI 1

/*typedef enum INTERFACE_TYPE {
    TYPE_CLI,
    TYPE_GUI
} INTERFACE_TYPE;*/

#define SNAKE_KEY_NONE 0
#define SNAKE_KEY_ESCAPE 1
#define SNAKE_KEY_UP 2
#define SNAKE_KEY_RIGHT 3
#define SNAKE_KEY_DOWN 4
#define SNAKE_KEY_LEFT 5

/*typedef enum SNAKE_KEY {
    SNAKE_KEY_NONE,
    SNAKE_KEY_ESCAPE,
    SNAKE_KEY_UP,
    SNAKE_KEY_RIGHT,
    SNAKE_KEY_DOWN,
    SNAKE_KEY_LEFT
} SNAKE_KEY;*/

/**
 * @brief A megjelenítés adatait tároló struct.
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

/**
 * @brief Inicializálja a megjelenítő környezetet.
 * 
 * Attól függően, hogy a type-ban mit adunk meg, inicializálja az econio (CLI) vagy az SDL2 (GUI) könyvtárat.
 * 
 * @param w A pálya szélessége, de nem pixelben, hanem blokkszámban (a pálya kerete nem számít bele, csak a játéktér)
 * @param h A pálya magassága, de nem pixelben, hanem blokkszámban (a pálya kerete nem számít bele, csak a játéktér)
 * @param type TYPE_CLI vagy TYPE_GUI
 * @param block_size Grafikus megjelenítés esetén hány pixel legyen egy blokk mérete
 * @param game_loop Felesleges, törölni!!!
 * 
 * @return Dinamikusan foglalt Screen struct, amivel a megjelenítő függvényeket kell hívni. Használat végén `free_screen`-nel fel kell szabadítani!
 */
Screen *init_screen(int w, int h, int type, int block_size, void (*game_loop)(void));

/**
 * @brief Kirajzolja a pályát.
 * 
 * Felrajzolja a pálya szélét.
 * Nem flush-olja a kijelzőt! (TODO!!!)
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 */
void draw_map(Screen const *sc);

/**
 * @brief Kirajzol egy blokkot.
 * 
 * Kirajzol egy blokkot a képernyő flush-olása nélkül. (TODO!!!)
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param b Mutató a kirajzolandó blokkra
 */
void draw_block(Screen const *sc, Block const *b);

/**
 * @brief Kirajzolja a kígyót.
 * 
 * Kirajzol egy kígyót a képernyő flush-olása nélkül. (TODO!!!)
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param s Mutató a kirajzolandó kígyóra
 */
void draw_snake(Screen const *sc, Snake const *s);

/**
 * @brief Letöröl egy blokkot a képernyőről.
 * 
 * Letöröl egy blokkot a képernyő flush-olása nélkül. (TODO!!!)
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param b Mutató a letörlendő blokkra
 */
void erase_block(Screen const *sc, Block const *b);

/**
 * @brief Letöröli a kígyót a képernyőről.
 * 
 * Letöröl egy kígyót a képernyő flush-olása nélkül. (TODO!!!)
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param s Mutató a letörlendő kígyóra
 */
void erase_snake(Screen const *sc, Snake const *s);

/**
 * @brief Kiírja a pontszámot.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param score A megszerzett pontszám
 */
void draw_score(Screen const *sc, int score);

/**
 * @brief Megkérdezi a játékos nevét.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param name A nevet ide másolja be
 * @param maxlen A név maximum ilyen hosszú lehet (0 nélkül)
 */
void ask_name(Screen const *sc, char *name, int maxlen);

/**
 * @brief Kiírja az 5 legjobb eredményt.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param lb A Leaderboard struct
 */
void draw_top5(Screen const *sc, Leaderboard const *lb);

/**
 * @brief Megkérdezi a játékostól, hogy szeretne-e még egyet játszani.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * 
 * @return `true` - még egy játék, `false` - ki szeretne lépni a programból
 */
bool ask_new_game(Screen const *sc);

/**
 * @brief A következő képkocka kezelése.
 * 
 * Vár az s->speed által megadott másodpercet, és beolvassa az adott idő alatt utoljára lenyomott gombot a billentyűzetről.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param s Mutató a kígyóra (s->speed miatt)
 * 
 * @return A leütött billentyű kódja, vagy SNAKE_KEY_NONE (TODO enum)
 */
int next_frame(Screen const *sc, Snake *s);

/**
 * @brief Lezárja az `init_screen` által betöltött könyvtárakat és felszabadítja a Screen structot.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 */
void free_screen(Screen *sc);

#endif
