#include "interface.h"

#include "cli.h"
#include "gui.h"

#include "debugmalloc.h"

Screen *init_screen(int w, int h, INTERFACE_TYPE type, int block_size) {
    Screen *sc = (Screen *) malloc(sizeof(Screen));

    if (sc == NULL) return NULL;

    sc->dim.x = w;
    sc->dim.y = h;
    sc->type = type;
    sc->block_size = block_size;
    sc->window = NULL;
    sc->renderer = NULL;

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

void flush_screen(Screen const *sc) {
    if (sc->type == TYPE_CLI) {
        cli_flush_screen();
    } else {
        gui_flush_screen(sc);
    }
}

void draw_score(Screen const *sc, int score) {
    if (sc->type == TYPE_CLI) {
        cli_draw_score(sc, score);
    } else {
        gui_draw_score(sc, score);
    }
}

void ask_name(Screen const *sc, char *name, int maxlen) {
    if (maxlen < 0) maxlen = 0;
    if (maxlen > 50) maxlen = 50;
    
    if (sc->type == TYPE_CLI) {
        cli_ask_name(name, maxlen);
    } else {
        gui_ask_name(sc, name, maxlen);
    }
}

void draw_top5(Screen const *sc, Leaderboard const *lb) {
    if (sc->type == TYPE_CLI) {
        cli_draw_top5(lb);
    } else {
        gui_draw_top5(sc, lb);
    }
}

bool ask_new_game(Screen const *sc) {
    if (sc->type == TYPE_CLI) {
        return cli_ask_new_game();
    } else {
        return gui_ask_new_game(sc);
    }
}

int next_frame(Screen const *sc, double wait_time, SNAKE_KEY *keybuf, int bufsize) {
    if (sc->type == TYPE_CLI) {
        return cli_next_frame(wait_time, keybuf, bufsize);
    } else {
        return gui_next_frame(wait_time, keybuf, bufsize);
    }
}

void free_screen(Screen *sc) {
    if (sc->type == TYPE_CLI) {
        cli_exit();
    } else {
        gui_exit(sc);
    }

    free(sc);
}
