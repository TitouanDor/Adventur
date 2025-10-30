#include "Log.h"
#include "canva.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int InitLog(void){
    FILE *fichier = fopen("Log.txt", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;
    }
    return 0;
}

int WriteLog(char *texte) {
    FILE *fichier = fopen("Log.txt", "a");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;
    }
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char s_now[20];
    strftime(s_now, sizeof s_now, "%H:%M:%S", tm_now);
    fprintf(fichier, "[%s] ", s_now);
    fprintf(fichier, "%s", texte);
    fprintf(fichier, "\n");
    fclose(fichier);
    return 0;
}

int Import_canva(Canva* canva){
    FILE *fichier = fopen("Log.txt", "a");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;
    }
    fprintf(fichier, "|   id : %d\n", canva->id);
    fprintf(fichier,"|   nb_wall : %d\n", canva->nb_wall);
    fprintf(fichier,"|   nb_gate : %d\n", canva->nb_gate);
    fprintf(fichier,"|   id_next_canva :\n");
    for(int i = 0; i < 4; i++) {
        fprintf(fichier,"|   |   [%d] : %d\n", i, canva->id_next_canva[i]);
    }
    fprintf(fichier,"------------------\n");
    fclose(fichier);
    return 0;
}