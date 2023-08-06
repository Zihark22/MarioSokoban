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



void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction)
{
    switch(direction)
    {
        case HAUT:
            if (pos->y - 1 < 0) // Si le joueur dépasse l'écran, on arrête
                break;
            if (carte[pos->x][pos->y - 1] == MUR) // S'il y a un mur, on arrête
                break;
            // Si on veut pousser une caisse, il faut vérifier qu'il n'y a pas de mur derrière (ou une autre caisse, ou la limite du monde)
            if ((carte[pos->x][pos->y - 1] == CAISSE || carte[pos->x][pos->y - 1] == CAISSE_OK) &&
                (pos->y - 2 < 0 || carte[pos->x][pos->y - 2] == MUR ||
                carte[pos->x][pos->y - 2] == CAISSE || carte[pos->x][pos->y - 2] == CAISSE_OK))
                break;

            // Si on arrive là, c'est qu'on peut déplacer le joueur !
            // On vérifie d'abord s'il y a une caisse à déplacer
            deplacerCaisse(&carte[pos->x][pos->y - 1], &carte[pos->x][pos->y - 2]);

            pos->y--; // On peut enfin faire monter le joueur (oufff !)
            break;

        case BAS:
            if (pos->y + 1 >= NB_BLOCS_HAUTEUR)
                break;
            if (carte[pos->x][pos->y + 1] == MUR)
                break;

            if ((carte[pos->x][pos->y + 1] == CAISSE || carte[pos->x][pos->y + 1] == CAISSE_OK) &&
                (pos->y + 2 >= NB_BLOCS_HAUTEUR || carte[pos->x][pos->y + 2] == MUR ||
                carte[pos->x][pos->y + 2] == CAISSE || carte[pos->x][pos->y + 2] == CAISSE_OK))
                break;


            deplacerCaisse(&carte[pos->x][pos->y + 1], &carte[pos->x][pos->y + 2]);

            pos->y++;
            break;

        case GAUCHE:
            if (pos->x - 1 < 0)
                break;
            if (carte[pos->x - 1][pos->y] == MUR)
                break;

            if ((carte[pos->x - 1][pos->y] == CAISSE || carte[pos->x - 1][pos->y] == CAISSE_OK) &&
                (pos->x - 2 < 0 || carte[pos->x - 2][pos->y] == MUR ||
                carte[pos->x - 2][pos->y] == CAISSE || carte[pos->x - 2][pos->y] == CAISSE_OK))
                break;


            deplacerCaisse(&carte[pos->x - 1][pos->y], &carte[pos->x - 2][pos->y]);

            pos->x--;
            break;

        case DROITE:
            if (pos->x + 1 >= NB_BLOCS_LARGEUR)
                break;
            if (carte[pos->x + 1][pos->y] == MUR)
                break;

            if ((carte[pos->x + 1][pos->y] == CAISSE || carte[pos->x + 1][pos->y] == CAISSE_OK) &&
                (pos->x + 2 >= NB_BLOCS_LARGEUR || carte[pos->x + 2][pos->y] == MUR ||
                carte[pos->x + 2][pos->y] == CAISSE || carte[pos->x + 2][pos->y] == CAISSE_OK))
                break;

            deplacerCaisse(&carte[pos->x + 1][pos->y], &carte[pos->x + 2][pos->y]);

            pos->x++;
            break;
    }
}

void deplacerCaisse(int *premiereCase, int *secondeCase)
{
    if (*premiereCase == CAISSE)
    {
        if (*secondeCase == OBJECTIF){
            *secondeCase = CAISSE_OK;
            *premiereCase = VIDE;
            if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
                SDL_Quit();
                exit(EXIT_FAILURE);
            }
            Mix_AllocateChannels(1); // Allouer 2 cannaux
            Mix_Volume(0, MIX_MAX_VOLUME); // Mets le son a 100% en volume pour le premier cannaux

            Mix_Chunk* soundA = Mix_LoadWAV("eat.wav");
            Mix_PlayChannel(0, soundA, 0); // Joue soundA deux fois sur le canal 1
            SDL_Delay(100);
            Mix_FreeChunk(soundA);
            Mix_CloseAudio();
        }
        else if(*secondeCase == VIDE){
            *secondeCase = CAISSE;
            *premiereCase = VIDE;
        }
    }
    else if (*premiereCase == CAISSE_OK){
        if(*secondeCase == VIDE){
            *premiereCase = OBJECTIF;
            *secondeCase = CAISSE;
        }
        else if(*secondeCase == OBJECTIF){
            *premiereCase = OBJECTIF;
            *secondeCase = CAISSE_OK;
        }
    }
}

