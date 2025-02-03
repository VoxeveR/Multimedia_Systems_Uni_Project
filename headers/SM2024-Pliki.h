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
void zczytajDane16();
void clearVector24();
void clearVector8();
void clearVector16();
void save(std::string nazwa);
void narysujDane24(int xStart, int yStart);
void narysujDane8(int xStart, int yStart);
bool read(std::string nazwa);
bool odczytajPlik_bmp(std::string nazwa);
void saveRightHalfBMP(std::string nazwa);
void ladujBMP(char const* nazwa, int x, int y);

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
