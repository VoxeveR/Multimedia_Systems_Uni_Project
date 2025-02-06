#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"
#include <algorithm>

SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q){

    SDL_Color RGBret;
    float r, g, b;

    r = y + ((i-128) * 1.1395) + ((q-128) * 0.647);
    g = y + ((i-128) * (-0.3242)) + ((q-128) * (-0.6762));
    b = y + ((i-128) * (-1.3182)) + ((q-128) * 1.78);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

void setYIQ(int xx, int yy, Uint8 y, Uint8 i, Uint8 q){
    SDL_Color color = YIQtoRGB(y, i, q);

    setPixel(xx, yy, color.r, color.g, color.b);
}

YIQ RGBtoYIQ(int xx, int yy){

    YIQ YIQret;

    SDL_Color color = getPixel(xx, yy);
    float y, i, q;

    y = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    i = 128 + (0.5 * color.r) + (-0.23032 * color.g) + (-0.2696 * color.b);
    q = 128 + (0.2023 * color.r) + (-0.5 * color.g) + (0.2977 * color.b);

    YIQret.Y = (y<0 ? 0 : (y>255 ? 255 : y));
    YIQret.I = (i<0 ? 0 : (i>255 ? 255 : i));
    YIQret.Q = (q<0 ? 0 : (q>255 ? 255 : q));

    return YIQret;
}