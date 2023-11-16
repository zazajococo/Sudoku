#include <stdio.h>

#define TAILLE 9

typedef int tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille grille) {
    // Implémentez la logique de chargement de la grille selon vos besoins
}

void afficherGrille(tGrille grille) {
    // Implémentez la logique d'affichage de la grille selon vos besoins
}

int possible(tGrille grille, int numLigne, int numColonne, int valeur) {
    // Implémentez la logique pour vérifier si la valeur peut être insérée dans la grille
    // Retournez 1 si possible, 0 sinon
}

int grilleEstPleine(tGrille grille) {
    // Implémentez la logique pour vérifier si la grille est pleine
    // Retournez 1 si pleine, 0 sinon
}

int main() {
    tGrille grille1;
    int numLigne, numColonne, valeur;

    chargerGrille(grille1);

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
