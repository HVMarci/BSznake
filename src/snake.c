#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include "econio.h"
#include "debugmalloc.h"

void init_snake(Snake *s, int len, int x, int y, double speed) {
    s->len = len;
    s->speed = speed;
    s->head = malloc(sizeof(Block));
    s->head->pos.x = x;
    s->head->pos.y = y;
    s->head->type = TP_HEAD;
    s->head->dir = DIR_R;
    s->head->next = NULL;
    s->head->prev = NULL;

    Block* cur = s->head;
    for (int i = 1; i < len; i++) {
        cur->next = malloc(sizeof(Block));
        cur->next->pos.x = x - i;
        cur->next->pos.y = y;
        cur->next->type = TP_VSZ;
        cur->next->dir = DIR_R;
        s->head->dir = DIR_R;
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
Snake *new_snake(int len, int x, int y, double speed) {
    Snake *s = malloc(sizeof(Snake));
    init_snake(s, len, x, y, speed);
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
void move_snake(Snake *s, int dir) {
    Block *head = malloc(sizeof(Block));
    head->dir = dir;
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

int check_snake(Coord dim, Snake const *s, Block const *apple) {
    if (s->head->pos.x < 0 || s->head->pos.y < 0 || s->head->pos.x >= dim.x || s->head->pos.y >= dim.y) return COLL_WALL;

    if (s->head->pos.x == apple->pos.x && s->head->pos.y == apple->pos.y) return COLL_APPLE;

    for (Block const *ptr = s->head->next; ptr != s->tail; ptr = ptr->next) {
        if (s->head->pos.x == ptr->pos.x && s->head->pos.y == ptr->pos.y) return COLL_SELF;
    }

    return COLL_NONE;
}

int cmp(const void *pa, const void *pb) {
    int a = *(const int *) pa;
    int b = *(const int *) pb;
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
}

int exclude_snake(Coord dim, Snake const *s, int pos, int *posbuf) {
    int bufi = 0;
    for (Block const *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        posbuf[bufi++] = ptr->pos.y * dim.x + ptr->pos.x;
    }

    qsort(posbuf, s->len, sizeof(int), cmp);
    for (int i = 0; i < s->len; i++) {
        if (posbuf[i] <= pos) pos++;
    }

    return pos;
}

