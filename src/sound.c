#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void jouer_son(const char *fichier) {
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "play -q '%s' &", fichier);
    system(cmd);
}

void arreter_sons(void) {
    system("kill $(pidof play) 2>/dev/null");
}

