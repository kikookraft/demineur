#include "mat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void up(int lines) { //remonter de x lignes dans la console
    for(int i=0;i<lines;i++) {
        printf("\033[1A");
    }
}

struct neighbour { //structure pour les voisins d'une case
    /* U = up, D = down, l = left, r = right
    . = la case de départ
        UL  U  UR
         L  .  R
        DL  D  DR
    */
    int* U;
    int* UL;
    int* L;
    int* DL;
    int* D;
    int* DR;
    int* R;
    int* UR;
} ;

struct case { //la structure de chaque case du jeux
    int x = NULL; // position x
    int y = NULL; // position y
    int isBomb = 0; // définit si la case est une bombe (0=Faux, 1=Vrai)
    int count = 0; // nombres de mines a proximité
    struct neighbour voisins; // les voisins de la case 
} ;




int main(int argc, char * argv[]) {
    
    return 0;
}
