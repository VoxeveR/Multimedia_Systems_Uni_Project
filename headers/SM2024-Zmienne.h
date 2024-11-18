// zmienne globalne
#ifndef SM2024_ZMIENNE_H_INCLUDED
#define SM2024_ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>

extern int szerokosc;
extern int wysokosc;
extern int COLORS_N;

#define tytul "SM2024 - Projekt - Zespol 23"

extern SDL_Window* window;
extern SDL_Surface* screen;
extern SDL_Surface* screen2;

extern SDL_Color* obrazek;
extern SDL_Color* dane;

extern SDL_Color paleta5[32];
extern SDL_Color paletaCala[32];

extern int ileKubelkow;
extern int ileKolorow;

extern int tryb;
extern int dithering;
extern int wysokoscObrazka;
extern int szerokoscObrazka;
extern char identyfikator[];

//wartosci domyslne

//FIXME: WYSOKOSC JEST ZLE - POPRAW TO PO ZAJECIACH
struct dane888
{
    Uint8 comp1[320*200];
    Uint8 comp2[320*200];
    Uint8 comp3[320*200];
};


extern float dither[8][8];

#endif // SM2024_ZMIENNE_H_INCLUDED
