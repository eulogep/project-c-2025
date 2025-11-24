#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

Map* charger_map(const char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) return NULL;

    Map *m = malloc(sizeof(Map));
    if (!m) { fclose(f); return NULL; }

    m->largeur = 0;
    m->hauteur = 0;

    // First pass to get dimensions
    char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        int len = strlen(buf);
        if (buf[len-1] == '\n') len--;
        if (len > m->largeur) m->largeur = len;
        m->hauteur++;
    }

    rewind(f);

    m->grille = malloc(m->hauteur * sizeof(char*));
    for(int i = 0; i < m->hauteur; i++) {
        m->grille[i] = malloc((m->largeur + 1) * sizeof(char));
        // Fill with spaces by default
        memset(m->grille[i], ' ', m->largeur);
        m->grille[i][m->largeur] = '\0';
    }

    int y = 0;
    while (fgets(buf, sizeof(buf), f)) {
        int len = strlen(buf);
        if (buf[len-1] == '\n') buf[len-1] = '\0';
        // Copy content
        for(int x = 0; x < len && x < m->largeur; x++) {
            m->grille[y][x] = buf[x];
        }
        y++;
    }

    fclose(f);
    return m;
}

void liberer_map(Map *m) {
    if (!m) return;
    for (int i = 0; i < m->hauteur; i++) {
        free(m->grille[i]);
    }
    free(m->grille);
    free(m);
}

char get_case(Map *m, int x, int y) {
    if (!m || x < 0 || y < 0 || x >= m->largeur || y >= m->hauteur) return '#';
    return m->grille[y][x];
}

void set_case(Map *m, int x, int y, char c) {
    if (m && x >= 0 && y >= 0 && x < m->largeur && y < m->hauteur) {
        m->grille[y][x] = c;
    }
}

int est_mur(Map *m, int x, int y) {
    char c = get_case(m, x, y);
    return (c == '#');
}

int est_entree(Map *m, int x, int y) {
    return (get_case(m, x, y) == 'E');
}

int est_sortie(Map *m, int x, int y) {
    return (get_case(m, x, y) == 'S');
}
