#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "fgifsdl.h"
#include "constantes.h"
#include "jeu.h"
#include "editeur.h"
#include "fichiers.h"

int KeyHit()
{
    SDL_Event e;
    if (SDL_PollEvent(&e))
        if (e.type == SDL_KEYDOWN)
            return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    SDL_Surface *menu = NULL, *instructJeu = NULL, *instructEdit = NULL, *text=NULL;
    SDL_Event event;
    SDL_Surface* tmp,*tmp2,*screen=SDL_CreateRGBSurface (0, LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, 0, 0, 0, 0);
    int continuer = 1;
    SDL_Gif* gif,* gif2;
    SDL_Rect position,position2,pos2,posTitre;
    SDL_Texture *Tri,* Tri2,* texture,* instText;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Mario Sokoban", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *fenetre = SDL_CreateRenderer(window, -1, render_flags);
    SDL_SetWindowIcon(window,IMG_Load("caisse.jpg"));

    menu = IMG_Load("menu.jpg");
    text = IMG_Load("titreMario.png");
    instructJeu = IMG_Load("instructions_jeu.png");
    instructEdit = IMG_Load("instructions.jpg");
    
    SDL_Rect positionMenu={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
    SDL_Rect src1={0, 0, 0, 0};
    SDL_Rect pos={0, 0, LARGEUR_FENETRE, HAUTEUR_FENETRE};
/*    
    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_Color couleur={250,0,0,255 };
    TTF_Font* font = TTF_OpenFont("SuperMario256.ttf", 25); // Crée un police avec la police "ariali.ttf" et de taille 18 pixels

    if (font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());

    }
    SDL_Surface* text = TTF_RenderText_Blended(font, "Mario Sokoban", couleur); // Crée un surface qui contient le texte
*/
    SDL_Texture* test=SDL_CreateTextureFromSurface(fenetre,text);
    SDL_QueryTexture(test, NULL, NULL, &posTitre.w, &posTitre.h);
    posTitre.y=0;
    posTitre.x=LARGEUR_FENETRE/2-posTitre.w/2;
    SDL_DestroyTexture(test);
    if (text == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
    }

    if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    //Mix_AllocateChannels(1); // Allouer 1 cannal
    //Mix_Volume(0, MIX_MAX_VOLUME/2); // Mets le son a 100% en volume pour le premier cannaux
    Mix_Music* soundA = Mix_LoadMUS("SuperMarioBros.wav"); // charge la musique

    Uint8 volume = MIX_MAX_VOLUME/2; Mix_VolumeMusic(volume); // Mets le volume a 0
    Mix_PlayMusic(soundA,-1); // Joue soundA 1 fois sur le canal 1

    ////////////////////////////////////////  GIF  //////////////////////////////////////////////
    gif = SDLLoadGif("supermario.gif");
    gif2 = SDLLoadGif("key.gif");

    while(!KeyHit())
    {
        
        tmp2 = SDLGifAutoFrame(gif2);
        tmp = SDLGifAutoFrame(gif);
        Tri=SDL_CreateTextureFromSurface(fenetre, tmp);
        Tri2=SDL_CreateTextureFromSurface(fenetre, tmp2);

        SDL_QueryTexture(Tri, NULL, NULL, &position.w, &position.h);
        // Centre le GIF
        pos.x=LARGEUR_FENETRE/2-5*position.w/8;
        pos.y=HAUTEUR_FENETRE/2-4*position.h/8;
        pos.w=0;
        pos.h=0;
        SDL_BlitSurface(tmp,NULL,screen,&pos);

        
        SDL_QueryTexture(Tri2, NULL, NULL, &position2.w, &position2.h);
        // Centre le GIF
        pos2.x=LARGEUR_FENETRE/2-position2.w/2;
        pos2.y=HAUTEUR_FENETRE/2+position.h/2;
        pos2.w=0;
        pos2.h=0;
        SDL_BlitSurface(tmp2,NULL,screen,&pos2);
        SDL_BlitSurface(text, NULL, screen, &posTitre); // Affiche Niveau

        //SDL_SetRenderDrawColor(fenetre, 255, 255, 255, 255); 
        texture= SDL_CreateTextureFromSurface(fenetre, screen);
        SDL_RenderClear(fenetre);
            SDL_RenderCopy(fenetre, texture, NULL, NULL);
        SDL_RenderPresent(fenetre);
        
    }
    SDL_FreeSurface(text);
    SDLFreeGif(gif);
    SDLFreeGif(gif2);
    SDL_DestroyTexture(Tri);
    SDL_DestroyTexture(Tri2);

    texture = SDL_CreateTextureFromSurface(fenetre, menu);
    SDL_QueryTexture(texture, NULL, NULL, &src1.w, &src1.h);
    SDL_FreeSurface(menu);
    /////////////////////////////////////////////////////////////////////////////////////////////
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: // Veut arrêter le jeu
                        continuer = 0;
                        break;
                    case SDLK_j: // Demande à jouer
                        instText = SDL_CreateTextureFromSurface(fenetre, instructJeu);
                        
                        while(!KeyHit())
                        {
                            SDL_RenderClear(fenetre);
                            SDL_RenderCopy(fenetre, instText, NULL, NULL);
                            SDL_RenderPresent(fenetre);
                        }
                        //SDL_Delay(1000);
                        jouer(fenetre,texture);
                        break;
                    case SDLK_e: // Demande l'éditeur de niveaux
                        instText = SDL_CreateTextureFromSurface(fenetre, instructEdit);
                        
                        while(!KeyHit())
                        {
                            SDL_RenderClear(fenetre);
                            SDL_RenderCopy(fenetre, instText, NULL, NULL);
                            SDL_RenderPresent(fenetre);
                        }
                        //SDL_Delay(1000);
                        editeur(fenetre,texture);
                        break;
                }
                break;
        }
        SDL_SetRenderDrawColor(fenetre, 255, 255, 255, 255); 
        SDL_RenderClear(fenetre);
        
        //SDL_BlitSurface(menu, NULL, ecran, &positionMenu);
        SDL_RenderCopy(fenetre, texture, &src1, &positionMenu);
        SDL_RenderPresent(fenetre);
    }
    SDL_FreeSurface(instructJeu);
    SDL_FreeSurface(instructEdit);
    SDL_DestroyTexture(instText);
    Mix_FreeMusic(soundA); // Libére la mémoire allouer pour le son
    Mix_CloseAudio();
    SDL_DestroyTexture(texture);

    // destroy renderer
    SDL_DestroyRenderer(fenetre);
 
    // destroy window
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
