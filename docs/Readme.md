# Limite de socket sur un noyau linux

## La limite globale du système (file-max)
C'est le nombre total de descripteurs de fichiers que l'ensemble du système (tous les processus confondus) peut ouvrir.

Pour voir ta limite : cat /proc/sys/fs/file-max

Ordre de grandeur : Sur un système moderne, c'est souvent plusieurs millions.

## La limite par processus (ulimit)
C'est celle qui va t'impacter directement. Par défaut, Linux limite souvent chaque processus à 1024 fichiers ouverts.
Si ton moteur essaie d'accepter la 1025ème connexion (le 1025ème Mock), accept() renverra une erreur et ton programme ne pourra plus rien ouvrir.

Pour voir ta limite actuelle : ulimit -n

Soft Limit vs Hard Limit : * Soft : Ce que le processus peut changer lui-même.

Hard : Le plafond maximum autorisé par l'administrateur (root).