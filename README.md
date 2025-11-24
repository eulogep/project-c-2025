# Simulateur de Parking ESIEA - Projet C

## Description
Ce projet est un simulateur de parking réalisé dans le cadre du cours de langage C à l'ESIEA.
Il simule un parking vu de dessus avec gestion des véhicules, collisions, temps de stationnement et paiement.

## Fonctionnalités
- **Chargement de plan** : Le plan du parking est chargé depuis `plan.txt`.
- **Modes de jeu** :
  - **Fluide** : Circulation calme.
  - **Chargé** : Circulation dense, risques de bouchons.
  - **Manuel** : Le joueur contrôle un véhicule (Magenta/Rouge) avec ZQSD.
- **Gestion des véhicules** : Entrées, sorties, stationnement, collisions (simple blocage).
- **Paiement** : Calcul du prix en fonction du temps passé (0.5€/unité de temps).
- **Affichage** :
  - Mode ASCII standard (console).
  - Mode Graphique (Bonus) utilisant `ncurses`.

## Compilation
Le projet nécessite `gcc` et `make`. La bibliothèque `ncurses` est requise pour le mode graphique.

### Pré-requis (Linux/WSL)
```bash
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev
```

### Compiler
```bash
make
```
Cela génère l'exécutable `bin/parking`.

### Nettoyer
```bash
make clean
```

## Exécution

### Mode Standard (ASCII)
```bash
./bin/parking
```

### Mode Graphique (Ncurses)
```bash
./bin/parking --ncurses
```

## Contrôles
- **Menu** : Entrez le numéro du mode (1, 2, 3) ou 4 pour quitter.
- **En jeu** :
  - `Q` : Quitter la simulation.
  - **Mode Manuel** :
    - `Z` : Haut
    - `S` : Bas
    - `Q` : Gauche
    - `D` : Droite
    - (Les flèches directionnelles fonctionnent aussi en mode ncurses).

## Structure du projet
- `src/` : Code source (.c)
  - `main.c` : Point d'entrée.
  - `game.c` : Logique du jeu et boucle principale.
  - `map.c` : Gestion de la carte.
  - `vehicules.c` : Gestion de la liste chaînée des véhicules.
  - `input.c` : Gestion des entrées clavier non bloquantes.
  - `menu.c` : Menu principal.
- `include/` : En-têtes (.h)
- `plan.txt` : Plan du parking (éditable).
- `Makefile` : Script de compilation.

## Auteurs
Projet étudiant ESIEA.
