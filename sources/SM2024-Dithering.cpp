// funkcje do obs�ugi ditheringu
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Paleta.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                  BAYER                        ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

float sredniaKrokR(){
    float srednia = 0;
    int n = 0;
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < i; j++)
        {
            srednia += abs(paleta5[i].r - paleta5[j].r);
            n++;
        }
    }
    return srednia / n;
}

float sredniaKrokG(){
    float srednia = 0;
    int n = 0;
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < i; j++)
        {
            srednia += abs((int)paleta5[i].g - (int)paleta5[j].g);
            n++;
        }
    }
    return srednia / n;
}

float sredniaKrokB(){
    float srednia = 0;
    int n = 0;
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < i; j++)
        {
            srednia += abs(paleta5[i].b -paleta5[j].b);
            n++;
        }
    }
    return srednia / n;
}

void ditheringBayer(){
    SDL_Color kolor;
    int red, green, blue;
    
    float m;
    float krok = 255/31;

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2; x++)
        {
            kolor = getPixel(x,y);

            m = ((dither[x % 8][y % 8] / 64) - 0.5);

            red = ( kolor.r + krok * m);
            green = ( kolor.g + krok * m);
            blue = ( kolor.b + krok * m);

            if(red > 255) red = 255;
            if(red < 0) red = 0;

            if(green > 255) green = 255;
            if(green < 0) green = 0;

            if(blue > 255) blue = 255;
            if(blue < 0) blue = 0;

            setRGB555(x + szerokosc/2, y, (Uint8)red, (Uint8)green, (Uint8)blue);
        }
    }
    SDL_UpdateWindowSurface(window);
}



