#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "input.h"

static struct termios oldt, newt;
static int old_flags;

void input_init(void) {
    // Save terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Save and set file status flags
    old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
}

void input_cleanup(void) {
    // Restore terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Restore file status flags
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
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
