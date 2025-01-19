// zmienne globalne
#ifndef SM2024_ZMIENNE_H_INCLUDED
#define SM2024_ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>

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
extern int blackandwhite;
extern int yiqstatus;
extern int bit;
extern int prediction;
extern int compression;
extern int wysokoscObrazka;
extern int szerokoscObrazka;
extern char identyfikator[];

//wartosci domyslne

//FIXME: WYSOKOSC JEST ZLE - POPRAW TO PO ZAJECIACH
struct dane888
{
    std::vector<Uint8> comp1;
    std::vector<Uint8> comp2;
    std::vector<Uint8> comp3;
};

struct daneBW{ 
    std::vector<Uint8> comp;
};

extern dane888 dane24;
extern daneBW dane8;

enum FilterType {
    NO_FILTER,
    DIFFERENTIAL,
    LINE_DIFFERENCE,
    AVERAGING,
    PAETH
};

extern float dither[8][8];

#endif // SM2024_ZMIENNE_H_INCLUDED
