//modele barwne
#ifndef SM2024_MODELE_H_INCLUDED
#define SM2024_MODELE_H_INCLUDED

struct YIQ{
    Uint8 Y;
    Uint8 I;
    Uint8 Q;
};

SDL_Color YIQtoRGB(Uint8 y, Uint8 i, Uint8 q);
YIQ RGBtoYIQ(int xx, int yy);
void setYIQ(int xx, int yy, Uint8 y, Uint8 i, Uint8 q);

#endif // SM2024_MODELE_H_INCLUDED


