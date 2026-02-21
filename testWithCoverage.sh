#!/bin/bash

# coverage.sh
# Script pour compiler, lancer les tests et générer un rapport de coverage HTML pour AquaSim

set -e

# Répertoire de build
BUILD_DIR="build"

# Créer le build s'il n'existe pas
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo "==> Configuration du projet avec coverage..."
cmake -DENABLE_COVERAGE=ON ..

echo "==> Compilation du projet..."
cmake --build .

echo "==> Lancement des tests..."
./tests/runTests

echo "==> Capture de la couverture..."
lcov --capture --directory CMakeFiles/eventserver.dir --output-file coverage.info

echo "==> Nettoyage des fichiers systèmes..."
lcov --remove coverage.info '/usr/*' --output-file coverage.info --ignore-errors unused

echo "==> Génération du rapport HTML..."
genhtml coverage.info --output-directory coverage-report

# Détection WSL pour ouvrir dans Windows
if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null ; then
    echo "==> Ouverture du rapport dans Windows..."
    explorer.exe coverage-report
else
    echo "==> Ouverture du rapport dans le navigateur..."
    xdg-open coverage-report/index.html || echo "Ouvre manuellement build/coverage-report/index.html"
fi

echo "==> Terminé ! Rapport disponible dans $BUILD_DIR/coverage-report"