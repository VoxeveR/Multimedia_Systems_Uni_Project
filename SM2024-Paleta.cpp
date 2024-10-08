// funkcje do redukcji kolorów i tworzenia palet
#include "SM2024-Paleta.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Funkcje.h"
#include "SM2024-Dithering.h"

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
