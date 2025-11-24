#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#ifdef USE_NCURSES
#include <ncurses.h>
#endif

#include "game.h"
#include "input.h"
#include "vehicules.h"
#include "map.h"
#include <unistd.h>

#define SPAWN_RATE_FLUIDE 20
#define SPAWN_RATE_CHARGE 5

// --- HELPER FUNCTIONS DECLARATIONS ---
static void render_game(Game *g);
static void update_physics(Game *g);
static void handle_input(Game *g);
static void spawn_vehicles(Game *g);
static int is_free(Game *g, int x, int y);
static void draw_ascii(Game *g);
#ifdef USE_NCURSES
static void draw_ncurses(Game *g);
#endif

// --- IMPLEMENTATION ---

void game_init(Game *g, GameMode mode, int use_ncurses) {
    g->map = charger_map("plan.txt");
    if (!g->map) {
        fprintf(stderr, "Erreur: Impossible de charger plan.txt\n");
        exit(1);
    }
    g->vehicules = NULL;
    g->mode = mode;
    g->tickets_sold = 0;
    g->total_revenue = 0.0;
    g->running = 1;
    g->ticks = 0;
    g->use_ncurses = use_ncurses;

    if (g->mode == MODE_MANUEL) {
        // Find entry to spawn player
        for(int y=0; y<g->map->hauteur; y++) {
            for(int x=0; x<g->map->largeur; x++) {
                if (get_case(g->map, x, y) == 'E') {
                    Vehicule *v = creer_vehicule(x, y, 1);
                    v->dir = DIR_LEFT;
                    ajouter_vehicule(&g->vehicules, v);
                    goto found_entry;
                }
            }
        }
        found_entry:;
    }
}

void game_loop(Game *g) {
    if (!g->use_ncurses) {
        input_init();
    }
    #ifdef USE_NCURSES
    else {
        initscr();
        noecho();
        cbreak();
        curs_set(FALSE);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        start_color();

        // Colors mapping
        init_pair(1, COLOR_CYAN, COLOR_BLACK);   // Walls/Borders
        init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Free Spots (Vertical Bars)
        init_pair(3, COLOR_RED, COLOR_BLACK);    // Occupied Spots
        init_pair(4, COLOR_MAGENTA, COLOR_BLACK);// Player
        init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Text
        init_pair(6, COLOR_WHITE, COLOR_BLACK);  // Road
        init_pair(7, COLOR_CYAN, COLOR_BLACK);   // Panel Borders
        init_pair(8, COLOR_BLACK, COLOR_CYAN);   // Panel Header (Cyan BG)
        init_pair(9, COLOR_WHITE, COLOR_RED);    // Car type 1
        init_pair(10, COLOR_WHITE, COLOR_YELLOW);// Car type 2
        init_pair(11, COLOR_BLACK, COLOR_WHITE); // Car Windows/Detail
    }
    #endif

    while (g->running) {
        handle_input(g);
        update_physics(g);
        render_game(g);

        usleep(100000); // 100ms
        g->ticks++;
    }

    game_cleanup(g);
}

void game_cleanup(Game *g) {
    if (g->use_ncurses) {
        #ifdef USE_NCURSES
        endwin();
        #endif
    } else {
        input_cleanup();
    }
    liberer_vehicules(g->vehicules);
    liberer_map(g->map);
}

static void handle_input(Game *g) {
    int key = -1;

    #ifdef USE_NCURSES
    if (g->use_ncurses) {
        key = getch();
    } else {
    #endif
        if (kbhit()) {
            key = read_char();
        }
    #ifdef USE_NCURSES
    }
    #endif

    if (key == 'q' || key == 'Q') {
        g->running = 0;
        return;
    }

    if (g->mode == MODE_MANUEL) {
        Vehicule *player = NULL;
        for(Vehicule *v = g->vehicules; v; v=v->next) {
            if(v->type == 1) { player = v; break; }
        }

        if (player) {
            int dx = 0, dy = 0;
            if (key == 'z' || key == 'Z') dy = -1;
            if (key == 's' || key == 'S') dy = 1;
            if (key == 'q' || key == 'Q') dx = -1;
            if (key == 'd' || key == 'D') dx = 1;

            #ifdef USE_NCURSES
            if (g->use_ncurses) {
                if (key == KEY_UP) dy = -1;
                if (key == KEY_DOWN) dy = 1;
                if (key == KEY_LEFT) dx = -1;
                if (key == KEY_RIGHT) dx = 1;
            }
            #endif

            if (dx != 0 || dy != 0) {
                int nx = player->x + dx;
                int ny = player->y + dy;
                if (!est_mur(g->map, nx, ny)) {
                   if (is_free(g, nx, ny)) {
                       player->x = nx;
                       player->y = ny;
                       if (dx == 1) player->dir = DIR_RIGHT;
                       if (dx == -1) player->dir = DIR_LEFT;
                       if (dy == 1) player->dir = DIR_DOWN;
                       if (dy == -1) player->dir = DIR_UP;
                   }
                }
            }
        }
    }
}

