#include "cli.h"

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

int cli_next_frame(double wait_time, SNAKE_KEY *keybuf, int bufsize) {
    econio_sleep(wait_time);

    int eddig = 0;
    while (econio_kbhit()) {
        int k2 = econio_getch();
        // elvileg dobálhat random nullákat a beolvasás
        if (k2 != 0) {
            SNAKE_KEY key = SNAKE_KEY_NONE;
            switch (k2) {
                case KEY_ESCAPE:
                    return -1;
                case KEY_UP:
                    key = SNAKE_KEY_UP;
                    break;
                case KEY_RIGHT:
                    key = SNAKE_KEY_RIGHT;
                    break;
                case KEY_DOWN:
                    key = SNAKE_KEY_DOWN;
                    break;
                case KEY_LEFT:
                    key = SNAKE_KEY_LEFT;
                    break;
                case 'w':
                    key = SNAKE_KEY_W;
                    break;
                case 'a':
                    key = SNAKE_KEY_A;
                    break;
                case 's':
                    key = SNAKE_KEY_S;
                    break;
                case 'd':
                    key = SNAKE_KEY_D;
                    break;
                case 'i':
                    key = SNAKE_KEY_I;
                    break;
                case 'j':
                    key = SNAKE_KEY_J;
                    break;
                case 'k':
                    key = SNAKE_KEY_K;
                    break;
                case 'l':
                    key = SNAKE_KEY_L;
                    break;
                case 't':
                    key = SNAKE_KEY_T;
                    break;
                case 'f':
                    key = SNAKE_KEY_F;
                    break;
                case 'g':
                    key = SNAKE_KEY_G;
                    break;
                case 'h':
                    key = SNAKE_KEY_H;
                    break;
            }
            if (key != SNAKE_KEY_NONE && eddig < bufsize) {
                keybuf[eddig++] = key;
            }
        }
    }

    return eddig;
}

int cli_draw_bsz_feladat(BSzFeladat feladat) {
    switch (feladat.type) {
        case LNKO:
            printf("Határozd meg %d és %d legnagyobb osztóját! ", feladat.a, feladat.b);
            break;
        case KONGRUENCIA:
            printf("Határozd meg a következő kongruencia legkisebb pozitív megoldását:\n%dx === %d (mod %d)\n", feladat.a, feladat.b, feladat.c);
            printf("(Ha nincs megoldás, legyen -1 a válasz!) ");
            break;
        case PRIME:
            printf("Prím a következő szám: %d?\n(Igen - 1, Nem - 0) ", feladat.a);
            break;
        case DETERMINANS:
            printf("Határozd meg a következő mátrix determinánsát!\n");
            for (int i = 0; i < feladat.a; i++) {
                for (int j = 0; j < feladat.a; j++) {
                    printf("%g ", feladat.mx[i][j]);
                }
                printf("\n");
            }
            break;
    }
    
#ifdef _WIN32
    ShowConsoleCursor(true);
#endif // _WIN32
    int ans;
    scanf("%d", &ans);
#ifdef _WIN32
    ShowConsoleCursor(false);
#endif // _WIN32
    return ans;
}

void cli_draw_bsz_result(bool siker, int jo) {
    if (siker) {
        printf("Jó megoldás!\n");
    } else {
        printf("Rossz megoldás, a helyes: %d\n", jo);
    }
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
