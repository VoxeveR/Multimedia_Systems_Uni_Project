// transformata DCT
#ifndef SM2024_DCT_H_INCLUDED
#define SM2024_DCT_H_INCLUDED
#include <exception>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

const int rozmiarBloku = 8;

struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    float dctI[rozmiarBloku][rozmiarBloku];
    float dctQ[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
    Uint8 daneI[rozmiarBloku][rozmiarBloku];
    Uint8 daneQ[rozmiarBloku][rozmiarBloku];
};


void kwantyzujDCT(macierz& blok);
void dekwantyzujDCT(macierz& blokDCT);
void wyswietlDane(macierz blok);
void wyswietlDCT(macierz blok);
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

#endif // SM2024_DCT_H_INCLUDED
