#include "gui.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_TTF.h>

#include "debugmalloc.h"

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

    SDL_RenderPresent(sc->renderer);
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
    SDL_RenderPresent(sc->renderer);
}

void gui_erase_block(Screen const *sc, Block const *b) {
    filledCircleRGBA(sc->renderer, (b->pos.x + 1) * sc->block_size + sc->block_size/2, (b->pos.y + 1) * sc->block_size + sc->block_size/2, sc->block_size/2 - 1, 0x00, 0x00, 0x00, 0xFF); // kell az R-1, különben túllóg a mezőn
}

void gui_erase_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        gui_erase_block(sc, ptr);
    }
    SDL_RenderPresent(sc->renderer);
}

void write_text(Screen const *sc, char *szoveg, int y) {
    SDL_Color feher = { 255, 255, 255, 255 }, fekete = { 0, 0, 0, 255 };
    SDL_Surface *felirat = TTF_RenderUTF8_Shaded(sc->font, szoveg, feher, fekete);
    SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(sc->renderer, felirat);
    SDL_Rect hova = { ((sc->dim.x+2)*sc->block_size - felirat->w)/2, y, felirat->w, felirat->h };
    SDL_RenderCopy(sc->renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
}

void gui_draw_score(Screen const *sc, int score) {
    static char szoveg[20+1];
    sprintf_s(szoveg, 21, "Pontszám: %d", score);

    write_text(sc, szoveg, 3*32);

    SDL_RenderPresent(sc->renderer);
}

bool gui_ask_new_game(Screen const *sc) {
    char *szoveg1 = "Szeretnél még egyet játszani?", *szoveg2 = "Enter: igen, Escape: nem"; // global
    write_text(sc, szoveg2, 6*32);
    write_text(sc, szoveg1, 5*32);
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
    return 0; // ne legyen automatikusan újraindítva
}

int gui_next_frame(Screen const *sc, Snake *s) {
    bool done = false;
    SDL_AddTimer(s->speed * 1000, idozit, NULL);

    int key = SNAKE_KEY_NONE;
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

void gui_exit(Screen const *sc) {
    TTF_CloseFont(sc->font);
    SDL_Quit();
}
//void gui_set_game_loop(void (*f)(void)) {}
