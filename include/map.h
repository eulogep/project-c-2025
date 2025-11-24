#ifndef MAP_H
#define MAP_H

#define MAX_MAP_WIDTH 200
#define MAX_MAP_HEIGHT 100

// Using struct Map as requested by the user prompt
typedef struct Map {
    char **grille;
    int largeur;
    int hauteur;
} Map;

Map* charger_map(const char *fichier);
void liberer_map(Map *m);
char get_case(Map *m, int x, int y);
void set_case(Map *m, int x, int y, char c);
int est_mur(Map *m, int x, int y);
int est_entree(Map *m, int x, int y);
int est_sortie(Map *m, int x, int y);

#endif
