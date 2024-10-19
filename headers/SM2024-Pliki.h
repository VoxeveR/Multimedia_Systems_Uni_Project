// funkcje do operacji na plikach
#ifndef SM2024_PLIKI_H_INCLUDED
#define SM2024_PLIKI_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>

void zczytajDane(int xStart, int yStart);
//void unpack8Colors(Uint8* input, Uint8* output);
//void pack8Colors(Uint8* input, Uint8* output);
void save(std::string nazwa);
void narysujDane(int xStart, int yStart);
bool read(std::string nazwa);
bool odczytajPlik_bmp(std::string nazwa);
void saveRightHalfBMP(std::string nazwa);
void ladujBMP(char const* nazwa, int x, int y);

#endif // SM2024_PLIKI_H_INCLUDED