void ditheringBayerPaletowy(){
    SDL_Color kolor;
    int red, green, blue;
    float m;
    float krokR = sredniaKrokR();
    float krokG = sredniaKrokG();
    float krokB = sredniaKrokB();

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2 ; x++)
        {
            kolor = getPixel(x,y);

            m = ((dither[x % 8][y % 8] / 64) - 0.5);

            red = ( kolor.r + krokR * m);
            green = ( kolor.g + krokG * m);
            blue = ( kolor.b + krokB * m);

            if(red > 255) red = 255;
            if(red < 0) red = 0;

            if(green > 255) green = 255;
            if(green < 0) green = 0;

            if(blue > 255) blue = 255;
            if(blue < 0) blue = 0;

            SDL_Color nowyKolor = {(Uint8)red, (Uint8)green, (Uint8)blue};

            Uint8 index = znajdzSasiada(nowyKolor);
            setPixel(x + szerokosc/2,y, paleta5[index].r, paleta5[index].g, paleta5[index].b);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void ditheringBayerBW(){
    SDL_Color kolor;
    float m;

    float krok = 255/32.0;

    Uint8 szary;
    int nowySzary;

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2; x++)
        {
            kolor = getPixel(x,y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;

            m = ((dither[x % 8][y % 8] / 64) - 0.5);

            nowySzary = ( szary + krok * m);

            if(nowySzary > 255) nowySzary = 255;
            if(nowySzary < 0) nowySzary = 0;

            SDL_Color nowyKolorBW = {(Uint8)nowySzary, (Uint8)nowySzary, (Uint8)nowySzary};

            Uint8 kolor5bit = z24RGBna5BW(nowyKolorBW);
            SDL_Color nowyKolor = z5BWna24RGB(kolor5bit);

            setPixel(x + szerokosc/2, y, nowyKolor.r, nowyKolor.g, nowyKolor.b);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void ditheringBayerPaletowyBW(){
    SDL_Color kolor;
    float m;
    float krok = sredniaKrokR();
    Uint8 szary;
    int nowySzary;

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2; x++)
        {
            kolor = getPixel(x,y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;

            m = ((dither[x % 8][y % 8] / 64) - 0.5);

            nowySzary = ( szary + krok * m);

            if(nowySzary > 255) nowySzary = 255;
            if(nowySzary < 0) nowySzary = 0;

            Uint8 index = znajdzSasiadaBW(nowySzary);
            setPixel(x + szerokosc/2 ,y, paleta5[index].r, paleta5[index].g, paleta5[index].b);
        }
    }
    SDL_UpdateWindowSurface(window);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++              FLOYD-STEINBERG                  ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void ditheringFloyd(){

    Uint8 kolor5b;
    SDL_Color kolor, tymKolor;
    SDL_Color nowyKolor;

    int tymczasowyKolorr, tymczasowyKolorg, tymczasowyKolorb;

    int przesuniecie = 1;
    float** bledyR = new float*[(szerokosc / 2) + 2];
    float** bledyG = new float*[(szerokosc / 2) + 2];
    float** bledyB = new float*[(szerokosc / 2) + 2];
    
    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        bledyR[i] = new float[wysokosc + 2];
        bledyG[i] = new float[wysokosc + 2];
        bledyB[i] = new float[wysokosc + 2];
        // Initialize arrays with zeros
        memset(bledyR[i], 0, (wysokosc + 2) * sizeof(float));
        memset(bledyG[i], 0, (wysokosc + 2) * sizeof(float));
        memset(bledyB[i], 0, (wysokosc + 2) * sizeof(float));
    }

    memset(bledyR, 0, sizeof(bledyR));
    memset(bledyG, 0, sizeof(bledyG));
    memset(bledyB, 0, sizeof(bledyB));

    int bladR = 0, bladG = 0, bladB = 0;

    for(int y=0; y<wysokosc; y++){
        for(int x=0; x<szerokosc/2;x++){
            kolor = getPixel(x,y);


            tymczasowyKolorr = kolor.r + bledyR[x+przesuniecie][y];
            tymczasowyKolorg = kolor.g + bledyG[x+przesuniecie][y];
            tymczasowyKolorb = kolor.b + bledyB[x+przesuniecie][y];

            if(tymczasowyKolorr > 255) tymczasowyKolorr = 255;
            if(tymczasowyKolorr < 0) tymczasowyKolorr = 0;

            if(tymczasowyKolorg > 255) tymczasowyKolorg = 255;
            if(tymczasowyKolorg < 0) tymczasowyKolorg = 0;

            if(tymczasowyKolorb > 255) tymczasowyKolorb = 255;
            if(tymczasowyKolorb < 0) tymczasowyKolorb = 0;

            tymKolor.r = tymczasowyKolorr;
            tymKolor.g = tymczasowyKolorg;
            tymKolor.b = tymczasowyKolorb;

            kolor5b = z24RGBna5RGB(tymKolor);
            nowyKolor = z5RGBna24RGB(kolor5b);

            bladR = tymczasowyKolorr - nowyKolor.r;
            bladG = tymczasowyKolorg - nowyKolor.g;
            bladB = tymczasowyKolorb - nowyKolor.b;

            bledyR[x+1+przesuniecie][y] += (bladR*7.0/16.0);
            bledyR[x-1+przesuniecie][y+1] += (bladR*3.0/16.0);
            bledyR[x+przesuniecie][y+1] += (bladR*5.0/16.0);
            bledyR[x+1+przesuniecie][y+1] += (bladR*1.0/16.0);

            bledyG[x+1+przesuniecie][y] += (bladG*7.0/16.0);
            bledyG[x-1+przesuniecie][y+1] += (bladG*3.0/16.0);
            bledyG[x+przesuniecie][y+1] += (bladG*5.0/16.0);
            bledyG[x+1+przesuniecie][y+1] += (bladG*1.0/16.0);

            bledyB[x+1+przesuniecie][y] += (bladB*7.0/16.0);
            bledyB[x-1+przesuniecie][y+1] += (bladB*3.0/16.0);
            bledyB[x+przesuniecie][y+1] += (bladB*5.0/16.0);
            bledyB[x+1+przesuniecie][y+1] += (bladB*1.0/16.0);

            setPixel(x + szerokosc/2, y, nowyKolor.r, nowyKolor.g, nowyKolor.b);
        }
    }

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        delete[] bledyR[i];
        delete[] bledyG[i];
        delete[] bledyB[i];
    }

    delete[] bledyR;
    delete[] bledyG;
    delete[] bledyB;

    SDL_UpdateWindowSurface(window);
}

void ditheringFloydBW() {
    SDL_Color kolor, kolor_new, tymKolor;

    Uint8 szary, nowySzary;
    int tymSzary;

    int przesuniecie = 1;
    float** bledy = new float*[(szerokosc / 2) + 2];

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        bledy[i] = new float[wysokosc + 2];
        memset(bledy[i], 0, (wysokosc + 2) * sizeof(float));
    }

    int blad = 0;

    for (int y = 0; y < wysokosc; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            kolor = getPixel(x, y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            tymSzary = szary + bledy[x + przesuniecie][y];

            if (tymSzary > 255) tymSzary = 255;
            if (tymSzary < 0) tymSzary = 0;

            tymKolor.r = tymSzary;
            tymKolor.g = tymSzary;
            tymKolor.b = tymSzary;

            Uint8 kolor5bit = z24RGBna5BW(tymKolor);
            kolor_new = z5BWna24RGB(kolor5bit);

            nowySzary = kolor_new.r;
            blad = tymSzary - nowySzary;

            setPixel(x + szerokosc / 2, y, kolor_new.r, kolor_new.g, kolor_new.b);

            bledy[x + 1 + przesuniecie][y] += (blad * 7.0 / 16.0);
            bledy[x - 1 + przesuniecie][y + 1] += (blad * 3.0 / 16.0);
            bledy[x + przesuniecie][y + 1] += (blad * 5.0 / 16.0);
            bledy[x + 1 + przesuniecie][y + 1] += (blad * 1.0 / 16.0);
        }
    }

    SDL_UpdateWindowSurface(window);

    // Free dynamically allocated memory
    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        delete[] bledy[i];
    }

    delete[] bledy;
}

