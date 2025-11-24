#include <stdlib.h>
#include <time.h>
#include "vehicules.h"

static int id_counter = 0;

Vehicule* creer_vehicule(int x, int y, int type) {
    Vehicule *v = malloc(sizeof(Vehicule));
    if (!v) return NULL;
    v->id = ++id_counter;
    v->x = x;
    v->y = y;
    v->type = type; // 0=IA, 1=Player
    v->dir = DIR_LEFT; // Default
    v->state = 0; // Active
    v->entry_time = time(NULL);
    v->next = NULL;
    return v;
}

void ajouter_vehicule(Vehicule **liste, Vehicule *v) {
    if (!v) return;
    v->next = *liste;
    *liste = v;
}

void supprimer_vehicule(Vehicule **liste, int id) {
    Vehicule *curr = *liste;
    Vehicule *prev = NULL;

    while (curr) {
        if (curr->id == id) {
            if (prev) {
                prev->next = curr->next;
            } else {
                *liste = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void liberer_vehicules(Vehicule *liste) {
    Vehicule *curr = liste;
    while (curr) {
        Vehicule *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}
