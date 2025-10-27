#ifndef _GUI_H
#define _GUI_H

#include "snake.h"
#include "interface.h"

void gui_init(Screen const *sc);
void gui_draw_map(Screen const *sc);
void gui_draw_block(Block const *b);
void gui_draw_snake(Snake const *s);
void gui_exit(Screen const *sc);

#endif
