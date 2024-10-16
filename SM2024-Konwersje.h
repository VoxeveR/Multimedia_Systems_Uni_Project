// konwersje
#ifndef SM2024_KONWERSJE_H_INCLUDED
#define SM2024_KONWERSJE_H_INCLUDED

//#include <SDL2/SDL.h>
//#include <windows.h>

Uint8 z24RGBna5RGB(SDL_Color kolor);
Uint8 z24RGBna8BW(SDL_Color kolor);
SDL_Color z5RGBna24RGB(Uint8 kolor5b);
Uint8 z24RGBna5BW(SDL_Color kolor);
SDL_Color z5BWna24RGB(Uint8 szary5b);

#endif // SM2024_KONWERSJE_H_INCLUDED
