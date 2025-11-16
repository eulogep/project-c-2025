#ifndef DISPLAY_H
#define DISPLAY_H

#include "map.h"
#include "vehicle.h"

void clear_screen(void);
void gotoxy(int x, int y);
void afficher_map(MAP *m);
void afficher_vehicule(VEHICULE *v);
void afficher_vehicules(VEHICULE *liste);
void rafraichir_zone(int x, int y, int w, int h, MAP *m, VEHICULE *liste);
void afficher_interface(int tickets, float prix_total, int mode);

#endif

