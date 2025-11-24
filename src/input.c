#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "input.h"

static struct termios oldt, newt;

void input_init(void) {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void input_cleanup(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int kbhit(void) {
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int read_char(void) {
    return getchar();
}
