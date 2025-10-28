#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include "Player.h"
#include "constant_global.h"
#include "canva.h"

static Player* p = NULL;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int id_canva;
    int nb_canva;
    Cache* cache;
}Gamectx;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** gamestate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Gamectx* gameState = (Gamectx*)SDL_calloc(1, sizeof(Gamectx));
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


    if (!SDL_CreateWindowAndRenderer("Adventur", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &gameState->window, &gameState->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    p = InitPlayer();
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* gamestate, SDL_Event* event)
{
    if(event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* gamestate)
{
    Gamectx* gameState = (Gamectx *)gamestate;
    Uint32 frameSart = SDL_GetTicks();
    
    const bool* state = SDL_GetKeyboardState(NULL);
    p = MovePlayer(p, state);

    SDL_SetRenderDrawColor(gameState->renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); /*Grey, full alpha*/
    /* clear the window to the draw color. */
    SDL_RenderClear(gameState->renderer);

    /*Render canva*/
    /*Search if canva is already load*/
    Canva* canva;
    int loaded = 0, i = 0;
    Cache* temp = gameState->cache;
    while(loaded == 0 && temp->next==NULL){
        if(temp->canva->id == gameState->id_canva){
            loaded = 1;
        }
        else{
            temp = temp->next;
        }
    }

    if(loaded==1 && temp != NULL){/*If already loaded*/
        canva = temp->canva;
    }
    else{ /*Else load it and insert it into the cache*/
        canva = Get_Canva(gameState->id_canva);
        temp = (Cache*)SDL_calloc(1, sizeof(Cache));
        if (!temp) {
            return SDL_APP_FAILURE;
        }
        temp->next = NULL;
        temp->canva = canva;
        gameState->nb_canva++;
        gameState->cache->next = temp;
    }

    SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    SDL_RenderRects(gameState->renderer, canva->Walls, canva->nb_wall);
    SDL_RenderFillRects(gameState->renderer,canva->Walls, canva->nb_wall);

    /*Render player*/
    SDL_SetRenderDrawColor(gameState->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    SDL_RenderRect(gameState->renderer, &p->skin);
    SDL_RenderFillRect(gameState->renderer, &p->skin);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(gameState->renderer);

    int frameTime = SDL_GetTicks() - frameSart;
    if(FRAME_DELAY > frameTime){
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* gamestate, SDL_AppResult result)
{
    if (gamestate != NULL) {
        Gamectx* as = (Gamectx*)gamestate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
    /* SDL will clean up the window/renderer for us. */
    printf("Fin du programme\n");
}
