#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "constantes.h"
#include "editeur.h"
#include "fichiers.h"
#include "fgifsdl.h"
#include "jeu.h"
#include "fgif.h"

int getnbNV()
{
    FILE* fichier = NULL;
    size_t TailleLigne=NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1;
    int nbrNV=0;
    char* Buffer[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};

    fichier = fopen("MarioDocs/niveaux.lvl", "r");
    while(getline(Buffer,&TailleLigne,fichier)>2)
        nbrNV++;
    fclose(fichier);
    return nbrNV;
}
int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR], int NV)
{
    FILE* fichier = NULL;
    char ligneFichier[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};
    char Buffer[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};
    int i = 0, j = 0;

    fichier = fopen("MarioDocs/niveaux.lvl", "r");
    if (fichier == NULL)
        return 0;

    for ( i = 0; i < NV; ++i)
    {
        fgets(ligneFichier, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1, fichier);
        fgets(Buffer, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1, fichier);
    }
    //printf("Ligne de la carte:%s\n",ligneFichier );
    //fgets(ligneFichier, NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1, fichier);

    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
        {
            switch (ligneFichier[(i * NB_BLOCS_LARGEUR) + j])
            {
                case '0':
                    niveau[j][i] = 0;
                    break;
                case '1':
                    niveau[j][i] = 1;
                    break;
                case '2':
                    niveau[j][i] = 2;
                    break;
                case '3':
                    niveau[j][i] = 3;
                    break;
                case '4':
                    niveau[j][i] = 4;
                    break;
                case '5':
                    niveau[j][i] = 5;
                    break;
            }
        }
    }
    fclose(fichier);
    return 1;
}

int sauvegarderNiveau(int niveau[][NB_BLOCS_HAUTEUR])
{
    FILE* fichier = NULL;
    int i = 0, j = 0;
    fichier = fopen("MarioDocs/niveaux.lvl", "a");
    if (fichier == NULL)
        return 0;
    
    fprintf(fichier,"\n");
    for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
    {
        for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
        {
            fprintf(fichier, "%d", niveau[j][i]);
        }
    }

    fclose(fichier);
    return 1;
}