void messageFin(SDL_Renderer* fenet){

    //SDL_Rect pos={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
    //SDL_Rect src1={0, 0, 0, 0};
    char message1[]="FIN des niveaux";
    char message2[]="Retour au menu principal";
    SDL_Surface *surface = SDL_CreateRGBSurface (0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
    SDL_Texture* texture;
    SDL_Rect ptxt;
    SDL_Rect ptxt2;


    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Color couleur={ 250, 0, 0, 0 };
    TTF_Font* font = TTF_OpenFont("MarioDocs/batmfa.ttf", 18); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());

    }
    SDL_Surface* text = TTF_RenderText_Blended(font, message1, couleur);
    if (text == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }
    SDL_Texture * texture1=SDL_CreateTextureFromSurface(fenet, text);
    SDL_QueryTexture(texture1, NULL, NULL, &ptxt.w, &ptxt.h);
    ptxt.x = LARGEUR_FENETRE / 2 - ptxt.w / 2;
    ptxt.y = HAUTEUR_FENETRE / 2 - ptxt.h / 2-50;
    SDL_DestroyTexture(texture1);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    SDL_BlitSurface(text, NULL, surface, &ptxt);
    SDL_Color couleur2={ 255, 255, 255, 0 };
    SDL_Surface* text2 = TTF_RenderText_Blended(font, message2, couleur2);
    if (text2 == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }
    SDL_Texture * texture2=SDL_CreateTextureFromSurface(fenet, text2);
    SDL_QueryTexture(texture2, NULL, NULL, &ptxt2.w, &ptxt2.h);
    ptxt2.x = LARGEUR_FENETRE / 2 - ptxt2.w / 2;
    ptxt2.y = HAUTEUR_FENETRE / 2 - ptxt2.h / 2 + 50;
    SDL_DestroyTexture(texture2);

    SDL_BlitSurface(text2, NULL, surface, &ptxt2);
    texture= SDL_CreateTextureFromSurface(fenet, surface);
    SDL_RenderClear(fenet);
    SDL_RenderCopy(fenet, texture, NULL, NULL);
    SDL_RenderPresent(fenet);
    SDL_Delay(1000);

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void jouer(SDL_Renderer* fenetre, SDL_Texture* ecran)
{
    SDL_Surface *mario[4] = {NULL}; // 4 surfaces pour chacune des directions de mario
    SDL_Surface *mur = NULL, *caisse = NULL, *caisseOK = NULL, *objectif = NULL, *marioActuel = NULL, *bravo=NULL;
    SDL_Surface *surface;
    SDL_Rect position, positionJoueur;
    SDL_Event event;
    int continuer, objectifsRestants = 0, i = 0, j = 0,quitter=0,next=0;
    SDL_Texture *texture,* Text;
    int Niveau=1,nbrNV;
    SDL_Rect ptxt={10,10,50,30},finNVX/*,src1={0,0,0,0}*/;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};
    char AffNiv[]="NIVEAU X";
    // Chargement des sprites (décors, personnage...)
    
    bravo = IMG_Load("MarioDocs/bravo.jpg");
    mur = IMG_Load("MarioDocs/mur.jpg");
    caisse = IMG_Load("MarioDocs/caisse.jpg");
    caisseOK = IMG_Load("MarioDocs/caisse_ok.jpg");
    objectif = IMG_Load("MarioDocs/objectif.png");
    mario[BAS] = IMG_Load("MarioDocs/mario_bas.gif");
    mario[GAUCHE] = IMG_Load("MarioDocs/mario_gauche.gif");
    mario[HAUT] = IMG_Load("MarioDocs/mario_haut.gif");
    mario[DROITE] = IMG_Load("MarioDocs/mario_droite.gif");

    marioActuel = mario[BAS]; // Mario sera dirigé vers le bas au départ
    //strcat(AffNiv,pNiv);
   
    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Color couleur={ 0, 0, 250, 255 };
    TTF_Font* font = TTF_OpenFont("MarioDocs/batmfa.ttf", 22); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());

    }
    
    nbrNV=getnbNV(); // nombre de niveaux enregistrés

    for(Niveau=1;Niveau<=nbrNV;Niveau++)
    {
        next=0;
        if(Niveau<10)
            AffNiv[7]=Niveau+'0'; // limitation à 9 niveaux (1 à 9)
        else
             AffNiv[7]='X';
        surface=SDL_CreateRGBSurface(0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
        //SDL_Rect src1={0, 0, 0, 0};
        continuer = 1, objectifsRestants = 0, i = 0, j = 0;

        // Chargement du niveau
        if (!chargerNiveau(carte,Niveau))
            exit(EXIT_FAILURE); // On arrête le jeu si on n'a pas pu charger le niveau

        SDL_Surface* text = TTF_RenderText_Blended(font, AffNiv, couleur); // Crée un surface qui contient le texte

        if (text == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        }
        // Recherche de la position de Mario au départ
        for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
        {
            for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
            {
                if (carte[i][j] == MARIO) // Si Mario se trouve à cette position sur la carte
                {
                    positionJoueur.x = i;
                    positionJoueur.y = j;
                    carte[i][j] = VIDE;
                }
            }
        }
        //SDL_RenderClear(fenetre);
        // Activation de la répétition des touches
        //SDL_EnableKeyRepeat(100, 100);

        while (continuer)
        {
            SDL_WaitEvent(&event);
            switch(event.type)
            {
                case SDL_QUIT:
                    quitter = 1;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quitter = 1;
                            break;
                        case SDLK_p:
                            next=1;
                            break;
                        case SDLK_r:
                            chargerNiveau(carte,Niveau);
                            // Recherche de la position de Mario au départ
                            for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
                            {
                                for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
                                {
                                    if (carte[i][j] == MARIO) // Si Mario se trouve à cette position sur la carte
                                    {
                                        positionJoueur.x = i;
                                        positionJoueur.y = j;
                                        carte[i][j] = VIDE;
                                    }
                                }
                            }
                            break;
                        case SDLK_UP:
                            marioActuel = mario[HAUT];
                            deplacerJoueur(carte, &positionJoueur, HAUT);
                            break;
                        case SDLK_z:
                            marioActuel = mario[HAUT];
                            deplacerJoueur(carte, &positionJoueur, HAUT);
                            break;
                        case SDLK_DOWN:
                            marioActuel = mario[BAS];
                            deplacerJoueur(carte, &positionJoueur, BAS);
                            break;
                        case SDLK_s:
                            marioActuel = mario[BAS];
                            deplacerJoueur(carte, &positionJoueur, BAS);
                            break;
                        case SDLK_RIGHT:
                            marioActuel = mario[DROITE];
                            deplacerJoueur(carte, &positionJoueur, DROITE);
                            break;
                        case SDLK_d:
                            marioActuel = mario[DROITE];
                            deplacerJoueur(carte, &positionJoueur, DROITE);
                            break;
                        case SDLK_LEFT:
                            marioActuel = mario[GAUCHE];
                            deplacerJoueur(carte, &positionJoueur, GAUCHE);
                            break;
                        case SDLK_q:
                            marioActuel = mario[GAUCHE];
                            deplacerJoueur(carte, &positionJoueur, GAUCHE);
                            break;
                    }
                    break;
            }
            if(next)
                break;
            // Efface les positions précédentes du perso et caisses
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
            
            // Placement des objets à l'écran
            objectifsRestants = 0;

            for (i = 0 ; i < NB_BLOCS_LARGEUR ; i++)
            {
                for (j = 0 ; j < NB_BLOCS_HAUTEUR ; j++)
                {
                    position.x = i * TAILLE_BLOC;
                    position.y = j * TAILLE_BLOC;

                    switch(carte[i][j])
                    {
                        case MUR:
                            SDL_BlitSurface(mur, NULL, surface, &position);
                            break;
                        case CAISSE:
                            SDL_BlitSurface(caisse, NULL, surface, &position);
                            break;
                        case CAISSE_OK:
                            SDL_BlitSurface(caisseOK, NULL, surface, &position);
                            break;
                        case OBJECTIF:
                            SDL_BlitSurface(objectif, NULL, surface, &position);
                            objectifsRestants = 1;
                            break;
                    }
                }
            }

            SDL_BlitSurface(text, NULL, surface, &ptxt); // Affiche Niveau

            if(quitter)
                break;
            // Si on n'a trouvé aucun objectif sur la carte, c'est qu'on a gagné
            if(!objectifsRestants){
                Text=SDL_CreateTextureFromSurface(fenetre, bravo);
                SDL_QueryTexture(Text, NULL, NULL, &finNVX.w, &finNVX.h);
                finNVX.x=LARGEUR_FENETRE/2-finNVX.w/2;
                finNVX.y=HAUTEUR_FENETRE/2-finNVX.h/2;

                SDL_BlitSurface(bravo, NULL, surface, &finNVX);
                texture= SDL_CreateTextureFromSurface(fenetre, surface);
                SDL_RenderClear(fenetre);
                SDL_RenderCopy(fenetre, texture, NULL, NULL);
                SDL_RenderPresent(fenetre);
                SDL_Delay(600);
                continuer = 0;
            }
            else{
            // On place le joueur à la bonne position
                position.x = positionJoueur.x * TAILLE_BLOC;
                position.y = positionJoueur.y * TAILLE_BLOC;
                SDL_BlitSurface(marioActuel, NULL, surface, &position);
                texture= SDL_CreateTextureFromSurface(fenetre, surface);
                SDL_RenderClear(fenetre);
                SDL_RenderCopy(fenetre, texture, NULL, NULL);
                SDL_RenderPresent(fenetre);
            }
            // calculates to 60 fps
            //SDL_Delay(1000 / 60);
        }
        if(quitter)
            break;
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        SDL_FreeSurface(text);
    }
    SDL_DestroyTexture(Text);
    TTF_CloseFont(font);
    TTF_Quit();
    messageFin(fenetre);
    // Désactivation de la répétition des touches (remise à 0)
    //SDL_EnableKeyRepeat(0, 0);
    // Libération des surfaces chargées
    SDL_FreeSurface(mur);
    SDL_FreeSurface(bravo);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOK);
    SDL_FreeSurface(objectif);
    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(mario[i]);
}
