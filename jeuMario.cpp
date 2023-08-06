#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstring>

#include "jeuMario.h"
#include "mainwindow.h"

char* chemin(char objet[]){ // donne le chemin des images,... en fonction du dossier source choisit
    QString source = "/Users/badajozj/Documents/Coding/Qt_Creator/Projects/Mario/MarioApp/MarioDocs/"; // chemin ressources : ":/MarioDocs/"
    QString str(objet);
    QString cheminObjet = source+objet;
    string cheminBravo=cheminObjet.toStdString();//cheminBravo.toStdString()
    //char chem[cheminBravo.size()];
    char * chem = (char *) malloc(cheminBravo.size()*sizeof(char));
    for(unsigned long i=0;i<cheminBravo.size();i++)
        chem[i]=cheminBravo[i];
    chem[cheminBravo.size()]='\0';
    return chem;
}

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
    TTF_Font* font = TTF_OpenFont(chemin((char*)"batmfa.ttf"), 18); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

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

void jouer(SDL_Renderer* fenetre)
{
    SDL_Surface *mario[4] = {NULL}; // 4 surfaces pour chacune des directions de mario
    SDL_Surface *mur = NULL, *caisse = NULL, *caisseOK = NULL, *objectif = NULL, *marioActuel = NULL, *bravo=NULL;
    SDL_Surface *surface;
    SDL_Rect position, positionJoueur;
    SDL_Event event;
    int continuer, objectifsRestants, i, j,quitter=0,next=0;
    SDL_Texture *texture,* Text;
    int Niveau=1,nbrNV;
    SDL_Rect ptxt={10,10,50,30},finNVX/*,src1={0,0,0,0}*/;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR];
    for(int i=0;i<NB_BLOCS_LARGEUR;i++){
        for(int j=0;j<NB_BLOCS_HAUTEUR;j++){
            carte[i][j]=0;
        }
    }
    char AffNiv[]="NIVEAU XX";
    // Chargement des sprites (décors, personnage...)

    //////////////  CHEMIN ////////////////////////////////////////////////////////////////////////////
/*
    string cheminBravo=chemin("bravo.jpg").toStdString();//cheminBravo.toStdString()
    char chem[cheminBravo.size()];
    for(unsigned long i=0;i<cheminBravo.size();i++)
        chem[i]=cheminBravo[i];
    chem[cheminBravo.size()]='\0';
    std::cout << "Le chemin de bravo est : " << chem << std::endl;
*/
    ////////////////////////////////////////////////////////////////////////////////////////////////
    bravo = IMG_Load(chemin((char*)"bravo.jpg"));
    mur = IMG_Load(chemin((char*)"mur.jpg"));
    caisse = IMG_Load(chemin((char*)"caisse.jpg"));
    caisseOK = IMG_Load(chemin((char*)"caisse_ok.jpg"));
    objectif = IMG_Load(chemin((char*)"objectif.png"));
    mario[BAS] = IMG_Load(chemin((char*)"mario_bas.gif"));
    mario[GAUCHE] = IMG_Load(chemin((char*)"mario_gauche.gif"));
    mario[HAUT] = IMG_Load(chemin((char*)"mario_haut.gif"));
    mario[DROITE] = IMG_Load(chemin((char*)"mario_droite.gif"));

    marioActuel = mario[BAS]; // Mario sera dirigé vers le bas au départ
    //strcat(AffNiv,pNiv);

    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Color couleur={ 0, 0, 250, 255 };
    TTF_Font* font = TTF_OpenFont(chemin((char*)"batmfa.ttf"), 22); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());

    }

    nbrNV=getnbNV(); // nombre de niveaux enregistrés

    for(Niveau=1;Niveau<=nbrNV;Niveau++)
    {
        next=0;
        if(Niveau<10){
            AffNiv[7]=Niveau+'0'; // limitation à 9 niveaux (1 à 9)
            AffNiv[8]=' ';
        }
        else if(Niveau>=10 && Niveau < 20){
             AffNiv[7]='1';
             AffNiv[8]=Niveau-10+'0';
        }
        else if(Niveau>=20 && Niveau < 30){
             AffNiv[7]='2';
             AffNiv[8]=Niveau-20+'0';
        }
        surface=SDL_CreateRGBSurface(0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
        //SDL_Rect src1={0, 0, 0, 0};
        continuer = 1;
        //objectifsRestants = 0;

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

    SDL_FreeSurface(mur);
    SDL_FreeSurface(bravo);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOK);
    SDL_FreeSurface(objectif);
    for (i = 0 ; i < 4 ; i++)
        SDL_FreeSurface(mario[i]);
}

