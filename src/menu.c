#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

GameMode afficher_menu() {
    int choix = 0;
    while (1) {
        printf("\033[2J\033[1;1H"); // Clear screen
        printf("==========================================\n");
        printf("   SIMULATEUR DE PARKING ESIEA - MENU\n");
        printf("==========================================\n");
        printf("1. Mode Fluide\n");
        printf("2. Mode Chargé\n");
        printf("3. Mode Manuel (Joueur)\n");
        printf("4. Quitter\n");
        printf("\nVotre choix : ");

        char buf[10];
        if (fgets(buf, sizeof(buf), stdin)) {
            if (sscanf(buf, "%d", &choix) == 1) {
                if (choix >= 1 && choix <= 3) return (GameMode)choix;
                if (choix == 4) exit(0);
            }
        }
    }
}
