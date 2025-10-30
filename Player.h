#ifndef PLAYER_H
#define PLAYER_H
#include <SDL3/SDL.h>
#include "canva.h"
#include "constant_global.h"

enum player_state{
    STOP,
    RUNNING,
    SNEAK,
};

typedef struct key_inv{
    int id;
    struct key_inv *next;
}key_inv;

typedef struct{
    SDL_FRect skin;
    enum player_state state;
    key_inv *Key;

}Player;

Player* InitPlayer(void);
Player* MovePlayer(Player *p, const bool *state);
Player* Get_renderSkin(Player *p);
Player* Change_Canva(Player *p, Canva* canva, Gamectx **gameState);
Player* Collision(Player *p, Canva* canva);
Player* Get_Key(Player *p, Canva** pcanva);
void print_player(Player *p);

#endif