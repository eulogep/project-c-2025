#ifndef VEHICLE_H
#define VEHICLE_H

#define MAX_CARROSSERIE 4
#define MAX_LINE_LENGTH 30

typedef struct voiture {
    char direction;
    int posx, posy;
    int vitesse;
    char alignement;
    char type;
    char Carrosserie[MAX_CARROSSERIE][MAX_LINE_LENGTH];
    int code_couleur;
    char etat;
    unsigned long int tps;
    struct voiture *NXT;
} VEHICULE;

VEHICULE* creer_vehicule(char type, int x, int y, char dir);
void ajouter_vehicule(VEHICULE **liste, VEHICULE *v);
void supprimer_vehicule(VEHICULE **liste, VEHICULE *v);
void deplacer_vehicule(VEHICULE *v, char **map, int max_x, int max_y);
int collision(VEHICULE *v1, VEHICULE *v2);
void charger_vehicule_ascii(VEHICULE *v, const char *fichier);
void liberer_liste_vehicules(VEHICULE **liste);

#endif

