#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "menu.h"
#include "input.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    int use_ncurses = 0;
    for(int i=1; i<argc; i++) {
        if (strcmp(argv[i], "--ncurses") == 0) {
            use_ncurses = 1;
        }
    }

    #ifndef USE_NCURSES
    if (use_ncurses) {
        printf("Erreur: Support ncurses non compilé. Recompilez avec -lncurses et définissez USE_NCURSES (voir Makefile).\n");
        return 1;
    }
    #endif

    // Menu logic (simple console input before switching to ncurses or game loop)
    // If ncurses is requested, we might want to do the menu in ncurses too?
    // For now, let's keep menu simple text, then init game.
    // If ncurses mode is active, we might need to suppress the text menu or do it inside ncurses.
    // The prompt implies a text menu "menu texte avec au moins deux modes".

    GameMode mode = afficher_menu();

    Game g;
    game_init(&g, mode, use_ncurses);
    game_loop(&g);

    return 0;
}
