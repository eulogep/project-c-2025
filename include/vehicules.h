#ifndef VEHICULES_H
#define VEHICULES_H

#include <time.h>

#define DIR_UP    0
#define DIR_DOWN  1
#define DIR_LEFT  2
#define DIR_RIGHT 3

typedef struct Vehicule {
    int id;
    int x, y;
    int dir;
    int type; // 0 = IA, 1 = PLAYER
    time_t entry_time;
    int state; // 0 = Active, 1 = Parked, 2 = Exiting
    struct Vehicule *next;
} Vehicule;

Vehicule* creer_vehicule(int x, int y, int type);
void ajouter_vehicule(Vehicule **liste, Vehicule *v);
void supprimer_vehicule(Vehicule **liste, int id);
void liberer_vehicules(Vehicule *liste);

#endif
