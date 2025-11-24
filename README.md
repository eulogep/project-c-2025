# Simulateur de Parking - ESIEA

Projet étudiant de 3ème année - ESIEA (S1)
Auteurs : Jules (Assistant IA) pour l'étudiant

## 🚨 DÉPANNAGE RAPIDE (ERREUR DE COMPILATION)

Si vous rencontrez l'erreur suivante :
```
src/display.c:5:10: fatal error: display.h: No such file or directory
```
Cela signifie que le dossier `include` n'est pas trouvé par le compilateur.

**SOLUTION :**
1. Assurez-vous d'avoir extrait **tous** les fichiers et dossiers du ZIP, en conservant la structure.
2. Vérifiez que vous avez bien le dossier `include` (contenant `display.h`, `engine.h`, etc.) au même niveau que le `Makefile` et le dossier `src`.
3. Lancez la commande `ls -R` (ou regardez dans votre explorateur de fichiers) pour vérifier la présence de :
   - `./Makefile`
   - `./src/display.c`
   - `./include/display.h`

---

## 🚀 Fonctionnalités

*   **Affichage ASCII** : Interface entièrement en texte avec couleurs ANSI.
*   **Véhicules Multi-cellules** : Voitures, Camions et Motos avec des formes distinctes (3x3).
*   **Collisions Réalistes** : Détection de collision entre véhicules et avec les murs/obstacles.
*   **Gestion du Flux** :
    *   Barrière d'entrée s'ouvrant périodiquement.
    *   Barrière de sortie s'ouvrant après paiement.
    *   Ticket de sortie virtuel avec calcul du prix en fonction du temps.
*   **Modes de Jeu** :
    1.  **Fluide** : Trafic modéré.
    2.  **Chargé** : Trafic dense, risque de bouchons.
    3.  **Manuel** : Contrôlez un véhicule avec ZQSD.

## 🛠 Compilation et Exécution

### Prérequis
*   Un système Linux ou WSL (Windows Subsystem for Linux).
*   GCC (Compilateur C).
*   Make.

### Instructions

1.  Ouvrez un terminal dans le dossier du projet.
2.  Compilez le projet :
    ```bash
    make
    ```
3.  Lancez le simulateur :
    ```bash
    ./bin/parking
    ```
4.  Nettoyez les fichiers de compilation (optionnel) :
    ```bash
    make clean
    ```

## 🎮 Contrôles

*   **Dans les menus** : Utilisez le clavier numérique (1-5) pour choisir une option.
*   **En jeu** :
    *   `Q` : Quitter la simulation et revenir au menu.
    *   **Mode Manuel uniquement** :
        *   `Z` : Haut
        *   `S` : Bas
        *   `Q` : Gauche
        *   `D` : Droite

## 📂 Structure du Projet

*   `src/` : Code source (.c)
*   `include/` : En-têtes (.h)
*   `assets/` : Fichiers de données (map.txt, véhicules)
*   `bin/` : Exécutable final
*   `obj/` : Fichiers objets intermédiaires
*   `Makefile` : Script de compilation

## 📝 Notes Techniques

Le projet utilise des structures de données dynamiques (listes chaînées) pour gérer un nombre variable de véhicules. L'affichage est géré par rafraîchissement complet ou partiel de la console à l'aide de codes d'échappement ANSI pour positionner le curseur sans scintillement excessif.
