#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Player.h"
#include "constant_global.h"



/* We will use this renderer to draw into this window every frame. */
static Player* p = NULL;
int frameTime;
Uint32 frameSart;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int canva;
}Gamectx;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Gamectx* gameState = (Gamectx*)SDL_calloc(1, sizeof(Gamectx));
    if (!gameState) {
        return SDL_APP_FAILURE;
    }
    *appstate = gameState;

    if (!SDL_CreateWindowAndRenderer("Adventur", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &gameState->window, &gameState->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    p = InitPlayer();

    

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if(event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    Gamectx* gameState = (Gamectx *)appstate;
    frameSart = SDL_GetTicks();
    
    const bool* state = SDL_GetKeyboardState(NULL);
    p = MovePlayer(p, state);

    SDL_SetRenderDrawColor(gameState->renderer, 155, 155, 155, SDL_ALPHA_OPAQUE); /*Grey, full alpha*/
    /* clear the window to the draw color. */
    SDL_RenderClear(gameState->renderer);

    /*Render player*/
    SDL_SetRenderDrawColor(gameState->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    SDL_RenderRect(gameState->renderer, &p->skin);
    SDL_RenderFillRect(gameState->renderer, &p->skin);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(gameState->renderer);

    frameTime = SDL_GetTicks() - frameSart;
    if(FRAME_DELAY > frameTime){
        SDL_Delay(FRAME_DELAY - frameTime);
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate != NULL) {
        Gamectx* as = (Gamectx*)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
    /* SDL will clean up the window/renderer for us. */
    printf("Fin du programme\n");
}
