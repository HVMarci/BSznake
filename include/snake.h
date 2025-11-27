/** @file snake.h
 * @brief A kígyó objektumot kezelő struktúrák, függvények és értékek.
 */

#ifndef _SNAKE_H
#define _SNAKE_H

#include <stdbool.h>

// blokk típusok
// vízszintes
/*#define TP_VSZ 0
// függőleges
#define TP_FG 1
// jobb alsó
#define TP_JA 2
// bal alsó
#define TP_BA 3
// jobb felső
#define TP_JF 4
// bal felső
#define TP_BF 5
// kacsacsőrők
#define TP_HEAD 6
// x
#define TP_TAIL 7
// O
#define TP_APPLE 8*/

/**
 * @brief A blokk típusa.
 */
typedef enum TP {
    TP_VSZ, /**< Vízszintes */
    TP_FG, /**< Függőleges */
    TP_JA, /**< Jobb alsó sarok */
    TP_BA, /**< Bal alsó sarok */
    TP_JF, /**< Jobb felső sarok */
    TP_BF, /**< Bal felső sarok */
    TP_HEAD, /**< Fej */
    TP_TAIL, /**< Farok */
    TP_APPLE /**< Alma */
} TP;

/*#define DIR_U 0
#define DIR_R 1
#define DIR_D 2
#define DIR_L 3*/

/**
 * @brief A blokk iránya. Használható indexelésre (URDL).
 */
typedef enum DIR {
    DIR_U = 0, /**< Fel */
    DIR_R = 1, /**< Jobbra */
    DIR_D = 2, /**< Le */
    DIR_L = 3 /**< Balra */
} DIR;

/*#define COLL_NONE 0
#define COLL_APPLE 1
#define COLL_SELF 2
#define COLL_WALL 3*/

/**
 * @brief Ütközés típusa.
 */
typedef enum COLL {
    COLL_NONE, /**< Semmivel */
    COLL_APPLE, /**< Almával */
    COLL_SNAKE, /**< Kígyóval */
    COLL_WALL /**< Fallal */
} COLL;

/**
 * @brief Koordinátapár.
 * 
 * Nem pixelekben, hanem blokkokban megadva ( (0,0) -> (w,h) ) kell használni!
 */
typedef struct Coord {
    int x, y;
} Coord;

/**
 * @brief RGB érték.
 */
typedef struct Color {
    int r, g, b;
    int cli_code;
} Color;

/**
 * @brief A blokkokat tároló struct.
 * 
 * Minden 1x1-es dolgot blokknak hívok. Ez lehet a kígyó egyik tagja, vagy az alma.
 */
typedef struct Block {
    Coord pos; /**< A blokk helyzete (0,0) a bal felső, (w,h) a jobb alsó sarok */
    TP type; /**< Konzolon milyen karakterként jelenjen meg? */
    DIR dir; /**< Milyen irányban van a fej felé lévő következő blokk? (konzolos megjelenítés miatt) */
    Color col; /**< A megjelenítés színe */
    struct Block *next, *prev; /**< A duplán láncolt lista előző és következő eleme (TODO jó ez így, vagy két komment kell?) */
} Block;

/**
 * @brief A kígyót tároló struct.
 * 
 * A kígyó tagjai duplán láncolt listában vannak tárolva.
 */
typedef struct Snake {
    int len; /**< A kígyó hossza */
    bool alive; /**< Él-e még a kígyó */
    //double speed; /**< A kígyó "sebessége" - hány mp telik el két képkocka között */
    Block *head; /**< A láncolt lista eleje */
    Block *tail; /**< A láncolt lista vége */
} Snake;

/**
 * @brief Egy létező Snake struktúra inicializálása.
 * 
 * Feltölti a struktúra paramétereit és létrehoz egy len hossz láncolt listát. `(x,y)`-ban lesz a feje, `dir`-től függ, hogy hol lesz a farka.
 * 
 * @param s Az inicializálandó Snake struct
 * @param len A kígyó hossza
 * @param x A fej pozíciója
 * @param y TODO Coord legyen!
 * @param dir A kígyó iránya
 * @param col A kígyó színe - GUI-ban a feje ennek az invertáltja lesz
 */
void init_snake(Snake* s, int len, int x, int y, DIR dir, Color col);

/**
 * @brief Új Snake struktúra dinamikus foglalása, majd inicializálása.
 * 
 * A létrehozott kígyót `free_snake`-kel fel kell szabadítani!
 * 
 * @param len A kígyó hossza
 * @param x A fej pozíciója
 * @param y TODO Coord legyen!
 * @param dir A kígyó iránya
 * @param col A kígyó színe - GUI-ban a feje ennek az invertáltja lesz
 * 
 * @return Mutató a heap-en létrejött Snake struct-ra
 */
Snake *new_snake(int len, int x, int y, DIR dir, Color col);

/**
 * @brief A Snake adatstrukrúra felszabadítása.
 * 
 * Felszabadítja a Snake structot és a benne tárolt láncolt listát.
 * 
 * @param s A felszabadítandó struct
 */
void free_snake(Snake* s);

/**
 * @brief A kígyó mozgatása.
 * 
 * Hozzáfűz egy új elemet a kígyó fejéhez `dir` irányban. (Hosszabb lesz tőle a kígyó.)
 * 
 * @param s A Snake struct
 * @param dir A mozgás iránya
 */
void move_snake(Snake *s, DIR dir);

/**
 * @brief A kígyó farkának levágása
 * 
 * Mozgatás után, hogy ne legyen hosszabb, ez a függvény vágja le a végét.
 * 
 * @param s A Snake struct
 */
void shorten_snake(Snake *s);

/**
 * @brief Feltölt egy tömböt a kígyók pozícióival.
 * 
 * TODO mi van itt a const-okkal?
 * @param snakes A kígyókra mutató mutatók tömbje
 * @param snake_count A kígyók száma
 * @param posbuf Mutató egy legalább `SUM(s->len)+1` méretű tömbre, amiben a függvény eltárolhatja a kígyók pozícióit
 */
void fill_posbuf(Coord dim, Snake **snakes, int snake_count, int *posbuf);

/**
 * @brief Megnézi, hogy nekiment-e valaminek a kígyó feje.
 * 
 * @param dim A pálya méretei
 * @param s A Snake struct
 * @param posbuf A `fill_posbuf` által feltöltött tömb, amiben a kígyók pozíciói vannak eltárolva
 * 
 * @return Ütközés semmivel - COLL_NONE, almával - COLL_APPLE, kígyóval - COLL_SELF, fallal - COLL_WALL
 */
int check_snake(Coord dim, Snake const *s, int const *posbuf);

/**
 * @brief Új alma pozicionálásához segédfüggvény.
 * 
 * A megadott `pos` és a visszatérési érték ezt jelenti: `h*y + x`. A koordinátákat átszámozza úgy, hogy azok a mezők, amiken kígyó van, ne számítsanak bele, így alma generálásakor ne kígyó testébe kerüljön az alma.
 * A megadott `pos` maximális értéke `w*h - SUM(s->len)` lehet a helyes visszatérési értékhez. Szükséges kívül létrehozni egy `>=SUM(s->len)` méretű tömböt a segédszámításokhoz, amit fel kell `fill_posbuf`-fal tölteni.
 * 
 * @param pos A pozíció kígyó nélküli számozás esetén
 * @param posbuf A `fill_posbuf` által feltöltött tömb, amiben a kígyók pozíciói vannak eltárolva
 * 
 * @return Az áthelyezett pozíció a tényleges koordináták szerint
 */
int exclude_snakes(int pos, int const *posbuf);

#endif // _SNAKE_H
