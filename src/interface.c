#include "interface.h"

#include "cli.h"
#include "gui.h"

#include "debugmalloc.h"

Screen *init_screen(int w, int h, int type, int block_size, void (*game_loop)(void)) {
    Screen *sc = malloc(sizeof(Screen));

    if (sc == NULL) return NULL;

    sc->w = w;
    sc->h = h;
    sc->type = type;
    sc->block_size = block_size;
    sc->window = NULL;
    sc->renderer = NULL;
    sc->game_loop = game_loop;

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
        gui_draw_block(sc, b);
    }
}

void draw_snake(Screen const *sc, Snake const *s) {
    if (sc->type == TYPE_CLI) {
        cli_draw_snake(sc, s);
    } else {
        gui_draw_snake(sc, s);
    }
}

void erase_block(Screen const *sc, Block const *b) {
    if (sc->type == TYPE_CLI) {
        cli_erase_block(b);
    } else {
        gui_erase_block(sc, b);
    }
}

void erase_snake(Screen const *sc, Snake const *s) {
    if (sc->type == TYPE_CLI) {
        cli_erase_snake(sc, s);
    } else {
        gui_erase_snake(sc, s);
    }
}

int next_frame(Screen const *sc, Snake *s) {
if (sc->type == TYPE_CLI) {
        return cli_next_frame(sc, s);
    } else {
        return gui_next_frame(sc, s);
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
