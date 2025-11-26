/** @file cli.h
 * @brief A konzolos megjelenítésért felelős kódok.
 * 
 * Az `interface.h`-ban dokumentált függvények megvalósítása CLI-re. Innen nem érdemes őket hívogatni!
 */

#ifndef _CLI_H
#define _CLI_H

#include "snake.h"
#include "interface.h"
#include "bsz.h"

void cli_init();
void cli_draw_map(Screen const *sc);
void cli_draw_block(Block const *b);
void cli_draw_snake(Screen const *sc, Snake const *s);
void cli_erase_block(Block const *b);
void cli_erase_snake(Screen const *sc, Snake const *s);
void cli_flush_screen();
void cli_draw_score(Screen const *sc, int score);
void cli_ask_name(char *name, int maxlen);
void cli_draw_top5(Leaderboard const *lb);
bool cli_ask_new_game();
int cli_next_frame(double wait_time, SNAKE_KEY *keybuf, int bufsize);
int cli_draw_bsz_feladat(BSzFeladat feladat);
void cli_exit();

#endif // _CLI_H
