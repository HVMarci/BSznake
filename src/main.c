#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "econio.h"
#include "snake.h"
#include "interface.h"
#include "helper.h"

#include "debugmalloc.h"

void game_loop() {
    printf("meghivva");
}

#include "cli.h"
void start_game(int interface_type) {
    // TODO query window size - windows.h-ból?
    Screen *sc = init_screen(70, 20, interface_type, game_loop);
    Snake *s = new_snake(5, 10, 3, .1);

    draw_map(sc);
    draw_snake(sc, s);

    bool exit = false;

    while (!exit) {
        //exit = next_frame();
        exit = cli_play_game(sc, s);
    }
    
    free_snake(s);
    free_screen(sc);
}

int main(int argc, char **argv) {
    // TODO a végén TYPE_GUI legyen!!
    int interface_type = TYPE_CLI;
    if (argc > 1) {
        interface_type = stoi(argv[1], TYPE_CLI);
    }

    start_game(interface_type);

    /*draw_map(sc);
    draw_snake(sc, s);

    move_snake(s, DIR_R);
    draw_snake(sc, s);
    move_snake(s, DIR_R);
    draw_snake(sc, s);
    move_snake(s, DIR_R);
    draw_snake(sc, s);
    move_snake(s, DIR_R);
    draw_snake(sc, s);
    move_snake(s, DIR_U);
    draw_snake(sc, s);
    move_snake(s, DIR_U);
    draw_snake(sc, s);
    move_snake(s, DIR_R);
    draw_snake(sc, s);
    move_snake(s, DIR_D);
    draw_snake(sc, s);
    move_snake(s, DIR_D);
    draw_snake(sc, s);
    move_snake(s, DIR_D);
    draw_snake(sc, s);
    move_snake(s, DIR_L);
    draw_snake(sc, s);
    move_snake(s, DIR_L);
    draw_snake(sc, s);
    move_snake(s, DIR_L);
    draw_snake(sc, s);
    move_snake(s, DIR_D);
    draw_snake(sc, s);
    move_snake(s, DIR_L);
    draw_snake(sc, s);
    move_snake(s, DIR_U);
    draw_snake(sc, s);*/
    
    return 0;
}
