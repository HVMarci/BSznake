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
    econio_rawmode();
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

    econio_flush();
}

// char* helyett char**-gal jobb lenne és printf("%s")-sel?
// TODO a karaktereknek 2 szélesnek kéne lenniük, hogy vízszintesen is ugyanolyan gyors legyen a kígyó, mint függőlegesen
void cli_draw_block(Block const *b) {
    econio_gotoxy(b->x, b->y);
    /*for (int i = 0; i < TP_CH_SIZE[b->type]; i++) {
        printf("%c", TP_CH[TP_CH_INDEX[b->type] + i]);
    }*/
    printf("%s", TP_CH[b->type]);
    //printf("o");
}

void cli_draw_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        cli_draw_block(ptr);
    }
    econio_gotoxy(0, sc->h + 1);
}

void cli_erase_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        econio_gotoxy(ptr->x, ptr->y);
        printf(" ");
    }
    econio_gotoxy(0, sc->h + 1);
}

// 0: okay, 1: exit
// TODO tisztázni, hogy ez mit csinál
/* Itt kéne, hogy legyen a várakozás és a billentyűzet olvasás, majd az értékkel való visszatérés
 * A main-ben kéne a többi dolgot lekezelni
 * Aztán a main-nek megint meg kéne hívnia a rajzoló függvényeket
 * És kezdődne előröl az egész
 */
int cli_play_game(Screen const *sc, Snake *s) {
    econio_sleep(s->speed);

    // ha egy frame alatt több billentyű is le lett ütve, akkor az utolsó számítson
    int key = 0;
    while (econio_kbhit()) {
        int k2 = econio_getch();
        if (k2 != 0) key = k2;
        // elvileg dobálhat random nullákat a beolvasás
    }

    int dir = s->head->dir;
    switch (key) {
        case KEY_UP:
            if (s->head->dir != DIR_D) 
                dir = DIR_U;
            break;
        case KEY_RIGHT:
            if (s->head->dir != DIR_L)
                dir = DIR_R;
            break;
        case KEY_DOWN:
            if (s->head->dir != DIR_U)
                dir = DIR_D;
            break;
        case KEY_LEFT:
            if (s->head->dir != DIR_R)
                dir = DIR_L;
            break;
        case KEY_ESCAPE:
            return 1;
    }

    cli_erase_snake(sc, s);
    move_snake(s, dir);
    draw_snake(sc, s);
    econio_flush();

    return 0;
}

void cli_exit(Screen const *sc) {
    econio_gotoxy(0, sc->h + 2);
    econio_normalmode();
}
