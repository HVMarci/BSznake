#ifndef _CLI_H
#define _CLI_H

#include "snake.h"
#include "interface.h"

void cli_init();
void cli_draw_map(Screen const *sc);
void cli_draw_block(Block const *b);
void cli_draw_snake(Screen const *sc, Snake const *s);
void cli_erase_snake(Screen const *sc, Snake const *s);
int cli_next_frame(Screen const *sc, Snake *s);
void cli_exit(Screen const *sc);
//void cli_set_game_loop(void (*f)(void));

#endif
