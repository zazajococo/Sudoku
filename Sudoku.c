/**
*Le fichier source est le programme complet
*d'un jeu de SUDOKU
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// Pour que ce sois jolie/propre
// {
// Codes pour avoir de la couleur
#define RED "\033[0;31m" /** Code ANSI pour la couleur rouge */
#define BLUE "\033[34m" /** Code ANSI pour la couleur bleu */
#define GREEN  "\033[32m" /** Code ANSI pour la couleur vert */
#define CYAN "\033[36m" /** Code ANSI pour la couleur bleu ciel */
#define RESET "\033[0m" /** Code ANSI pour enelver la couleur ANSI */

// Codes pour les possibles erreurs (code = nom abrege en 3 lettres sous fromat ascii)
#define VALEUR_EXISTE_LIGNE 76105103 
#define VALEUR_EXISTE_COLONNE 67111108
#define VALEUR_EXISTE_BLOC 6610899
#define SAISIR_ENTIER 69110116
#define SAISIR_DANS_BORNES 66114110
#define CASE_DE_BASE 67115101
#define ENTREE_TROP_LONGUE 76110103

// }

#define TAILLE 3
#define BLOC TAILLE*TAILLE
typedef int tGrille[BLOC][BLOC];

const int TAILLE_MIN = 1;

// Initialisation des prototypes
void chargerGrille(tGrille grille);
void copieGrille(tGrille grilleDeBase, tGrille grille); // Juste pour copier la grille de base dans une grille modifiable
void afficherGrille(tGrille grille, tGrille grilleDeBase);
void saisir(int *valeur);

bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault);
bool grilleEstPleine(tGrille grille);

void erreurs(int id);
void chargement();



int main()
{
    char rejouer;

    do
    {
        // initialisation
        tGrille grille, grilleDeBase;
        int numLigne, numColonne, valeur;

        chargerGrille(grilleDeBase);
        copieGrille(grilleDeBase, grille);
        printf("Initialisation");
        chargement();

        // tant que la grille n'est pas finie
        while (!grilleEstPleine(grille))
        {
            afficherGrille(grille, grilleDeBase);

            printf("Indice de " CYAN "ligne" RESET ": ");
            saisir(&numLigne); // Il y a une erreur ici car le programme renvoi une erreur pour aucune raison et je ne sais pas comment fixer 
            numLigne--; // Passe de [1-9] à [0-8], borne de la grille

            printf("Indice de " CYAN "colonne" RESET ": ");
            saisir(&numColonne);
            numColonne--; // Passe de [1-9] à [0-8], borne de la grille

            printf(CYAN "Valeur" RESET " à mettre en " CYAN "(%d, %d): " RESET, numLigne + 1, numColonne + 1);
            saisir(&valeur);

            printf("\033[2J\033[1;1H"); // Vide le terminal

            if (possible(grille, numLigne, numColonne, valeur, grilleDeBase))
            {
                // Si la valeur n'est pas out of range ou deja presente
                grille[numLigne][numColonne] = valeur; // alors elle est ajoutee à la grille
            }
        }
        printf("Grille pleine, fin de partie\n");

        // Demander à l'utilisateur s'il veut rejouer
        printf("Voulez-vous rejouer ? (O/N): ");
        scanf(" %c", &rejouer);
        if (rejouer == 'O' || rejouer == 'o')
        {
            printf("Reinitialisation");
            chargement();
        }

    } while (rejouer == 'O' || rejouer == 'o');

    return EXIT_SUCCESS;
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
        fread(grille, sizeof(int), BLOC*BLOC, file);       
    }
    fclose(file);
    /*
    liste des grilles:
    Grille1.sud
    ...
    Grille10.sud
    */
}

void copieGrille(tGrille grilleDeBase, tGrille grille) // Copie la grille de base en une grille modifiable
{
    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            grille[ligne][colonne] = grilleDeBase[ligne][colonne];
        }
    }
}

void afficherGrille(tGrille grille, tGrille grilleDeBase)
{
    printf(GREEN "    1 2 3   4 5 6   7 8 9  \n" RESET); // Affichage des chiffres en haut (pour choisir la colonne)
    printf("  +-------+-------+-------+\n");

    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        if (ligne % 3 == 0 && ligne != 0) {
            printf("  +-------+-------+-------+\n"); // Affichage d'une ligne de separation pour les blocs de 3x3
        }

        printf(GREEN "%d" RESET " | ", ligne + 1); // Affichage des chiffres a droite (pour choisir la ligne)

        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            if (colonne % 3 == 0 && colonne != 0)
            {
                printf("| "); // Affichage d'une colonne de separation pour les blocs de 3x3
            }

            if (grille[ligne][colonne] == 0)
            {
                printf(". "); // Affichage d'un point si valeur vide
            }
            else if (grille[ligne][colonne] == grilleDeBase[ligne][colonne])
            {
                printf(RED "%d " RESET, grille[ligne][colonne]); // La valeur sinon, si case de base en rouge
            }
            else
            {
                printf(BLUE "%d " RESET, grille[ligne][colonne]); // Bleu sinon
            }
        }
        printf("|\n");
    }
    printf("  +-------+-------+-------+\n");
}

bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault) // Retourne true si l'entree de la valeur est possible, false sinon
{
    // Verifie la ligne
    for (int colonne = 0; colonne < BLOC; colonne++)
    {
        if (grille[numLigne][colonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_LIGNE);
            return false;
        }
    }

    // Verifie la colonne
    for (int ligne = 0; ligne < BLOC; ligne++)
    {
        if (grille[ligne][numColonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_COLONNE);
            return false;
        }
    }

    // Verifie le bloc 3x3
    int debutBlocLigne = (numLigne / 3) * 3; // val = [0,1,2], x3 = [0, 3, 6]
    int debutBlocColonne = (numColonne / 3) * 3; // Donc prends l'input est quelque soit la valeur la transforme en [0,3,6], permettant de savoir son bloc

    for (int ligne = debutBlocLigne; ligne < debutBlocLigne + 3; ++ligne) // Le +3 est pour avoir la fin du bloc
    {
        for (int colonne = debutBlocColonne; colonne < debutBlocColonne + 3; ++colonne)
        {
            if (grille[ligne][colonne] == valeur) // Puis verifie si la valeur existe
            {
                erreurs(VALEUR_EXISTE_BLOC);
                return false;
            }
        }
    }

    // Verifie si il s'agit d'une case de base
    if (grilleDefault[numLigne][numColonne] != 0)
    {
        erreurs(CASE_DE_BASE);
        return false;
    }
    

    return true;
}

bool grilleEstPleine(tGrille grille) // Retourne true si pleine, false sinon
{
    for (int ligne = 0; ligne < BLOC; ++ligne)
    {
        for (int colonne = 0; colonne < BLOC; colonne++)
        {
            if (grille[ligne][colonne] == 0) // Si il reste au moins une case vide
            {
                return false; 
            }
        }
    }
    return true; // Toutes les cases sont remplies
}

void saisir(int *valeur)
{
    bool valide = false; // Condition

    do
    {
        char chaine[30] = "";

        fgets(chaine, sizeof(chaine), stdin); // Lit l'entree
        fflush(stdin);  // Vide le buffer d'entrée

        // Enleve le caractere `retour a la ligne` de l'entree si present
        int longueur = strlen(chaine);
        if (longueur > 0 && chaine[longueur-1] == '\n')
        {
            chaine[longueur-1] = '\0';
        }
        else // si l'entree est trop longue
        {
            erreurs(ENTREE_TROP_LONGUE);
            while (getchar() != '\n');  // Vide le buffer d'entree
            continue; // Pour que le message d'erreur ne se repete pas
        }

        if (strlen(chaine) <= 30) // Si l'entree n'est pas trop longue
        {
            if (sscanf(chaine, "%d", valeur) != 0)
            {
                // Conversion reussie
                if (*valeur >= TAILLE_MIN && *valeur <= BLOC)
                {
                    valide = true; // La valeur est correct, on sort de la boucle
                }
                else
                {
                    erreurs(SAISIR_DANS_BORNES);
                }
            }
            else
            {
                erreurs(SAISIR_ENTIER);
            }
        }
        else // Car mieux vaut etre sur :)
        {
            erreurs(ENTREE_TROP_LONGUE);
        }
    } while (!valide);
}

void erreurs(int id)
{
    switch (id)
    {
    case VALEUR_EXISTE_LIGNE:
        printf(RED "Erreur" RESET "!\nLa valeur est deja " RED "presente" RESET " dans la " RED "ligne" RESET ". Veuillez reessayer.\n");
        break;

    case VALEUR_EXISTE_COLONNE:
        printf(RED "Erreur" RESET "!\nLa valeur est deja " RED "presente" RESET " dans la " RED "colonne" RESET ". Veuillez reessayer.\n");
        break;

    case VALEUR_EXISTE_BLOC:
        printf(RED "Erreur" RESET "!\nLa valeur est deja " RED "presente" RESET " dans le " RED "bloc" RESET ". Veuillez reessayer.\n");
        break;

    case SAISIR_ENTIER:
        printf(RED "Erreur" RESET "!\nVeuillez " RED "saisir" RESET " un " RED "entier" RESET ". Veuillez reessayer.\n");
        break;

    case SAISIR_DANS_BORNES:
        printf(RED "Erreur" RESET "!\nVeuillez " RED "saisir" RESET " un " RED "entier entre 1 & 9" RESET ". Veuillez reessayer.\n");
        break;

    case CASE_DE_BASE:
        printf(RED "Erreur" RESET "!\nLa case que vous cherchez a " RED "modifier" RESET " est une case de la " RED "grille de base" RESET ". Veuillez reessayer.\n");
        break;

    case ENTREE_TROP_LONGUE:
        printf(RED "Erreur" RESET "!\n" RED "L'entree est trop longue" RESET". Veuillez reessayer.\n");
        break;

    default:
        break;
    }
}

void chargement() // Qnimation pour la deco
{
    for (int _ = 0; _ < 3; _++)
    {
        fflush(stdout);
        sleep(1);
        printf(".");
    }
    printf("\n");
}
