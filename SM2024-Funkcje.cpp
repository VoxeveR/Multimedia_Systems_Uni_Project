// podstawowe funkcje
#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Paleta.h"
#include "SM2024-MedianCut.h"
#include "SM2024-Pliki.h"
#include "SM2024-Dithering.h"


// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                Funkcje napisane               ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

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

void RightToLeft(){
    SDL_Color kolor;
     for(int x = 0; x < szerokosc/2; x++)
    {
        for(int y = 0; y < wysokosc; y++)
        {
            kolor = getPixel(x+szerokosc/2,y);
            setPixel(x, y, kolor.r, kolor.g, kolor.b);
        }
    }
    SDL_UpdateWindowSurface(window);
}

int znajdzSasiada(SDL_Color kolor){
    int minimum = 999;
    int indexMinimum = 0;
    int odleglosc = 0;
    SDL_Color kolorPaleta;
    for (int i= 0; i < COLORS_N; i++){
        kolorPaleta = paleta5[i];
        odleglosc = abs(kolor.r - kolorPaleta.r) + abs(kolor.g - kolorPaleta.g) + abs(kolor.b - kolorPaleta.b);
        if(odleglosc < minimum) {
            indexMinimum = i;
            minimum = odleglosc;
        }
    }
    return indexMinimum;
}

int znajdzSasiadaBW(Uint8 wartosc){
    int minimum = 999;
    int indexMinimum = 0;

    int odleglosc = 0;

    for (int i= 0; i < COLORS_N; i++){
        odleglosc = abs(wartosc - paleta5[i].r);
        if(odleglosc < minimum){
            minimum = odleglosc;
            indexMinimum = i;
        }
    }
    return indexMinimum;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++        Funkcje zmapowane do klawiszy          ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

//paleta narzucona
void Funkcja1() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 1;

    RightToLeft();

    if(dithering == 0) paletaNarzucona();
    if(dithering == 1) ditheringFloyd();
    if(dithering == 2) ditheringBayer();

    SDL_UpdateWindowSurface(window);
    zczytajDane(szerokosc/2, 0);
}

//szarosc narzucona
void Funkcja2() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 2;

    RightToLeft();

    if(dithering == 0) szaryNarzucony();
    if(dithering == 1) ditheringFloydBW();
    if(dithering == 2) ditheringBayerBW();

    SDL_UpdateWindowSurface(window);
    zczytajDane(szerokosc/2, 0);
}

//szarosc dedykowana
void Funkcja3() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 3;
    obrazek = new SDL_Color[szerokosc/2*wysokosc];

    RightToLeft();
    if(dithering==0) szaryDedykowany();
    if(dithering==1){
        ileKubelkow = 0;
        int numer = 0;

        SDL_Color kolor;

        for(int y = 0; y < wysokosc; y++)
            {
                for(int x = 0; x < szerokosc/2; x++)
                {
                    kolor = getPixel(x,y);
                    obrazek[numer] = {kolor.r, kolor.g, kolor.b};
                    numer++;
                }
            }
        medianCutBW(0, numer-1, 5);
        ditheringFloydPaletowyBW();
    }
    if(dithering==2){
        ileKubelkow = 0;
        int numer = 0;

        SDL_Color kolor;

        for(int y = 0; y < wysokosc; y++)
            {
                for(int x = 0; x < szerokosc/2; x++)
                {
                    kolor = getPixel(x,y);
                    obrazek[numer] = {kolor.r, kolor.g, kolor.b};
                    numer++;
                }
            }
        medianCutBW(0, numer-1, 5);
        ditheringBayerPaletowyBW();
    }
    SDL_UpdateWindowSurface(window);
    zczytajDane(szerokosc/2, 0);
}

//paleta wykryta
void Funkcja4() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 4;

    paletaWykryta();

    SDL_UpdateWindowSurface(window);
    zczytajDane(szerokosc/2, 0);
}

//paleta dedykowana (median cut)
void Funkcja5() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 5;

    obrazek = new SDL_Color[(szerokosc/2) * wysokosc];

    RightToLeft();
    if(dithering==0) medianCutRun();
    if(dithering==1){
        ileKubelkow = 0;
        int numer = 0;

        SDL_Color kolor;

        for(int y = 0; y < wysokosc; y++)
        {
            for(int x = 0; x < szerokosc/2; x++)
            {
                kolor = getPixel(x,y);
                obrazek[numer] = {kolor.r, kolor.g, kolor.b};
                numer++;
            }
        }
        medianCut(0, numer-1, 5);
        ditheringFloydPaletowy();
    }
    if(dithering==2){
        ileKubelkow = 0;
        int numer = 0;

        SDL_Color kolor;

        for(int y = 0; y < wysokosc; y++)
        {
            for(int x = 0; x < szerokosc/2; x++)
            {
                kolor = getPixel(x,y);
                obrazek[numer] = {kolor.r, kolor.g, kolor.b};
                numer++;
            }
        }
        medianCut(0, numer-1, 5);
        ditheringBayerPaletowy();
    }
    SDL_UpdateWindowSurface(window);
    zczytajDane(szerokosc/2, 0);
}

//przelaczanie ditheringu
void Funkcja6() {
    dithering = ++dithering % 3;
    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {

    int index = 0;

    for (int r = 0; r < 4; r++) {
        for (int g = 0; g < 4; g++) {
            for (int b = 0; b < 2; b++) {
                paletaCala[index].r = r * 80;
                paletaCala[index].g = g * 80;
                paletaCala[index].b = b * 255;
                index++;
                if (index >= 32) {
                    break;
                }
            }
            if (index >= 32) {
                break;
            }
        }
        if (index >= 32) {
            break;
        }
    }

    narysujPalete(0,0,paletaCala);
    SDL_UpdateWindowSurface(window);
}

void Funkcja8() {

    for (int i = 0; i < 32; i++) {
        paletaCala[i].r = i*8;
        paletaCala[i].g = i*8;
        paletaCala[i].b = i*8;
    }

    narysujPalete(0,0,paletaCala);
    SDL_UpdateWindowSurface(window);
}

void Funkcja9() {

    //...

    SDL_UpdateWindowSurface(window);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++             Funkcje Wbudowane                 ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B){
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B){
  if ((x>=0) && (x<szerokosc*2) && (y>=0) && (y<wysokosc*2))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;

        //przesunięcie względem y
        pPosition+=(screen->pitch*y*2) ;

        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x*2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        //określamy pozycję
        char* pPosition=(char*)surface->pixels ;

        //przesunięcie względem y
        pPosition+=(surface->pitch*y) ;

        //przesunięcie względem x
        pPosition+=(surface->format->BytesPerPixel*x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

void czyscEkran(Uint8 R, Uint8 G, Uint8 B){
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}

void color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y){
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

