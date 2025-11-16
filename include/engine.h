#ifndef ENGINE_H
#define ENGINE_H

#include "map.h"
#include "vehicle.h"

typedef enum {
    MODE_FLUIDE = 1,
    MODE_CHARGE = 2,
    MODE_MANUEL = 3
} MODE_JEU;

typedef struct {
    int tickets_entree;
    int tickets_sortie;
    float prix_total;
    MODE_JEU mode;
    int barriere_entree_ouverte;
    int barriere_sortie_ouverte;
    unsigned long int temps_jeu;
} ETAT_JEU;

void initialiser_jeu(ETAT_JEU *jeu, MODE_JEU mode);
void boucle_jeu(ETAT_JEU *jeu, MAP *map, VEHICULE **liste_vehicules);
void generer_vehicule_aleatoire(VEHICULE **liste, MAP *map, MODE_JEU mode, ETAT_JEU *jeu);
void gerer_barrieres(ETAT_JEU *jeu, unsigned long int temps);
float calculer_prix(unsigned long int temps_sejour);
void detecter_collisions(VEHICULE *liste);

#endif

