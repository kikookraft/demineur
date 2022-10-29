#include "mat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void up(int lines) { //remonter de x lignes dans la console
    for(int i=0;i<lines;i++) {
        printf("\033[1A");
    }
}

void clear() { //effacer la console
    printf("\033[2J");
}

struct neighbour { //structure pour les voisins d'une case
    /* U = up, D = down, l = left, r = right
    . = la case de départ
        UL  U  UR
         L  .  R
        DL  D  DR
    */
    struct celule * U;
    struct celule * UL;
    struct celule * L;
    struct celule * DL;
    struct celule * D;
    struct celule * DR;
    struct celule * R;
    struct celule * UR;
} ;

struct celule { //la structure de chaque case du jeux
    int x; // position x
    int y; // position y
    int isBomb; // définit si la case est une bombe (0=Faux, 1=Vrai)
    int count; // nombres de mines a proximité
    struct neighbour voisins; // les voisins de la case 
} ;

struct celule ** initMat(int size) { //initialisation de la matrice
    struct celule ** mat = malloc(size * sizeof(struct celule *)); //allocation de la matrice
    if (mat == NULL) {return NULL;} //si l'allocation a échoué
    for(int i=0;i<size;i++) {
        mat[i] = malloc(size * sizeof(struct celule));
        if (mat[i] == NULL) {
            for(int j=0;j<i;j++) {
                free(mat[j]);
            }
            free(mat);
            return NULL;
        }
    }
    for(int i=0;i<size;i++) { //initialisation des cases
        for(int j=0;j<size;j++) {
            mat[i][j].x = i;
            mat[i][j].y = j;
            mat[i][j].isBomb = 0;
            mat[i][j].count = 0;
            mat[i][j].voisins.U = NULL;
            mat[i][j].voisins.UL = NULL;
            mat[i][j].voisins.L = NULL;
            mat[i][j].voisins.DL = NULL;
            mat[i][j].voisins.D = NULL;
            mat[i][j].voisins.DR = NULL;
            mat[i][j].voisins.R = NULL;
            mat[i][j].voisins.UR = NULL;
        }
    }
    return mat;
}

void freeMat(struct celule ** mat, int size) { //libération de la matrice
    for(int i=0;i<size;i++) {
        free(mat[i]);
    }
    free(mat);
}

void setNeighbours(struct celule ** mat, int size) { //définition des voisins de chaque case
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (i>0) {
                mat[i][j].voisins.U = &mat[i-1][j];
                if (j>0) {
                    mat[i][j].voisins.UL = &mat[i-1][j-1];
                }
                if (j<size-1) {
                    mat[i][j].voisins.UR = &mat[i-1][j+1];
                }
            }
            if (i<size-1) {
                mat[i][j].voisins.D = &mat[i+1][j];
                if (j>0) {
                    mat[i][j].voisins.DL = &mat[i+1][j-1];
                }
                if (j<size-1) {
                    mat[i][j].voisins.DR = &mat[i+1][j+1];
                }
            }
            if (j>0) {
                mat[i][j].voisins.L = &mat[i][j-1];
            }
            if (j<size-1) {
                mat[i][j].voisins.R = &mat[i][j+1];
            }
        }
    }
}

void setBombs(struct celule ** mat, int size, int nbBombs) { //définition des bombes
    srand(time(NULL));
    int x,y;
    for(int i=0;i<nbBombs;i++) {
        x = rand() % size;
        y = rand() % size;
        if (mat[x][y].isBomb == 0) {
            mat[x][y].isBomb = 1;
        } else {
            i--;
        }
    }
}

void setCount(struct celule ** mat, int size) { //définition du nombre de bombes a proximité
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isBomb == 0) {
                if (mat[i][j].voisins.U != NULL && mat[i][j].voisins.U->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.UL != NULL && mat[i][j].voisins.UL->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.L != NULL && mat[i][j].voisins.L->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.DL != NULL && mat[i][j].voisins.DL->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.D != NULL && mat[i][j].voisins.D->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.DR != NULL && mat[i][j].voisins.DR->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.R != NULL && mat[i][j].voisins.R->isBomb == 1) {
                    mat[i][j].count++;
                }
                if (mat[i][j].voisins.UR != NULL && mat[i][j].voisins.UR->isBomb == 1) {
                    mat[i][j].count++;
                }
            }
        }
    }
}

void printMatCheat(struct celule ** mat, int size) { //affichage de la matrice avec compte et bombes
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isBomb == 1) {
                printf("B ");
            } else {
                printf("%d ", mat[i][j].count);
            }
        }
        printf("\n");
    }
}

int main(int argc, char * argv[]) {
    int size = 20; // taille de la matrice
    int nbBombs = 50; // nombre de bombes
    struct celule ** mat = initMat(size);
    if (mat == NULL) {
        printf("Erreur d'allocation de la matrice");
        return 1;
    }
    setNeighbours(mat, size);
    setBombs(mat, size, nbBombs);
    setCount(mat, size);
    printMatCheat(mat, size);
    return 0;
}
