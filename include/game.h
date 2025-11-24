#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "vehicules.h"

typedef enum {
    MODE_FLUIDE = 1,
    MODE_CHARGE = 2,
    MODE_MANUEL = 3
} GameMode;

typedef struct {
    Map *map;
    Vehicule *vehicules;
    GameMode mode;
    int tickets_sold;
    double total_revenue;
    int running;
    int ticks;
    int use_ncurses;
} Game;

void game_init(Game *g, GameMode mode, int use_ncurses);
void game_loop(Game *g);
void game_cleanup(Game *g);

#endif
