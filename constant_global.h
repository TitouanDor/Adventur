#ifndef CONSTANT_GLOBAL_H
#define CONSTANT_GLOBAL_H

#define SPEED 0.5
#define COEF_SNEAK 3.f
#define FPS 60
#define FRAME_DELAY 1000/60
#define WINDOW_WIDTH_START 1280
#define WINDOW_HEIGHT_START 720

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int id_canva;
    int nb_canva;
    Cache* cache;
}Gamectx;


#endif
