//modele barwne
#ifndef SM2024_MODELE_H_INCLUDED
#define SM2024_MODELE_H_INCLUDED

struct YCbCr{
    Uint8 Y;
    Uint8 Cb;
    Uint8 Cr;
};

struct YUV{
    Uint8 Y;
    Uint8 U;
    Uint8 V;
};

struct YIQ{
    Uint8 Y;
    Uint8 I;
    Uint8 Q;
};

struct HSL{
    Uint8 H;
    Uint8 S;
    Uint8 L;
};

SDL_Color YCbCrtoRGB(Uint8 y, Uint8 cb, Uint8 cr);
YCbCr RGBtoYCbCr(int xx, int yy);

SDL_Color YUVtoRGB(Uint8 y, Uint8 u, Uint8 v);
YUV RGBtoYUV(int xx, int yy);

SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q);
YIQ RGBtoYIQ(int xx, int yy);

SDL_Color HSLtoRGB(Uint8 h, Uint8 s, Uint8 l);
HSL RGBtoHSL(int xx, int yy);

#endif // SM2024_MODELE_H_INCLUDED


