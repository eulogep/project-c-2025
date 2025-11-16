#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vehicle.h"
#include "map.h"

VEHICULE* creer_vehicule(char type, int x, int y, char dir) {
    VEHICULE *v = malloc(sizeof(VEHICULE));
    if (!v) return NULL;

    v->type = type;
    v->posx = x;
    v->posy = y;
    v->direction = dir;
    v->vitesse = 1;
    v->alignement = 'C';
    v->code_couleur = 0;
    v->etat = 'A';
    v->tps = time(NULL);
    v->NXT = NULL;

    for (int i = 0; i < MAX_CARROSSERIE; i++) {
        memset(v->Carrosserie[i], ' ', MAX_LINE_LENGTH);
    }

    char fichier[50];
    snprintf(fichier, sizeof(fichier), "assets/vehicles/%c.txt", type);
    charger_vehicule_ascii(v, fichier);

    return v;
}

void ajouter_vehicule(VEHICULE **liste, VEHICULE *v) {
    if (!v) return;
    v->NXT = *liste;
    *liste = v;
}

void supprimer_vehicule(VEHICULE **liste, VEHICULE *v) {
    if (!liste || !*liste || !v) return;

    if (*liste == v) {
        *liste = v->NXT;
        free(v);
        return;
    }

    VEHICULE *courant = *liste;
    while (courant->NXT && courant->NXT != v) {
        courant = courant->NXT;
    }

    if (courant->NXT == v) {
        courant->NXT = v->NXT;
        free(v);
    }
}

void deplacer_vehicule(VEHICULE *v, char **map, int max_x, int max_y) {
    if (!v || !map) return;

    int dx = 0, dy = 0;
    switch (v->direction) {
        case 'N': dy = -v->vitesse; break;
        case 'S': dy = v->vitesse; break;
        case 'E': dx = v->vitesse; break;
        case 'W': dx = -v->vitesse; break;
        default: return;
    }

    int new_x = v->posx + dx;
    int new_y = v->posy + dy;

    if (new_x >= 0 && new_x < max_x && new_y >= 0 && new_y < max_y) {
        char case_map = map[new_y][new_x];
        if (case_map != '#' && case_map != 'X' && case_map != '|') {
            v->posx = new_x;
            v->posy = new_y;
        }
    }
}

int collision(VEHICULE *v1, VEHICULE *v2) {
    if (!v1 || !v2) return 0;
    return (v1->posx == v2->posx && v1->posy == v2->posy);
}

void charger_vehicule_ascii(VEHICULE *v, const char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        strcpy(v->Carrosserie[0], "[]");
        return;
    }

    int i = 0;
    char ligne[MAX_LINE_LENGTH];
    while (fgets(ligne, sizeof(ligne), f) && i < MAX_CARROSSERIE) {
        int len = strlen(ligne);
        if (ligne[len-1] == '\n') ligne[len-1] = '\0';
        strncpy(v->Carrosserie[i], ligne, MAX_LINE_LENGTH - 1);
        i++;
    }
    fclose(f);
}

void liberer_liste_vehicules(VEHICULE **liste) {
    VEHICULE *courant = *liste;
    while (courant) {
        VEHICULE *suivant = courant->NXT;
        free(courant);
        courant = suivant;
    }
    *liste = NULL;
}

