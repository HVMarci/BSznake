#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "debugmalloc.h"
 
typedef struct Snake {
    int len;
    struct Golyo *head, *tail;
} Snake;

typedef struct Golyo {
    int x, y;
    int vx, vy;
    struct Golyo *next, *prev;
} Golyo;
 
/* ablak megnyitasa */
void sdl_init(int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
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
 
    *pwindow = window;
    *prenderer = renderer;
}
 
 
/* ez a fuggveny hivodik meg az idozito altal.
 * betesz a feldolgozando esemenyek koze (push) egy felhasznaloi esemenyt */
Uint32 _idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    if (param != NULL) {}
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

void draw_golyo(SDL_Renderer *renderer, Golyo *g) {
    filledCircleRGBA(renderer, g->x, g->y, 10, 0x80, 0x70, 0x90, 0xFF);
}
void remove_golyo(SDL_Renderer *renderer, Golyo *g){
    filledCircleRGBA(renderer, g->x, g->y, 10, 0x00, 0x00, 0x00, 0xFF);
}
 
int main123(int argc, char *argv[]) {
    enum { ABLAK=720 };
    enum { GOLYO_R=10 };
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init(ABLAK, ABLAK, &window, &renderer);
 
    /* idozito hozzaadasa: 20 ms; 1000 ms / 20 ms -> 50 fps */
    SDL_TimerID id = SDL_AddTimer(100, _idozit, NULL);
 
    /* animaciohoz */
    Snake s;
    s.len = 5;
    s.head = malloc(sizeof(Golyo));
    Golyo *g = s.head;
    g->y = 100;
    g->x = 150;
    g->prev = NULL;
    draw_golyo(renderer, g);
    for (int i = 1; i < 5; i++) {
        g->next = malloc(sizeof(Golyo));
        g->next->prev = g;
        g = g->next;
        g->y = 100;
        g->x = 150 - 20*i;
        g->next = NULL;
        draw_golyo(renderer, g);
    }
    s.tail = g;
 
    /* szokasos esemenyhurok */
    bool quit = false;
    int dir = 1; // URDL
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
 
        switch (event.type) {
            /* felhasznaloi esemeny: ilyeneket general az idozito fuggveny */
            case SDL_USEREVENT:
                Golyo *ujfej = malloc(sizeof(Golyo));

                ujfej->x = s.head->x;
                ujfej->y = s.head->y;
                ujfej->prev = NULL;

                if (dir == 0) {
                    ujfej->y -= 20;
                } else if (dir == 1) {
                    ujfej->x += 20;
                } else if (dir == 2) {
                    ujfej->y += 20;
                } else {
                    ujfej->x -= 20;
                }
                
                draw_golyo(renderer, ujfej);
                ujfej->next = s.head;
                s.head->prev = ujfej;
                s.head = ujfej;

                Golyo *oldtail = s.tail;
                remove_golyo(renderer, oldtail);
                s.tail = oldtail->prev;
                free(oldtail);

                SDL_RenderPresent(renderer);
                break;

            case SDL_KEYDOWN: // nincs egyhangban az fps-sel, így lehet csalni picit
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (dir != 2) dir = 0; break;
                    case SDLK_RIGHT: if (dir != 3) dir = 1; break;
                    case SDLK_DOWN: if (dir != 0) dir = 2; break;
                    case SDLK_LEFT: if (dir != 1) dir = 3; break;
                    case SDLK_ESCAPE: quit = true; break;
                }
                break;
 
            case SDL_QUIT:
                quit = true;
                break;
        }
    }
    /* idozito torlese */
    SDL_RemoveTimer(id);
 
    SDL_Quit();
 
    return 0;
}