void ditheringFloydPaletowy() {

    SDL_Color kolor, kolor_new, tymKolor;

    int tymR, tymG, tymB;

    Uint8 nowyR, nowyG, nowyB;

    int przesuniecie = 1;

    float** bledyR = new float*[(szerokosc / 2) + 2];
    float** bledyG = new float*[(szerokosc / 2) + 2];
    float** bledyB = new float*[(szerokosc / 2) + 2];

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        bledyR[i] = new float[wysokosc + 2];
        bledyG[i] = new float[wysokosc + 2];
        bledyB[i] = new float[wysokosc + 2];

        memset(bledyR[i], 0, (wysokosc + 2) * sizeof(float));
        memset(bledyG[i], 0, (wysokosc + 2) * sizeof(float));
        memset(bledyB[i], 0, (wysokosc + 2) * sizeof(float));
    }
    int blad = 0;

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2; x++)
        {

            kolor = getPixel(x,y);
            tymR = kolor.r + bledyR[x+przesuniecie][y];
            tymG = kolor.g + bledyG[x+przesuniecie][y];
            tymB = kolor.b + bledyB[x+przesuniecie][y];


            if(tymR > 255) tymR = 255;
            if(tymR < 0) tymR = 0;

            if(tymG > 255) tymG = 255;
            if(tymG < 0) tymG = 0;

            if(tymB > 255) tymB = 255;
            if(tymB < 0) tymB = 0;

            tymKolor.r = tymR;
            tymKolor.g = tymG;
            tymKolor.b = tymB;

            kolor_new = paleta5[znajdzSasiada(tymKolor)];

            nowyR = kolor_new.r;
            blad = tymR - nowyR;

            bledyR[x+1+przesuniecie][y] += (blad*7.0/16.0);
            bledyR[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledyR[x+przesuniecie][y+1] += (blad*5.0/16.0);
            bledyR[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

            nowyG = kolor_new.g;
            blad = tymG - nowyG;

            bledyG[x+1+przesuniecie][y] += (blad*7.0/16.0);
            bledyG[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledyG[x+przesuniecie][y+1] += (blad*5.0/16.0);
            bledyG[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

            nowyB = kolor_new.b;
            blad = tymB - nowyB;

            bledyB[x+1+przesuniecie][y] += (blad*7.0/16.0);
            bledyB[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledyB[x+przesuniecie][y+1] += (blad*5.0/16.0);
            bledyB[x+1+przesuniecie][y+1] += (blad*1.0/16.0);

            setPixel(x + szerokosc/2, y, kolor_new.r, kolor_new.g, kolor_new.b);
        }
    }
    SDL_UpdateWindowSurface(window);

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        delete[] bledyR[i];
        delete[] bledyG[i];
        delete[] bledyB[i];
    }

    delete[] bledyR;
    delete[] bledyG;
    delete[] bledyB;
}

void ditheringFloydPaletowyBW() {

    SDL_Color kolor, kolor_new, tymKolor;

    Uint8 szary, nowySzary;
    int tymSzary;

    int przesuniecie = 1;

    float** bledy = new float*[(szerokosc / 2) + 2];

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
            bledy[i] = new float[wysokosc + 2];
            memset(bledy[i], 0, (wysokosc + 2) * sizeof(float));
        }

    int blad = 0;

    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc/2; x++)
        {

            kolor = getPixel(x,y);
            szary = 0.299 * kolor.r + 0.587 * kolor.g + 0.114 * kolor.b;
            tymSzary = szary + bledy[x+przesuniecie][y];

            if(tymSzary > 255) tymSzary = 255;
            if(tymSzary < 0) tymSzary = 0;

            tymKolor.r = tymSzary;
            tymKolor.g = tymSzary;
            tymKolor.b = tymSzary;

            kolor_new = paleta5[znajdzSasiadaBW(tymSzary)];

            nowySzary = kolor_new.r;
            blad = tymSzary - nowySzary;

            setPixel(x + szerokosc/2, y, kolor_new.r, kolor_new.g, kolor_new.b);

            bledy[x+1+przesuniecie][y] += (blad*7.0/16.0);
            bledy[x-1+przesuniecie][y+1] += (blad*3.0/16.0);
            bledy[x+przesuniecie][y+1] += (blad*5.0/16.0);
            bledy[x+1+przesuniecie][y+1] += (blad*1.0/16.0);
        }
    }

    SDL_UpdateWindowSurface(window);

    for (int i = 0; i < (szerokosc / 2) + 2; ++i) {
        delete[] bledy[i];
    }
    delete[] bledy;
}
