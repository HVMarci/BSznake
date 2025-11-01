#include "gui.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "debugmalloc.h"

void gui_init(Screen *sc) {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }

    SDL_Window *window = SDL_CreateWindow("BSznake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sc->block_size * (sc->w + 2), sc->block_size * (sc->h + 2), 0);
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

    sc->window = window;
    sc->renderer = renderer;
}

void gui_draw_map(Screen const *sc) {
    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = (sc->w + 2) * sc->block_size;
    rect.h = (sc->h + 2) * sc->block_size;

    SDL_SetRenderDrawColor(sc->renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(sc->renderer, &rect);

    rect.x = rect.y = sc->block_size;
    rect.w = (sc->w) * sc->block_size;
    rect.h = (sc->h) * sc->block_size;
    SDL_SetRenderDrawColor(sc->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(sc->renderer, &rect);

    SDL_RenderPresent(sc->renderer);
}

void gui_draw_block(Screen const *sc, Block const *b) {
    filledCircleRGBA(sc->renderer, (b->x + 1) * sc->block_size + sc->block_size/2, (b->y + 1) * sc->block_size + sc->block_size/2, sc->block_size/2 - 1, 0xFF, 0x00, 0x00, 0xFF); // kell az R-1, különben túllóg a mezőn
}

void gui_draw_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        gui_draw_block(sc, ptr);
    }
    SDL_RenderPresent(sc->renderer);
}

void gui_erase_block(Screen const *sc, Block const *b) {
    filledCircleRGBA(sc->renderer, (b->x + 1) * sc->block_size + sc->block_size/2, (b->y + 1) * sc->block_size + sc->block_size/2, sc->block_size/2 - 1, 0x00, 0x00, 0x00, 0xFF); // kell az R-1, különben túllóg a mezőn
}

void gui_erase_snake(Screen const *sc, Snake const *s) {
    for (Block *ptr = s->head; ptr != NULL; ptr = ptr->next) {
        gui_erase_block(sc, ptr);
    }
    SDL_RenderPresent(sc->renderer);
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
    SDL_Quit();
}
//void gui_set_game_loop(void (*f)(void)) {}
