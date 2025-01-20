// podstawowe funkcje
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Probkowanie.h"
#include "../headers/SM2024-Kompresje.h"
#include "../headers/SM2024-Filtrowanie.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                Funkcje napisane               ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

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





// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++        Funkcje zmapowane do klawiszy          ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

//Skala szarości
void Funkcja1() {
    RightToLeft();
    clearVector24();
    clearVector8();

    blackandwhite = 1;

    SDL_Color color;
    Uint8 new_color;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            color = getPixel(x, y);
            new_color = z24RGBna8BW(color);
            setPixel(x+szerokosc/2, y, new_color, new_color, new_color);
        }
    }

    SDL_UpdateWindowSurface(window);
    zczytajDaneBW();
}




//16-bit
void Funkcja2() {

    RightToLeft();
    clearVector24();
    clearVector8();

    bit = 16;
    std::cout << "Dithering" << dither << std::endl;
    if(blackandwhite == 1){
        if(dithering == 1) ditheringBayerBW();
        else{
            SDL_Color color;
            for(int y = 0; y < wysokosc; y++){
                for(int x = 0; x < szerokosc/2; x++){
                    color = getPixel(x, y);
                    setRGB565(x+szerokosc/2, y, color.r, color.g, color.b);
                }
            }
            
        }
        
    }else{
        if(dithering == 1) ditheringBayer();
        else{
            SDL_Color color;
            for(int y = 0; y < wysokosc; y++){
                for(int x = 0; x < szerokosc/2; x++){
                    color = getPixel(x, y);
                    setRGB565(x+szerokosc/2, y, color.r, color.g, color.b);
                }
            }

        }

    }

    zczytajDane16();

    SDL_UpdateWindowSurface(window);
}

//YIQ
void Funkcja3(){
    RightToLeft();
    clearVector24();
    clearVector8();

    yiqstatus = 1;

    YIQ color;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            color = RGBtoYIQ(x, y);
            setYIQ(x + szerokosc/2, y, color.Y, color.I, color.Q);
        }
    }

    SDL_UpdateWindowSurface(window);
    zczytajDane();
}

//YIQ Sampling
void Funkcja4() {
    RightToLeft();
    clearVector24();
    clearVector8();

    yiqstatus = 2;

    YIQSampling();

    SDL_UpdateWindowSurface(window);
    zczytajDane();
}

//filtrowanie danych
void Funkcja5() {
    prediction = 1;


    SDL_UpdateWindowSurface(window);
}

// Kompresja bezstratna LZ77
void Funkcja6() {
    compression = 1;

    SDL_UpdateWindowSurface(window);
}

// Transformata dct
void Funkcja7() {
    compression = 2;
    
    SDL_UpdateWindowSurface(window);
}

//Wybór ditheringu
void Funkcja8() {
    dithering = ++dithering % 2;
    SDL_UpdateWindowSurface(window);
}

void Funkcja9() {

    // int nieskompresowane[] = {0, 0, 0, 1, 1, 1, 1, 2, 0, 0, 3, 1, 3,
    // 2, 2, 0, 0, 0, 3, 3, 3, 3, 1, 2, 1, 2, 3, 1, 2, 0, 0, 1, 1, 1, 3, 3};
    // int dlugosc = 36;
    // std::cout << "wejscie:" << std::endl;
    // for(int c = 0; c < dlugosc; c++){
    //     std::cout << (int)nieskompresowane[c] << ", ";
    // }
    // std::cout << std::endl;
    // LZWKompresja(nieskompresowane, dlugosc);
    // std::cout << std::endl;

    narysujDane24(szerokosc/2,0);

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

