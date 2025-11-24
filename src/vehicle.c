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
        memset(v->Carrosserie[i], 0, MAX_LINE_LENGTH);
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

static int check_collision_map(int x, int y, const char Carrosserie[MAX_CARROSSERIE][MAX_LINE_LENGTH], char **map, int max_x, int max_y) {
    for (int i = 0; i < MAX_CARROSSERIE; i++) {
        for (int j = 0; j < MAX_LINE_LENGTH; j++) {
            if (Carrosserie[i][j] != 0 && Carrosserie[i][j] != ' ') {
                int cx = x + j;
                int cy = y + i;
                if (cx < 0 || cx >= max_x || cy < 0 || cy >= max_y) return 1;
                char c = map[cy][cx];
                if (c == '#' || c == 'X' || c == '|') return 1;
            }
        }
    }
    return 0;
}

void deplacer_vehicule(VEHICULE *v, char **map, int max_x, int max_y) {
    if (!v || !map || v->etat == 'C') return; // Cannot move if crashed

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

    if (!check_collision_map(new_x, new_y, (const char (*)[MAX_LINE_LENGTH])v->Carrosserie, map, max_x, max_y)) {
        v->posx = new_x;
        v->posy = new_y;
    } else {
        // Simple AI: if blocked, try random direction or stop?
        // For now, simple bounce logic or stop?
        // The project requirements mention "realistic collisions", so we let collision detection handle vehicle-vehicle.
        // Wall collision just stops movement here.
    }
}

int intersect(int x1, int y1, const char c1[MAX_CARROSSERIE][MAX_LINE_LENGTH],
              int x2, int y2, const char c2[MAX_CARROSSERIE][MAX_LINE_LENGTH]) {
    for (int i1 = 0; i1 < MAX_CARROSSERIE; i1++) {
        for (int j1 = 0; j1 < MAX_LINE_LENGTH; j1++) {
            if (c1[i1][j1] != 0 && c1[i1][j1] != ' ') {
                int px1 = x1 + j1;
                int py1 = y1 + i1;

                for (int i2 = 0; i2 < MAX_CARROSSERIE; i2++) {
                    for (int j2 = 0; j2 < MAX_LINE_LENGTH; j2++) {
                        if (c2[i2][j2] != 0 && c2[i2][j2] != ' ') {
                            int px2 = x2 + j2;
                            int py2 = y2 + i2;
                            if (px1 == px2 && py1 == py2) return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int collision(VEHICULE *v1, VEHICULE *v2) {
    if (!v1 || !v2 || v1 == v2) return 0;
    return intersect(v1->posx, v1->posy, (const char (*)[MAX_LINE_LENGTH])v1->Carrosserie,
                     v2->posx, v2->posy, (const char (*)[MAX_LINE_LENGTH])v2->Carrosserie);
}

void charger_vehicule_ascii(VEHICULE *v, const char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        strcpy(v->Carrosserie[0], "Error");
        return;
    }

    int i = 0;
    char ligne[MAX_LINE_LENGTH];
    while (fgets(ligne, sizeof(ligne), f) && i < MAX_CARROSSERIE) {
        int len = strlen(ligne);
        if (ligne[len-1] == '\n') ligne[len-1] = '\0';
        strncpy(v->Carrosserie[i], ligne, MAX_LINE_LENGTH - 1);
        // Ensure null termination and padding if needed? No, just keep as is.
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
