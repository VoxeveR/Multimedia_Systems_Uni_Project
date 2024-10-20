#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"

Uint8 z24RGBna5RGB(SDL_Color kolor){

    int R, G, B;
    int nowyR, nowyG, nowyB;
    Uint8 kolor5b;

    R = kolor.r;
    G = kolor.g;
    B = kolor.b;

    nowyR = round(R*3.0/255.0);
    nowyG = round(G*3.0/255.0);
    nowyB = round(B*1.0/255.0);

    kolor5b = (nowyR<<3) | (nowyG<<1) | (nowyB);

    return kolor5b;
}

Uint8 z24RGBna8BW(SDL_Color kolor){
    return 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
}

SDL_Color z5RGBna24RGB(Uint8 kolor5b){

    int nowyR, nowyG, nowyB;
    SDL_Color kolor;

    nowyR = kolor5b&0b00011000;
    nowyG = kolor5b&0b00000110;
    nowyB = kolor5b&0b00000001;

    nowyR = nowyR>>3;
    nowyG = nowyG>>1;

    kolor.r = nowyR*255.0/3.0;
    kolor.g = nowyG*255.0/3.0;
    kolor.b = nowyB*255.0/1.0;

    return kolor;
}

Uint8 z24RGBna5BW(SDL_Color kolor){
    Uint8 szary8b;
    Uint8 szary5b;

    szary8b = 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
    szary5b = round(szary8b*31.0/255.0);

    return szary5b;

}

SDL_Color z5BWna24RGB(Uint8 szary5b){
    Uint8 szary8b;
    SDL_Color kolor;

    szary8b = szary5b*255.0/31.0;

    kolor.r = szary8b;
    kolor.g = szary8b;
    kolor.b = szary8b;

    return kolor;
}
