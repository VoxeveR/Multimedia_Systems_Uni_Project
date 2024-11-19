#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"

#include <iostream>

using namespace std;

void ByteRunKompresja(Uint8 *wejscie, int dlugosc, std::string fileName) {

    std::string outputFile = fileName;
    int counter = 0;
    int i = 0;
    Uint8 resultArr[dlugosc];
    
    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            int j = 0;
            while((i+  j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + 1 + j])
            && (j < 127)){
                j++;
            }

            // cout<<"("<< -j << "), " << (int)wejscie[i + j] << ", ";


            std::cout << "ELement: " << -j << std::endl;
            resultArr[counter] == -j;
            std::cout << "Value: " << (int) wejscie[i + j] << std::endl;
            resultArr[++counter] == wejscie[i + j];
            counter++;
            
            i += (j+1);
        } else {
            int j = 0;
            while((i + j < dlugosc - 1) && ((wejscie[i + j] != wejscie[i + j + 1])) 
            && (j < 128)){
                j++;
            }

            if((i+j == dlugosc - 1) && (j < 128)){
                j++;
            }

           // cout<<"("<< (j - 1) << "), ";
            resultArr[counter++] == j - 1;

            
            for(int k = 0; k < j; k++){
                // cout << (int) wejscie[i + k] << ", ";
                resultArr[counter++] == wejscie[i + j];
            }

            i += j;
        }
    }

    for (size_t i = 0; i < dlugosc; i++) {
        std::cout << (int)resultArr[i] << " ";
    };

}

void ByteRunDekompresja(Uint8 *wejscie, int dlugosc){
    int j = 0;
    while (j < dlugosc){
        if(wejscie[j] < 0){
            int iters = (-1) * wejscie[j] + 1;
            j++;
            for(int i = 0; i < iters; i ++){
                std::cout << wejscie[j] << ", ";

            }
        } else {
            int iters = wejscie[j] + 1;
            for(int i = 0; i < iters; i ++){
                j++;
                std::cout << wejscie[j] << ", ";
            }
        }
        j++;
    }
}