static int is_free(Game *g, int x, int y) {
    if (x < 0 || x >= g->map->largeur || y < 0 || y >= g->map->hauteur) return 0;
    if (est_mur(g->map, x, y)) return 0;

    Vehicule *v = g->vehicules;
    while (v) {
        if (v->x == x && v->y == y) return 0;
        v = v->next;
    }
    return 1;
}

static void spawn_vehicles(Game *g) {
    int rate = (g->mode == MODE_FLUIDE) ? SPAWN_RATE_FLUIDE : SPAWN_RATE_CHARGE;
    if (g->mode == MODE_MANUEL) rate = SPAWN_RATE_FLUIDE * 2;

    if (g->ticks % rate == 0) {
         for(int y=0; y<g->map->hauteur; y++) {
            for(int x=0; x<g->map->largeur; x++) {
                if (get_case(g->map, x, y) == 'E') {
                    if (is_free(g, x, y)) {
                         Vehicule *v = creer_vehicule(x, y, 0);
                         v->dir = DIR_LEFT;
                         ajouter_vehicule(&g->vehicules, v);
                         goto spawned;
                    } else {
                        // Try adjacent
                         int dx[] = {-1, 0, 1, 0};
                        int dy[] = {0, -1, 0, 1};
                        for(int k=0; k<4; k++) {
                            int nx = x+dx[k], ny = y+dy[k];
                            if (!est_mur(g->map, nx, ny) && is_free(g, nx, ny)) {
                                Vehicule *v = creer_vehicule(nx, ny, 0);
                                v->dir = DIR_LEFT;
                                ajouter_vehicule(&g->vehicules, v);
                                goto spawned;
                            }
                        }
                    }
                }
            }
        }
    }
    spawned:;
}

static void update_physics(Game *g) {
    spawn_vehicles(g);

    Vehicule *v = g->vehicules;
    Vehicule *prev = NULL;

    while (v) {
        if (v->type == 1) { // Player
             if (est_sortie(g->map, v->x, v->y)) {
                time_t now = time(NULL);
                double diff = difftime(now, v->entry_time);
                double price = diff * 0.5;
                g->tickets_sold++;
                g->total_revenue += price;

                 for(int y=0; y<g->map->hauteur; y++) {
                    for(int x=0; x<g->map->largeur; x++) {
                        if (get_case(g->map, x, y) == 'E') {
                            v->x = x; v->y = y; v->entry_time = time(NULL);
                        }
                    }
                 }
            }
            prev = v;
            v = v->next;
            continue;
        }

        // IA Movement
        int dx=0, dy=0;
        if (v->dir == DIR_UP) dy=-1;
        else if (v->dir == DIR_DOWN) dy=1;
        else if (v->dir == DIR_LEFT) dx=-1;
        else if (v->dir == DIR_RIGHT) dx=1;

        int nx = v->x + dx;
        int ny = v->y + dy;

        int blocked = 0;
        if (!is_free(g, nx, ny)) blocked = 1;

        if (!blocked) {
            if (rand()%20 == 0) {
                 v->dir = rand()%4;
            } else {
                 v->x = nx;
                 v->y = ny;
            }
        } else {
            int dirs[4] = {0, 1, 2, 3};
            for(int i=0; i<4; i++) {
                int r = rand()%4;
                int temp = dirs[i]; dirs[i] = dirs[r]; dirs[r] = temp;
            }

            for(int i=0; i<4; i++) {
                int test_dir = dirs[i];
                int tdx=0, tdy=0;
                if (test_dir == DIR_UP) tdy=-1;
                else if (test_dir == DIR_DOWN) tdy=1;
                else if (test_dir == DIR_LEFT) tdx=-1;
                else if (test_dir == DIR_RIGHT) tdx=1;

                if (is_free(g, v->x + tdx, v->y + tdy)) {
                    v->dir = test_dir;
                    break;
                }
            }
        }

        if (est_sortie(g->map, v->x, v->y)) {
             g->tickets_sold++;
             g->total_revenue += 2.50;

             Vehicule *to_free = v;
             if (prev) prev->next = v->next;
             else g->vehicules = v->next;
             v = v->next;
             free(to_free);
             continue;
        }

        prev = v;
        v = v->next;
    }
}

static void render_game(Game *g) {
    if (g->use_ncurses) {
        #ifdef USE_NCURSES
        draw_ncurses(g);
        #endif
    } else {
        draw_ascii(g);
    }
}

static void draw_ascii(Game *g) {
    printf("\033[2J\033[1;1H"); // Clear

    printf("SORTIE -- TOTAL: %.2f EUR  |  TICKETS: %d\n", g->total_revenue, g->tickets_sold);

    for (int y = 0; y < g->map->hauteur; y++) {
        for (int x = 0; x < g->map->largeur; x++) {
            Vehicule *found = NULL;
            for(Vehicule *v = g->vehicules; v; v=v->next) {
                if (v->x == x && v->y == y) { found = v; break; }
            }

            if (found) {
                if (found->type == 1) printf("\033[35m@\033[0m");
                else printf("\033[36mV\033[0m");
            } else {
                char c = g->map->grille[y][x];
                if (c == '#') printf("\033[31m#\033[0m");
                else if (c == 'P') printf("\033[32mP\033[0m");
                else if (c == 'E' || c == 'S') printf("\033[33m%c\033[0m", c);
                else printf("%c", c);
            }
        }
        printf("\n");
    }
    printf("ENTREE -- PRENEZ VOTRE TICKET\n");
    printf("Controles: ZQSD (Manuel) | q (Quitter)\n");
}

