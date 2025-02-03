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
#include "../headers/SM2024-DCT.h"

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

//paleta narzucona
void Funkcja1() {
    float kolor = 0;
    // for (int y=0; y< wysokosc; y++) {
    //     for (int x=0; x<szerokosc/2; x++) {
    //         setPixel(x, y, kolor, kolor, kolor);
    //         kolor += 256.0 / (szerokosc * wysokosc);
    //     }
    // }
    // for (int y=1; y<wysokosc; y+=2) {
    //     for (int x=1; x<szerokosc/2; x+=2) {
    //         setPixel(x , y , 255, 255, 255);
    //         setPixel(x -1, y - 1 , 128, 128, 128);
    //         setPixel(x , y - 1, 0, 0, 0);
    //     }
    // }
    RightToLeft();
    for(int i = szerokosc/2; i < szerokosc; i++){
        for(int y = 0; y < wysokosc; y++){
            setPixel(i,y,0,0,0);
        }
    }
    SDL_UpdateWindowSurface(window);
}


std::vector<Uint8> zczytajDaneBW(){
    std::vector<Uint8> output;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            SDL_Color color = getPixel(x, y);
            output.push_back(z24RGBna8BW(color));
        }
    }

    return output;
}

//szarosc narzucona
void Funkcja2() {
    macierz blokDCT_Y;
    macierz blokDCT_I;
    macierz blokDCT_Q;

    macierz blokDane_Y;
    macierz blokDane_I;
    macierz blokDane_Q;

    macierz noweDane_Y;
    macierz noweDane_I;
    macierz noweDane_Q;
   
for (int by = 0; by < wysokosc; by += rozmiarBloku) {
    for (int bx = 0; bx < szerokosc/2; bx += rozmiarBloku) {
        // Process the current block
        for (int y = 0; y < rozmiarBloku; y++) {
            for (int x = 0; x < rozmiarBloku; x++) {
                YIQ YIQret = RGBtoYIQ(bx + x, by + y);
                blokDane_Y.dane[x][y] = YIQret.Y;
                blokDane_Y.dct[x][y] = 0;

                blokDane_I.dane[x][y] = YIQret.I;
                blokDane_I.dct[x][y] = 0;

                blokDane_Q.dane[x][y] = YIQret.Q;
                blokDane_Q.dct[x][y] = 0;
            }
        }
        // std::cout<<"BLOK Y" << std::endl;
        // wyswietlDane(blokDane_Y);

        // std::cout<<"BLOK I" << std::endl;
        // wyswietlDane(blokDane_I);

        // std::cout<<"BLOK Q" << std::endl;
        // wyswietlDane(blokDane_Q);
        // std::cout << std::endl;
        blokDCT_Y = dct(blokDane_Y.dane);
        blokDCT_I = dct(blokDane_I.dane);
        blokDCT_Q = dct(blokDane_Q.dane);
        // std::cout<<"DCT Y" <<std::endl;
        // wyswietlDCT(blokDCT_Y);
        // std::cout << std::endl;
        //         std::cout<<"DCT I" <<std::endl;
        // wyswietlDCT(blokDCT_I);
        // std::cout << std::endl;
        //         std::cout<<"DCT Q" <<std::endl;
        // wyswietlDCT(blokDCT_Q);
        // std::cout << std::endl;
        noweDane_Y = idct(blokDCT_Y.dct);
        noweDane_I = idct(blokDCT_I.dct);
        noweDane_Q = idct(blokDCT_Q.dct);
        // std::cout<<"NOWY BLOK Y" << std::endl;
        // wyswietlDane(noweDane_Y);

        // std::cout<<"NOWY BLOK I" << std::endl;
        // wyswietlDane(noweDane_I);

        // std::cout<<"NOWY BLOK Q" << std::endl;
        // wyswietlDane(noweDane_Q);
        // std::cout << std::endl;

        for (int y = 0; y < rozmiarBloku; y++) {
            for (int x = 0; x < rozmiarBloku; x++) {
                setYIQ(bx + x + szerokosc/2, by + y,
                         noweDane_Y.dane[x][y], noweDane_I.dane[x][y], noweDane_Q.dane[x][y]);
            }
        }
           SDL_UpdateWindowSurface(window);
    }
}
    SDL_UpdateWindowSurface(window);

//     macierz blokDCT;
//     macierz blokDane; 
//     macierz noweDane;
   
// for (int by = 0; by < wysokosc; by += rozmiarBloku) {
//     for (int bx = 0; bx < szerokosc/2; bx += rozmiarBloku) {
//         // Process the current block
//         for (int y = 0; y < rozmiarBloku; y++) {
//             for (int x = 0; x < rozmiarBloku; x++) {
//                 blokDane.dane[x][y] = getPixel(bx + x, by + y).r;
//                 blokDane.dct[x][y] = 0;
//             }
//         }

//      //   wyswietlDane(blokDane);
// //std::cout << std::endl;
//         blokDCT = dct(blokDane.dane);
//       //  wyswietlDCT(blokDCT);
//      //   std::cout << std::endl;
//         noweDane = idct(blokDCT.dct);
//        // wyswietlDane(noweDane);
//        // std::cout << std::endl;

//         for (int y = 0; y < rozmiarBloku; y++) {
//             for (int x = 0; x < rozmiarBloku; x++) {
//                 setPixel(bx + x + szerokosc/2, by + y,
//                          noweDane.dane[x][y], noweDane.dane[x][y], noweDane.dane[x][y]);
//             }
//         }
//     }
// }
}

//szarosc dedykowana
void Funkcja3() {

        SDL_UpdateWindowSurface(window);
    // identyfikator[0] = 'P';
    // identyfikator[1] = 'J';
    // tryb = 3;
    // obrazek = new SDL_Color[szerokosc/2*wysokosc];

    // RightToLeft();
    // if(dithering==0) szaryDedykowany();
    // if(dithering==1){
    //     ileKubelkow = 0;
    //     int numer = 0;

    //     SDL_Color kolor;

    //     for(int y = 0; y < wysokosc; y++)
    //         {
    //             for(int x = 0; x < szerokosc/2; x++)
    //             {
    //                 kolor = getPixel(x,y);
    //                 obrazek[numer] = {kolor.r, kolor.g, kolor.b};
    //                 numer++;
    //             }
    //         }
    //     medianCutBW(0, numer-1, 5);
    //     ditheringFloydPaletowyBW();
    // }
    // if(dithering==2){
    //     ileKubelkow = 0;
    //     int numer = 0;

    //     SDL_Color kolor;

    //     for(int y = 0; y < wysokosc; y++)
    //         {
    //             for(int x = 0; x < szerokosc/2; x++)
    //             {
    //                 kolor = getPixel(x,y);
    //                 obrazek[numer] = {kolor.r, kolor.g, kolor.b};
    //                 numer++;
    //             }
    //         }
    //     medianCutBW(0, numer-1, 5);
    //     ditheringBayerPaletowyBW();
    // }
    // SDL_UpdateWindowSurface(window);
    // zczytajDane8x8(szerokosc/2, 0);
}

//paleta wykryta
void Funkcja4() {
    identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 4;

    paletaWykryta();

    SDL_UpdateWindowSurface(window);
    zczytajDane8x8(szerokosc/2, 0);
}

//paleta dedykowana (median cut)
void Funkcja5() { //FIXME: do poprawy 8bit nie dziala
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
    zczytajDane8x8(szerokosc/2, 0);
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

