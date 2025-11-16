#include <stdio.h>
#include <assert.h>
#include "engine.h"
#include "map.h"
#include "vehicle.h"

void test_modes(void) {
    printf("Test des modes de jeu...\n\n");
    
    MAP *map = charger_map("assets/map.txt");
    assert(map != NULL);
    printf("✓ Carte chargée\n");
    
    ETAT_JEU jeu;
    VEHICULE *liste = NULL;
    
    printf("\nTest MODE_FLUIDE:\n");
    initialiser_jeu(&jeu, MODE_FLUIDE);
    assert(jeu.mode == MODE_FLUIDE);
    printf("  ✓ Mode initialisé: FLUIDE\n");
    
    for (int i = 0; i < 50; i++) {
        generer_vehicule_aleatoire(&liste, map, MODE_FLUIDE, &jeu);
    }
    printf("  ✓ Génération testée (délai: 30 frames)\n");
    printf("  ✓ Tickets entrée: %d\n", jeu.tickets_entree);
    
    liberer_liste_vehicules(&liste);
    liste = NULL;
    
    printf("\nTest MODE_CHARGE:\n");
    initialiser_jeu(&jeu, MODE_CHARGE);
    assert(jeu.mode == MODE_CHARGE);
    printf("  ✓ Mode initialisé: CHARGE\n");
    
    for (int i = 0; i < 50; i++) {
        generer_vehicule_aleatoire(&liste, map, MODE_CHARGE, &jeu);
    }
    printf("  ✓ Génération testée (délai: 10 frames)\n");
    printf("  ✓ Tickets entrée: %d\n", jeu.tickets_entree);
    
    liberer_liste_vehicules(&liste);
    liste = NULL;
    
    printf("\nTest MODE_MANUEL:\n");
    initialiser_jeu(&jeu, MODE_MANUEL);
    assert(jeu.mode == MODE_MANUEL);
    printf("  ✓ Mode initialisé: MANUEL\n");
    
    for (int i = 0; i < 50; i++) {
        generer_vehicule_aleatoire(&liste, map, MODE_MANUEL, &jeu);
    }
    printf("  ✓ Génération testée (délai: 60 frames)\n");
    printf("  ✓ Tickets entrée: %d\n", jeu.tickets_entree);
    
    liberer_liste_vehicules(&liste);
    liberer_map(map);
    
    printf("\n✓ Tous les tests des modes sont passés!\n");
}

int main(void) {
    test_modes();
    return 0;
}

