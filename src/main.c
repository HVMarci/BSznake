#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "econio.h"
#include "snake.h"
#include "interface.h"
#include "helper.h"
#include "leaderboard.h"
#include "bsz.h"

#include "debugmalloc.h"

int play_game(Screen *sc, int starting_score) {
    // TODO legyen lassabb!
    Snake *s = new_snake(starting_score + 5, 10, 8, .1);
    int *posbuf = malloc(sc->dim.x * sc->dim.y * sizeof(int));
    Block *apple = malloc(sizeof(Block));
    apple->pos.x = 30;
    apple->pos.y = 8;
    apple->type = TP_APPLE;
    // a többi apple->... értéket nem használom
    
    draw_map(sc);
    draw_snake(sc, s);
    draw_block(sc, apple);
    flush_screen(sc);

    bool end_game = false, exit = false;

    while (!end_game) {
        //end_game = next_frame();
        SNAKE_KEY key = next_frame(sc, s);
        DIR dir = s->head->dir;
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
                end_game = true;
                exit = true;
                break;
            default:
                break;
        }
    
        erase_snake(sc, s);
        move_snake(s, dir);

        switch (check_snake(sc->dim, s, apple)) {
            case COLL_APPLE:
                s->len++;
                // replace apple
                erase_block(sc, apple);

                int newpos = exclude_snake(sc->dim, s, randint(0, sc->dim.x * sc->dim.y - s->len - 1), posbuf);

                apple->pos.x = newpos % sc->dim.x;
                apple->pos.y = newpos / sc->dim.x;

                draw_block(sc, apple);

                // legyen a kígyó gyorsabb
                s->speed *= .98;

                break;
            case COLL_SELF:
                end_game = true;
                break;
            case COLL_WALL:
                end_game = true;
                break;
            default:
                shorten_snake(s);
                break;
        }

        draw_snake(sc, s);
        flush_screen(sc);
    }
    
    int score = s->len - 5;

    free(apple);
    free(posbuf);
    free_snake(s);

    return exit ? -1 : score;
}

void run_app(int interface_type) {
    srand(time(NULL));

    // TODO query window size - windows.h-ból?
    Screen *sc = init_screen(35, 20, interface_type, 20);

    bool exit = false;
    while (!exit) {
        int score = play_game(sc, 0);

        if (score == -1) break;

        draw_score(sc, score);
        char name[30+1];
        ask_name(sc, name, 30);
        Leaderboard *lb = open_leaderboard("results.txt");
        if (lb != NULL) {
            if (name[0] != '\0') {
                // üres nevet ne mentsünk (GUI esetén történhet ilyen)
                add_score(lb, name, score);
                save_leaderboard(lb);
            }
            draw_top5(sc, lb);
            close_leaderboard(lb);
        }

        // BUG néha újraindul kérdés nélkül konzolban, ha nekimegy az x-nek (farkának) - szerintem már megjavult, input probléma lehetett
        exit = !ask_new_game(sc);
    }

    free_screen(sc);
}

int main(int argc, char **argv) {
    /*printf("%d\n", (2324 % 232));
    printf("%d\n", kongruencia(57, 99, 273));
    return 0;*/

    int interface_type = TYPE_GUI;
    if (argc > 1) {
        interface_type = stoi(argv[1], TYPE_CLI);
    }

    run_app(interface_type);

    return 0;
}
