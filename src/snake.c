#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include "econio.h"
#include "debugmalloc.h"
#include "cli.h"

void init_snake(Snake *s, int len, int x, int y, double speed) {
    s->len = len;
    s->speed = speed;
    s->head = malloc(sizeof(Block));
    s->head->x = x;
    s->head->y = y;
    s->head->type = TP_HEAD;
    s->head->dir = DIR_R;
    s->head->next = NULL;
    s->head->prev = NULL;

    Block* cur = s->head;
    for (int i = 1; i < len; i++) {
        cur->next = malloc(sizeof(Block));
        cur->next->x = x - i;
        cur->next->y = y;
        cur->next->type = TP_VSZ;
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
    head->x = s->head->x;
    head->y = s->head->y;
    switch (dir) {
    case DIR_U:
        head->y -= 1;
        if (d == DIR_R) s->head->type = TP_BF;
        else if (d == DIR_U) s->head->type = TP_FG;
        else if (d == DIR_L) s->head->type = TP_JF;
        break;
    case DIR_R:
        head->x += 1;
        if (d == DIR_D) s->head->type = TP_JF;
        else if (d == DIR_R) s->head->type = TP_VSZ;
        else if (d == DIR_U) s->head->type = TP_JA;
        break;
    case DIR_D:
        head->y += 1;
        if (d == DIR_L) s->head->type = TP_JA;
        else if (d == DIR_D) s->head->type = TP_FG;
        else if (d == DIR_R) s->head->type = TP_BA;
        break;
    case DIR_L:
        head->x -= 1;
        if (d == DIR_U) s->head->type = TP_BA;
        else if (d == DIR_L) s->head->type = TP_VSZ;
        else if (d == DIR_D) s->head->type = TP_BF;
        break;
    }

    //cli_draw_block(s->head);
    //cli_draw_block(head);

    // TODO végét törölni
    Block *oldtail = s->tail;
    s->tail = s->tail->prev;
    s->tail->type = TP_TAIL;
    free(oldtail);
    s->tail->next = NULL;

    head->next = s->head;
    s->head->prev = head;
    s->head = head;
}
