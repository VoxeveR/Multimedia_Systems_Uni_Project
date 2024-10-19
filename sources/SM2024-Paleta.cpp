// funkcje do redukcji kolor�w i tworzenia palet
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-MedianCut.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                   PALETA                      ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void czyscPalete(){
    ileKolorow = 0;
    for(int k=0;k<32;k++){
        paleta5[k] = (SDL_Color){0,0,0};
    }
}

int dodajKolor(SDL_Color kolor){
    int aktualnyKolor = ileKolorow;
        if(ileKolorow<32){
        paleta5[aktualnyKolor] = kolor;
        std::cout<<aktualnyKolor<<": ["<<(int)kolor.r<<","<<(int)kolor.g<<","<<(int)kolor.b<<"]"<<std::endl;
        }
        ileKolorow++;
        return aktualnyKolor;
}

bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2){
    if(kolor1.r != kolor2.r)
        return false;
    if(kolor1.g != kolor2.g)
        return false;
    if(kolor1.b != kolor2.b)
        return false;
    return true;
}

int sprawdzKolor(SDL_Color kolor){
    if(ileKolorow>0){
        for(int k=0; k<ileKolorow;k++){
            if(porownajKolory(kolor,paleta5[k]))
                return k;
        }
    }
    return dodajKolor(kolor);
}

void narysujPalete(int px, int py, SDL_Color paleta5[]){
    int x, y;
    for(int k=0; k<32; k++) {
        y = k / 8;
        x = k - y * 8;
        for (int xx=0; xx < 40; xx++) {
            for(int yy=0; yy < 50; yy++) {
                setPixel(x * 40 + xx + px, y * 50 + yy + py, paleta5[k].r, paleta5[k].g, paleta5[k].b);
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void paletaWykryta(){
        czyscPalete();

        SDL_Color kolor;
        for(int y=0; y<wysokosc; y++){
            for(int x=szerokosc/2; x<szerokosc; x++){
                kolor = getPixel(x,y);
                if(sprawdzKolor(kolor) > 32) break;
            }
        }
        std::cout<<"Ile kolorow: "<<ileKolorow<<std::endl;
        if(ileKolorow <=32){
            std::cout<<"Paleta spelnia ograniczenia 5-bit / piksel"<<std::endl;
            narysujPalete(0, 0, paleta5);
        }
        else{
            color(12);
            std::cout<<"Paleta przekracza ograniczenia 5-bit / piksel"<<std::endl;
            std::cout <<"Wpisz dowolny znak, aby przejsc dalej "<<std::endl;
            color(15);
        }
}

void paletaNarzucona(){
    //dane 5 bit
    Uint8 kolor5b;
    SDL_Color kolor;
    for(int y=0; y<wysokosc; y++){
        for(int x=0; x<szerokosc/2;x++){
            kolor = getPixel(x,y);
            kolor5b = z24RGBna5RGB(kolor);
            kolor = z5RGBna24RGB(kolor5b);
            setPixel(x + szerokosc/2, y, kolor.r, kolor.g, kolor.b);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void szaryNarzucony(){

    Uint8 kolor5b;
    SDL_Color kolor, tymczasowyKolor, nowyKolor;

    Uint8 szary, nowySzary;
    int tymczasowySzary;

    int przesuniecie = 1;
    float bledy[(szerokosc/2)+2][(wysokosc)+2];
    memset(bledy, 0, sizeof(bledy));
    int blad = 0;

    for(int y=0; y<wysokosc; y++){
        for(int x=0; x<szerokosc/2;x++){
            kolor = getPixel(x,y);

            szary = 0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b;
            tymczasowySzary = szary + bledy[x+przesuniecie][y];

            if(tymczasowySzary > 255) tymczasowySzary = 255;
            if(tymczasowySzary < 0) tymczasowySzary = 0;

            tymczasowyKolor.r = tymczasowySzary;
            tymczasowyKolor.g = tymczasowySzary;
            tymczasowyKolor.b = tymczasowySzary;

            kolor5b = z24RGBna5BW(tymczasowyKolor);
            nowyKolor = z5BWna24RGB(kolor5b);

            nowySzary = nowyKolor.r;

            blad = tymczasowySzary - nowySzary;

            setPixel(x + szerokosc/2, y, nowyKolor.r, nowyKolor.g, nowyKolor.b);

            bledy[x+1+przesuniecie][y] += (blad*7.0/16.0);
            bledy[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledy[x+przesuniecie][y+1] += (blad*5.0/16.0);
            bledy[x+1+przesuniecie][y+1] += (blad*1.0/16.0);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void szaryDedykowany(){
    Uint8 szary;
    SDL_Color kolor;
    int index, numer = 0;
    ileKubelkow = 0;

    for(int y=0; y<wysokosc; y++){
        for(int x=0; x<szerokosc/2;x++){
            kolor = getPixel(x,y);
            szary = z24RGBna8BW(kolor);
            obrazek[numer] = {szary, szary, szary};
            numer++;
        }
    }

    medianCutBW(0, numer-1, 5);

    for(int y=0; y<wysokosc; y++){
        for(int x=0; x<szerokosc/2;x++){
            kolor = getPixel(x,y);
            szary = z24RGBna8BW(kolor);
            index = znajdzSasiadaBW(szary);
            setPixel(x + szerokosc/2, y, paleta5[index].r, paleta5[index].g, paleta5[index].b);
        }
    }

    SDL_UpdateWindowSurface(window);
}
