# Jeux du Démineur
Ce projet est un jeu du démineur en ligne de commande.

## Installation
Pour installer le projet, il faut:
1. Cloner le projet: `git clone https://github.com/kikookraft/demineur.git`
2. Aller dans le dossier: `cd demineur`
3. Compiler le projet: `gcc main.c -o demineur -lncurses`
4. Lancer le projet: `./demineur`


## Utilisation
Pour jouer vous pouvez simplement lancer le programme avec `./demineur`.
Cela lancera une partie avec une grille de 20x20 et 50 mines.

Vous pouvez aussi lancer le programme avec des arguments:
`./demineur [taille] [nombre de mines]`

Par exemple, pour une grille de 30x30 et 100 mines, vous pouvez lancer le programme avec:
`./demineur 30 100`

## Controles
Pour jouer, vous pouvez utiliser les touches suivantes:
- `z q s d` pour deplacer le curseur
- `a` pour dévoiler une case
- `e` pour marquer/oublier une case
- `r` pour quitter

Si vous souhaitez changer les touches, vous pouvez le faire dans le fichier `main.c` en modifiant la fonction `getCusrorPosition()` vers la ligne 346.