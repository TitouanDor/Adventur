#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>

typedef struct{
    SDL_FRect skin;
}Player;

Player* InitPlayer(void);
Player* MovePlayer(Player *p, const bool *state);
Player* Get_renderSkin(Player *P);
void print_player(Player *p);

#endif