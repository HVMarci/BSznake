/** @file gui.h
 * @brief A grafikus megjelenítésért felelős kódok.
 * 
 * Az `interface.h`-ban dokumentált függvények megvalósítása GUI-ra. Innen nem érdemes őket hívogatni!
 */

#ifndef _GUI_H
#define _GUI_H

#include "snake.h"
#include "interface.h"

void gui_init(Screen *sc);
void gui_draw_map(Screen const *sc);
void gui_draw_block(Screen const *sc, Block const *b);
void gui_draw_snake(Screen const *sc, Snake const *s);
void gui_erase_block(Screen const *sc, Block const *b);
void gui_erase_snake(Screen const *sc, Snake const *s);
void gui_flush_screen(Screen const *sc);
void gui_draw_score(Screen const *sc, int score);
void gui_ask_name(Screen const *sc, char *name, int maxlen);
void gui_draw_top5(Screen const *sc, Leaderboard const *lb);
bool gui_ask_new_game(Screen const *sc);
SNAKE_KEY gui_next_frame(Snake *s);
void gui_exit(Screen *sc);

#endif
