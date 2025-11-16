#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "display.h"

#define MAP_START_X 5
#define MAP_START_Y 3

void clear_screen(void) {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void afficher_map(MAP *m) {
    if (!m) return;
    for (int y = 0; y < m->hauteur; y++) {
        gotoxy(MAP_START_X, MAP_START_Y + y);
        for (int x = 0; x < m->largeur; x++) {
            putchar(m->grille[y][x]);
        }
    }
}

void afficher_vehicule(VEHICULE *v) {
    if (!v || v->etat == 'C') return;
    for (int i = 0; i < MAX_CARROSSERIE; i++) {
        if (v->Carrosserie[i][0] != '\0' && v->Carrosserie[i][0] != ' ') {
            gotoxy(MAP_START_X + v->posx, MAP_START_Y + v->posy + i);
            printf("%s", v->Carrosserie[i]);
        }
    }
}

void afficher_vehicules(VEHICULE *liste) {
    VEHICULE *courant = liste;
    while (courant) {
        afficher_vehicule(courant);
        courant = courant->NXT;
    }
}

void rafraichir_zone(int x, int y, int w, int h, MAP *m, VEHICULE *liste) {
    for (int py = y; py < y + h && py < m->hauteur; py++) {
        for (int px = x; px < x + w && px < m->largeur; px++) {
            gotoxy(px, py);
            putchar(m->grille[py][px]);
        }
    }
    afficher_vehicules(liste);
}

void afficher_interface(int tickets, float prix_total, int mode) {
    gotoxy(5, 1);
    printf("===============================================");
    gotoxy(5, 2);
    char mode_str[20];
    switch (mode) {
        case 1: strcpy(mode_str, "FLUIDE"); break;
        case 2: strcpy(mode_str, "CHARGE"); break;
        case 3: strcpy(mode_str, "MANUEL"); break;
        default: strcpy(mode_str, "INCONNU"); break;
    }
    printf("Tickets: %-5d | Prix: %6.2f EUR | Mode: %s", tickets, prix_total, mode_str);
    gotoxy(5, 27);
    printf("Appuyez sur Q pour quitter");
}

