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

void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b){
    
    r = (r >> 3) << 3;
    g = (g >> 3) << 3;
    b = (b >> 3) << 3;

    setPixel(xx, yy, r, g, b);
}

void setRGB555(int xx, int yy, Uint16 rgb555){
    Uint8 r, g, b;

    r = ((((rgb555 << 1) >> 11) << 3));
    g = ((((rgb555 << 6) >> 11) << 3));
    b = ((((rgb555 << 11) >> 11) << 3));

    setPixel(xx, yy, r, g, b);
    return;
}

void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b){
    
    r = (r >> 3) << 3;
    g = (g >> 2) << 2;
    b = (b >> 3) << 3;

    setPixel(xx, yy, r, g, b);
}

void setRGB565(int xx, int yy, Uint16 rgb565){
    Uint8 r, g, b;

    r = (((rgb565 >> 11) << 3));
    g = ((((rgb565 << 5) >> 10) << 2));
    b = ((((rgb565 << 11) >> 11) << 3));

    setPixel(xx, yy, r, g, b);
    return;
}

SDL_Color getRGB555(int xx, int yy){
    SDL_Color color = getPixel(xx, yy);
    
    color.r = (color.r >> 3) << 3;
    color.g = (color.g >> 3) << 3;
    color.b = (color.b >> 3) << 3;

    return color;
}

Uint16 getRGB555_(int xx, int yy){

    SDL_Color color = getPixel(xx, yy);
    Uint16 retColor, r, g, b;

    r = color.r;
    g = color.g;
    b = color.b;

    r = (r >> 3) << 10;
    g = (g >> 3) << 5;
    b = (b >> 3);

    return retColor = r | g | b;
}

SDL_Color getRGB565(int xx, int yy){
    SDL_Color color = getPixel(xx, yy);
    
    color.r = (color.r >> 3) << 3;
    color.g = (color.g >> 2) << 2;
    color.b = (color.b >> 3) << 3;

    return color;
}

Uint16 getRGB565_(int xx, int yy){
    SDL_Color color = getPixel(xx, yy);
    Uint16 retColor, r, g, b;

    r = color.r;
    g = color.g;
    b = color.b;

    r = (r >> 3) << 11;
    g = (g >> 2) << 5;
    b = (b >> 3);

    return retColor = r | g | b;
}