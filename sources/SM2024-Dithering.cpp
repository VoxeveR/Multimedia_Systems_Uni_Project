// funkcje do obs�ugi ditheringu
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Kompresje.h"
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                  BAYER                        ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++


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

            setRGB565(x + szerokosc/2, y, (Uint8)red, (Uint8)green, (Uint8)blue);
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

            setRGB565(x + szerokosc/2, y, nowyKolor.r, nowyKolor.g, nowyKolor.b);
        }
    }
    SDL_UpdateWindowSurface(window);
}
