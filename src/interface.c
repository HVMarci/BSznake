#include "interface.h"

#include "cli.h"
#include "gui.h"

#include "debugmalloc.h"

Screen *init_screen(int w, int h, int type) {
    Screen *sc = malloc(sizeof(Screen));

    if (sc == NULL) return NULL;

    sc->w = w;
    sc->h = h;
    sc->type = type;

    if (type == TYPE_CLI) {
        cli_init();
    } else {
        gui_init(sc);
    }

    return sc;
}

void draw_map(Screen const *sc) {
    if (sc->type == TYPE_CLI) {
        cli_draw_map(sc);
    } else {
        gui_draw_map(sc);
    }
}

void draw_block(Screen const *sc, Block const *b) {
    if (sc->type == TYPE_CLI) {
        cli_draw_block(b);
    } else {
        gui_draw_block(b);
    }
}

void draw_snake(Screen const *sc, Snake const *s) {
    if (sc->type == TYPE_CLI) {
        cli_draw_snake(s);
    } else {
        gui_draw_snake(s);
    }
}

void free_screen(Screen *sc) {
    if (sc->type == TYPE_CLI) {
        cli_exit(sc);
    } else {
        gui_exit(sc);
    }

    free(sc);
}
