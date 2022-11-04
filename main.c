//#include "mat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <termios.h>
#include <string.h>


void up(int lines) { //remonter de x lignes dans la console
    for(int i=0;i<lines;i++) {
        printf("\033[1A");
    }
}

void printcolor(int fg, int bg) { //affichage de la couleur
    printf("\033[%d;%dm", fg, bg);
}

struct colors {
    char *red;
    char *green;
    char *yellow;
    char *blue;
    char *magenta;
    char *cyan;
    char *white;
    char *reset;
    char *clear;
};

struct colors textcolor = {
    .red = "\033[31m",
    .green = "\033[32m",
    .yellow = "\033[33m",
    .blue = "\033[34m",
    .magenta = "\033[35m",
    .cyan = "\033[36m",
    .white = "\033[37m",
    .reset = "\033[0m",
    .clear = "\033[2J"
};

struct coords {
    int x;
    int y;
};

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
    struct coords coord;
    int isBomb; // définit si la case est une bombe (0=Faux, 1=Vrai)
    int count; // nombres de mines a proximité
    struct neighbour voisins; // les voisins de la case 
    int isRevealed; // définit si la case est révélée (0=Faux, 1=Vrai)
    int isFlagged; // définit si la case est marquée (0=Faux, 1=Vrai)
    int isSelected; // définit si la case est selectionnée (0=Faux, 1=Vrai)
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
            mat[i][j].coord.x = i;
            mat[i][j].coord.y = j;
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
            mat[i][j].isRevealed = 0;
            mat[i][j].isFlagged = 0;
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
            mat[x][y].count = 9;
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

void printMatCheat(struct celule ** mat, int size) { //affichage de la matrice
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isBomb == 1) {
                printcolor(30, 41);
                printf("💣");
            } else if (mat[i][j].isRevealed == 1) {
                int color1 = 30;
                int color2 = 40;
                switch (mat[i][j].count) { //couleur des chiffres
                    case 0:
                        color1 = 30;
                        break;
                    case 1:
                        color1 = 34;
                        break;
                    case 2:
                        color1 = 32;
                        break;
                    case 3:
                        color1 = 31;
                        break;
                    default:
                        color1 = 31;
                        break;
                }
                printcolor(color1, color2);
                printf("%d ", mat[i][j].count);
            } else if (mat[i][j].isFlagged == 1) {
                printcolor(30, 44);
                printf("🚩");
            } else {
                printcolor(30, 47);
                printf("  ");
            }
        }
        printcolor(0, 0);
        printf("\n");
    }
}

void printPlayerMat(struct celule ** mat, int size, struct coords pos) { //affichage de la matrice
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isRevealed == 1) {
                int color1 = 30;
                int color2 = 40;
                if (i == pos.x && j == pos.y) {
                    color2 = 43;
                }
                switch (mat[i][j].count) { //couleur des chiffres
                    case 0:
                        color1 = 30;
                        break;
                    case 1:
                        color1 = 34;
                        break;
                    case 2:
                        color1 = 32;
                        break;
                    case 3:
                        color1 = 31;
                        break;
                    default:
                        color1 = 31;
                        break;
                }
                printcolor(color1, color2);
                printf("%d ", mat[i][j].count);
            } else if (mat[i][j].isFlagged == 1) {
                printcolor(30, 44);
                printf("🚩");
            } else if (i == pos.x && j == pos.y) {
                    printcolor(30, 43);
                    printf("  ");
            } else {
                printcolor(30, 47);
                printf("  ");
            }
        }
        printcolor(0, 0);
        printf("\n");
    }
}

void discover(struct celule ** mat, int size, int x, int y) { //découverte des cases
    if (mat[x][y].isRevealed == 0) {
        mat[x][y].isRevealed = 1;
        if (mat[x][y].count == 0) {
            if (mat[x][y].voisins.U != NULL && mat[x][y].voisins.U->isRevealed == 0) {
                discover(mat, size, x-1, y);
            }
            if (mat[x][y].voisins.UL != NULL && mat[x][y].voisins.UL->isRevealed == 0) {
                discover(mat, size, x-1, y-1);
            }
            if (mat[x][y].voisins.L != NULL && mat[x][y].voisins.L->isRevealed == 0) {
                discover(mat, size, x, y-1);
            }
            if (mat[x][y].voisins.DL != NULL && mat[x][y].voisins.DL->isRevealed == 0) {
                discover(mat, size, x+1, y-1);
            }
            if (mat[x][y].voisins.D != NULL && mat[x][y].voisins.D->isRevealed == 0) {
                discover(mat, size, x+1, y);
            }
            if (mat[x][y].voisins.DR != NULL && mat[x][y].voisins.DR->isRevealed == 0) {
                discover(mat, size, x+1, y+1);
            }
            if (mat[x][y].voisins.R != NULL && mat[x][y].voisins.R->isRevealed == 0) {
                discover(mat, size, x, y+1);
            }
            if (mat[x][y].voisins.UR != NULL && mat[x][y].voisins.UR->isRevealed == 0) {
                discover(mat, size, x-1, y+1);
            }
        }
    }
}

