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

static int is_connected(char c) {
    return (c == '-' || c == '|' || c == '+');
}

void print_unicode_cell(MAP *m, int x, int y) {
    char c = m->grille[y][x];

    // Colors
    if (c == '#') {
        printf("\033[36m\u2588\033[0m"); // Cyan Full Block
        return;
    }
    if (c == '|') {
        // Red vertical barrier in specific zones, otherwise white line
        // Barrier zones: Top Left (Exit) and Bottom Right (Entrance).
        // x < 15 && y < 10 -> Exit Barrier?
        // x > width-15 && y > height-10 -> Entrance Barrier?
        if ((x < 15 && y < 10) || (x > m->largeur - 15 && y > m->hauteur - 10)) {
             printf("\033[31m\u2502\033[0m"); // Red Vertical
        } else {
             printf("\033[37m\u2502\033[0m"); // White Vertical
        }
        return;
    }
    if (c == '-') {
        printf("\033[37m\u2500\033[0m"); // White Horizontal
        return;
    }
    if (c == '+') {
        // Context aware intersection
        int n = (y > 0 && is_connected(m->grille[y-1][x]));
        int s = (y < m->hauteur-1 && is_connected(m->grille[y+1][x]));
        int e = (x < m->largeur-1 && is_connected(m->grille[y][x+1]));
        int w = (x > 0 && is_connected(m->grille[y][x-1]));

        printf("\033[37m"); // White

        if (n && s && e && w) printf("\u253C"); // Cross ┼
        else if (s && e && w) printf("\u252C"); // T down ┬
        else if (n && e && w) printf("\u2534"); // T up ┴
        else if (n && s && e) printf("\u251C"); // T right ├
        else if (n && s && w) printf("\u2524"); // T left ┤
        else if (s && e) printf("\u250C"); // Corner Top-Left ┌
        else if (s && w) printf("\u2510"); // Corner Top-Right ┐
        else if (n && e) printf("\u2514"); // Corner Bottom-Left └
        else if (n && w) printf("\u2518"); // Corner Bottom-Right ┘
        else if (e || w) printf("\u2500"); // Horizontal
        else if (n || s) printf("\u2502"); // Vertical
        else printf("+");

        printf("\033[0m");
        return;
    }

    // Other chars
    if (c == 'T' || c == '^' || c == '>' || c == '<') printf("\033[33m%c\033[0m", c);
    else putchar(c);
}

void afficher_map(MAP *m) {
    if (!m) return;
    for (int y = 0; y < m->hauteur; y++) {
        gotoxy(MAP_START_X, MAP_START_Y + y);
        for (int x = 0; x < m->largeur; x++) {
            print_unicode_cell(m, x, y);
        }
    }
}

void afficher_vehicule(VEHICULE *v) {
    if (!v) return;

    int color_code = (v->code_couleur > 0) ? v->code_couleur : (int)(((unsigned long)v % 6) + 31);
    if (v->etat == 'C') color_code = 31;

    printf("\033[%dm", color_code);

    for (int i = 0; i < MAX_CARROSSERIE; i++) {
        for (int j = 0; j < MAX_LINE_LENGTH; j++) {
            char c = v->Carrosserie[i][j];
            if (c == 0) break;
            if (c != ' ' && c != '\0') {
                gotoxy(MAP_START_X + v->posx + j, MAP_START_Y + v->posy + i);
                // Print solid block for vehicle body if it's # or similar, or just the char
                // Since our vehicles are now ###, we can print block.
                if (c == '#') printf("\u2588");
                else putchar(c);
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
}

void afficher_interface(int tickets, float prix_total, int mode) {
    gotoxy(MAP_START_X + 2, MAP_START_Y + 3);
    printf("\033[36mTOTAL A PAYER : \033[0m");
    gotoxy(MAP_START_X + 2, MAP_START_Y + 4);
    printf("\033[33m%6.2f EUR    \033[0m", prix_total);

    gotoxy(5, 1);
    char mode_str[20];
    switch (mode) {
        case 1: strcpy(mode_str, "FLUIDE"); break;
        case 2: strcpy(mode_str, "CHARGE"); break;
        case 3: strcpy(mode_str, "MANUEL"); break;
        default: strcpy(mode_str, "INCONNU"); break;
    }
    printf("Mode: %s | Tickets Entree: %d", mode_str, tickets);
}
