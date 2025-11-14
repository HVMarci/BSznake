/** @file snake.h
 * @brief A kígyó objektumot kezelő struktúrák, függvények és értékek.
 */

#ifndef _SNAKE_H
#define _SNAKE_H

// blokk típusok - TODO enum
// vízszintes
#define TP_VSZ 0
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
#define TP_APPLE 8

/*typedef enum TP {
    TP_VSZ,
    TP_FG,
    TP_JA,
    TP_BA,
    TP_JF,
    TP_BF,
    TP_HEAD,
    TP_TAIL,
    TP_APPLE
} TP;*/

#define DIR_U 0
#define DIR_R 1
#define DIR_D 2
#define DIR_L 3

//typedef enum DIR { DIR_U, DIR_R, DIR_D, DIR_L } DIR;

#define COLL_NONE 0
#define COLL_APPLE 1
#define COLL_SELF 2
#define COLL_WALL 3

//typedef enum COLL { COLL_NONE, COLL_APPLE, COLL_SELF, COLL_WALL } COLL;

/**
 * @brief Koordinátapár.
 * 
 * Nem pixelekben, hanem blokkokban megadva ( (0,0) -> (w,h) ) kell használni!
 */
typedef struct Coord {
    int x, y;
} Coord;

/**
 * @brief A blokkokat tároló struct.
 * 
 * Minden 1x1-es dolgot blokknak hívok. Ez lehet a kígyó egyik tagja, vagy az alma.
 */
typedef struct Block {
    Coord pos; /**< A blokk helyzete (0,0) a bal felső, (w,h) a jobb alsó sarok */
    int type; /**< Konzolon milyen karakterként jelenjen meg? */
    int dir; /**< Milyen irányban van a fej felé lévő következő blokk? (konzolos megjelenítés miatt) */
    struct Block *next, *prev; /**< A duplán láncolt lista előző és következő eleme (TODO jó ez így, vagy két komment kell?) */
} Block;

/**
 * @brief A kígyót tároló struct.
 * 
 * A kígyó tagjai duplán láncolt listában vannak tárolva.
 */
typedef struct Snake {
    int len; /**< A kígyó hossza */
    double speed; /**< A kígyó "sebessége" - hány mp telik el két képkocka között */
    Block *head; /**< A láncolt lista eleje */
    Block *tail; /**< A láncolt lista vége */
} Snake;

/**
 * @brief RGB érték.
 */
typedef struct Color {
    int r, g, b;
} Color;

/**
 * @brief Egy létező Snake struktúra inicializálása
 * 
 * Feltölti a struktúra paramétereit és létrehoz egy len hossz láncolt listát. (x,y)-ban lesz a feje, (x-len+1,y)-ban a farka.
 * 
 * @param s Az inicializálandó Snake struct
 * @param len A kígyó hossza
 * @param x A fej pozíciója
 * @param y TODO Coord legyen!
 * @param speed A kígyó "sebessége" (ld. typedef Snake)
 */
void init_snake(Snake* s, int len, int x, int y, double speed);

/**
 * @brief Új Snake struktúra dinamikus foglalása, majd inicializálása.
 * 
 * A létrehozott kígyót `free_snake`-kel fel kell szabadítani!
 * 
 * @param len A kígyó hossza
 * @param x A fej pozíciója
 * @param y TODO Coord legyen!
 * @param speed A kígyó "sebessége" (ld. typedef Snake)
 * 
 * @return Mutató a heap-en létrejött Snake struct-ra
 */
Snake *new_snake(int len, int x, int y, double speed);

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
void move_snake(Snake *s, int dir);

/**
 * @brief A kígyó farkának levágása
 * 
 * Mozgatás után, hogy ne legyen hosszabb, ez a függvény vágja le a végét.
 * 
 * @param s A Snake struct
 */
void shorten_snake(Snake *s);

/**
 * @brief Megnézi, hogy nekiment-e valaminek a kígyó feje.
 * 
 * @param dim A pálya méretei
 * @param s A Snake struct
 * @param apple Az alma
 * 
 * @return Ütközés semmivel - COLL_NONE, almával - COLL_APPLE, kígyóval - COLL_SELF, fallal - COLL_WALL
 */
int check_snake(Coord dim, Snake const *s, Block const *apple);

/**
 * @brief Új alma pozicionálásához segédfüggvény.
 * 
 * A megadott `pos` és a visszatérési érték ezt jelenti: `h*y + x`. A koordinátákat átszámozza úgy, hogy azok a mezők, amiken kígyó van, ne számítsanak bele, így alma generálásakor ne a kígyó testébe kerüljön az alma.
 * A megadott `pos` maximális értéke `w*h - s->len` lehet a helyes visszatérési értékhez. Szükséges kívül létrehozni egy `>=s->len` méretű tömböt a segédszámításokhoz.
 * 
 * @param dim A pálya méretei
 * @param s A Snake struct
 * @param pos A pozíció kígyó nélküli számozás esetén
 * @param posbuf Mutató egy legalább `s->len` méretű tömbre, amit a függvény felhasználhat segédszámítások végzésére
 * 
 * @return Az áthelyezett pozíció a tényleges koordináták szerint
 */
int exclude_snake(Coord dim, Snake const *s, int pos, int *posbuf);

#endif // _SNAKE_H
