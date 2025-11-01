#ifndef _GUI_H
#define _GUI_H

#include "snake.h"
#include "interface.h"

void gui_init(Screen *sc);
void gui_draw_map(Screen const *sc);
void gui_draw_block(Screen const *sc, Block const *b);
void gui_draw_snake(Screen const *sc, Snake const *s);
void gui_erase_snake(Screen const *sc, Snake const *s);
int gui_next_frame(Screen const *sc, Snake *s);
void gui_exit(Screen const *sc);
//void gui_set_game_loop(void (*f)(void));

#endif
