#ifndef MAP_H
#define MAP_H

#define MAX_MAP_WIDTH 200
#define MAX_MAP_HEIGHT 100

typedef struct {
    char **grille;
    int largeur;
    int hauteur;
} MAP;

MAP* charger_map(const char *fichier);
void liberer_map(MAP *m);
char get_case(MAP *m, int x, int y);
void set_case(MAP *m, int x, int y, char c);
int est_barriere(int x, int y);
int est_entree(int x, int y);
int est_sortie(int x, int y);

#endif

