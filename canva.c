#include <SDL3/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <json-c/json.h>
#include "canva.h"
#include "Log.h"
#include <time.h>

extern int window_width, window_height;

/*Canvas are store as a list of object in this form in the canva.json

{"id" : int, "Walls" : [[x,y,w,h]], "id_next_canva" : [-1,-1,-1,-1], "gate" : [], "key" : []}

*/

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
    Gate *L_gate;
    Key *L_Key;
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
            struct json_object *id, *walls, *next_canva, *gates, *gate, *id_gate, *skin_gate, *color_gate, *keys, *key, *id_key, *skin_key, *color_key;

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
                                temp[j] = (float)json_object_get_int(W);
                                temp[j] /= 100.f;
                                
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

                if(json_object_object_get_ex(obj, "gate", &gates)){ /*recupére les value de la clef "gate" du dico "obj" et les mets dans &gates*/
                    int len_gates = json_object_array_length(gates);
                    canva->nb_gate = len_gates;
                    L_gate = (Gate*)malloc(len_gates*sizeof(Gate));
                    for(int j = 0;j<len_gates;j++){
                        L_gate[j].state=CLOSE;
                        gate = json_object_array_get_idx(gates, j);/*récuper la valeur à l'indice j du la liste gates et la met dans gate*/
                        if (json_object_object_get_ex(gate, "id", &id_gate)){
                            L_gate[j].id_gate = json_object_get_int(id_gate);
                        }
                        else{
                            return NULL;
                        }

                        int temp_color[4];
                        float temp_skin[4];
                        if(json_object_object_get_ex(gate, "skin", &skin_gate) && json_object_is_type(skin_gate, json_type_array)){

                            int len_skin = json_object_array_length(skin_gate);
                            if(len_skin != 4){
                                return NULL;
                            }

                            for(int k=0;k<len_skin;k++){
                                struct json_object *C = json_object_array_get_idx(skin_gate, k);
                                if(C == NULL || !json_object_is_type(C, json_type_int)){
                                    return NULL;
                                }
                                temp_skin[k] = (float)json_object_get_int(C);
                                temp_skin[k] /= 100.f;
                            }
                        }
                        else{
                            return NULL;
                        }

                        if(json_object_object_get_ex(gate, "color", &color_gate) && json_object_is_type(color_gate, json_type_array)){

                            int len_color = json_object_array_length(color_gate);
                            if(len_color != 4){
                                return NULL;
                            }

                            for(int k=0;k<len_color;k++){
                                struct json_object *C = json_object_array_get_idx(color_gate, k);
                                if(C == NULL || !json_object_is_type(C, json_type_int)){
                                    return NULL;
                                }
                                temp_color[k] = json_object_get_int(C);
                            }
                        }
                        else{
                            return NULL;
                        }

                        L_gate[j].skin.x = temp_skin[0];
                        L_gate[j].skin.y = temp_skin[1];
                        L_gate[j].skin.w = temp_skin[2];
                        L_gate[j].skin.h = temp_skin[3];
                        L_gate[j].color.r = temp_color[0];
                        L_gate[j].color.g = temp_color[1];
                        L_gate[j].color.b = temp_color[2];
                        L_gate[j].color.a = temp_color[3];
                    }
                }
                else{
                    return NULL;
                }

                if(json_object_object_get_ex(obj, "key", &keys)){
                    int len_keys = json_object_array_length(keys);
                    canva->nb_key = len_keys;
                    L_Key = (Key*)malloc(len_keys*sizeof(Key));
                    for(int j = 0;j<len_keys;j++){
                        L_Key[j].state=ON_GROUND;
                        key = json_object_array_get_idx(keys, j);
                        if (json_object_object_get_ex(key, "id", &id_key)){
                            L_Key[j].id_key= json_object_get_int(id_key);
                        }
                        else{
                            return NULL;
                        }

                        int temp_color[4];
                        float temp_skin[4];
                        if(json_object_object_get_ex(key, "skin", &skin_key) && json_object_is_type(skin_key, json_type_array)){

                            int len_skin = json_object_array_length(skin_key);
                            if(len_skin != 4){
                                return NULL;
                            }

                            for(int k=0;k<len_skin;k++){
                                struct json_object *C = json_object_array_get_idx(skin_key, k);
                                if(C == NULL || !json_object_is_type(C, json_type_int)){
                                    return NULL;
                                }
                                temp_skin[k] = (float)json_object_get_int(C);
                                temp_skin[k] /= 100.f;
                            }
                        }
                        else{
                            return NULL;
                        }

                        if(json_object_object_get_ex(key, "color", &color_key) && json_object_is_type(color_key, json_type_array)){

                            int len_color = json_object_array_length(color_key);
                            if(len_color != 4){
                                return NULL;
                            }

                            for(int k=0;k<len_color;k++){
                                struct json_object *C = json_object_array_get_idx(color_key, k);
                                if(C == NULL || !json_object_is_type(C, json_type_int)){
                                    return NULL;
                                }
                                temp_color[k] = json_object_get_int(C);
                            }
                        }
                        else{
                            return NULL;
                        }

                        L_Key[j].skin.x = temp_skin[0];
                        L_Key[j].skin.y = temp_skin[1];
                        L_Key[j].skin.w = temp_skin[2];
                        L_Key[j].skin.h = temp_skin[3];
                        L_Key[j].color.r = temp_color[0];
                        L_Key[j].color.g = temp_color[1];
                        L_Key[j].color.b = temp_color[2];
                        L_Key[j].color.a = temp_color[3];
                    }
                }
                else{
                    return NULL;
                }
                
                canva->Walls = L_walls;
                canva->gates = L_gate;
                canva->keys = L_Key;
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
    int cpt_gate = 0, cpt_key = 0;
    render_canva->id_next_canva = (int*)malloc(4*sizeof(int));
    render_canva->id = canva->id;
    render_canva->nb_wall = canva->nb_wall;
    for(int i = 0; i < 4; i++){
        render_canva->id_next_canva[i] = canva->id_next_canva[i];
    }

    render_canva->Walls = (SDL_FRect*)malloc(canva->nb_wall*sizeof(SDL_FRect));
    
    for(int i = 0; i < canva->nb_wall; i++){
        render_canva->Walls[i].x = canva->Walls[i].x*window_width;
        render_canva->Walls[i].y = canva->Walls[i].y*window_height;
        render_canva->Walls[i].w = canva->Walls[i].w*window_width;
        render_canva->Walls[i].h = canva->Walls[i].h*window_height;
    }

    cpt_gate = 0;
    for(int i = 0; i < canva->nb_gate; i++){
        if(canva->gates[i].state == CLOSE){
            cpt_gate++;
        }
    }
    render_canva->gates = (Gate*)malloc(cpt_gate*sizeof(Gate));
    int index = 0;
    for(int i = 0; i < canva->nb_gate; i++){
        if(canva->gates[i].state == CLOSE){
            render_canva->gates[index] = canva->gates[i];
            render_canva->gates[index].skin.x *= window_width;
            render_canva->gates[index].skin.y *= window_height;
            render_canva->gates[index].skin.w *= window_width;
            render_canva->gates[index].skin.h *= window_height;
            index++;
        }
    }

    cpt_key = 0;
    for(int i = 0; i < canva->nb_key; i++){
        if(canva->keys[i].state == ON_GROUND){
            cpt_key++;
        }
    }
    render_canva->keys = (Key*)malloc(cpt_key*sizeof(Key));
    index = 0;
    for(int i = 0; i < canva->nb_key; i++){
        if(canva->keys[i].state == ON_GROUND){
            render_canva->keys[index] = canva->keys[i];
            render_canva->keys[index].skin.x *= window_width;
            render_canva->keys[index].skin.y *= window_height;
            render_canva->keys[index].skin.w *= window_width;
            render_canva->keys[index].skin.h *= window_height;
            index++;
        }
    }

    render_canva->nb_gate = cpt_gate;
    render_canva->nb_key = cpt_key;

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

void afficher_gate(Gate* g){
    printf("Gate ID : %d\n", g->id_gate);
    printf("Skin (x=%.2f, y=%.2f, w=%.2f, h=%.2f)\n", g->skin.x, g->skin.y, g->skin.w, g->skin.h);
    printf("Color (r=%d, g=%d, b=%d, a=%d)\n", g->color.r, g->color.g, g->color.b, g->color.a);
    printf("State : ");
    switch(g->state){
        case CLOSE:
            printf("CLOSE\n");
            break;
        case OPEN:
            printf("OPEN\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}