char getTerlinalKeyPress() { //detection des touches
    //je sais plus d'où j'ai copié la source de cette fonction
    char c;
    struct termios oldattr, newattr;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    c = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return c;
}

void getCusrorPosition(struct coords* pos, int* stop, int size, struct celule** mat) { //recuperation de la position du curseur
    char c;
    c = getTerlinalKeyPress();

    if (c == 'z' && pos->x > 0) { //haut
        pos->x--;
    } else if (c == 's' && pos->x < size-1) { //bas
        pos->x++;
    } else if (c == 'q' && pos->y > 0) { //gauche
        pos->y--;
    } else if (c == 'd' && pos->y < size-1) { //droite
        pos->y++;
    } else if (c == 'a') { //demine
        discover(mat, size, pos->x, pos->y);
    } else if (c == 'e') { //flag
        mat[pos->x][pos->y].isFlagged = !mat[pos->x][pos->y].isFlagged;
    } else if (c == 'r') { //quit
        *stop = 1;
    }
}

int loose(struct celule** mat, int size) { //perdu
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isBomb == 1 && mat[i][j].isRevealed == 1) {
                return 1;
            }
        }
    }
    return 0;
}

int win(struct celule** mat, int size, int nbBomb) { //gagné
    int count = 0;
    for(int i=0;i<size;i++) {
        for(int j=0;j<size;j++) {
            if (mat[i][j].isRevealed == 1 && mat[i][j].isBomb == 0) {
                count++;
            }
        }
    }
    if (count == size*size - nbBomb) {
        return 1;
    }
}

int main(int argc, char const *argv[]) {
    //gérer les parametres
    int size = 20;
    int nbBombs = 50;
    if (argc > 1 && argc != 3) {
        printf("Utilisation : ./demineur [taille_grille] [nombre_bombes]");
    } else if (argc == 3) {
        size = atoi(argv[1]);
        nbBombs = atoi(argv[2]);
        printf("Jeux sur une grille de %d cases avec %d bombes\n", size, nbBombs);
    }


    //initialisations du début
    srand(time(NULL));
    int stop=0;
    struct coords pos = {0, 0};

    //initialisation de la matrice
    struct celule ** mat = initMat(size);
    if (mat == NULL) {
        printf("Erreur d'allocation de la matrice");
        return 1;
    }

    //initialisation de chaques cellules
    printf("\e[1;1H\e[2J");
    setNeighbours(mat, size);
    setBombs(mat, size, nbBombs);
    setCount(mat, size);

    //découverte du début
    int x,y;
    int init_done=0;
    while (init_done == 0) {
        x = rand() % size;
        y = rand() % size;
        if (mat[x][y].count != 0) {
            continue;
        } else {
            discover(mat, size, x, y);
            init_done = 1;
        }
    }

    //boucle principale
    while (stop == 0) {
        up(size+6); //on remonte le curseur pour ne pas effacer le terminal
        printf("Contrôles:\nz q s d pour se déplacer,   \na pour découvrir une case, \ne pour marquer une case,\nr pour quitter\n");
        printf("Position du curseur   x: %d, y: %d, %d Bombes   \n", pos.x, pos.y, nbBombs);
        printPlayerMat(mat, size, pos); //affichage de la matrice
        getCusrorPosition(&pos, &stop, size, mat); //recuperation de la position du curseur
        if (loose(mat, size) == 1) { //perdu
            printf("Vous avez perdu !\n");
            stop = 1;
        } else if (win(mat, size, nbBombs) == 1) { //gagné
            printf("Vous avez gagné !\n");
            stop = 1;
        }
    }

    // afficher la matrice complète
    up(size+1);
    printMatCheat(mat, size);

    //liberer les tableaux
    for (int i = 0; i < size; i++) {
        free(mat[i]);
    }
    free(mat);
    printf("\n\n");
    return 0;
}
