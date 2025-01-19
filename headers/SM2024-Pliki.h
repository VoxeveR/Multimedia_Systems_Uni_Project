// funkcje do operacji na plikach
#ifndef SM2024_PLIKI_H_INCLUDED
#define SM2024_PLIKI_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

void zczytajDane();
void zczytajDaneBW();
void zczytajDane8x8(int xStart, int yStart);
void clearVector24();
void clearVector8();
//void unpack8Colors(Uint8* input, Uint8* output);
//void pack8Colors(Uint8* input, Uint8* output);
void save(std::string nazwa);
void narysujDane(int xStart, int yStart);
void narysujDane24(int xStart, int yStart);
void narysujDane8(int xStart, int yStart);
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

template <typename T>
void saveVector(std::vector<T> v, std::string fileName) {
    std::ofstream output(fileName, std::ios::binary);

    if (!output) {
        std::cerr << "Nie udało się otworzyć pliku do zapisu: " << fileName << "\n";
        exit(1);
    }


    output.write(reinterpret_cast<char*>(v.data()), v.size()*sizeof(T));
    std::cout << "Zapisano plik \n";
    output.close();
};

template <typename T>
std::vector<T> readVector(std::string fileName){
    std::ifstream input(fileName, std::ios::binary | std::ios::ate);
    //wejscie.read( (char*)&paleta5[i].r, sizeof(Uint8));
    if (!input) {
        std::cerr << "Nie udało się otworzyć pliku: " << fileName << "\n";
        exit(1);
    }

    std::streamsize fileSize = input.tellg();
    
    input.seekg(0, std::ios::beg);

    std::vector<T> buff(fileSize / sizeof(T));
    
    if(!input.read(reinterpret_cast<char*>(buff.data()), fileSize)){
        std::cerr << "Bład podczas odczytu\n";
        exit(1);
    }

    std::cout << "Odczytano plik\n";

  return buff;
};

struct daneProbkowanie
{
    Uint8 comp1[320*200];
    Uint8 comp2[160*100];
    Uint8 comp3[160*100];
};


#endif // SM2024_PLIKI_H_INCLUDED
