#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include "Player.h"
#include "constant_global.h"
#include "canva.h"
#include "Log.h"

static Player *p = NULL;
int window_width = WINDOW_WIDTH_START, window_height = WINDOW_HEIGHT_START;
int temp_window_width, temp_window_height;
float window_ratio;
int debug = 1;

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
SDL_AppResult SDL_AppInit(void **gamestate, int argc, char *argv[]){
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        WriteLog("Initialisation SDL_VIDEO : ERROR");
        return SDL_APP_FAILURE;
    }

    WriteLog("Initialisation SDL_VIDEO : OK");

    Gamectx *gameState = (Gamectx*)SDL_calloc(1, sizeof(Gamectx));
    if (!gameState) {
        WriteLog("Initialisation gameState : ERROR");
        return SDL_APP_FAILURE;
    }

    p = InitPlayer();
    if(!p){
        WriteLog("Initialisation player : ERROR");
        return SDL_APP_FAILURE;
    }
    WriteLog("Initialisation player : OK");

    gameState->cache = (Cache*)SDL_calloc(1, sizeof(Cache));
    if (!gameState->cache) {
        WriteLog("Initialisation gameState->cache : ERROR");
        return SDL_APP_FAILURE;
    }
    *gamestate = gameState;
    gameState->id_canva = 0;
    gameState->nb_canva = 1;
    gameState->cache->canva = Get_Canva(0);
    if(gameState->cache->canva == NULL){
        WriteLog("Initialisation gameState->cache->canva : ERROR");
        return SDL_APP_FAILURE;
    }
    gameState->cache->next = NULL;
    WriteLog("Initialisation gameState : OK");

    if (!SDL_CreateWindowAndRenderer("Adventur", window_width, window_height, SDL_WINDOW_RESIZABLE, &gameState->window, &gameState->renderer)) {
        WriteLog("Couldn't create window/renderer");
        return SDL_APP_FAILURE;
    }
    WriteLog("Initialisation Window et Render : OK");
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *gamestate, SDL_Event *event){
    Gamectx *gameState = (Gamectx *)gamestate;
    if(event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if(event->type == SDL_EVENT_KEY_DOWN){
        if(event->key.key == SDLK_R){ /*Reload the cache*/
            WriteLog("Reload cache");
            gameState->cache->canva = Get_Canva(gameState->id_canva);
            if(gameState->cache->canva == NULL){
                WriteLog("Initialisation gameState->cache->canva : ERROR");
                return SDL_APP_FAILURE;
            }
            gameState->cache->next = NULL;
            gameState->nb_canva = 1;
        }
        if(event->key.key == SDLK_P){ /*Reset du jeu*/
            WriteLog("Reset Game");
            gameState->id_canva = 0;
            gameState->cache->canva = Get_Canva(gameState->id_canva);
            if(gameState->cache->canva == NULL){
                WriteLog("Initialisation gameState->cache->canva : ERROR");
                return SDL_APP_FAILURE;
            }
            gameState->cache->next = NULL;
            gameState->nb_canva = 1;
        }
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *gamestate){
    Gamectx *gameState = (Gamectx *)gamestate;
    Uint32 frameSart = SDL_GetTicks();

    SDL_GetWindowSize(gameState->window, &window_width, &window_height);

    const bool *state = SDL_GetKeyboardState(NULL);
    Canva *canva = Get_Canva_from_Cache(gameState->id_canva, gameState);
    p = MovePlayer(p, state);
    p = Change_Canva(p, canva, &gameState);
    p = Collision(p, canva);
    SDL_SetRenderDrawColor(gameState->renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); /*Grey, full alpha*/
    SDL_RenderClear(gameState->renderer); /* clear the window to the draw color. */

    /*Render canva*/
    /*Search if canva is already load*/
    canva = Get_Canva_from_Cache(gameState->id_canva, gameState);
    if(canva != NULL){/*If already loaded*/
        canva = Get_render_Canva(canva);
    }
    else{ /*Else load it and insert it into the cache*/
        Canva* temp_canva = Get_Canva(gameState->id_canva);
        if(gameState->cache->canva == NULL){
            WriteLog("ERROR : reload cache");
            return SDL_APP_FAILURE;
        }
        Cache* temp = (Cache*)SDL_calloc(1, sizeof(Cache));
        Cache *l_cache = gameState->cache;
        if (!temp) {
            WriteLog("Initialisation temp : ERROR");
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


    if(debug == 1){
        char debug_text [200];
        sprintf(debug_text, "Debug|Canva_id : %d|Player State : %d", gameState->id_canva, p->state);
        SDL_SetRenderDrawColor(gameState->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(gameState->renderer,0,0,debug_text);
    }
    

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(gameState->renderer);

    int frameTime = SDL_GetTicks() - frameSart;
    if(FRAME_DELAY > frameTime){
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *gamestate, SDL_AppResult result){
    if (gamestate != NULL) {
        Gamectx *as = (Gamectx*)gamestate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
    /* SDL will clean up the window/renderer for us. */
    WriteLog("Fin du programme\n\n\n");
}
