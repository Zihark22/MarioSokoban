/*
editeur.h
---------

Par mateo21, pour Le Site du Z�r0 (www.siteduzero.com)

R�le : prototypes des fonctions de gestion de l'�diteur de niveaux.
*/
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef DEF_EDITEUR
#define DEF_EDITEUR

    void editeur(SDL_Renderer* fenetre, SDL_Texture* ecran);

#endif
