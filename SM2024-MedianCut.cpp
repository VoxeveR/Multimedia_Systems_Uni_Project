// metody do algorytmu kwantyzacji (MedianCut)
#include "SM2024-MedianCut.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Funkcje.h"
#include "SM2024-Dithering.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                 MEDIAN CUT                    ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

using namespace std;

void sortujKubelekKolor(int start, int koniec, int sortowanie) {
    for(int i = start; i <= koniec; i++) {
        int maxI = i;
        for(int ii = i; ii <= koniec; ii++) {
            switch (sortowanie)
            {
            case 1:
                if(obrazek[ii].r > obrazek[maxI].r)
                    maxI = ii;
                break;
            case 2:
                if(obrazek[ii].g > obrazek[maxI].g)
                    maxI = ii;
                break;
            case 3:
                if(obrazek[ii].b > obrazek[maxI].b)
                    maxI = ii;
                break;
            default:
                break;
            }
        }
        swap(obrazek[i], obrazek[maxI]);
    }
}

int najwiekszaRoznica(int start, int koniec) {
    int minR=0, minG=0, minB=0;
    int maxR=0, maxG=0, maxB=0;

    int roznicaR, roznicaG, roznicaB;

    for(int i = start; i < koniec; i++)
    {
        if(obrazek[i].r < obrazek[minR].r) minR = i;
        if(obrazek[i].g < obrazek[minB].g) minG = i;
        if(obrazek[i].b < obrazek[minG].b) minB = i;

        if(obrazek[i].r > obrazek[maxR].r) minR = i;
        if(obrazek[i].g > obrazek[maxG].g) maxG = i;
        if(obrazek[i].b > obrazek[maxB].b) maxB = i;
    }

    roznicaR = obrazek[maxR].r - obrazek[minR].r;
    roznicaG = obrazek[maxG].g - obrazek[minG].g;
    roznicaB = obrazek[maxB].b - obrazek[minB].b;

    int wynik = max(max(roznicaR, roznicaG), roznicaB);

    if(wynik == roznicaR) return 1;
    if(wynik == roznicaG) return 2;
    if(wynik == roznicaB) return 3;

    return 0;
}

void medianCut(int start, int koniec, int iter) {
    if(iter > 0)
    {

        Uint8 sortowanie = najwiekszaRoznica(start,koniec);


       // cout << "start: " << start << ", koniec: " << koniec << ", iteracja: " << iter << endl;
        sortujKubelekKolor(start, koniec, sortowanie);
        //wyswietlWartosci();

        int sr = (start + koniec + 1)/2;
        medianCut(start, sr - 1, iter -1);
        medianCut(sr, koniec, iter-1);

    }
    else{

        //cout << "start: " << start << ", koniec: " << koniec << ", iteracja: " << iter << endl;

        int sumaR = 0, sumaG = 0, sumaB = 0;
        for(int i = start; i <= koniec; i++)
        {
            sumaR += obrazek[i].r;
            sumaG += obrazek[i].g;
            sumaB += obrazek[i].b;
        }

        int n = koniec + 1 - start;

        SDL_Color nowyKolor = {sumaR/n, sumaG/n, sumaB/n};

        paleta5[ileKubelkow] = nowyKolor;

        //cout << "Kolor " << ileKubelkow << "(" << (int)nowyKolor.r << "," << (int)nowyKolor.g << "," << (int)nowyKolor.b << ")\n";

        ileKubelkow++;
    }
}

void medianCutRun(){
    ileKubelkow = 0;
    int numer = 0;
    SDL_Color kolor;
    int indeks = 0;

    for(int y=0;y<wysokosc;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor = getPixel(x,y);
            obrazek[numer] = kolor;

            //setPixel(x+szerokosc/2, y, kolor.r, kolor.g, kolor.b);
            numer++;
        }
    }

    medianCut(0, numer-1, 5);

    for(int y=0;y<wysokosc;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor = getPixel(x,y);
            indeks = znajdzSasiada(kolor);
            setPixel(x + szerokosc/2, y, paleta5[indeks].r, paleta5[indeks].g, paleta5[indeks].b);
        }
    }

   // narysujPalete(0, 0 + wysokosc/2, paleta);
    SDL_UpdateWindowSurface(window);
}

int partition(int start, int koniec){
    int pivot = obrazek[start].r;

    int count = 0;
    for (int i = start + 1; i <= koniec; i++) {
        if (obrazek[i].r <= pivot)
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(obrazek[pivotIndex], obrazek[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = koniec;

    while (i < pivotIndex && j > pivotIndex) {

        while (obrazek[i].r <= pivot) {
            i++;
        }

        while (obrazek[j].r > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(obrazek[i++], obrazek[j--]);
        }
    }

    return pivotIndex;
}

void sortujKubelekBW(int start, int koniec){
    if (start >= koniec)
        return;

    // partitioning the array
    int p = partition(start, koniec);

    // Sorting the left part
    sortujKubelekBW(start, p - 1);

    // Sorting the right part
    sortujKubelekBW(p + 1, koniec);
}

void medianCutBW(int start, int koniec, int iteracja){

    if(iteracja > 0){
        cout << "start: " << start << " koniec: " << koniec << " iteracja: " << iteracja << endl;

        sortujKubelekBW(start, koniec);
        //wyswietlWartosci();

        int srodek = (start + koniec+1)/2;

        medianCutBW(start, srodek-1, iteracja-1);
        medianCutBW(srodek, koniec, iteracja-1);
    }else{
        cout << "start: " << start << ", koniec: " << koniec << ", iteracja: " << endl;
        int sumaBW = 0;

        for(int i = start; i <= koniec; i++){
            sumaBW += obrazek[i].r;
        };

        Uint8 nowyBW = sumaBW / (koniec+1 - start);
        SDL_Color nowyKolor = {nowyBW, nowyBW, nowyBW};
        paleta5[ileKubelkow] = nowyKolor;
        cout << " Kubelek " << ileKubelkow << ": (s:" << start << " k: " << koniec << "e: " << (koniec+1 - start) <<endl;
        cout << "Kolor: "<< ileKubelkow << endl;
        ileKubelkow++;
    }
}
