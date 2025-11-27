#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include "econio.h"
#include "debugmalloc.h"

void init_snake(Snake *s, int len, int x, int y, DIR dir, Color col) {
    s->len = len;
    s->alive = true;
    s->head = (Block *) malloc(sizeof(Block));
    s->head->pos.x = x;
    s->head->pos.y = y;
    s->head->type = TP_HEAD;
    s->head->dir = dir;
    s->head->col = col;
    s->head->next = NULL;
    s->head->prev = NULL;

    Block* cur = s->head;
    for (int i = 1; i < len; i++) {
        cur->next = (Block *) malloc(sizeof(Block));
        switch (dir) {
            case DIR_U:
                cur->next->pos.x = x;
                cur->next->pos.y = y + i;
                break;
            case DIR_R:
                cur->next->pos.x = x - i;
                cur->next->pos.y = y;
                break;
            case DIR_D:
                cur->next->pos.x = x;
                cur->next->pos.y = y - i;
                break;
            case DIR_L:
                cur->next->pos.x = x + i;
                cur->next->pos.y = y;
                break;
        }
        cur->next->type = (dir == DIR_R || dir == DIR_L) ?  TP_VSZ : TP_FG;
        cur->next->dir = dir;
        cur->next->col = col;
        //s->head->dir = dir; // ?
        cur->next->next = NULL;
        cur->next->prev = cur;

        cur = cur->next;
    }
    cur->type = TP_TAIL;
    s->tail = cur;
}

/**
 * Snake must be freed!
 */
Snake *new_snake(int len, int x, int y, DIR dir, Color col) {
    Snake *s = (Snake *) malloc(sizeof(Snake));
    init_snake(s, len, x, y, dir, col);
    return s;
}

void free_snake(Snake *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        if (ptr->prev != NULL) free(ptr->prev);
        ptr->prev = NULL;
    }
    free(s->tail);
    free(s);
}

// TODO kígyó hossz min 5!!
void move_snake(Snake *s, DIR dir) {
    Block *head = (Block *) malloc(sizeof(Block));
    head->dir = dir;
    head->col = s->head->col;
    head->type = TP_HEAD;
    head->prev = NULL;

    int d = s->head->dir;
    head->pos.x = s->head->pos.x;
    head->pos.y = s->head->pos.y;
    switch (dir) {
    case DIR_U:
        head->pos.y -= 1;
        if (d == DIR_R) s->head->type = TP_BF;
        else if (d == DIR_U) s->head->type = TP_FG;
        else if (d == DIR_L) s->head->type = TP_JF;
        break;
    case DIR_R:
        head->pos.x += 1;
        if (d == DIR_D) s->head->type = TP_JF;
        else if (d == DIR_R) s->head->type = TP_VSZ;
        else if (d == DIR_U) s->head->type = TP_JA;
        break;
    case DIR_D:
        head->pos.y += 1;
        if (d == DIR_L) s->head->type = TP_JA;
        else if (d == DIR_D) s->head->type = TP_FG;
        else if (d == DIR_R) s->head->type = TP_BA;
        break;
    case DIR_L:
        head->pos.x -= 1;
        if (d == DIR_U) s->head->type = TP_BA;
        else if (d == DIR_L) s->head->type = TP_VSZ;
        else if (d == DIR_D) s->head->type = TP_BF;
        break;
    }

    //cli_draw_block(s->head);
    //cli_draw_block(head);

    head->next = s->head;
    s->head->dir = head->dir;
    s->head->prev = head;
    s->head = head;
}

// almák miatt különszedtem
void shorten_snake(Snake *s) {
    Block *oldtail = s->tail;
    s->tail = s->tail->prev;
    s->tail->type = TP_TAIL;
    free(oldtail);
    s->tail->next = NULL;
}

int cmp(const void *pa, const void *pb) {
    int a = *(const int *) pa;
    int b = *(const int *) pb;
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}

void fill_posbuf(Coord dim, Snake **snakes, int snake_count, int *posbuf) {
    int bufi = 0;
    for (int i = 0; i < snake_count; i++) {
        Snake const *s = snakes[i];
        if (!s->alive) continue; // csak élő kígyókkal foglalkozunk
        for (Block const *ptr = s->head; ptr != NULL; ptr = ptr->next) {
            posbuf[bufi++] = ptr->pos.y * dim.x + ptr->pos.x;
        }
    }
    posbuf[bufi] = -1;
    qsort(posbuf, bufi, sizeof(int), cmp);
}

int check_snake(Coord dim, Snake const *s, int const *posbuf) {
    if (s->head->pos.x < 0 || s->head->pos.y < 0 || s->head->pos.x >= dim.x || s->head->pos.y >= dim.y) return COLL_WALL;

    int pos = s->head->pos.y * dim.x + s->head->pos.x;

    // annyira rövid a tömb, hogy nem érdemes bináris kereséssel vacakolni, főleg, hogy nem tudjuk a posbuf méretét
    int talalat = 0;
    for (int i = 0; posbuf[i] != -1; i++) {
        if (posbuf[i] == pos) talalat++;

        if (talalat > 1) return COLL_SNAKE;
    }

    return COLL_NONE;
}

int exclude_snakes(int pos, int const *posbuf) {
    for (int i = 0; posbuf[i] != -1; i++) {
        if (posbuf[i] <= pos) pos++;
    }

    return pos;
}

