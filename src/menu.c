#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "menu.h"
#include "input.h"
#include "display.h"

void afficher_menu_principal(void) {
    clear_screen();
    
    gotoxy(20, 3);
    printf("===============================================");
    gotoxy(20, 4);
    printf("           SIMULATEUR DE PARKING ESIEA         ");
    gotoxy(20, 5);
    printf("===============================================");
    
    gotoxy(25, 9);
    printf("1. Mode Fluide");
    
    gotoxy(25, 11);
    printf("2. Mode Charge");
    
    gotoxy(25, 13);
    printf("3. Mode Manuel");
    
    gotoxy(25, 15);
    printf("4. Regles");
    
    gotoxy(25, 17);
    printf("5. Credits");
    
    gotoxy(25, 20);
    printf("0. Quitter");
    
    fflush(stdout);
}

MODE_JEU selectionner_mode(void) {
    configurer_terminal();
    char choix = 0;
    while (choix == 0) {
        if (key_pressed()) {
            choix = lire_touche();
        }
    }
    restaurer_terminal();

    switch (choix) {
        case '1': return MODE_FLUIDE;
        case '2': return MODE_CHARGE;
        case '3': return MODE_MANUEL;
        default: return MODE_FLUIDE;
    }
}

void afficher_regles(void) {
    restaurer_terminal();
    clear_screen();
    
    gotoxy(20, 3);
    printf("===============================================");
    gotoxy(20, 4);
    printf("                    REGLES                     ");
    gotoxy(20, 5);
    printf("===============================================");
    
    gotoxy(20, 9);
    printf("Mode Fluide: Generation lente de vehicules");
    
    gotoxy(20, 11);
    printf("Mode Charge: Generation rapide + collisions");
    
    gotoxy(20, 13);
    printf("Mode Manuel: Controle manuel (bonus)");
    
    gotoxy(20, 18);
    printf("Appuyez sur Entree pour revenir au menu");
    
    fflush(stdout);
    char buffer[32];
    fgets(buffer, sizeof(buffer), stdin);
}

void afficher_credits(void) {
    restaurer_terminal();
    clear_screen();
    
    gotoxy(20, 3);
    printf("===============================================");
    gotoxy(20, 4);
    printf("                   CREDITS                     ");
    gotoxy(20, 5);
    printf("===============================================");
    
    gotoxy(20, 9);
    printf("Projet ESIEA - Simulateur de Parking");
    
    gotoxy(20, 11);
    printf("Langage: C");
    
    gotoxy(20, 16);
    printf("Appuyez sur Entree pour revenir au menu");
    
    fflush(stdout);
    char buffer[32];
    fgets(buffer, sizeof(buffer), stdin);
}

