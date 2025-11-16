#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

MAP* charger_map(const char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        return NULL;
    }

    MAP *m = malloc(sizeof(MAP));
    if (!m) {
        fclose(f);
        return NULL;
    }

    char ligne[MAX_MAP_WIDTH];
    int max_largeur = 0;
    int nb_lignes = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        int len = strlen(ligne);
        if (ligne[len-1] == '\n') ligne[len-1] = '\0';
        if (len > max_largeur) max_largeur = len;
        nb_lignes++;
    }

    m->largeur = max_largeur;
    m->hauteur = nb_lignes;

    m->grille = malloc(m->hauteur * sizeof(char*));
    for (int i = 0; i < m->hauteur; i++) {
        m->grille[i] = calloc(m->largeur + 1, sizeof(char));
    }

    rewind(f);
    int y = 0;
    while (fgets(ligne, sizeof(ligne), f) && y < m->hauteur) {
        int len = strlen(ligne);
        if (ligne[len-1] == '\n') ligne[len-1] = '\0';
        strncpy(m->grille[y], ligne, m->largeur);
        y++;
    }

    fclose(f);
    return m;
}

void liberer_map(MAP *m) {
    if (m) {
        for (int i = 0; i < m->hauteur; i++) {
            free(m->grille[i]);
        }
        free(m->grille);
        free(m);
    }
}

char get_case(MAP *m, int x, int y) {
    if (!m || x < 0 || y < 0 || x >= m->largeur || y >= m->hauteur) {
        return ' ';
    }
    return m->grille[y][x];
}

void set_case(MAP *m, int x, int y, char c) {
    if (m && x >= 0 && y >= 0 && x < m->largeur && y < m->hauteur) {
        m->grille[y][x] = c;
    }
}

int est_barriere(int x, int y) {
    (void)x;
    (void)y;
    return 0;
}

int est_entree(int x, int y) {
    (void)x;
    (void)y;
    return 0;
}

int est_sortie(int x, int y) {
    (void)x;
    (void)y;
    return 0;
}