#ifdef USE_NCURSES
static void draw_ncurses(Game *g) {
    erase();

    // --- PANELS ---

    // TOP LEFT: "SORTIE" (Small) & "TOTAL" (Larger)
    // Box 1: SORTIE
    attron(COLOR_PAIR(8)); // Cyan BG, Black Text? Or just Cyan Border
    // Replicating image: "SORTIE" is a small tag on top of the box?
    // Image: [SORTIE] box, then [TOTAL] box below.

    // Sortie Tag
    attron(COLOR_PAIR(7)); // Cyan
    mvprintw(0, 0, "+--------+");
    mvprintw(1, 0, "| SORTIE |");
    mvprintw(2, 0, "+--------+");

    // Total Box
    mvprintw(3, 0, "+---------------------+");
    mvprintw(4, 0, "| TOTAL A PAYER :     |");
    mvprintw(5, 0, "| %6.2f EUR          |", g->total_revenue);
    mvprintw(6, 0, "+---------------------+");
    attroff(COLOR_PAIR(7));

    // BOTTOM RIGHT: "BONJOUR" (Large) & "ENTREE" (Small)
    int my = g->map->hauteur + 2; // Offset below map
    int mx = g->map->largeur - 25;
    if (mx < 0) mx = 0;

    // Bonjour Box
    attron(COLOR_PAIR(7));
    mvprintw(my, mx,   "+-----------------------+");
    mvprintw(my+1, mx, "| BONJOUR :             |");
    mvprintw(my+2, mx, "| PRENEZ VOTRE TICKET   |");
    mvprintw(my+3, mx, "+-----------------------+");

    // Entree Tag
    mvprintw(my+4, mx+13, "+--------+");
    mvprintw(my+5, mx+13, "| ENTREE |");
    mvprintw(my+6, mx+13, "+--------+");
    attroff(COLOR_PAIR(7));

    // --- MAP ---
    int offset_y = 4; // Below top panel
    int offset_x = 2; // Right of left border

    for (int y = 0; y < g->map->hauteur; y++) {
        for (int x = 0; x < g->map->largeur; x++) {
            char c = g->map->grille[y][x];
            int px = offset_x + x;
            int py = offset_y + y;

            Vehicule *found = NULL;
            for(Vehicule *v = g->vehicules; v; v=v->next) {
                if (v->x == x && v->y == y) { found = v; break; }
            }

            if (found) {
                // Car Logic
                int col = (found->type == 1) ? 4 : (found->id % 2 ? 9 : 10);
                attron(COLOR_PAIR(col));
                // Draw a box-like car?
                // e.g. [ ]
                mvaddch(py, px, ' ' | A_REVERSE); // Solid block of color
                attroff(COLOR_PAIR(col));

                // Add detail?
                // mvaddch(py, px, ACS_DIAMOND);
            } else {
                if (c == '#') {
                    // Wall -> Cyan Line
                    attron(COLOR_PAIR(1));
                    // Simple logic for corners could be added here, but simple line is safer
                    // Let's use ACS_HLINE / VLINE based on neighbors?
                    // Too complex for now, use CKBOARD or #
                    // Image shows thin lines.
                    // Let's try to use standard ACS_HLINE if y is constant border?
                    // Use a solid line char if available
                    mvaddch(py, px, ACS_HLINE);
                    // Actually HLINE looks weird for vertical walls.
                    // Fallback to simple '#' or '+' if ACS logic is hard without neighbor context
                    // Let's stick to ACS_CKBOARD or just a cyan space?
                    // mvaddch(py, px, ' ' | A_REVERSE); // Solid cyan wall
                    attroff(COLOR_PAIR(1));
                }
                else if (c == 'P') {
                    // Parking Spot -> Green Vertical Line
                    attron(COLOR_PAIR(2));
                    mvaddch(py, px, ACS_VLINE);
                    attroff(COLOR_PAIR(2));
                }
                else if (c == 'E' || c == 'S') {
                    attron(COLOR_PAIR(5) | A_BLINK);
                    mvaddch(py, px, c);
                    attroff(COLOR_PAIR(5) | A_BLINK);
                }
                else {
                     // Empty space
                     // mvaddch(py, px, ' ');
                }
            }
        }
    }

    // Draw Map Border manually to look clean?
    // attron(COLOR_PAIR(1));
    // rectangle(offset_y-1, offset_x-1, offset_y+g->map->hauteur, offset_x+g->map->largeur);
    // attroff(COLOR_PAIR(1));

    refresh();
}
#endif
