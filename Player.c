#include <SDL3/SDL.h>
#include "Player.h"
#include <stdlib.h>
#include <stdio.h>
#include "constant_global.h"

extern int window_width, window_height;

Player* InitPlayer(void){
    Player *new_p = (Player*)malloc(sizeof(Player));
    new_p->skin.h = 0.027;
    new_p->skin.w = 0.015;
    new_p->skin.x = 0.5;
    new_p->skin.y = 0.5;
    return new_p;
}

Player* MovePlayer(Player *p, const bool *state){
    float dx = 0;
    float dy = 0;

    if(state[SDL_SCANCODE_RIGHT]){
        dx += SPEED/100.f;
    }
    if(state[SDL_SCANCODE_LEFT]){
        dx -= SPEED/100.f;
    }
    if(state[SDL_SCANCODE_DOWN]){
        dy += SPEED/100.f;
    }
    if(state[SDL_SCANCODE_UP]){
        dy -= SPEED/100.f;
    }

    p->skin.x = p->skin.x+dx;
    p->skin.y = p->skin.y+dy;

    return p;
}

Player* Get_renderSkin(Player *P){
    Player *render_p = (Player*)malloc(sizeof(Player));
    render_p->skin.x = P->skin.x*window_width;
    render_p->skin.y = P->skin.y*window_height;
    render_p->skin.w = P->skin.w*window_width;
    render_p->skin.h = P->skin.h*window_height;
    return render_p;
}

void print_player(Player *p){
    printf("Player : x : %f, y : %f, w : %f, h : %f\n", 
        p->skin.x, 
        p->skin.y,
        p->skin.w,
        p->skin.h 
    );
}