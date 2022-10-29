/*
Module pour gérer les matrices
et les tableuaux simples
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// char LINE_UP = '\033[1A';
// char LINE_CLEAR = '\x1b[2K';

// TABLEAUX SIMPLES

void tabPrint(float ** tab, int row, int cols) { //afficher les tableaux
    for (int i=0; i<row; i++) {
        for (int j=0; j<cols; j++) {
            printf("%.2f ", tab[i][j]);
        }
        printf("\n");
    }
}

float * initTabAvecZero(int row) { // créer et initialiser des tableau simples avec des zeros
    float * tab = malloc(row*sizeof(float*));
    if (tab==NULL) { // sortir si erreur d'aloc
        printf("Erreur d'allocation!");
        return NULL;
    }
    for (int i=0; i<row; i++) {
        tab[i] = 0;
    }
    return tab;
}

// TABLEAUX A 2 DIMENSIONS

float * initTab2DCopy(int row, int cols, float * mat) { // créer et initialiser des tableau simples avec un autre tableau
    float * tab = malloc(row*sizeof(float*));
    if (tab==NULL) { // sortir si erreur d'aloc
        printf("Erreur d'allocation!");
        return NULL;
    }
    for (int i=0; i<row; i++) {
        tab[i] = mat[i];
    }
    return tab;
}

void initTab2d(float ** tab, int row, int cols) { //afficher les tableaux 2d
    for (int i=0; i<row; i++) {
        for (int j=0; j<cols; j++) {
            tab[i][j] = 0;
        }
    }
}

void manualInitTab2D(float ** tab, int row, int cols) { // remplir la matrice a la main
    printf("Remplace les valeurs de la matrice:\n");
    tabPrint(tab, row, cols);
    for (int t = 0; t < row; t++) {
        printf("\033[1A");
    }
    for (int i=0; i<row; i++) {
        for (int j=0; j<cols; j++) {
            scanf("%f", &tab[i][j]);
            printf("\033[1A");
            for (int f = 0; f < j+1; f++) {
                printf("%.2f ", tab[i][f]);
            }
        }
        printf("\n");
    }
}

void tabPrint2D(float ** tab, int row, int cols) { //afficher les tableaux 2D
    for (int i=0; i<row; i++) {
        for (int j=0; j<cols; j++) {
            printf("%.2f ", tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

float ** createTab(int row, int cols) { // créer des tableaux 2D
    float ** tab = malloc(row*sizeof(float*));
    if (tab==NULL) { // sortir si erreur d'aloc
        printf("Erreur d'allocation!");
        return NULL;
    }
    for (int i=0; i<row; i++) {
        tab[i] = malloc(cols*sizeof(float));
        if (tab[i]==NULL) { // tout libérer si erreur d'alloc
            printf("Erreur d'allocation!");
            for (int j=0; j<i; j++) {
                free(tab[j]);
            }
            return NULL;
        }
        //tab[i] = tab2; // ajouter la ligne
    }
    return tab;   
}

void tab2DFree(float ** tab, int row, int cols) { // libérer la mémoire des tableaux 2D
    for (int i=0; i<row; i++) {
        free(tab[i]);
        }
    free(tab);
    tab = NULL;
    printf("Tableau libéré!\n");
}