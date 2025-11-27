/** @file interface.h
 * @brief A megjelenítésért felelős kódok.
 * 
 * Eltakarja a különbségeket a CLI és GUI módok között.
 */

#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "snake.h"
#include "leaderboard.h"
#include "bsz.h"
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>

/**
 * @brief A megjelenítés típusa.
 */
typedef enum INTERFACE_TYPE {
    TYPE_CLI, /**< Konzolos megjelenítés */
    TYPE_GUI /**< Grafikus megjelenítés */
} INTERFACE_TYPE;

/**
 * @brief A lenyomott gomb értéke.
 */
typedef enum SNAKE_KEY {
    SNAKE_KEY_NONE, /**< Semmi nem lett lenyomva */
    SNAKE_KEY_ESCAPE, /**< Esc */
    SNAKE_KEY_UP, /**< Fel nyíl */
    SNAKE_KEY_RIGHT, /**< Jobbra nyíl */
    SNAKE_KEY_DOWN, /**< Le nyíl */
    SNAKE_KEY_LEFT, /**< Balra nyíl */
    SNAKE_KEY_W = 'w', /**< W */
    SNAKE_KEY_A = 'a', /**< A */
    SNAKE_KEY_S = 's', /**< S */
    SNAKE_KEY_D = 'd', /**< D */
    SNAKE_KEY_I = 'i', /**< I */
    SNAKE_KEY_J = 'j', /**< J */
    SNAKE_KEY_K = 'k', /**< K */
    SNAKE_KEY_L = 'l', /**< L */
    SNAKE_KEY_T = 't', /**< T */
    SNAKE_KEY_F = 'f', /**< F */
    SNAKE_KEY_G = 'g', /**< G */
    SNAKE_KEY_H = 'h' /**< H */
} SNAKE_KEY;

/**
 * @brief A megjelenítés adatait tároló struct.
 */
typedef struct Screen {
    Coord dim; /**< A pálya méretei, nem pixelben, hanem blokkszámban */
    INTERFACE_TYPE type; /**< A megjelenítés típusa: grafikus vagy konzolos */
    int block_size; /**< Grafikus megjelenítés esetén hány pixel legyen egy blokk */
    SDL_Window *window; /**< Grafikus megjelenítéshez */
    SDL_Renderer *renderer; /**< Grafikus megjelenítéshez */
    TTF_Font *font; /**< Grafikus megjelenítéshez */
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
Screen *init_screen(int w, int h, INTERFACE_TYPE type, int block_size);

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
 * @brief A pufferekben tárolt adatokat kirajzolja a képernyőre.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 */
void flush_screen(Screen const *sc);

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
 * Vár az s->speed által megadott másodpercet, és beolvassa az adott idő alatt lenyomott gombokat a billentyűzetről. Az első `bufsize` darabot eltárolja `keybuf`-ban.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 * @param wait_time Hány mp teljen el a következő képkocka előtt?
 * @param keybuf Egy puffer, amiben a függvény eltárolhatja a lenyomott billentyűket
 * @param bufsize A puffer mérete - ennél több billentyűlenyomást nem tárol el a függvény
 * 
 * @return A beolvasott billentyűk száma
 */
int next_frame(Screen const *sc, double wait_time, SNAKE_KEY *keybuf, int bufsize);

// TODO doksi
int draw_bsz_feladat(Screen const *sc, BSzFeladat feladat);

/**
 * @brief Lezárja az `init_screen` által betöltött könyvtárakat és felszabadítja a Screen structot.
 * 
 * @param sc Az `init_screen`-től kapott mutató a Screen struct-ra
 */
void free_screen(Screen *sc);

#endif // _INTERFACE_H
