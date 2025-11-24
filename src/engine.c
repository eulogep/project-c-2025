#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "engine.h"
#include "map.h"
#include "vehicle.h"
#include "display.h"
#include "input.h"

// New Coordinates based on map.txt
// Exit (Sortie) Top Left
// Entrance (Entree) Bottom Right
// Entrance Barrier is near x=70, y=35?
// Map is width ~85, height ~40.
// Let's use approximate values.
// Entrance 'ENTREE' text is near bottom right.
// The barrier `|` for entrance is likely at:
// x=76, y=34 (approx)
// Exit barrier `|` is at top left:
// x=8, y=5 (approx)

// Update logic:
// Spawn at Bottom Right, drive North/West.
// Exit at Top Left.

#define ENT_BARRIER_X 76
#define ENT_BARRIER_Y 34
#define EXT_BARRIER_X 8
#define EXT_BARRIER_Y 5

void initialiser_jeu(ETAT_JEU *jeu, MODE_JEU mode) {
    jeu->tickets_entree = 0;
    jeu->tickets_sortie = 0;
    jeu->prix_total = 0.0;
    jeu->mode = mode;
    jeu->barriere_entree_ouverte = 0;
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
    
    // Spawn near entrance (Bottom Right)
    int x = 74;
    int y = 37;
    char dir = 'N'; // Drive up

    VEHICULE *v = creer_vehicule(type, x, y, dir);
    if (v) {
        // Check collision at spawn
        VEHICULE *check = *liste;
        int busy = 0;
        while(check) {
            if (collision(v, check)) {
                busy = 1;
                break;
            }
            check = check->NXT;
        }

        if (!busy) {
            if (jeu) jeu->tickets_entree++;
            // Set random color
            v->code_couleur = (rand() % 6) + 31;
            ajouter_vehicule(liste, v);
        } else {
            free(v);
        }
    }
}

void gerer_barrieres(ETAT_JEU *jeu, MAP *map, unsigned long int temps) {
    // Entrance barrier logic: Periodic
    int cycle = 20; // 2 seconds approx
    int phase = temps % cycle;

    int open_ent = (phase < 10); // Open half the time

    if (open_ent != jeu->barriere_entree_ouverte) {
        jeu->barriere_entree_ouverte = open_ent;
        // Check map bounds before toggling
        if (map) {
             // Try to find '|' near the hardcoded pos if not there
             toggle_barriere(map, ENT_BARRIER_X, ENT_BARRIER_Y, open_ent);
        }
    }
}

float calculer_prix(unsigned long int temps_sejour) {
    // 0.5 EUR per second for simulation speed
    return (float)temps_sejour * 0.5f;
}

void detecter_collisions(VEHICULE *liste) {
    VEHICULE *v1 = liste;
    while (v1) {
        VEHICULE *v2 = v1->NXT;
        while (v2) {
            if (v1->etat != 'C' && v2->etat != 'C' && collision(v1, v2)) {
                v1->etat = 'C';
                v2->etat = 'C';
            }
            v2 = v2->NXT;
        }
        v1 = v1->NXT;
    }
}

void gerer_sorties(ETAT_JEU *jeu, MAP *map, VEHICULE **liste_vehicules) {
    VEHICULE *courant = *liste_vehicules;
    VEHICULE *prev = NULL;

    while (courant) {
        // Check if near exit barrier (Top Left)
        if (courant->posx >= EXT_BARRIER_X - 3 && courant->posx <= EXT_BARRIER_X + 3 &&
            courant->posy >= EXT_BARRIER_Y - 3 && courant->posy <= EXT_BARRIER_Y + 3) {

            // Pay and Exit
            unsigned long int now = time(NULL);
            unsigned long int duree = now - courant->tps;
            float prix = calculer_prix(duree);

            jeu->tickets_sortie++;
            jeu->prix_total += prix;

            // Remove vehicle
            VEHICULE *to_delete = courant;
            courant = courant->NXT;
            if (prev) {
                prev->NXT = courant;
            } else {
                *liste_vehicules = courant;
            }

            // Temporarily open exit barrier visual (flicker)
            toggle_barriere(map, EXT_BARRIER_X, EXT_BARRIER_Y, 1);

            free(to_delete);
            continue;
        }

        prev = courant;
        courant = courant->NXT;
    }

    // Close exit barrier if no one is there
    toggle_barriere(map, EXT_BARRIER_X, EXT_BARRIER_Y, 0);
}

void boucle_jeu(ETAT_JEU *jeu, MAP *map, VEHICULE **liste_vehicules) {
    configurer_terminal();
    clear_screen();

    unsigned long int frame_counter = 0;

    while (1) {
        if (key_pressed()) {
            char touche = lire_touche();
            if (touche == 'q' || touche == 'Q') break;

            // Manual mode controls (ZQSD)
            if (jeu->mode == MODE_MANUEL && *liste_vehicules) {
                 // Control the LAST vehicle added (usually the player if spawned last)
                 // Or just the first one? Let's say first one.
                 VEHICULE *player = *liste_vehicules;
                 if (touche == 'z') player->direction = 'N';
                 if (touche == 's') player->direction = 'S';
                 if (touche == 'q') player->direction = 'W';
                 if (touche == 'd') player->direction = 'E';
            }
        }

        gerer_barrieres(jeu, map, frame_counter);

        // Spawn logic
        if (jeu->mode != MODE_MANUEL && jeu->barriere_entree_ouverte) {
             generer_vehicule_aleatoire(liste_vehicules, map, jeu->mode, jeu);
        } else if (jeu->mode == MODE_MANUEL && *liste_vehicules == NULL) {
             // Spawn player
             generer_vehicule_aleatoire(liste_vehicules, map, MODE_MANUEL, jeu);
        }

        VEHICULE *courant = *liste_vehicules;
        while (courant) {
            deplacer_vehicule(courant, map->grille, map->largeur, map->hauteur);
            courant = courant->NXT;
        }

        detecter_collisions(*liste_vehicules);
        gerer_sorties(jeu, map, liste_vehicules);

        clear_screen();
        afficher_map(map); // Draw Map First
        afficher_vehicules(*liste_vehicules); // Draw Vehicles
        afficher_interface(jeu->tickets_entree, jeu->prix_total, jeu->mode); // Draw UI over Map

        // Frame delay
        struct timespec ts = {0, 100000000}; // 100ms = 10 FPS
        nanosleep(&ts, NULL);
        frame_counter++;
    }

    restaurer_terminal();
}
