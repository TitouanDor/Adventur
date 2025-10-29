#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include "Player.h"
#include "constant_global.h"
#include "canva.h"

static Player *p = NULL;
int window_width = 1280, window_height = 720;
int temp_window_width, temp_window_height;
float window_ratio;

Canva* Get_Canva_from_Cache(int id_canva, Gamectx *gameState){
    Cache *temp = gameState->cache;
    while(temp->next!= NULL && temp->canva->id != id_canva){
        temp=temp->next;
    }
    if(temp->canva->id == id_canva){
        return temp->canva;
    }
    return NULL;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **gamestate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Gamectx *gameState = (Gamectx*)SDL_calloc(1, sizeof(Gamectx));
    if (!gameState) {
        return SDL_APP_FAILURE;
    }
    gameState->cache = (Cache*)SDL_calloc(1, sizeof(Cache));
    if (!gameState->cache) {
        return SDL_APP_FAILURE;
    }
    *gamestate = gameState;
    gameState->id_canva = 0;
    gameState->nb_canva = 1;
    gameState->cache->canva = Get_Canva(0);
    gameState->cache->next = NULL;


    if (!SDL_CreateWindowAndRenderer("Adventur", window_width, window_height, SDL_WINDOW_RESIZABLE, &gameState->window, &gameState->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    p = InitPlayer();
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *gamestate, SDL_Event *event)
{
    if(event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *gamestate)
{
    Gamectx *gameState = (Gamectx *)gamestate;
    Uint32 frameSart = SDL_GetTicks();

    SDL_GetWindowSize(gameState->window, &window_width, &window_height);

    const bool *state = SDL_GetKeyboardState(NULL);
    p = MovePlayer(p, state);
    p = Change_Canva(p, Get_Canva_from_Cache(gameState->id_canva, gameState), &gameState);

    SDL_SetRenderDrawColor(gameState->renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); /*Grey, full alpha*/
    SDL_RenderClear(gameState->renderer); /* clear the window to the draw color. */

    /*Render canva*/
    /*Search if canva is already load*/
    Canva *canva = Get_Canva_from_Cache(gameState->id_canva, gameState);
    if(canva != NULL){/*If already loaded*/
        canva = Get_render_Canva(canva);
    }
    else{ /*Else load it and insert it into the cache*/
        Canva* temp_canva = Get_Canva(gameState->id_canva);
        Cache* temp = (Cache*)SDL_calloc(1, sizeof(Cache));
        Cache *l_cache = gameState->cache;
        if (!temp) {
            return SDL_APP_FAILURE;
        }
        temp->next = NULL;
        temp->canva = temp_canva;
        gameState->nb_canva++;
        while(l_cache->next!=NULL){
            l_cache=l_cache->next;
        }
        l_cache->next = temp;
        canva = Get_render_Canva(temp->canva);
    }

    SDL_SetRenderDrawColor(gameState->renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);  /* Dark Grey, full alpha */
    SDL_RenderRects(gameState->renderer, canva->Walls, canva->nb_wall);
    SDL_RenderFillRects(gameState->renderer,canva->Walls, canva->nb_wall);

    /*Render player*/
    Player *render_player = Get_renderSkin(p);
    SDL_SetRenderDrawColor(gameState->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    SDL_RenderRect(gameState->renderer, &render_player->skin);
    SDL_RenderFillRect(gameState->renderer, &render_player->skin);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(gameState->renderer);

    int frameTime = SDL_GetTicks() - frameSart;
    if(FRAME_DELAY > frameTime){
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *gamestate, SDL_AppResult result)
{
    if (gamestate != NULL) {
        Gamectx *as = (Gamectx*)gamestate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
    /* SDL will clean up the window/renderer for us. */
    printf("Fin du programme\n");
}
