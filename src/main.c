#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "econio.h"
#include "snake.h"
#include "interface.h"
#include "helper.h"

#include "debugmalloc.h"

// lehet felesleges :)
void game_loop() {
    printf("meghivva");
}

void play_game(int interface_type) {
    srand(time(NULL));

    // TODO query window size - windows.h-ból?
    Screen *sc = init_screen(35, 20, interface_type, 20, game_loop);
    Snake *s = new_snake(10, 10, 8, .1);
    int *posbuf = malloc(sc->dim.x * sc->dim.y * sizeof(int));
    Block *apple = malloc(sizeof(Block));
    apple->pos.x = 30;
    apple->pos.y = 8;
    apple->type = TP_APPLE;
    // a többi értéket nem használom
    
    draw_map(sc);
    draw_snake(sc, s);
    draw_block(sc, apple);

    bool exit = false;

    while (!exit) {
        //exit = next_frame();
        int key = next_frame(sc, s);
        int dir = s->head->dir;
        switch (key) {
            case SNAKE_KEY_UP:
                if (s->head->dir != DIR_D) 
                    dir = DIR_U;
                break;
            case SNAKE_KEY_RIGHT:
                if (s->head->dir != DIR_L)
                    dir = DIR_R;
                break;
            case SNAKE_KEY_DOWN:
                if (s->head->dir != DIR_U)
                    dir = DIR_D;
                break;
            case SNAKE_KEY_LEFT:
                if (s->head->dir != DIR_R)
                    dir = DIR_L;
                break;
            case SNAKE_KEY_ESCAPE:
                exit = true;
                break;
        }
    
        erase_snake(sc, s);
        move_snake(s, dir);

        switch (check_snake(sc->dim, s, apple)) {
            case COLL_APPLE:
                // replace apple
                erase_block(sc, apple);

                int newpos = exclude_snake(sc->dim, s, randint(0, sc->dim.x * sc->dim.y - s->len - 1), posbuf);

                apple->pos.x = newpos % sc->dim.x;
                apple->pos.y = newpos / sc->dim.x;

                draw_block(sc, apple);

                break;
            case COLL_SELF:
                exit = true;
                break;
            case COLL_WALL:
                exit = true;
                break;
            default:
                shorten_snake(s);
                break;
        }

        draw_snake(sc, s);
    }
    
    free(apple);
    free(posbuf);
    free_snake(s);
    free_screen(sc);
}

int main(int argc, char **argv) {
    // TODO a végén TYPE_GUI legyen!!
    int interface_type = TYPE_GUI;
    if (argc > 1) {
        interface_type = stoi(argv[1], TYPE_CLI);
    }

    play_game(interface_type);

    return 0;
}
