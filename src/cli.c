#include "cli.h"

#include "snake.h"
#include "debugmalloc.h"
#include "econio.h"

#ifdef _WIN32
    #include <windows.h>
#endif // _WIN32

// TODO külön függvény flush-ra

// Vigyázat, UTF-8, több bájtos karakterek!
// printf("%s")-sel kiírhatóak!
char const * const TP_CH[] = { "═", "║", "╔", "╗", "╚", "╝", "o", "x", "O" };
char const * const TP_CH_HEAD[] = { "Ʌ", ">", "V", "<" };
//const char TP_CH_SIZE[] = {3, 3, 3, 3, 3, 3, 1, 1};
//const char TP_CH_INDEX[] = {0, 3, 6, 9, 12, 15, 18, 19};

#ifdef _WIN32
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
#endif

void cli_init() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    ShowConsoleCursor(false);
#endif // _WIN32

    econio_clrscr();
    econio_rawmode();
}

void cli_draw_map(Screen const *sc) {
    econio_clrscr();
    econio_gotoxy(0, 0);
    printf("┌");
    for (int i = 1; i <= sc->dim.x * 2; i++) {
        printf("─");
    }
    printf("┐");
    for (int i = 1; i <= sc->dim.y; i++) {
        econio_gotoxy(0, i);
        printf("│");
        econio_gotoxy((sc->dim.x*2) + 1, i);
        printf("│");
    }
    econio_gotoxy(0, sc->dim.y + 1);
    printf("└");
    for (int i = 1; i <= sc->dim.x * 2; i++) {
        printf("─");
    }
    printf("┘");

    econio_gotoxy(0, sc->dim.y + 2);
    econio_flush();
}

// char* helyett char**-gal jobb lenne és printf("%s")-sel?
// TODO a karaktereknek 2 szélesnek kéne lenniük, hogy vízszintesen is ugyanolyan gyors legyen a kígyó, mint függőlegesen
// minden ptlan x koordinátájú karakterben van kígyó, párosokban a filler vonalak
void cli_draw_block(Block const *b) {
    if (b->dir == DIR_R && b->type != TP_HEAD) {
        econio_gotoxy(b->pos.x * 2 + 1, b->pos.y + 1);
        printf("%s%s", TP_CH[b->type], TP_CH[TP_VSZ]);
    } else if (b->dir == DIR_L && b->type != TP_HEAD) {
        econio_gotoxy(b->pos.x * 2, b->pos.y + 1);
        printf("%s%s", TP_CH[TP_VSZ], TP_CH[b->type]);
    } else {
        econio_gotoxy(b->pos.x * 2 + 1, b->pos.y + 1);
        if (b->type != TP_HEAD) {
            printf("%s", TP_CH[b->type]);
        } else {
            printf("%s", TP_CH_HEAD[b->dir]);
        }
    }
    /*for (int i = 0; i < TP_CH_SIZE[b->type]; i++) {
        printf("%c", TP_CH[TP_CH_INDEX[b->type] + i]);
    }*/
    //printf("o");
}

void cli_draw_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        cli_draw_block(ptr);
    }
    econio_gotoxy(0, sc->dim.y + 2);
    econio_flush();
}

void cli_erase_block(Block const *b) {
    if (b->dir == DIR_R && b->type != TP_HEAD) {
        econio_gotoxy(b->pos.x * 2 + 1, b->pos.y + 1);
        printf("  ");
    } else if (b->dir == DIR_L && b->type != TP_HEAD) {
        econio_gotoxy(b->pos.x * 2, b->pos.y + 1);
        printf("  ");
    } else {
        econio_gotoxy(b->pos.x * 2 + 1, b->pos.y + 1);
        printf(" ");
    }
}

void cli_erase_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        cli_erase_block(ptr);
    }
    econio_gotoxy(0, sc->dim.y + 2);
    econio_flush();
}

// 0: okay, 1: exit
// TODO tisztázni, hogy ez mit csinál
/* Itt kéne, hogy legyen a várakozás és a billentyűzet olvasás, majd az értékkel való visszatérés
 * A main-ben kéne a többi dolgot lekezelni
 * Aztán a main-nek megint meg kéne hívnia a rajzoló függvényeket
 * És kezdődne előröl az egész
 * return: key pressed -1 -> játék vége
 */
int cli_next_frame(Screen const *sc, Snake *s) {
    econio_sleep(s->speed);

    // ha egy frame alatt több billentyű is le lett ütve, akkor az utolsó számítson
    int key = SNAKE_KEY_NONE;
    while (econio_kbhit()) {
        int k2 = econio_getch();
        if (k2 != 0) key = k2;
        // elvileg dobálhat random nullákat a beolvasás
    }

    //int dir = s->head->dir;
    switch (key) {
        case KEY_UP:
            return SNAKE_KEY_UP;
        case KEY_RIGHT:
            return SNAKE_KEY_RIGHT;
        case KEY_DOWN:
            return SNAKE_KEY_DOWN;
        case KEY_LEFT:
            return SNAKE_KEY_LEFT;
        case KEY_ESCAPE:
            return SNAKE_KEY_ESCAPE;
    }

    /*erase_snake(sc, s);
    move_snake(s, dir);
    draw_snake(sc, s);
    econio_flush();*/

    return SNAKE_KEY_NONE;
}

void cli_exit(Screen const *sc) {
    econio_gotoxy(0, sc->dim.y + 2);
    econio_normalmode();

#ifdef _WIN32
    ShowConsoleCursor(true);
#endif
}
