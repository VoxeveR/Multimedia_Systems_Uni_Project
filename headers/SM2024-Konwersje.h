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

void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB555(int xx, int yy, Uint16 rgb555);
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB565(int xx, int yy, Uint16 rgb565);
SDL_Color getRGB555(int xx, int yy);
Uint16 getRGB555_(int xx, int yy);
SDL_Color getRGB565(int xx, int yy);
Uint16 getRGB565_(int xx, int yy);

#endif // SM2024_KONWERSJE_H_INCLUDED
