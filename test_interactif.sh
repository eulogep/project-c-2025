#!/bin/bash

cd "/home/euloge/projet c"

echo "=== Test interactif du menu ==="
echo ""
echo "Test 1: Affichage du menu"
echo "------------------------"
timeout 1 ./bin/parking 2>&1 | head -15 || true

echo ""
echo "Test 2: Sélection d'option (mode 1)"
echo "-----------------------------------"
echo "1" | timeout 2 ./bin/parking 2>&1 | head -20 || true

echo ""
echo "Test 3: Option Règles"
echo "---------------------"
echo -e "4\nq" | timeout 2 ./bin/parking 2>&1 | head -20 || true

echo ""
echo "✓ Tests terminés"
echo ""
echo "Pour tester manuellement:"
echo "  ./bin/parking"
echo "  Tapez 1, 2, 3, 4, 5 ou Q puis Entrée"

