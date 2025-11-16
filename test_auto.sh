#!/bin/bash

cd "/home/euloge/projet c"

echo "=== Test automatique du simulateur de parking ==="
echo ""

echo "Test 1: Entrée vide (doit afficher erreur)"
echo "-------------------------------------------"
echo "" | timeout 2 ./bin/parking 2>&1 | grep -q "Entree invalide" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 2: Entrée invalide (lettre)"
echo "--------------------------------"
echo "a" | timeout 2 ./bin/parking 2>&1 | grep -q "Entree invalide" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 3: Choix valide (1 - Mode Fluide)"
echo "--------------------------------------"
echo -e "1\nq" | timeout 3 ./bin/parking 2>&1 | grep -q "FLUIDE" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 4: Choix valide (2 - Mode Charge)"
echo "--------------------------------------"
echo -e "2\nq" | timeout 3 ./bin/parking 2>&1 | grep -q "CHARGE" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 5: Option Règles (4)"
echo "-------------------------"
echo -e "4\n" | timeout 2 ./bin/parking 2>&1 | grep -q "REGLES" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 6: Option Crédits (5)"
echo "--------------------------"
echo -e "5\n" | timeout 2 ./bin/parking 2>&1 | grep -q "CREDITS" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 7: Quitter (0)"
echo "-------------------"
echo "0" | timeout 2 ./bin/parking 2>&1 | grep -q "Au revoir" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "Test 8: Choix invalide (99)"
echo "---------------------------"
echo -e "99\n0" | timeout 2 ./bin/parking 2>&1 | grep -q "Choix invalide" && echo "✓ OK" || echo "✗ ECHEC"

echo ""
echo "=== Tests terminés ==="

