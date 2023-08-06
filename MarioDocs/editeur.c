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

void editeur(SDL_Renderer* fenetre, SDL_Texture* ecran)
{
    SDL_Surface *mur = NULL, *caisse = NULL, *objectif = NULL, *mario = NULL, *caisseOK = NULL;
    SDL_Surface *surface=SDL_CreateRGBSurface (0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);;
    SDL_Rect position;
    SDL_Event event;
    SDL_Rect src1={0, 0, 0, 0},posObjActuel,fond,ptxt,souris;
    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0;
    int objetActuel = MUR, i = 0, j = 0;
    int carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};
    SDL_Texture *texture;
    //Uint32 SDL_Color couleurs={250,0,0,255};
    // Chargement des objets et du niveau
    mur = IMG_Load("mur.jpg");
    caisse = IMG_Load("caisse.jpg");
    objectif = IMG_Load("objectif.png");
    mario = IMG_Load("mario_bas.gif");
    caisseOK = IMG_Load("caisse_ok.jpg");
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
    TTF_Font* font = TTF_OpenFont("SuperMario256.ttf", 16); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

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
        
        //SDL_RenderClear(fenetre);
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
