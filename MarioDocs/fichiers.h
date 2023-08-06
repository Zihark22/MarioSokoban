/*
fichiers.h
----------

Par mateo21, pour Le Site du Z�r0 (www.siteduzero.com)

R�le : prototypes des fonctions de lecture / �criture de fichiers de niveau.
*/


#ifndef DEF_FICHIERS
#define DEF_FICHIERS

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "constantes.h"

int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR],int NV);
int sauvegarderNiveau(int niveau[][NB_BLOCS_HAUTEUR]);
int getnbNV();

#endif

