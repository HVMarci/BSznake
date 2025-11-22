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
char const * const TP_CH[] = { "\u2550", "\u2551", "\u2554", "\u2557", "\u255A", "\u255D", "o", "x", "O" };
char const * const TP_CH_HEAD[] = { "\u0245", ">", "V", "<" };
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
#endif // _WIN32

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
    econio_textcolor(COL_WHITE);
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
}

// minden ptlan x koordinátájú karakterben van kígyó, párosokban a filler vonalak
void cli_draw_block(Block const *b) {
    econio_textcolor(b->col.cli_code);
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
}

void cli_flush_screen() {
    econio_flush();
}

void cli_draw_score(Screen const *sc, int score) {
    econio_textcolor(COL_WHITE);
    econio_gotoxy(0, sc->dim.y + 2); // TODO jó?
    printf("Pontszám: %d\n", score);
}

void cli_ask_name(char *name, int maxlen) {
    econio_textcolor(COL_WHITE);
    printf("Milyen néven mentsük el az eredményt? (x - ne mentsünk) ");
    econio_normalmode();
#ifdef _WIN32
    ShowConsoleCursor(true);
#endif // _WIN32
    char format[5+1];
    sprintf(format, " %%%ds", maxlen);
    scanf(format, name);
#ifdef _WIN32
    ShowConsoleCursor(false);
#endif // _WIN32
    econio_rawmode();

    if (name[0] == 'x' && name[1] == '\0') name[0] = '\0';
}

void cli_draw_top5(Leaderboard const *lb) {
    econio_textcolor(COL_WHITE);
    Result *mozgo = lb->results;
    int i = 0;
    while (mozgo != NULL && i < 5) {
        printf("%d. %s: %d pont\n", i+1, mozgo->name, mozgo->score);
        i++;
        mozgo = mozgo->next;
    }
}

bool cli_ask_new_game() {
    econio_textcolor(COL_WHITE);
    printf("Szeretnél még egyet játszani? (I/N) ");
    char c = 'n'; // ha nem lenne input
    econio_normalmode();
#ifdef _WIN32
    ShowConsoleCursor(true);
#endif // _WIN32
    scanf(" %c", &c);
#ifdef _WIN32
    ShowConsoleCursor(false);
#endif // _WIN32
    econio_rawmode();
    return c == 'I' || c == 'i';
}

// 0: okay, 1: exit
// TODO tisztázni, hogy ez mit csinál
/* Itt kéne, hogy legyen a várakozás és a billentyűzet olvasás, majd az értékkel való visszatérés
 * A main-ben kéne a többi dolgot lekezelni
 * Aztán a main-nek megint meg kéne hívnia a rajzoló függvényeket
 * És kezdődne előröl az egész
 * return: key pressed -1 -> játék vége
 */
SNAKE_KEY cli_next_frame(double wait_time) {
    econio_sleep(wait_time);

    // ha egy frame alatt több billentyű is le lett ütve, akkor az utolsó számítson
    int key = 0;
    while (econio_kbhit()) {
        int k2 = econio_getch();
        if (k2 != 0) key = k2;
        // elvileg dobálhat random nullákat a beolvasás
    }

    //DIR dir = s->head->dir;
    switch (key) {
        case KEY_ESCAPE:
            return SNAKE_KEY_ESCAPE;
        case KEY_UP:
            return SNAKE_KEY_UP;
        case KEY_RIGHT:
            return SNAKE_KEY_RIGHT;
        case KEY_DOWN:
            return SNAKE_KEY_DOWN;
        case KEY_LEFT:
            return SNAKE_KEY_LEFT;
        case 'w':
            return SNAKE_KEY_W;
        case 'a':
            return SNAKE_KEY_A;
        case 's':
            return SNAKE_KEY_S;
        case 'd':
            return SNAKE_KEY_D;
        case 'i':
            return SNAKE_KEY_I;
        case 'j':
            return SNAKE_KEY_J;
        case 'k':
            return SNAKE_KEY_K;
        case 'l':
            return SNAKE_KEY_L;
        case 't':
            return SNAKE_KEY_T;
        case 'f':
            return SNAKE_KEY_F;
        case 'g':
            return SNAKE_KEY_G;
        case 'h':
            return SNAKE_KEY_H;
    }

    /*erase_snake(sc, s);
    move_snake(s, dir);
    draw_snake(sc, s);
    econio_flush();*/

    return SNAKE_KEY_NONE;
}

void cli_exit() {
    econio_textcolor(COL_WHITE);
    econio_clrscr();
    econio_gotoxy(0, 0);
    econio_normalmode();
#ifdef _WIN32
    ShowConsoleCursor(true);
#endif // _WIN32
}
