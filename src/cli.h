#ifndef _CLI_H
#define _CLI_H

#include "snake.h"
#include "interface.h"

void cli_init();
void cli_draw_map(Screen const *sc);
void cli_draw_block(Block const *b);
void cli_draw_snake(Snake const *s);
void cli_exit(Screen const *sc);

#endif
