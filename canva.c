#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
#include "canva.h"
#include "Log.h"

extern int window_width, window_height;

Canva* Get_Canva(int id_canva){
    /*take the json file*/
    FILE *f = fopen("./../canva.json", "r");
    if (!f) {
        printf("Erreur d'ouverture du fichier\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    
    Canva *canva = (Canva*)malloc(sizeof(Canva));
    SDL_FRect *L_walls;
    canva->id_next_canva = (int*)malloc(4*sizeof(int));
    if (canva->id_next_canva == NULL) {
        return NULL;
    }

    struct json_object *root = json_tokener_parse(buffer);
    free(buffer);

    if (json_object_is_type(root, json_type_array)) {
        int n = json_object_array_length(root);
        for (int i = 0; i < n; i++) {

            struct json_object *obj = json_object_array_get_idx(root, i);
            struct json_object *id, *walls, *next_canva;

            /*Search for id*/
            if (json_object_object_get_ex(obj, "id", &id)){
                canva->id = json_object_get_int(id);
            }
            else{
                return NULL;
            }

            if(canva->id == id_canva){

                L_walls = (SDL_FRect*)malloc(canva->nb_wall*sizeof(SDL_FRect));

                /*search for the list of Walls*/
                if(json_object_object_get_ex(obj, "Walls", &walls) && json_object_is_type(walls, json_type_array)){

                    int len = json_object_array_length(walls);
                    canva->nb_wall = len;
                    float temp[4];

                    for(int k=0;k<len;k++){

                        struct json_object *wall = json_object_array_get_idx(walls, k);

                        if(json_object_is_type(wall, json_type_array)){

                            /*Search for x,y,w and h of the wall*/
                            int len_sub = json_object_array_length(wall);
                            for(int j = 0; j < len_sub; j++){
                                struct json_object *W = json_object_array_get_idx(wall, j);
                                temp[j] = json_object_get_int(W);
                                temp[j] /= 100;
                                
                            }
                        }
                        else{
                            return NULL;
                        }

                        L_walls[k].x = temp[0];
                        L_walls[k].y = temp[1];
                        L_walls[k].w = temp[2];
                        L_walls[k].h = temp[3];
                    }
                }
                else{
                    return NULL;
                }

                if(json_object_object_get_ex(obj, "id_next_canva", &next_canva) && json_object_is_type(next_canva, json_type_array)){

                    int len_id = json_object_array_length(next_canva);
                    if(len_id != 4){
                        return NULL;
                    }

                    for(int k=0;k<len_id;k++){
                        struct json_object *C = json_object_array_get_idx(next_canva, k);
                        if(C == NULL || !json_object_is_type(C, json_type_int)){
                            return NULL;
                        }
                        canva->id_next_canva[k] = json_object_get_int(C);
                    }
                }
                else{
                    return NULL;
                }
                
                canva->Walls = L_walls;
                char texte[100];
                sprintf(texte, "Canva n°%d import :", canva->id);
                WriteLog(texte);
                Import_canva(canva);
                return canva;
            }
        }
    }

    json_object_put(root);
    printf("ERROR Get_Canva : Canva not found\n");
    return NULL;
}

Canva* Get_render_Canva(Canva *canva){
    Canva *render_canva = (Canva*)malloc(sizeof(Canva));
    render_canva->id_next_canva = (int*)malloc(4*sizeof(int));
    render_canva->id = canva->id;
    render_canva->nb_wall = canva->nb_wall;
    render_canva->id_next_canva = canva->id_next_canva;
    render_canva->Walls = (SDL_FRect*)malloc(canva->nb_wall*sizeof(SDL_FRect));
    for(int i = 0; i < canva->nb_wall; i++){
        render_canva->Walls[i].x = canva->Walls[i].x*window_width;
        render_canva->Walls[i].y = canva->Walls[i].y*window_height;
        render_canva->Walls[i].w = canva->Walls[i].w*window_width;
        render_canva->Walls[i].h = canva->Walls[i].h*window_height;
    }

    return render_canva;
}

void print_Canva(Canva *canva){
    printf("|   id : %d\n", canva->id);
    printf("|   nb_wall : %d\n", canva->nb_wall);
    printf("|   id_next_canva :\n");
    for(int i = 0; i < 4; i++) {
        printf("|   |   [%d] : %d\n", i, canva->id_next_canva[i]);
    }
    printf("------------------\n");
}