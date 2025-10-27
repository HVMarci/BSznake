#ifndef _SNAKE_H
#define _SNAKE_H

#include <stdlib.h>

// blokk típusok xo
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
// o
#define TP_HEAD 6
// x
#define TP_TAIL 7

#define DIR_U 0
#define DIR_R 1
#define DIR_D 2
#define DIR_L 3

typedef struct Block {
    int x, y;
    int type, dir; // kell?
    struct Block *next, *prev; // utóbbi kell?
} Block;

typedef struct Snake {
    int len;
    Block *head, *tail;
} Snake;

void init_snake(Snake* s, int len, int x, int y);

Snake *new_snake(int len, int x, int y);

void free_snake(Snake* s);
void move_snake(Snake *s, int dir);

#endif // _SNAKE_H
