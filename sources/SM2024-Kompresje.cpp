#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"



void ByteRunKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName) {
    int counter = 0;
    int i = 0;
    std::vector<Sint8> resultArr;
    
    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            int j = 0;
            while((i+  j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + 1 + j]) && (j < 127)){
                j++;
            }

            //cout<<"("<< -j << "), " << (int)wejscie[i + j] << ", ";


            //std::cout << "ELement: " << -j << std::endl;
            resultArr.push_back(-j);
            //std::cout << "Value: " << (int) wejscie[i + j] << std::endl;
            resultArr.push_back(wejscie[i + j]);
            counter++;
            
            i += (j+1);
        } else {
            int j = 0;
            while((i + j < dlugosc - 1) && ((wejscie[i + j] != wejscie[i + j + 1])) && (j < 128)){
                j++;
            }

            if((i+j == dlugosc - 1) && (j < 128)){
                j++;
            }

           // std::cout<<"("<< (j - 1) << "), ";
            resultArr.push_back(j - 1);

            
            for(int k = 0; k < j; k++){
              //  std::cout << (int) wejscie[i + k] << ", ";
                resultArr.push_back(wejscie[i + k]);
            }

            i += j;
        }
    }
    // std::cout << "\n";
    // for (size_t i = 0; i < resultArr.size(); i++) {
    //     std::cout << (int)resultArr[i] << ", ";
    // }
    // std::cout << "\n";
    std::cout << "Kompresja udana\n";
    saveVector<Sint8>(resultArr, fileName);
}


void ByteRunDekompresja(std::string fileName){
    int j = 0;
    std::vector<Sint8> wejscie = readVector<Sint8>(fileName);
    int dlugosc = wejscie.size();
    std::vector<Uint8> output;
    while (j < dlugosc){
        if(wejscie[j] < 0){
            int iters = (-1) * wejscie[j] + 1;
            j++;
            for(int i = 0; i < iters; i ++){
               // std::cout << (int)wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        } else {
            int iters = wejscie[j] + 1;
            for(int i = 0; i < iters; i ++){
                j++;
                //std::cout << (int)wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        }
        j++;
    }
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            setPixel(x, y, output[k], output[k], output[k]);
            k++;
        }
    }
}
