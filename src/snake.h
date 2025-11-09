#ifndef _SNAKE_H
#define _SNAKE_H

// blokk típusok - TODO enum
// ═
#define TP_VSZ 0
// ║
#define TP_FG 1
// ╔
#define TP_JA 2
// ╗
#define TP_BA 3
// ╚
#define TP_JF 4
// ╝
#define TP_BF 5
// o - TODO Ʌ>V<
#define TP_HEAD 6
// x
#define TP_TAIL 7
// O
#define TP_APPLE 8

#define DIR_U 0
#define DIR_R 1
#define DIR_D 2
#define DIR_L 3

#define COLL_NONE 0
#define COLL_APPLE 1
#define COLL_SELF 2
#define COLL_WALL 3

typedef struct Block {
    int x, y;
    int type, dir; // kell?
    struct Block *next, *prev; // utóbbi kell?
} Block;

typedef struct Snake {
    int len;
    double speed;
    Block *head, *tail;
} Snake;

typedef struct Color {
    int r, g, b;
} Color;

void init_snake(Snake* s, int len, int x, int y, double speed);

Snake *new_snake(int len, int x, int y, double speed);

void free_snake(Snake* s);
void move_snake(Snake *s, int dir);
void shorten_snake(Snake *s);

#include "interface.h"
int check_snake(Screen const *sc, Snake const *s, Block const *apple);
int exclude_snake(Screen const *sc, Snake const *s, int pos, int *posbuf);

#endif // _SNAKE_H
