#include <SDL3/SDL.h>
#include "Player.h"
#include <stdlib.h>
#include <stdio.h>
#include "constant_global.h"
#include "canva.h"
#include "Log.h"

extern int window_width, window_height;

Player* InitPlayer(void){
    Player *new_p = (Player*)malloc(sizeof(Player));
    if(!new_p){
        return NULL;
    }
    new_p->skin.h = 20.f/(float)window_height;
    new_p->skin.w = 20.f/(float)window_width;
    new_p->skin.x = 0.5;
    new_p->skin.y = 0.5;
    new_p->state = STOP;
    return new_p;
}

Player* MovePlayer(Player *p, const bool *state){
    float dx = 0.00;
    float dy = 0.00;

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

    
    if(SDL_abs(dy*10000) == 0 && SDL_abs(dx*10000) == 0){
        p->state = STOP;
    }
    else if(state[SDL_SCANCODE_LSHIFT]){
        p->state = SNEAK;
        dx /= COEF_SNEAK;
        dy /= COEF_SNEAK;

    }
    else{
        p->state = RUNNING;
    }

    p->skin.x += dx;
    p->skin.y += dy*((float)window_width/(float)window_height);

    return p;
}

Player* Get_renderSkin(Player *p){
    Player *render_p = (Player*)malloc(sizeof(Player));
    render_p->skin.x = p->skin.x*window_width;
    render_p->skin.y = p->skin.y*window_height;
    render_p->skin.w = p->skin.w*window_width;
    render_p->skin.h = p->skin.h*window_height;
    return render_p;
}

Player* Change_Canva(Player *p, Canva* canva, Gamectx **ctx){
    Gamectx *gameState = *ctx;
    if(p->skin.x <= 0){
        if(canva->id_next_canva[0] != -1){
            p->skin.x = 0.97;
            gameState->id_canva = canva->id_next_canva[0];
            char texte[100];
            sprintf(texte, "Change canva to %d", gameState->id_canva);
            WriteLog(texte);
        }
        else{
            p->skin.x = 0;
        }
    }
    else if (p->skin.x+p->skin.w >= 1){
        if(canva->id_next_canva[1] != -1){
            p->skin.x = 0.01;
            gameState->id_canva = canva->id_next_canva[1];
            char texte[100];
            sprintf(texte, "Change canva to %d", gameState->id_canva);
            WriteLog(texte);
        }
        else{
            p->skin.x = 1 - p->skin.w;
        }
    }
    if (p->skin.y <= 0){
        if(canva->id_next_canva[2] != -1){
            p->skin.y = 0.97;
            gameState->id_canva = canva->id_next_canva[2];
            char texte[100];
            sprintf(texte, "Change canva to %d", gameState->id_canva);
            WriteLog(texte);
        }
        else{
            p->skin.y = 0;
        }
        

    }
    else if (p->skin.y+p->skin.h >=1){
        if(canva->id_next_canva[3] != -1){
            p->skin.y = 0.1;
            gameState->id_canva = canva->id_next_canva[3];
            char texte[100];
            sprintf(texte, "Change canva to %d", gameState->id_canva);
            WriteLog(texte);
        }
        else{
            p->skin.y = 1 - p->skin.h;
        }
    }

    return p;
}

Player* Collision(Player *p, Canva* canva){
    for(int i=0;i<canva->nb_wall;i++){
        SDL_FRect wall = canva->Walls[i];
        SDL_FRect inter;
        if(SDL_GetRectIntersectionFloat(&p->skin, &wall, &inter)){
            if(inter.w < inter.h){
                if(p->skin.x < wall.x){
                    p->skin.x -= inter.w;
                }
                else{
                    p->skin.x += inter.w;
                }
            }
            else{
                if(p->skin.y < wall.y){
                    p->skin.y -= inter.h;
                }
                else{
                    p->skin.y += inter.h;
                }
            }
        }
    }
    return p;
}

void print_player(Player *p){
    printf("Player : x : %f, y : %f, w : %f, h : %f\n", 
        p->skin.x, 
        p->skin.y,
        p->skin.w,
        p->skin.h 
    );
}