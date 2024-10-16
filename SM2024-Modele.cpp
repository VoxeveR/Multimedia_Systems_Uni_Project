#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Paleta.h"
#include "SM2024-MedianCut.h"
#include "SM2024-Pliki.h"
#include "SM2024-Dithering.h"
#include "SM2024-Modele.h"

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

SDL_Color YIQtoRGB(float y, float i, float q){

    SDL_Color RGBret;
    float r, g, b;

    r = y + (i * 0.956) + (q * 0.619);
    g = y + (i * (-0.272)) + (q * (-0.647));
    b = y + (i * (-1.106)) + (q * 1.703);

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
    i = (0.5959 * color.r) + (-0.2746 * color.g) + (-0.3213 * color.b);
    q = (0.2115 * color.r) + (-0.5227 * color.g) + (0.3112 * color.b);

    YIQret.Y = (y<0 ? 0 : (y>255 ? 255 : y));
    YIQret.I = (i<0 ? 0 : (i>255 ? 255 : i));
    YIQret.Q = (q<0 ? 0 : (q>255 ? 255 : q));

    return YIQret;
}


