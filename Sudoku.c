#include <stdio.h>

#define RED "\033[0;31m"
#define RESET "\033[0m"
#define TAILLE 9

typedef int tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g);
void afficherGrille(tGrille grille);
int possible(tGrille grille, int numLigne, int numColonne, int valeur);
int grilleEstPleine(tGrille grille,tGrille grilleDefault);

int main()
{
    tGrille grille1, grilleDeBase;
    int numLigne, numColonne, valeur;

    chargerGrille(grille1);
    grilleDeBase = grille1;

    while (!grilleEstPleine(grille1)) {
        afficherGrille(grille1);
        printf("Indices de la case ? ");
        scanf("%d %d", &numLigne, &numColonne);

        if (grille1[numLigne][numColonne] != 0) {
            printf("IMPOSSIBLE, la case n'est pas libre.\n");
        } else {
            printf("Valeur a inserer ? ");
            scanf("%d", &valeur);

            if (possible(grille1, numLigne, numColonne, valeur)) {
                grille1[numLigne][numColonne] = valeur;
            }
        }
    }

    printf("Grille pleine, fin de partie\n");

    return 0;
}

void chargerGrille(tGrille grille)
{
    char nomFichier[30];
    FILE * file;

    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);

    file = fopen(nomFichier, "rb");
    if (file==NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(grille, sizeof(int), TAILLE*TAILLE, file);
    }
    fclose(file);
    /*
    liste des grilles:
    Grille1.sud
    ...
    Grille10.sud
    */
}

void afficherGrille(tGrille grille)
{
    printf("    1 2 3   4 5 6   7 8 9  \n");
    printf("  +-------+-------+-------+\n");

    for (int i = 0; i < TAILLE; ++i) {
        if (i % 3 == 0 && i != 0) {
            printf("  +-------+-------+-------+\n"); // Affichage d'une ligne de separation pour les carres de 3x3
        }

        printf("%d | ", i + 1); // Affichage des chiffres a droite (pour choisir la ligne)

        for (int j = 0; j < TAILLE; ++j)
        {
            if (j % 3 == 0 && j != 0)
            {
                printf("| "); // Affichage d'une colonne de separation pour les carres de 3x3
            }

            if (grille[i][j] == 0)
            {
                printf(". "); // Affichage d'un point si valeur vide
            }
            else
            {
                printf(RED "%d " RESET, grille[i][j]); // La valeur sinon
            }
        }
        printf("\n");
    }
    printf("  +-------+-------+-------+\n");
}

int possible(tGrille grille, int numLigne, int numColonne, int valeur) // Retourne 1 si possible, 0 sinon
{}

int grilleEstPleine(tGrille grille, tGrille grilleDefault) // Retourne 1 si pleine, 0 sinon
{}