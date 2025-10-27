#include "cli.h"

#include "snake.h"
#include "debugmalloc.h"
#include "econio.h"

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

// Vigyázzat, UTF-8, több bájtos karakterek!
// printf("%s")-sel kiírhatóak!
const char *TP_CH[] = {"═", "║", "╔", "╗", "╚", "╝", "o", "x"};
//const char TP_CH_SIZE[] = {3, 3, 3, 3, 3, 3, 1, 1};
//const char TP_CH_INDEX[] = {0, 3, 6, 9, 12, 15, 18, 19};

void cli_init() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif // _WIN32

    econio_clrscr();
}

void cli_draw_map(Screen const *sc) {
    econio_clrscr();
    econio_gotoxy(0, 0);
    printf("┌");
    for (int i = 1; i <= sc->w; i++) {
        printf("─");
    }
    printf("┐");
    for (int i = 1; i <= sc->h; i++) {
        econio_gotoxy(0, i);
        printf("│");
        econio_gotoxy(sc->w+1, i);
        printf("│");
    }
    econio_gotoxy(0, sc->h + 1);
    printf("└");
    for (int i = 1; i <= sc->w; i++) {
        printf("─");
    }
    printf("┘");
}

// char* helyett char**-gal jobb lenne és printf("%s")-sel?
void cli_draw_block(Block const *b) {
    econio_gotoxy(b->x, b->y);
    /*for (int i = 0; i < TP_CH_SIZE[b->type]; i++) {
        printf("%c", TP_CH[TP_CH_INDEX[b->type] + i]);
    }*/
   printf("%s", TP_CH[b->type]);
    //printf("o");
}

void cli_draw_snake(Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        cli_draw_block(ptr);
    }
}

void cli_exit(Screen const *sc) {
    econio_gotoxy(0, sc->h + 2);
}
