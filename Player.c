#include <SDL3/SDL.h>
#include "Player.h"
#include <stdlib.h>
#include <stdio.h>
#include "constant_global.h"



Player* InitPlayer(void){
    Player* new_p = (Player*)malloc(sizeof(Player));
    new_p->skin.h = new_p->skin.w = 20;
    new_p->skin.x = new_p->skin.y = 100;
    return new_p;
}

Player* MovePlayer(Player* p, const bool* state){
    int dx = 0;
    int dy = 0;

    if(state[SDL_SCANCODE_RIGHT]){
        dx += SPEED;
    }
    if(state[SDL_SCANCODE_LEFT]){
        dx -= SPEED;
    }
    if(state[SDL_SCANCODE_DOWN]){
        dy += SPEED;
    }
    if(state[SDL_SCANCODE_UP]){
        dy -= SPEED;
    }

    p->skin.x = p->skin.x+dx;
    p->skin.y = p->skin.y+dy;
    return p;
}