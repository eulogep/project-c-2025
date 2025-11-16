#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "map.h"
#include "vehicle.h"
#include "display.h"
#include "engine.h"
#include "menu.h"
#include "input.h"

int main(void) {
    srand(time(NULL));

    MAP *map = charger_map("assets/map.txt");
    if (!map) {
        fprintf(stderr, "Erreur: Impossible de charger la carte\n");
        return 1;
    }

    VEHICULE *liste_vehicules = NULL;
    ETAT_JEU jeu;
    char buffer[32];
    int choix = -1;

    do {
        restaurer_terminal();
        afficher_menu_principal();
        
        printf("Votre choix : ");
        fflush(stdout);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nErreur de lecture. Sortie.\n");
            break;
        }
        
        if (sscanf(buffer, "%d", &choix) != 1) {
            printf("Entree invalide. Tapez un chiffre entre 0 et 5.\n");
            sleep(1);
            continue;
        }

        switch (choix) {
            case 1:
            case 2:
            case 3: {
                MODE_JEU mode = (choix == 1) ? MODE_FLUIDE : 
                               (choix == 2) ? MODE_CHARGE : MODE_MANUEL;
                initialiser_jeu(&jeu, mode);
                boucle_jeu(&jeu, map, &liste_vehicules);
                liberer_liste_vehicules(&liste_vehicules);
                break;
            }
            case 4:
                afficher_regles();
                break;
            case 5:
                afficher_credits();
                break;
            case 0:
                printf("Au revoir.\n");
                break;
            default:
                printf("Choix invalide. Tapez un chiffre entre 0 et 5.\n");
                sleep(1);
                break;
        }
    } while (choix != 0);

    liberer_map(map);
    liberer_liste_vehicules(&liste_vehicules);
    return 0;
}

