/*
jeu.h
-----

Par mateo21, pour Le Site du Z�r0 (www.siteduzero.com)

R�le : prototypes des fonctions du jeu.
*/
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constantes.h"

#ifndef DEF_JEU
#define DEF_JEU

    void jouer(SDL_Renderer* fenetre, SDL_Texture* ecran);
    void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction);
    void deplacerCaisse(int *premiereCase, int *secondeCase);
    void messageFin(SDL_Renderer* fenetre);

#endif
