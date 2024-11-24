// podstawowe funkcje
#ifndef SM2024_KOMPRESJE_H_INCLUDED
#define SM2024_KOMPRESJE_H_INCLUDED
#include <vector>
#include <iostream>

void ByteRunKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName);
void ByteRunDekompresja(std::string fileName);

void RLEKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName);
void RLEDekompresja(std::string fileName);

#endif // SM2024_FUNKCJE_H_INCLUDED
