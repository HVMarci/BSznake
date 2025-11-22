#include "gui.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_TTF.h>

#include "debugmalloc.h"

SDL_Color const feher = { 0xFF, 0xFF, 0xFF, 0xFF };
SDL_Color const szurkes = { 0x05, 0x05, 0x05, 0xDD };
SDL_Color const kek = { 0x00, 0x00, 0xFF, 0xFF };
SDL_Color const fekete = { 0x00, 0x00, 0x00, 0xFF };

void gui_init(Screen *sc) {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("BSznake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sc->block_size * (sc->dim.x + 2), sc->block_size * (sc->dim.y + 2), 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    if (TTF_Init() < 0) {
        SDL_Log("Nem toltheto be a TTF engine: %s", SDL_GetError());
        exit(1);
    }

    sc->font = TTF_OpenFont("arial.ttf", 32);
    if (!sc->font) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    sc->window = window;
    sc->renderer = renderer;
}

void gui_draw_map(Screen const *sc) {
    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = (sc->dim.x + 2) * sc->block_size;
    rect.h = (sc->dim.y + 2) * sc->block_size;

    SDL_SetRenderDrawColor(sc->renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(sc->renderer, &rect);

    rect.x = rect.y = sc->block_size;
    rect.w = (sc->dim.x) * sc->block_size;
    rect.h = (sc->dim.y) * sc->block_size;
    SDL_SetRenderDrawColor(sc->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(sc->renderer, &rect);
}

void gui_draw_block(Screen const *sc, Block const *b) {
    Color col;
    if (b->type == TP_APPLE) {
        col.r = 0xFF;
        col.g = col.b = 0x00;
    } else {
        col.r = 0x53;
        col.g = 0x34;
        col.b = 0x9F;
    }
    filledCircleRGBA(sc->renderer, (b->pos.x + 1) * sc->block_size + sc->block_size/2, (b->pos.y + 1) * sc->block_size + sc->block_size/2, sc->block_size/2 - 1, col.r, col.g, col.b, 0xFF); // kell az R-1, különben túllóg a mezőn
}

void gui_draw_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        gui_draw_block(sc, ptr);
    }
}

void gui_erase_block(Screen const *sc, Block const *b) {
    filledCircleRGBA(sc->renderer, (b->pos.x + 1) * sc->block_size + sc->block_size/2, (b->pos.y + 1) * sc->block_size + sc->block_size/2, sc->block_size/2 - 1, 0x00, 0x00, 0x00, 0xFF); // kell az R-1, különben túllóg a mezőn
}

void gui_erase_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        gui_erase_block(sc, ptr);
    }
}

void gui_flush_screen(Screen const *sc) {
    SDL_RenderPresent(sc->renderer);
}

void write_text(Screen const *sc, SDL_Color background_color, char *szoveg, int y) {
    SDL_Surface *felirat = TTF_RenderUTF8_Shaded(sc->font, szoveg, feher, background_color);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(sc->renderer, felirat);
    SDL_Rect hova = { ((sc->dim.x+2)*sc->block_size - felirat->w)/2, y, felirat->w, felirat->h };
    SDL_RenderCopy(sc->renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

void gui_draw_score(Screen const *sc, int score) {
    static char szoveg[20+1];
    sprintf_s(szoveg, 21, "Pontszám: %d", score);

    write_text(sc, szurkes, szoveg, 3*32);
}

/* Forrás: InfoC
 * Beolvas egy szoveget a billentyuzetrol.
 * A rajzolashoz hasznalt font es a megjelenito az utolso parameterek.
 * Az elso a tomb, ahova a beolvasott szoveg kerul.
 * A masodik a maximális hossz, ami beolvasható.
 * A visszateresi erteke logikai igaz, ha sikerult a beolvasas.
 */
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char *textandcomposition = (char *) malloc((hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1) * sizeof(char));
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;
 
    dest[0] = '\0';
 
    bool enter = false;
    bool kilep = false;
 
    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);
 
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;
 
            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) <= hossz) {
                    strcat(dest, event.text.text);
                }
 
                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;
 
            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;
 
            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }
    
    free(textandcomposition);
    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

void gui_ask_name(Screen const *sc, char *name, int maxlen) {
    char *szoveg = "Milyen néven mentsük el az eredményt?";

    write_text(sc, fekete, szoveg, 4*35);

    SDL_Rect teglalap = { (sc->dim.x+2)*sc->block_size / 6, 5*35, (sc->dim.x+2)*sc->block_size * 2 / 3, 32 };
    SDL_Color hatter = { 0x11, 0x11, 0x11, 0xDD };
    input_text(name, maxlen, teglalap, hatter, feher, sc->font, sc->renderer);
}

void gui_draw_top5(Screen const *sc, Leaderboard const *lb) {
    static char szoveg[100+1];
    Result *mozgo = lb->results;
    int i = 0;
    while (mozgo != NULL && i < 5) {
        sprintf(szoveg, "%d. %s: %d pont", i+1, mozgo->name, mozgo->score);
        write_text(sc, kek, szoveg, (i + 2) * 35);
        i++;
        mozgo = mozgo->next;
    }
}

bool gui_ask_new_game(Screen const *sc) {
    char *szoveg1 = "Szeretnél még egyet játszani?", *szoveg2 = "Enter: igen, Escape: nem"; // global
    
    write_text(sc, szurkes, szoveg2, 9*35);
    write_text(sc, szurkes, szoveg1, 8*35);
    SDL_RenderPresent(sc->renderer);

    bool done = false, ujat = false;
    while (!done) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        ujat = true, done = true;
                        break;
                    case SDLK_ESCAPE:
                        ujat = false, done = true;
                        break;
                }
                break;
            case SDL_QUIT:
                ujat = false, done = true;
                break;
        }
    }

    return ujat;
}

Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    if (param != NULL) {}
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return 0&ms; // ne legyen automatikusan újraindítva + csalás, hogy ne legyen -Werror=unused-parameter
}

SNAKE_KEY gui_next_frame(Snake *s) {
    bool done = false;
    SDL_AddTimer(s->speed * 1000, idozit, NULL);

    SNAKE_KEY key = SNAKE_KEY_NONE;
    while (!done) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_USEREVENT:
                done = true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_UP: key = SNAKE_KEY_UP; break;
                    case SDLK_RIGHT: key = SNAKE_KEY_RIGHT; break;
                    case SDLK_DOWN: key = SNAKE_KEY_DOWN; break;
                    case SDLK_LEFT: key = SNAKE_KEY_LEFT; break;
                    case SDLK_ESCAPE: key = SNAKE_KEY_ESCAPE; break;
                }
                break;
            case SDL_QUIT:
                key = SNAKE_KEY_ESCAPE;
                done = true;
                break;
        }
    }

    return key;
}

void gui_exit(Screen *sc) {
    SDL_DestroyRenderer(sc->renderer);
    sc->renderer = NULL;

    SDL_DestroyWindow(sc->window);
    sc->window = NULL;

    TTF_CloseFont(sc->font);
    sc->font = NULL;

    SDL_Quit();
}
//void gui_set_game_loop(void (*f)(void)) {}
