#ifndef JEUMARIO_H
#define JEUMARIO_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


    #define TAILLE_BLOC         34 // Taille d'un bloc (carré) en pixels
    #define NB_BLOCS_LARGEUR    12
    #define NB_BLOCS_HAUTEUR    12
    #define LARGEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_LARGEUR
    #define HAUTEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_HAUTEUR


    enum {HAUT, BAS, GAUCHE, DROITE};
    enum {VIDE, MUR, CAISSE, OBJECTIF, MARIO, CAISSE_OK};

    void editeur(SDL_Renderer* fenetre);
    void jouer(SDL_Renderer* fenetre);
    void deplacerJoueur(int carte[][NB_BLOCS_HAUTEUR], SDL_Rect *pos, int direction);
    void deplacerCaisse(int *premiereCase, int *secondeCase);
    void messageFin(SDL_Renderer* fenetre);
    int chargerNiveau(int niveau[][NB_BLOCS_HAUTEUR],int NV);
    int sauvegarderNiveau(int niveau[][NB_BLOCS_HAUTEUR]);
    int getnbNV();

#endif // JEUMARIO_H
