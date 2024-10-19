// metody do algorytmu kwantyzacji (MedianCut)
#ifndef SM2024_MEDIANCUT_H_INCLUDED
#define SM2024_MEDIANCUT_H_INCLUDED

#include <SDL2/SDL.h>

//mediancut moze miec daddy issues
void medianCut(int start, int koniec, int iter);
void medianCutBW(int start, int koniec, int iteracja);
int znajdzSasiadaBW(Uint8 wartosc);
void sortujKubelekKolor(int start, int koniec, int sortowanie);
void medianCutRun();

int znajdzSasiada(SDL_Color kolor);
int znajdzSasiadaBW(Uint8 wartosc);
//int partition(int start, int koniec);
//void sortujKubelekBW(int start, int koniec);
//int najwiekszaRoznica(int start, int koniec);


#endif // SM2024_MEDIANCUT_H_INCLUDED