void editeur(SDL_Renderer* fenetre)
{
    SDL_Surface *mur = NULL, *caisse = NULL, *objectif = NULL, *mario = NULL, *caisseOK = NULL;
    SDL_Surface *surface=SDL_CreateRGBSurface (0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);;
    SDL_Rect position;
    SDL_Event event;
    SDL_Rect posObjActuel,fond,ptxt,souris;
    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = MUR, i = 0, j = 0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR];
    for(int i=0;i<NB_BLOCS_LARGEUR;i++){
        for(int j=0;j<NB_BLOCS_HAUTEUR;j++){
            carte[i][j]=0;
        }
    }
    SDL_Texture *texture;

    // Chargement des objets et du niveau
    mur = IMG_Load(chemin((char*)"mur.jpg"));
    caisse = IMG_Load(chemin((char*)"caisse.jpg"));
    objectif = IMG_Load(chemin((char*)"objectif.png"));
    mario = IMG_Load(chemin((char*)"mario_bas.gif"));
    caisseOK = IMG_Load(chemin((char*)"caisse_ok.jpg"));

    SDL_Texture *test=SDL_CreateTextureFromSurface(fenetre,mur);
    SDL_QueryTexture(test, NULL, NULL, &posObjActuel.w, &posObjActuel.h);
    posObjActuel.x=LARGEUR_FENETRE-posObjActuel.w-10;
    posObjActuel.y=10;
    fond.x=posObjActuel.x-75;
    fond.y=posObjActuel.y-10;
    fond.w=150+posObjActuel.w;
    fond.h=posObjActuel.h+20;
    SDL_DestroyTexture(test);

    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Color couleur={250,0,0,255 };
    TTF_Font* font = TTF_OpenFont(chemin((char*)"SuperMario256.ttf"), 16); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());

    }
    SDL_Surface* text = TTF_RenderText_Blended(font, "Objet  ", couleur); // Crée un surface qui contient le texte

    if (text == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }
    ptxt.x=fond.x+10;
    ptxt.y=posObjActuel.y+10;
    ptxt.w=fond.w-50;
    ptxt.h=fond.h;

    if (!chargerNiveau(carte,1))
        exit(EXIT_FAILURE);

    // Boucle infinie de l'éditeur
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // On met l'objet actuellement choisi (mur, caisse...) à l'endroit du clic
                    carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                    clicGaucheEnCours = 1; // On active un booléen pour retenir qu'un bouton est enfoncé
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) // Le clic droit sert à effacer
                {
                    carte[event.button.x / TAILLE_BLOC][event.button.y /TAILLE_BLOC] = VIDE;
                    clicDroitEnCours = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP: // On désactive le booléen qui disait qu'un bouton était enfoncé
                if (event.button.button == SDL_BUTTON_LEFT)
                    clicGaucheEnCours = 0;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    clicDroitEnCours = 0;
                break;
            case SDL_MOUSEMOTION:
                //SDL_Log("Mouvement de souris (%d %d) (%d %d)", event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                souris.x=event.motion.x;
                souris.y=event.motion.y;
                souris.w=posObjActuel.w;
                souris.h=posObjActuel.h;
                if (clicGaucheEnCours) // Si on déplace la souris et que le bouton gauche de la souris est enfoncé
                {
                    carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                }
                else if (clicDroitEnCours) // Pareil pour le bouton droit de la souris
                {
                    carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = VIDE;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_s:
                        sauvegarderNiveau(carte);
                        break;
                    case SDLK_c:
                        chargerNiveau(carte,1);
                        break;
                    case SDLK_m:
                        objetActuel = MUR;
                        break;
                    case SDLK_b:
                        objetActuel = CAISSE;
                        break;
                    case SDLK_o:
                        objetActuel = OBJECTIF;
                        break;
                    case SDLK_p:
                        objetActuel = MARIO;
                        break;
                    case SDLK_k:
                        objetActuel = CAISSE_OK;
                        break;
                }
                break;
        }
        // Effacement de l'écran
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

        // Placement des objets à l'écran
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
                    case OBJECTIF:
                        SDL_BlitSurface(objectif, NULL, surface, &position);
                        break;
                    case MARIO:
                        SDL_BlitSurface(mario, NULL, surface, &position);
                        break;
                    case CAISSE_OK:
                        SDL_BlitSurface(caisseOK, NULL, surface, &position);
                        break;
                }
            }
        }
        SDL_FillRect(surface,&fond,SDL_MapRGB(surface->format, 0, 0, 0));
        SDL_BlitSurface(text, NULL, surface, &ptxt);
        switch(objetActuel){
            case MUR:
                SDL_BlitSurface(mur, NULL, surface, &posObjActuel);
                SDL_BlitSurface(mur, NULL, surface, &souris);
                break;
            case OBJECTIF:
                SDL_BlitSurface(objectif, NULL, surface, &posObjActuel);
                SDL_BlitSurface(objectif, NULL, surface, &souris);
                break;
            case CAISSE:
                SDL_BlitSurface(caisse, NULL, surface, &posObjActuel);
                SDL_BlitSurface(caisse, NULL, surface, &souris);
                break;
            case MARIO:
                SDL_BlitSurface(mario, NULL, surface, &posObjActuel);
                SDL_BlitSurface(mario, NULL, surface, &souris);
                break;
            case CAISSE_OK:
                SDL_BlitSurface(caisseOK, NULL, surface, &posObjActuel);
                SDL_BlitSurface(caisseOK, NULL, surface, &souris);
                break;
        }
        texture = SDL_CreateTextureFromSurface(fenetre, surface);
        SDL_RenderClear(fenetre);
            SDL_RenderCopy(fenetre, texture, NULL, NULL);
        SDL_RenderPresent(fenetre);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeSurface(surface);
    SDL_FreeSurface(mur);
    SDL_FreeSurface(caisse);
    SDL_FreeSurface(caisseOK);
    SDL_FreeSurface(objectif);
    SDL_FreeSurface(mario);
}

