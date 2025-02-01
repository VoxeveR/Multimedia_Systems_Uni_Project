// podstawowe funkcje
#ifndef SM2024_KOMPRESJE_H_INCLUDED
#define SM2024_KOMPRESJE_H_INCLUDED
#include <vector>
#include <iostream>

struct slowo {
    Uint16 kod = 0;
    Uint8 dlugosc = 0;
    Uint8 element[4096];
    bool wSlowniku = false;
};

slowo noweSlowo();
slowo noweSlowo(Uint8 znak);
slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);
bool porownajSlowa(slowo slowo1, slowo slowo2);
int znajdzWSlowniku(slowo szukany);
void wyswietlSlowo(slowo aktualneSlowo);
void wyswietlSlownik();
int dodajDoSlownika(slowo nowy, bool czyWyswietlac);
void LZWinicjalizacja();
void LZWKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string filename);
void LZWDekompresja(std::string filename);

void ByteRunKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName);
void ByteRunDekompresja(std::string fileName);

void RLEKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName);
void RLEDekompresja(std::string fileName);


void LZ77Kompresja(const std::vector<Uint8> input, int length, std::string filename);
//void LZ77Dekompresja(std::string filename);


#endif // SM2024_FUNKCJE_H_INCLUDED
