// podstawowe funkcje
#ifndef SM2024_KOMPRESJE_H_INCLUDED
#define SM2024_KOMPRESJE_H_INCLUDED
#include <vector>
#include <iostream>

std::vector<Sint8> ByteRunKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName);
void ByteRunDekompresja(Uint8 *wejscie, int dlugosc);

#endif // SM2024_FUNKCJE_H_INCLUDED
