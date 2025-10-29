#ifndef CANVA_H
#define CANVA_H
#include <SDL3/SDL.h>

typedef struct{
    int id;
    int nb_wall;
    SDL_FRect *Walls;
    int *id_next_canva; /*Left, Right, Up, Bottom -1 if no canva*/
}Canva;

typedef struct cache{
    Canva *canva;
    struct cache *next;
}Cache;

Canva* Get_Canva(int id);
Canva* Get_render_Canva(Canva *canva);
void print_Canva(Canva *canva);

#endif
