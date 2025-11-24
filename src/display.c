#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "display.h"

#define MAP_START_X 2
#define MAP_START_Y 2

void clear_screen(void) {
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void set_color(const char *color) {
    printf("%s", color);
}

void reset_color(void) {
    printf("\033[0m");
}

void afficher_map(MAP *m) {
    if (!m) return;
    for (int y = 0; y < m->hauteur; y++) {
        gotoxy(MAP_START_X, MAP_START_Y + y);
        for (int x = 0; x < m->largeur; x++) {
            char c = m->grille[y][x];
            // Colors:
            // Walls (#): Cyan
            // Barriers (|): Red
            // Parking lines (+, -): White or Grey
            // Exit/Entrance signs: Special?

            if (c == '#') printf("\033[36m%c\033[0m", c); // Cyan walls
            else if (c == '|') printf("\033[31m%c\033[0m", c); // Red barriers
            else if (c == '+' || c == '-') printf("\033[37m%c\033[0m", c); // White lines
            else if (c == 'T' || c == '^' || c == '>' || c == '<') printf("\033[33m%c\033[0m", c); // Yellow arrows/indicators
            else putchar(c);
        }
    }
}

void afficher_vehicule(VEHICULE *v) {
    if (!v) return;

    // Cycle colors based on pointer address or ID to give variety
    // Or use code_couleur
    int color_code = (v->code_couleur > 0) ? v->code_couleur : (int)(((unsigned long)v % 6) + 31);
    if (v->etat == 'C') color_code = 31; // Red if crashed

    printf("\033[%dm", color_code);

    for (int i = 0; i < MAX_CARROSSERIE; i++) {
        for (int j = 0; j < MAX_LINE_LENGTH; j++) {
            char c = v->Carrosserie[i][j];
            if (c == 0) break;
            if (c != ' ' && c != '\0') {
                gotoxy(MAP_START_X + v->posx + j, MAP_START_Y + v->posy + i);
                // Use block character for body if ASCII art is generic, or print char
                putchar(c);
            }
        }
    }
    printf("\033[0m");
}

void afficher_vehicules(VEHICULE *liste) {
    VEHICULE *courant = liste;
    while (courant) {
        afficher_vehicule(courant);
        courant = courant->NXT;
    }
}

void rafraichir_zone(int x, int y, int w, int h, MAP *m, VEHICULE *liste) {
    (void)x; (void)y; (void)w; (void)h; (void)m; (void)liste;
    // Not strictly needed if we redraw efficiently or simple clear/redraw
    // Implementing partial refresh is complex with colors.
}

void afficher_interface(int tickets, float prix_total, int mode) {
    // Exit Box (Top Left)
    // Map starts at (2,2)
    // Box needs to be drawn over the map area reserved for it?
    // Or just let the map be the background.
    // Map text has "SORTIE" and "CAISSE".
    // We can overwrite the "CAISSE" text area with values if we want.

    // Hardcoded positions based on new map.txt
    // "TOTAL A PAYER :" box at top left
    gotoxy(MAP_START_X + 2, MAP_START_Y + 3);
    printf("\033[36mTOTAL A PAYER : \033[0m");
    gotoxy(MAP_START_X + 2, MAP_START_Y + 4);
    printf("\033[33m%6.2f EUR    \033[0m", prix_total);

    // Entrance Box (Bottom Right)
    // "BONJOUR :"
    // "PRENEZ VOTRE TICKET"
    // Map height approx 40?
    // Let's find bottom right coordinates.
    // Map width 85, Height ~40?
    // I need to know map dimensions.
    // Assuming bottom right is around (70, 35) based on map.txt

    // We can use a fixed position for "Status" or "Mode" info
    gotoxy(5, 1);
    char mode_str[20];
    switch (mode) {
        case 1: strcpy(mode_str, "FLUIDE"); break;
        case 2: strcpy(mode_str, "CHARGE"); break;
        case 3: strcpy(mode_str, "MANUEL"); break;
        default: strcpy(mode_str, "INCONNU"); break;
    }
    printf("Mode: %s | Tickets Entree: %d", mode_str, tickets);

    // Ticket Box area in map.txt is near "ENTREE"
    // Let's just overlay some text there if we can find it,
    // otherwise just rely on the static map text and maybe a blinking "PUSH BUTTON"
}
