#ifndef CANVA_H
#define CANVA_H
#include <SDL3/SDL.h>

typedef struct{
    int id;
    int nb_wall;
    SDL_FRect* Walls;
}Canva;

Canva* Get_Canva(int id);

#endif
