#ifndef CANVA_H
#define CANVA_H
#include <SDL3/SDL.h>

enum gate_state{
    CLOSE,
    OPEN,
};

typedef struct{
    int id_gate;
    SDL_FRect skin;
    SDL_Color color;
    enum gate_state state;
}Gate;

enum key_state{
    USED,
    TAKEN,
    ON_GROUND,
};

typedef struct{
    int id_key;
    SDL_FRect skin;
    SDL_Color color;
    enum key_state state;
}Key;

typedef struct{
    int id;
    int nb_wall;
    SDL_FRect *Walls;
    int nb_gate;
    Gate *gates;
    int nb_key;
    Key *keys;
    int *id_next_canva; /*Left, Right, Up, Bottom -1 if no canva*/
}Canva;

typedef struct cache{
    Canva *canva;
    struct cache *next;
}Cache;

Canva* Get_Canva(int id);
Canva* Get_render_Canva(Canva *canva);
void print_Canva(Canva *canva);
void afficher_gate(Gate* g);

#endif
