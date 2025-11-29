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

typedef struct InitData {
    int x, y;
    DIR dir;
    Color col;
    SNAKE_KEY keys[4]; // urdl order
} InitData;

// TODO gombok is Snake-be!
// TODO WASD irány jobb lenne! (WDSA helyett)
InitData const init_snake_data[4] = {
    {10, 8, DIR_R, {50, 168, 82, COL_GREEN}, {SNAKE_KEY_UP, SNAKE_KEY_RIGHT, SNAKE_KEY_DOWN, SNAKE_KEY_LEFT}},
    {20, 13, DIR_U, {43, 82, 173, COL_CYAN}, {SNAKE_KEY_W, SNAKE_KEY_D, SNAKE_KEY_S, SNAKE_KEY_A}},
    {26, 6, DIR_D, {196, 55, 76, COL_RED}, {SNAKE_KEY_I, SNAKE_KEY_L, SNAKE_KEY_K, SNAKE_KEY_J}},
    {29, 12, DIR_L, {184, 196, 55, COL_YELLOW}, {SNAKE_KEY_T, SNAKE_KEY_H, SNAKE_KEY_G, SNAKE_KEY_F}}
}; // hehe, ugyanazokkal a gombokkal kell mindet irányítani

int play_game(Screen *sc, int player_count, int starting_score) {
    // TODO legyen lassabb!
    Snake **snakes = (Snake **) malloc(player_count * sizeof(Snake *));
    double game_speed = .1; // nem lehet Snake-ben, mert minden játékos között közösnek kell lennie
    int alive_count = player_count; // hányan vannak még életben

    int starting_len = starting_score / player_count;

    int sumlen = player_count * (starting_len + 5); // A kígyók hossza összesen
    for (int i = 0; i < player_count; i++) {
        snakes[i] = new_snake(starting_len + 5, init_snake_data[i].x, init_snake_data[i].y, init_snake_data[i].dir, init_snake_data[i].col);
    }
    int *posbuf = (int *) malloc((sc->dim.x * sc->dim.y + 1) * sizeof(int));
    const int KEYBUF_SIZE = 32;
    SNAKE_KEY *keybuf = (SNAKE_KEY *) malloc(KEYBUF_SIZE * sizeof(SNAKE_KEY));

    int score = starting_len * player_count; // maradékok levágása

    Block *apple = (Block *) malloc(sizeof(Block));
    apple->pos.x = 30;
    apple->pos.y = 8;
    apple->type = TP_APPLE;
    apple->col = (Color) {255, 0, 0, COL_WHITE};
    // a többi apple->... értéket nem használom
    
    draw_map(sc);
    for (int i = 0; i < player_count; i++) {
        draw_snake(sc, snakes[i]);
    }
    draw_block(sc, apple);
    draw_score(sc, score);
    flush_screen(sc);

    bool end_game = false, exit = false;

    while (!end_game) {
        int rc = next_frame(sc, game_speed, keybuf, KEYBUF_SIZE);
        if (rc == -1) {
            end_game = true;
            exit = true;
            break;
        }

        for (int i = 0; i < player_count; i++) {
            Snake *s = snakes[i];
            if (!s->alive) continue;

            DIR dir = s->head->dir;
            for (int k = 0; k < rc; k++) {
                SNAKE_KEY key = keybuf[k];
                for (int j = 0; j < 4; j++) {
                    if (key == init_snake_data[i].keys[j]) dir = j;
                }
            }

            if (dir == (s->head->dir + 2u) % 4) dir = s->head->dir;
        
            erase_snake(sc, s);
            move_snake(s, dir);

            if (s->head->pos.x == apple->pos.x && s->head->pos.y == apple->pos.y) {
                // COLL_APPLE
                s->len++;
                sumlen++;

                erase_block(sc, apple);
                apple->pos.x = apple->pos.y = -1;
                
                // minél többen élnek még, annál több pontot ér az alma
                score += alive_count;
                // legyen a játék gyorsabb
                game_speed *= .98;
            } else {
                shorten_snake(s);
            }
        }

        draw_score(sc, score);
        fill_posbuf(sc->dim, snakes, player_count, posbuf);
        for (int i = 0; i < player_count; i++) {
            Snake *s = snakes[i];
            if (!s->alive) continue;

            switch (check_snake(sc->dim, s, posbuf)) {
                case COLL_SNAKE:
                case COLL_WALL:
                    s->alive = false;
                    alive_count--;
                    if (alive_count == 0) end_game = true;
                    break;
                default:
                    break;
            }

            if (s->alive) draw_snake(sc, s); // ha most ütközött volna, akkor már ne rajzoljuk ki újra!
        }

        if (apple->pos.x == -1) {
            // alma áthelyezése
            int newpos = exclude_snakes(randint(0, (sc->dim.x * sc->dim.y - sumlen - 1)/3), posbuf);

            apple->pos.x = newpos % sc->dim.x;
            apple->pos.y = newpos / sc->dim.x;
        }
        draw_block(sc, apple); // netán felülírná a pontszámláló
    
        flush_screen(sc);
    }
    
    free(apple);
    free(posbuf);
    free(keybuf);
    for (int i = 0; i < player_count; i++) {
        free_snake(snakes[i]);
    }
    free(snakes);

    return exit ? -1 : score;
}

void run_app(INTERFACE_TYPE interface_type, int player_count) {
    srand(time(NULL));

    // TODO query window size - windows.h-ból?
    Screen *sc = init_screen(35, 20, interface_type, 20);
    char results_filename[16];
    sprintf(results_filename, "results%dp.txt", player_count);

    bool exit = false, firstgame = true;
    int score;
    while (!exit) {
        int starting_score = 0;

        if (!firstgame) {
            BSzFeladat feladat = feladat_generalas();

            draw_map(sc); // leürítjük a képernyőt a pályán kívül
            int answer = draw_bsz_feladat(sc, feladat);
            int good = get_answer(feladat);

            draw_bsz_result(sc, answer == good, good);
            flush_screen(sc);
            if (next_frame(sc, 2.5, NULL, 0) == -1) {
                exit = true;
            }

            if (answer == good) {
                starting_score = score / 2;
            }

            feladat_free(feladat);
        } else {
            firstgame = false;
        }

        if (exit) break;

        score = play_game(sc, player_count, starting_score);

        if (score == -1) break;

        draw_score(sc, score);
        char name[30+1];
        ask_name(sc, name, 30);
        Leaderboard *lb = open_leaderboard(results_filename);
        if (lb != NULL) {
            if (name[0] != '\0') {
                // üres nevet ne mentsünk
                add_score(lb, name, score);
                save_leaderboard(lb);
            }
            draw_top5(sc, lb);
            close_leaderboard(lb);
        }

        exit = !ask_new_game(sc);
    }

    free_screen(sc);
}

int main(int argc, char **argv) {
    INTERFACE_TYPE interface_type = TYPE_GUI, player_count = 1;
    if (argc > 1 && argv[1][0] == '0' && argv[1][1] == '\0') {
        interface_type = TYPE_CLI;
    }
    if (argc > 2) {
        player_count = stoi(argv[2], 2);
    }
    if (player_count < 1) player_count = 1;
    if (player_count > 4) player_count = 4;

    run_app(interface_type, player_count);

    return 0;
}
