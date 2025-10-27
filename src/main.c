#include <stdio.h>
#include <stdlib.h>
#include "econio.h"
#include "snake.h"
#include "interface.h"
#include "helper.h"

#include "debugmalloc.h"

int main(int argc, char **argv) {
    // TODO a végén TYPE_GUI legyen!!
    int interface_type = TYPE_CLI;
    if (argc > 1) {
        interface_type = stoi(argv[1], TYPE_CLI);
    }

    // TODO query window size - windows.h-ból?
    Screen *sc = init_screen(70, 20, interface_type);
    Snake *s = new_snake(5, 10, 3);

    draw_map(sc);
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
    draw_snake(sc, s);
    free_snake(s);


    free_screen(sc);
    return 0;
}
