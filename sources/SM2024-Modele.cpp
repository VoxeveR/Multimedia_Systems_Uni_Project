#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"
#include <algorithm>

SDL_Color YCbCrtoRGB(Uint8 y, Uint8 cb, Uint8 cr){

    SDL_Color RGBret;
    float r, g, b;

    r = y + 1.402 * (cr - 128);
    g = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128);
    b = y + 1.772 * (cb - 128);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

YCbCr RGBtoYCbCr(int xx, int yy){
    YCbCr YCbCrret;

    SDL_Color color = getPixel(xx, yy);

    YCbCrret.Y = 0 + (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    YCbCrret.Cb = 128 - (0.168736 * color.r) - (0.331264 * color.g) + (0.5 * color.b);
    YCbCrret.Cr = 128 + (0.5 * color.r) - (0.418688 * color.g) - (0.081312 * color.b);

    return YCbCrret;
}

SDL_Color YUVtoRGB(Uint8 y, Uint8 u, Uint8 v){

    SDL_Color RGBret;
    float r, g, b;

    r = 1 * y + 0 * (u-128) + 1.401991 * (v - 128);
    g = 1 * y - 0.344135 * (u -128) - 0.714138 * (v - 128);
    b = 1 * y + 1.771999 * (u - 128) + 0 * (v - 128);

    RGBret.r = (r<0 ? 0 : (r>255 ? 255 : r));
    RGBret.g = (g<0 ? 0 : (g>255 ? 255 : g));
    RGBret.b = (b<0 ? 0 : (b>255 ? 255 : b));

    return RGBret;
}

YUV RGBtoYUV(int xx, int yy){

    YUV YUVret;

    SDL_Color color = getPixel(xx, yy);
    float y, u, v;

    y = (0.299 * color.r) + (0.587 * color.g) + (0.114 * color.b);
    u = 128 - (0.168736 * color.r) + (-0.331264 * color.g) + (0.5 * color.b);
    v = 128 + (0.5 * color.r) + (-0.418691 * color.g) + (-0.081309 * color.b);

    YUVret.Y = (y<0 ? 0 : (y>255 ? 255 : y));
    YUVret.U = (u<0 ? 0 : (u>255 ? 255 : u));
    YUVret.V = (v<0 ? 0 : (v>255 ? 255 : v));

    return YUVret;
}

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

SDL_Color HSLtoRGB(Uint8 h, Uint8 s, Uint8 l){

    SDL_Color RGBret;
    float zmienna1, zmienna2, barwa;
    float zmiennaR, zmiennaG, zmiennaB;

    float H = (h * 360.0) / 255.0;
    float S = s * 1.0 / 255.0;
    float L = l * 1.0 / 255.0;

    if(S == 0){
        RGBret.r = (L) * 255;
        RGBret.g = (L) * 255;
        RGBret.b = (L) * 255;
    } else {
        if (L < 0.5){
            zmienna1 = L * (1.0 + S);
        } else {
            zmienna1 = L + S - (L * S);
        }

        zmienna2 = 2 * L - zmienna1;
        barwa = H / 360;

        if (zmienna1 < 0) zmienna1 += 1;
        if (zmienna1 > 1) zmienna1 -= 1;

        if (zmienna2 < 0) zmienna2 += 1;
        if (zmienna2 > 1) zmienna2 -= 1;

        zmiennaR = barwa + 0.333;
        if (zmiennaR < 0) zmiennaR += 1;
        if (zmiennaR > 1) zmiennaR -= 1;

        zmiennaG = barwa;
        if (zmiennaG < 0) zmiennaG += 1;
        if (zmiennaG > 1) zmiennaG -= 1;

        zmiennaB = barwa - 0.333;
        if (zmiennaB < 0) zmiennaB += 1;
        if (zmiennaB > 1) zmiennaB -= 1;

        float czerwony;
        if((6 * zmiennaR) < 1){
            czerwony = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaR;
        } else if((2 * zmiennaR) < 1 ){
            czerwony = zmienna1;
        } else if((3 * zmiennaR) < 2) {
            czerwony = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaR) * 6;
        } else {
            czerwony = zmienna2;
        }

        float zielony;
        if((6 * zmiennaG) < 1){
            zielony = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaG;
        } else if((2 * zmiennaG) < 1 ){
            zielony = zmienna1;
        } else if((3 * zmiennaG) < 2) {
            zielony = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaG) * 6;
        } else {
            zielony = zmienna2;
        }
    
        float niebieski;
        if((6 * zmiennaB) < 1){
            niebieski = zmienna2 + (zmienna1 - zmienna2) * 6 * zmiennaB;
        } else if((2 * zmiennaB) < 1 ){
            niebieski = zmienna1;
        } else if((3 * zmiennaB) < 2) {
            niebieski = zmienna2 + (zmienna1 - zmienna2) * (0.666 - zmiennaB) * 6;
        } else {
            niebieski = zmienna2;
        }

        RGBret.r = czerwony * 255;
        RGBret.g = zielony * 255;
        RGBret.b = niebieski * 255;
    }

    return RGBret;
}

HSL RGBtoHSL(int xx, int yy){

    SDL_Color colorRGB = getPixel(xx, yy);
    float r, g, b, h, s, l, mmin, mmax;
    HSL HSLret;

    r = (float)colorRGB.r*1.0/255.0;
    g = (float)colorRGB.g*1.0/255.0;
    b = (float)colorRGB.b*1.0/255.0;

    mmin = std::min(r,(std::min(g,b)));
    mmax = std::max(r,(std::max(g,b)));

    l = (mmin+mmax)/2.0;

    if(mmin == mmax){
        s = 0;
    } else if(l <= 0.5){
        s = (mmax-mmin) / (mmax+mmin);
    } else{
        s = (mmax-mmin) / (2.0 - mmax - mmin);
    }

    if(r == mmax){
        h = (g - b) / (mmax-mmin);
    } else if(g == mmax){
        h = 2.0 + (b - r) / (mmax-mmin);
    } else{
        h = 4.0 + (r - g) / (mmax-mmin); // by³o r - b
    }

    h *= 60;
    if(h< 0) h += 360;

    h /= 360;
    h *= 255;
    s *= 255;
    l *= 255;

    HSLret.H = (int)h;
    HSLret.S = (int)s;
    HSLret.L = (int)l;

    return HSLret;
}
