#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>
#include "canva.h"
#include "constant_global.h"

enum player_state{
    SNEAK,
    RUNNING,
    STOP,
};

typedef struct{
    SDL_FRect skin;
    enum player_state state;

}Player;

Player* InitPlayer(void);
Player* MovePlayer(Player *p, const bool *state);
Player* Get_renderSkin(Player *P);
Player* Change_Canva(Player *p, Canva* canva, Gamectx **gameState);
Player* Collision(Player *p, Canva* canva);
void print_player(Player *p);

#endif