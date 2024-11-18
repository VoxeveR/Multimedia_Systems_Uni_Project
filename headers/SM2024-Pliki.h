// funkcje do operacji na plikach
#ifndef SM2024_PLIKI_H_INCLUDED
#define SM2024_PLIKI_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>

void zczytajDane(dane888* dataArr);
void zczytajDane8x8(int xStart, int yStart);
//void unpack8Colors(Uint8* input, Uint8* output);
//void pack8Colors(Uint8* input, Uint8* output);
void save(std::string nazwa);
void narysujDane(int xStart, int yStart);
bool read(std::string nazwa);
bool odczytajPlik_bmp(std::string nazwa);
void saveRightHalfBMP(std::string nazwa);
void ladujBMP(char const* nazwa, int x, int y);
void saveBW(std::string fileName, Uint8* dane, int size);
void saveRGB888(std::string fileName);
void saveYUV888(std::string fileName);
void saveYIQ888(std::string fileName);
void saveYCbCr888(std::string fileName);
void saveHSL888(std::string fileName);
void saveRGB555(std::string fileName);
void saveRGB565(std::string fileName);

struct daneProbkowanie
{
    Uint8 comp1[320*200];
    Uint8 comp2[160*100];
    Uint8 comp3[160*100];
};


#endif // SM2024_PLIKI_H_INCLUDED
