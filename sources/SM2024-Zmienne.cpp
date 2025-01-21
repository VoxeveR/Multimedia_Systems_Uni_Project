// zmienne globalne
#include "../headers/SM2024-Zmienne.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                    Definicje                  ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* screen2 = NULL;

SDL_Color* obrazek;
SDL_Color* dane;

SDL_Color paleta8[640*400];
int ileKubelkow = 0;
int ileKolorow = 0;

SDL_Color paleta8k[256];
SDL_Color paleta8s[256];

SDL_Color paleta5[32];
SDL_Color paletaCala[32];

int tryb = 0;
int dithering = 0;
int blackandwhite = 0;
int yiqstatus = 0;
int bit = 24;
int prediction = 0;
int compression = 0;
int wysokoscObrazka = 0;
int szerokoscObrazka = 0;
char identyfikator[] = "  ";

dane888 dane24;
daneBW dane8;
dane565 dane16;

//wartosci domyslne
int szerokosc = 640;
int wysokosc = 200;
int COLORS_N = 32;

//tablica bayera do ditheringu
float dither[8][8] = {
{ 0, 32, 8, 40, 2, 34, 10, 42},
{48, 16, 56, 24, 50, 18, 58, 26},
{12, 44, 4, 36, 14, 46, 6, 38},
{60, 28, 52, 20, 62, 30, 54, 22},
{ 3, 35, 11, 43, 1, 33, 9, 41},
{51, 19, 59, 27, 49, 17, 57, 25},
{15, 47, 7, 39, 13, 45, 5, 37},
{63, 31, 55, 23, 61, 29, 53, 21} };
