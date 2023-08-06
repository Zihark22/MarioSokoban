#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#pragma comment(lib,"sdl.lib")
//#pragma comment(lib,"sdlmain.lib")

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
