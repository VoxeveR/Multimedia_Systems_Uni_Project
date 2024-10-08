// podstawowe funkcje
#ifndef SM2024_FUNKCJE_H_INCLUDED
#define SM2024_FUNKCJE_H_INCLUDED

//#include <stdio.h>
//#include <math.h>
#include <SDL2/SDL.h>
#include <windows.h>

Uint8 z24RGBna5RGB(SDL_Color kolor);
Uint8 z24RGBna8BW(SDL_Color kolor);
SDL_Color z5RGBna24RGB(Uint8 kolor5b);
Uint8 z24RGBna5BW(SDL_Color kolor);
SDL_Color z5BWna24RGB(Uint8 szary5b);
void szaryNarzucony();
void szaryDedykowany();
void RightToLeft();
int znajdzSasiada(SDL_Color kolor);
int znajdzSasiadaBW(Uint8 wartosc);


void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface);

void gotoxy(int x, int y);
void color(int color);
#endif // SM2024_FUNKCJE_H_INCLUDED
