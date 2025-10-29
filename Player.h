#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>
#include "canva.h"
#include "constant_global.h"

typedef struct{
    SDL_FRect skin;
}Player;

Player* InitPlayer(void);
Player* MovePlayer(Player *p, const bool *state);
Player* Get_renderSkin(Player *P);
Player* Change_Canva(Player *p, Canva* canva, Gamectx **gameState);
void print_player(Player *p);

#endif