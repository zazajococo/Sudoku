#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Codes pour avoir du text de couleur :)
#define RED "\033[0;31m"
#define BLUE "\033[34m"
#define GREEN  "\033[32m"
#define CYAN "\033[36m"
#define RESET "\033[0m"


#define TAILLE 9

// Codes pour les possibles erreurs (code = nom abrege en 3 lettres sous fromat ascii)
#define VALEUR_EXISTE_LIGNE 76105103
#define VALEUR_EXISTE_COLONNE 67111108
#define VALEUR_EXISTE_BLOC 6610899
#define SAISIR_ENTIER 69110116
#define SAISIR_DANS_BORNES 66114110
#define CASE_DE_BASE 67115101
#define ENTREE_TROP_LONGUE 76110103

typedef int tGrille[TAILLE][TAILLE];

const int TAILLE_MIN = 1;

void chargerGrille(tGrille grille);
void copieGrille(tGrille grilleDeBase, tGrille grille);
void afficherGrille(tGrille grille, tGrille grilleDeBase);
bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault);
bool grilleEstPleine(tGrille grille);
void saisir(int *valeur);

void erreurs(int id);

int main()
{
    tGrille grille, grilleDeBase;
    int numLigne, numColonne, valeur;
    
    chargerGrille(grilleDeBase);
    copieGrille(grilleDeBase, grille);
    

    while (!grilleEstPleine(grille))
    {
        printf("\n");
        afficherGrille(grille, grilleDeBase);
        
        printf("Indice de " CYAN "ligne" RESET ": ");
        saisir(&numLigne);
        numLigne--;

        printf("Indice de " CYAN "colonne" RESET ": ");
        saisir(&numColonne);
        numColonne--;
        
        printf(CYAN "Valeur" RESET " a mettre en " CYAN "(%d, %d): " RESET, numLigne+1, numColonne+1);
        saisir(&valeur);

        printf("\033[2J\033[1;1H"); // Vide le terminal
        
        if (possible(grille, numLigne, numColonne, valeur, grilleDeBase))
        {
            grille[numLigne][numColonne] = valeur;
        }
    }
    printf("Grille pleine, fin de partie\n");

    return 0;
}

void chargerGrille(tGrille grille)
{
    char nomFichier[30] = "Grilles/Grille1.sud";
    FILE * file;

    printf("Nom du fichier ? \n");
    //scanf("%s", nomFichier);

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

void copieGrille(tGrille grilleDeBase, tGrille grille)
{
    for (int ligne = 0; ligne < TAILLE; ligne++)
    {
        for (int colonne = 0; colonne < TAILLE; colonne++)
        {
            grille[ligne][colonne] = grilleDeBase[ligne][colonne];
        }
    }
}

void afficherGrille(tGrille grille, tGrille grilleDeBase)
{
    printf(GREEN "    1 2 3   4 5 6   7 8 9  \n" RESET);
    printf("  +-------+-------+-------+\n");

    for (int ligne = 0; ligne < TAILLE; ligne++)
    {
        if (ligne % 3 == 0 && ligne != 0) {
            printf("  +-------+-------+-------+\n"); // Affichage d'une ligne de separation pour les carres de 3x3
        }

        printf(GREEN "%d" RESET " | ", ligne + 1); // Affichage des chiffres a droite (pour choisir la ligne)

        for (int colonne = 0; colonne < TAILLE; colonne++)
        {
            if (colonne % 3 == 0 && colonne != 0)
            {
                printf("| "); // Affichage d'une colonne de separation pour les carres de 3x3
            }

            if (grille[ligne][colonne] == 0)
            {
                printf(". "); // Affichage d'un point si valeur vide
            }
            else if (grille[ligne][colonne] == grilleDeBase[ligne][colonne])
            {
                printf(RED "%d " RESET, grille[ligne][colonne]); // La valeur sinon si case de base en rouge
            }
            else
            {
                printf(BLUE "%d " RESET, grille[ligne][colonne]); // En bleu sinon
            }
        }
        printf("|\n");
    }
    printf("  +-------+-------+-------+\n");
}

bool possible(tGrille grille, int numLigne, int numColonne, int valeur, tGrille grilleDefault) // Retourne true si l'entree de la valeur est possible, false sinon
{
    // Verifie la ligne
    for (int colonne = 0; colonne < TAILLE; colonne++)
    {
        if (grille[numLigne][colonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_LIGNE);
            return false;
        }
    }

    // Verifie la colonne
    for (int ligne = 0; ligne < TAILLE; ligne++)
    {
        if (grille[ligne][numColonne] == valeur)
        {
            erreurs(VALEUR_EXISTE_COLONNE);
            return false;
        }
    }

    // Verifie le bloc 3x3
    int debutBlocLigne = (numLigne / 3) * 3;
    int debutBlocColonne = (numColonne / 3) * 3;

    for (int ligne = debutBlocLigne; ligne < debutBlocLigne + 3; ++ligne)
    {
        for (int colonne = debutBlocColonne; colonne < debutBlocColonne + 3; ++colonne)
        {
            if (grille[ligne][colonne] == valeur) 
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
    for (int ligne = 0; ligne < TAILLE; ++ligne)
    {
        for (int colonne = 0; colonne < TAILLE; colonne++)
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
    do
    {
        char chaine[30] = "";

        fgets(chaine, sizeof(chaine), stdin);

        // Enleve le caractere de retour a la ligne
        int longueur = strlen(chaine);
        if (longueur > 0 && chaine[longueur-1] == '\n')
        {
            chaine[longueur-1] = '\0';
        }
        else // si l'entree est trop longue
        {
            
            erreurs(ENTREE_TROP_LONGUE);
            while (getchar() != '\n');  // Vide le buffer de l'input
            continue; // Pour que le message d'erreur ne se repete pas
        }

        if (strlen(chaine) <= 30)
        {
            if (sscanf(chaine, "%d", valeur) == 1)
            {
                // Conversion reussie
                if (*valeur >= TAILLE_MIN && *valeur <= TAILLE)
                {
                    break;
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
        else
        {
            erreurs(ENTREE_TROP_LONGUE);
        }
    } while (true);
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