int getnbNV()
{
    FILE* fichier = NULL;
    size_t TailleLigne=NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1;
    int nbrNV=0;
    char* Buffer[NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR + 1] = {0};

    fichier = fopen(chemin((char*)"niveaux.lvl"), "r");
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

    fichier = fopen(chemin((char*)"niveaux.lvl"), "r");
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
    fichier = fopen(chemin((char*)"niveaux.lvl"), "a");
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

/**/

#define GIF_DEFAULT_DELAY 10

uchar Fuchar(FILE* F)
{
    uchar res;
    fread(&res,1,1,F);
    return res;
}

ushort Fushort(FILE* F)
{
    ushort res;
    fread(&res,2,1,F);
    return res;
}

int Fnchain(FILE* F,int size,char* out)
{
    out[size] = '\0';
    fread(out,1,size,F);
    return 0;
}

int GIFCheckHead(FILE* F,char* version)
{
    char gif[4];
    Fnchain(F,3,gif);
    if (strcmp(gif,"GIF")!=0)
        return -1;
    Fnchain(F,3,version);
    return 0;
}

int GIFLSD(FILE* F,LSD* plsd)
{
    uchar pf;
    uchar pixelaspectratio;
    plsd->width = Fushort(F);
    plsd->height = Fushort(F);
    pf = Fuchar(F);
    plsd->GlobalColorTableFlag = (pf>>7);
    plsd->ColorResol = ((pf>>4)&7);
    plsd->SortFlag = ((pf>>3)&1);
    plsd->SizeofGlobalColorTable = (pf&7);
    plsd->BackGroundColorIndex = Fuchar(F);
    pixelaspectratio = Fuchar(F);
    plsd->AspectRatio = (pixelaspectratio+15)*1.0f/64;
    return 0;
}

int GIFPalette(FILE* F,uchar sizofglobalcolortable,Palette* ppal)
{
    int i;
    memset(ppal,0,sizeof(Palette));
    ppal->nbcol = 1<<(sizofglobalcolortable+1);
    for(i=0;i<ppal->nbcol;i++)
    {
        ppal->pix[i].r = Fuchar(F);
        ppal->pix[i].g = Fuchar(F);
        ppal->pix[i].b = Fuchar(F);
    }
    return 0;
}

int GIFanimate(FILE* F,Animate* pan)
{
    uchar BlockSize,pfield,terminator; // I'll be back.
    BlockSize = Fuchar(F);
    if (BlockSize!=4)
        return -11;
    pfield = Fuchar(F);
    pan->transparentflag = (pfield&1);
    pan->userinputflag = ((pfield>>1)&1);
    pan->disposalmethod = ((pfield>>2)&3);
    pan->delay = Fushort(F);
    if (pan->delay == 0)
        pan->delay = GIF_DEFAULT_DELAY;
    pan->transparentcolorindex = Fuchar(F);
    terminator = Fuchar(F);
    return 0;
}

int GIFAppBlocExtention(FILE* F)
{
    uchar BlockSize,len;
    char buf[9];
    char buf2[4];
    BlockSize = Fuchar(F);
    if (BlockSize!=11)
        return -12;
    Fnchain(F,8,buf);
    Fnchain(F,3,buf2);
    do
    {
        char* buf3;
        len = Fuchar(F);
        if (len==0)
            break;
        buf3 = (char*)malloc((len+1)*sizeof(char));
        Fnchain(F,len,buf3);
        free(buf3);
    } while (len!=0);
    return 0;
}

int GIFCommentBloc(FILE* F)
{
    uchar len;
    do
    {
        char* buf3;
        len = Fuchar(F);
        if (len==0)
            break;
        buf3 = (char*)malloc((len+1)*sizeof(char));
        Fnchain(F,len,buf3);
        free(buf3);
    } while (len!=0);
    return 0;
}

int GIFsubbloc(FILE* F,GIFBrut* g)
{
    uchar GCL;
    GCL = Fuchar(F);  //Graphic Control Label
    switch(GCL)
    {
    case 0xF9:
        return GIFanimate(F,&g->an);
    case 0xFF:
        return GIFAppBlocExtention(F);
    case 0xFE:
        return GIFCommentBloc(F);
    default:
        break;
    }
    return -10;
}

int GetBit2(BinFlux* pbin)
{
    short res;
    if (pbin->curs>=pbin->sizebuf)
    {
        pbin->sizebuf = Fuchar(pbin->F);
        pbin->lastsize = pbin->sizebuf;
        if (pbin->sizebuf==0)
        {
            memset(pbin->buf,0,256);
            pbin->sizebuf = 8*256;  // assume que des 0 a la fin.
        }
        else
        {
            fread(pbin->buf,pbin->sizebuf,1,pbin->F);
            pbin->sizebuf = 8*pbin->sizebuf;
        }
        pbin->curs = 0;
    }
    res = (((pbin->buf[(pbin->curs)/8])>>((pbin->curs)%8))&1);
    pbin->curs++;
    return res;
}

short GetData2(BinFlux* pbin,char codesize)
{
    char i;
    short res = 0;
    for(i=0;i<codesize;i++)
    {
        short bit = GetBit2(pbin);
        bit<<=i;
        res|=bit;
    }
    return res;
}

int GIFLZWInitDicoBin(LZWDico2* pdico,BinFlux* pbin,FILE* F)
{
    memset(pdico,0,sizeof(LZWDico2));
    memset(pbin,0,sizeof(BinFlux));
    pbin->F = F;
    pdico->codesize = Fuchar(F)+1;
    pdico->basiscodesize = pdico->codesize;
    pdico->nbcol = (1<<(pdico->codesize-1));   // pas nbcol (24.gif)
    pdico->entrysize = 0;
    return 0;
}

int LZWDico2_Reinit(LZWDico2* pdico)
{
    short i;
    pdico->codesize = pdico->basiscodesize;
    pdico->entrysize = pdico->nbcol +2;
    for(i=0;i<pdico->nbcol;i++)
    {
        pdico->e[i].prev = -1;
        pdico->e[i].data = (uchar)i;
    }
    return 0;
}

uchar LZWGetFirst(LZWDico2* pdico,LZWDico2_entry w)
{
    short ind = w.prev;
    if (ind==-1)
        return w.data;
    while(pdico->e[ind].prev!=-1)
        ind = pdico->e[ind].prev;
    return pdico->e[ind].data;
}

int GIFLZWoutput1(uchar val,uchar* out,int* outcurs,int nbpix)
{
    if (*outcurs==nbpix)
        return -1; // overflow : ignore.
    out[(*outcurs)++] = val;
    //printf("%d\t",val);
    return 0;
}

int GIFLZWoutputR(LZWDico2* pdico,short ind,uchar* out,int* outcurs,int nbpix)
{
    if (ind==-1)
        return 0;
    GIFLZWoutputR(pdico,pdico->e[ind].prev,out,outcurs,nbpix);
    GIFLZWoutput1(pdico->e[ind].data,out,outcurs,nbpix);
    return 0;
}

int GIFLZWoutput(LZWDico2* pdico,LZWDico2_entry w,uchar* out,int* outcurs,int nbpix)
{
    GIFLZWoutputR(pdico,w.prev,out,outcurs,nbpix);
    GIFLZWoutput1(w.data,out,outcurs,nbpix);
    return 0;
}

int GIF_UpdateCodeSizeIfNeeded2(LZWDico2* pdico)
{
    int max = 1<<(pdico->codesize);
    if (pdico->entrysize==max)
    {
        pdico->codesize++;
        if (pdico->codesize>12)
            pdico->codesize = 12;  // force. Et normalement, le suivant est un reset table.
    }
    return 0;
}

int GIFLZWblocs2(FILE* F,uchar* out,int nbcol,int nbpix)
{
    int bcurs,cpt,outcurs;
    short data;
    LZWDico2 dico;
    BinFlux bin;
    short lastwrank;
    LZWDico2_entry w;
    GIFLZWInitDicoBin(&dico,&bin,F);
    for(bcurs=0,cpt=0,outcurs=0,lastwrank=0;outcurs<nbpix;cpt++)
    {
        data = GetData2(&bin,dico.codesize);
        if (data==dico.nbcol)
        {
            LZWDico2_Reinit(&dico);
            data = GetData2(&bin,dico.codesize);
            w = dico.e[data];
            lastwrank = data;
            GIFLZWoutput1(data,out,&outcurs,nbpix);
            continue;
        }
        if (data == dico.nbcol+1)
            break;  // eof
        if (data<dico.entrysize)
        {
            uchar t0 = LZWGetFirst(&dico,dico.e[data]);
            GIFLZWoutput(&dico,dico.e[data],out,&outcurs,nbpix);
            dico.e[dico.entrysize].prev = lastwrank;
            dico.e[dico.entrysize].data = t0;
            lastwrank = data;
            w = dico.e[data];
            dico.entrysize++;
        }
        else
        {
            uchar w0 = LZWGetFirst(&dico,w);
            GIFLZWoutput(&dico,w,out,&outcurs,nbpix);
            GIFLZWoutput1(w0,out,&outcurs,nbpix);
            dico.e[dico.entrysize].prev = lastwrank;
            dico.e[dico.entrysize].data = w0;
            lastwrank = dico.entrysize;
            w = dico.e[dico.entrysize];
            dico.entrysize++;
        }
        GIF_UpdateCodeSizeIfNeeded2(&dico);
    }
    if (bin.lastsize!=0)
        Fuchar(F);  // 00
    return 0;
}

int GIFFrame(FILE* F,Frame* fr,int nbcol)
{
    uchar mask;
    fr->left = Fushort(F);
    fr->top = Fushort(F);
    fr->width = Fushort(F);
    fr->height = Fushort(F);
    mask = Fuchar(F);
    fr->m = (mask>>7);
    fr->i = ((mask>>6)&1);
    fr->sortflag = ((mask>>5)&1);
    fr->localcolortab = (mask&7);
    fr->localpal.nbcol = 0;
    if (fr->m)
    {
        GIFPalette(F,fr->localcolortab,&fr->localpal);
        nbcol = fr->localpal.nbcol;
    }
    fr->pix = (uchar*)malloc((fr->width)*(fr->height)*sizeof(uchar));
    //GIFLZWblocs(F,fr,nbcol);
    GIFLZWblocs2(F,fr->pix,nbcol,(fr->width)*(fr->height));
    return 0;
}

GIFBrut* GIFerror(GIFBrut* out,int errcode)
{
    //assert(0);
    out->lasterror = errcode;
    return out;
}

int PushFrame(GIFBrut* out,Frame* fr)
{
    if (out->tfr==NULL)
    {
        out->tfr = (Frame**)calloc(1,sizeof(Frame*));
        out->nb = 0;
        out->rsv = 1;
    }
    if (out->nb==out->rsv)
    {
        out->rsv*=2;
        out->tfr = (Frame**)realloc(out->tfr,out->rsv*sizeof(Frame*));
        memset(out->tfr + out->nb,0,out->rsv-out->nb);
    }
    out->tfr[out->nb++] = fr;
    return 0;
}

GIFBrut* LoadGif(const char* ingif)
{//http://www.u229.no/stuff/GIFFormat/#logical_screen_descriptor
//ftp://ftp.ncsa.uiuc.edu/misc/file.formats/graphics.formats/gif89a.doc
//http://www.martinreddy.net/gfx/2d/GIF-comp.txt
    char version[4];
    uchar descriptor;
    GIFBrut* out;
    int err=0;
    FILE* F;
    out = (GIFBrut*)calloc(1,sizeof(GIFBrut));
    F = fopen(ingif,"rb");
    if (!F)
        return GIFerror(out,-1);
    if (GIFCheckHead(F,version)!=0)
        return GIFerror(out,-2);
    if (GIFLSD(F,&out->lsd)!=0)
        return GIFerror(out,-3);
    if (out->lsd.GlobalColorTableFlag)
        GIFPalette(F,out->lsd.SizeofGlobalColorTable,&out->globalpal);  // sinon, palette locale.
    descriptor = 0;
    do
    {
        descriptor = Fuchar(F);
        //printf("%x\t",descriptor);
        switch(descriptor)
        {
        case 0x21:
            if (err==GIFsubbloc(F,out)!=0)
                GIFerror(out,err);
            break;
        case 0x2C:
            {
                Frame* fr = (Frame*)malloc(sizeof(Frame));
                if (GIFFrame(F,fr,out->globalpal.nbcol)!=0)
                    GIFerror(out,-7);
                fr->an = out->an;
                PushFrame(out,fr);
                //return out; // SHUNT 1 frame
            }
            break;
        case 0x3B: // final terminator.
            break;
        default:
                GIFerror(out,-8);
            break;
        }
    }
    while(descriptor!=0x3B);
    fclose(F);
    return out;
}

int ReleaseGif(GIFBrut* g)
{
    if (g)
    {
        if (g->tfr)
        {
            int i;
            for(i=0;i<g->nb;i++)
            {
                free(g->tfr[i]->pix);
                free(g->tfr[i]);
            }
            free(g->tfr);
        }
        free(g);
    }
    return 0;
}

void SDL_PutPixel32(SDL_Surface *surface, int x, int y, unsigned long pixel)
{
    unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * 4;
    *(unsigned long*)p = pixel;
}

unsigned long SDLGifGetPixelFromIndex(int ind,Palette* pal)
{
    return pal->pix[ind].r * 0x10000 + pal->pix[ind].g * 0x100 + pal->pix[ind].b;
}

int SDLProcessDisposal(Frame* fr,Palette* pal,SDL_Surface* srf,unsigned char backgroundcolorindex)
{ // http://www.webreference.com/content/studio/disposal.html
    SDL_Rect clipzone;
    clipzone.x = fr->left;
    clipzone.y = fr->top;
    clipzone.w = fr->width;
    clipzone.h = fr->height;
    switch (fr->an.disposalmethod)
    {
    case 1: // Do not dispose
        while(0);
        //SDL_FillRect(srf,&clipzone,0xFF000000);
        break;
    case 2: // Restore to background color
        SDL_FillRect(srf,&clipzone,0xFF000000); // ou NULL ?
        break;
    case 0: // No disposal specified
    case 3: // Restore to previous
    default:
        assert(0);
        break;
    }
    return 0;
}

SDL_Surface* SDLLoadGif_OneSurf(Frame* fr,Palette* pal,SDL_Surface* prec,unsigned char backgroundcolorindex)
{
    int i,j,cpt;
    unsigned long pixel;
    SDL_Surface* srf = SDL_CreateRGBSurface(0,prec->w,prec->h,32,0,0,0,0);
    SDL_FillRect(srf,NULL,0xFF000000);
    SDL_BlitSurface(prec,NULL,srf,NULL);
    SDLProcessDisposal(fr,pal,srf,backgroundcolorindex);
    if (SDL_MUSTLOCK(srf))
        SDL_LockSurface(srf);
    for(cpt=0,j=0;j<fr->height;j++)
        for(i=0;i<fr->width;i++,cpt++)
        {
            if (!fr->an.transparentflag || fr->an.transparentcolorindex!=fr->pix[cpt])
            {
                pixel = SDLGifGetPixelFromIndex(fr->pix[cpt],pal);
                SDL_PutPixel32(srf,i+fr->left,j+fr->top,pixel);
            }
        }
    if (SDL_MUSTLOCK(srf))
        SDL_UnlockSurface(srf);
    SDL_SetColorKey(srf,0 ,0xFF000000);
    return srf;
}

SDL_Gif* SDLLoadGif(const char* fic)
{
    int i;
    GIFBrut* g;
    SDL_Gif* out;
    SDL_Surface* prec;
    SDL_Surface* keepprec;
    g = LoadGif(fic);
    if (g->lasterror!=0)
    {
        ReleaseGif(g);
        return NULL;
    }
    prec = SDL_CreateRGBSurface(0,g->lsd.width,g->lsd.height,32,0,0,0,0);
    SDL_FillRect(prec,NULL,0xFF000000);
    keepprec = prec;
    out = (SDL_Gif*)malloc(sizeof(SDL_Gif));
    out->nbframes = g->nb;
    out->tab = (SDL_Surface**)malloc(g->nb*sizeof(SDL_Surface*));
    out->timers = (int*)malloc((g->nb+1)*sizeof(int));
    out->timers[0] = 0;
    for(i=0;i<g->nb;i++)
    {
        out->timers[i+1] = out->timers[i] + g->tfr[i]->an.delay;
        if (g->tfr[i]->m==0)
            out->tab[i] = SDLLoadGif_OneSurf(g->tfr[i],&g->globalpal,prec,g->lsd.BackGroundColorIndex);
        else
            out->tab[i] = SDLLoadGif_OneSurf(g->tfr[i],&g->tfr[i]->localpal,prec,g->lsd.BackGroundColorIndex);
        prec = out->tab[i];
    }
    if (out->timers[g->nb]==0)
        out->timers[g->nb] = 1;  // au moins 1 frame d'anim, meme si pas d'anims.
    SDL_FreeSurface(keepprec);
    ReleaseGif(g);
    return out;
}

int SDLFreeGif(SDL_Gif* g)
{
    if (g)
    {
        if (g->tab)
        {
            int i;
            for(i=0;i<g->nbframes;i++)
                SDL_FreeSurface(g->tab[i]);
            free(g->tab);
            free(g->timers);
        }
        free(g);
    }
    return 0;
}

SDL_Surface* SDLGifAutoFrame(SDL_Gif* g)
{
    int timer,i;
    timer = SDL_GetTicks()/10;
    timer = timer%(g->timers[g->nbframes]);
    for(i=1;i<=g->nbframes;i++)
        if (timer<g->timers[i])
            return g->tab[i-1];
    assert(0);
    return NULL;
}
