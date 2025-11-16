#!/bin/bash

echo "=== Test des modes de jeu ==="
echo ""

cd "/home/euloge/projet c"

echo "Test 1: Vérification de la compilation..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "✓ Compilation réussie"
else
    echo "✗ Erreur de compilation"
    exit 1
fi

echo ""
echo "Test 2: Vérification de l'exécutable..."
if [ -f "./bin/parking" ]; then
    echo "✓ Exécutable créé"
else
    echo "✗ Exécutable manquant"
    exit 1
fi

echo ""
echo "Test 3: Vérification des fichiers assets..."
if [ -f "assets/map.txt" ] && [ -f "assets/vehicles/car.txt" ]; then
    echo "✓ Assets présents"
else
    echo "✗ Assets manquants"
    exit 1
fi

echo ""
echo "=== Tests terminés ==="
echo ""
echo "Pour tester manuellement les modes:"
echo "  ./bin/parking"
echo "  Puis appuyez sur 1, 2 ou 3 pour les différents modes"
echo "  Appuyez sur Q pour quitter"

