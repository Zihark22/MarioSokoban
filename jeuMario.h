#ifndef JEUMARIO_H
#define JEUMARIO_H

#include <QString>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using std::cout; using std::endl;
using std::copy; using std::string;

    typedef unsigned char uchar;
    typedef unsigned short ushort;
    typedef unsigned int uint;

    typedef struct
    {
        ushort width;
        ushort height;
        uint GlobalColorTableFlag:1;
        uint ColorResol:3;
        uint SortFlag:1;
        uint SizeofGlobalColorTable:3;
        uchar BackGroundColorIndex;
        float AspectRatio;
    } LSD;   // hhaaaa !! C'est de la bonne !!

    typedef struct
    {
        uchar r,g,b;
    } DataRGB;

    typedef struct
    {
        DataRGB pix[256];
        int nbcol;
    } Palette;

    typedef struct
    {
        ushort delay;  // 1/100e
        uchar transparentcolorindex;
        uint disposalmethod:2;
        uint userinputflag:1;
        uint transparentflag:1;
    } Animate;

    typedef struct
    {
        ushort left,top,width,height;
        uint i:1; // I=0 - Image formatted in Sequential order  ;  I=1 - Image formatted in Interlaced order
        uint sortflag:1;
        // localpalette
        uint m:1; // M=0 - Use global color map, ignore 'pixel' ;  M=1 - Local color map follows, use 'pixel'
        uint localcolortab:3; // bpp pour calculer Palette.
        Palette localpal;
    // inherit subbloc
        Animate an;
    // datas
        uchar* pix;
    } Frame;

    typedef struct
    {
        LSD lsd;
        Palette globalpal;
        Animate an;
        int lasterror;
    // frames
        Frame** tfr;
        int nb;
        int rsv;
    } GIFBrut;

    GIFBrut* LoadGif(string* ingif);
    int ReleaseGif(GIFBrut*);


    typedef struct
    {
        short prev;
        uchar data;
    } LZWDico2_entry;

    typedef struct
    {
        LZWDico2_entry e[4096];
        short entrysize;
        char basiscodesize;
        char codesize;
        short nbcol;
    } LZWDico2;

    typedef struct
    {
        char buf[256];
        FILE* F;
        short sizebuf,curs;
        uchar lastsize;
    } BinFlux;


    //QString chemin(QString objet);
    char* chemin(char objet[]);

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

#ifdef __cplusplus
extern "C" {
    #endif

    typedef struct
    {
        SDL_Surface** tab;
        int* timers;
        int nbframes;
    } SDL_Gif;

    SDL_Gif* SDLLoadGif(const char* fic);
    int SDLFreeGif(SDL_Gif*);
    SDL_Surface* SDLGifAutoFrame(SDL_Gif*);

    #ifdef __cplusplus
}
#endif

#endif // JEUMARIO_H
