// funkcje do redukcji kolorów i tworzenia palet
#ifndef SM2024_PALETA_H_INCLUDED
#define SM2024_PALETA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

void czyscPalete();
//int dodajKolor(SDL_Color kolor);
//bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2);
//int sprawdzKolor(SDL_Color kolor);
void narysujPalete(int px, int py, SDL_Color paleta5[]);
void paletaWykryta();
void paletaNarzucona();



#endif // SM2024_PALETA_H_INCLUDED
