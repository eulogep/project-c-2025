#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "engine.h"
#include "map.h"
#include "vehicle.h"
#include "display.h"
#include "input.h"

void initialiser_jeu(ETAT_JEU *jeu, MODE_JEU mode) {
    jeu->tickets_entree = 0;
    jeu->tickets_sortie = 0;
    jeu->prix_total = 0.0;
    jeu->mode = mode;
    jeu->barriere_entree_ouverte = 1;
    jeu->barriere_sortie_ouverte = 0;
    jeu->temps_jeu = time(NULL);
}

void generer_vehicule_aleatoire(VEHICULE **liste, MAP *map, MODE_JEU mode, ETAT_JEU *jeu) {
    if (!map) return;

    static int compteur_frame = 0;
    compteur_frame++;
    
    int delai_frames = (mode == MODE_FLUIDE) ? 30 : (mode == MODE_CHARGE) ? 10 : 60;
    if (compteur_frame < delai_frames) return;
    compteur_frame = 0;

    char types[] = {'C', 'T', 'M'};
    char type = types[rand() % 3];
    
    int x = 3;
    int y = 3;
    char dir = 'S';

    VEHICULE *v = creer_vehicule(type, x, y, dir);
    if (v) {
        if (jeu) jeu->tickets_entree++;
        ajouter_vehicule(liste, v);
    }
}

void gerer_barrieres(ETAT_JEU *jeu, unsigned long int temps) {
    unsigned long int periode = 10;
    jeu->barriere_entree_ouverte = (temps % (periode * 2) < periode);
    jeu->barriere_sortie_ouverte = !jeu->barriere_entree_ouverte;
}

float calculer_prix(unsigned long int temps_sejour) {
    return temps_sejour * 0.5;
}

void detecter_collisions(VEHICULE *liste) {
    VEHICULE *v1 = liste;
    while (v1) {
        VEHICULE *v2 = v1->NXT;
        while (v2) {
            if (collision(v1, v2)) {
                v1->etat = 'C';
                v2->etat = 'C';
            }
            v2 = v2->NXT;
        }
        v1 = v1->NXT;
    }
}

void boucle_jeu(ETAT_JEU *jeu, MAP *map, VEHICULE **liste_vehicules) {
    configurer_terminal();
    clear_screen();

    while (1) {
        if (key_pressed()) {
            char touche = lire_touche();
            if (touche == 'q' || touche == 'Q') break;
        }

        unsigned long int maintenant = time(NULL);
        gerer_barrieres(jeu, maintenant);

        if (jeu->barriere_entree_ouverte) {
            generer_vehicule_aleatoire(liste_vehicules, map, jeu->mode, jeu);
        }

        VEHICULE *courant = *liste_vehicules;
        while (courant) {
            deplacer_vehicule(courant, map->grille, map->largeur, map->hauteur);
            courant = courant->NXT;
        }

        detecter_collisions(*liste_vehicules);

        clear_screen();
        afficher_interface(jeu->tickets_entree, jeu->prix_total, jeu->mode);
        afficher_map(map);
        afficher_vehicules(*liste_vehicules);

        {
            struct timespec ts = {0, 100000000};
            nanosleep(&ts, NULL);
        }
    }

    restaurer_terminal();
}

