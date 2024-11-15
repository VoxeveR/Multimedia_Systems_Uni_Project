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

enum FilterType {
    NO_FILTER,
    DIFFERENTIAL,
    LINE_DIFFERENCE,
    AVERAGING,
    PAETH
};

void zczytajDane(dane888* dataArr){
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            SDL_Color color = getPixel(x, y);
            dataArr->comp1[k] = color.r;
            dataArr->comp2[k] = color.g;
            dataArr->comp3[k] = color.b;
            k++;
        }
    }
}


dane888 filterData(FilterType type){
    dane888 data;
    zczytajDane(&data);

    switch(type){
        case NO_FILTER: //FIXME: NO_MAKEUP
            std::cout<<"to po co tego uzywasz?" << std::endl;
        break;
        case DIFFERENTIAL:
            for(int i = 0; i < ((szerokosc / 2) * wysokosc) - 1; i++){
                Uint8 tmp1 = data.comp1[i+1];
                Uint8 tmp2 = data.comp2[i+1];
                Uint8 tmp3 = data.comp3[i+1];

                data.comp1[i+1] = tmp1 - data.comp1[i];
                data.comp2[i+1] = tmp2 - data.comp2[i];
                data.comp3[i+1] = tmp3 - data.comp3[i];
            }
        break;
        case LINE_DIFFERENCE:


            for(int i = 0; i < ((szerokosc / 2) * wysokosc); i++){
                Uint8 tmp1 = data.comp1[i+szerokosc/2];
                Uint8 tmp2 = data.comp2[i+szerokosc/2];
                Uint8 tmp3 = data.comp3[i+szerokosc/2];

                data.comp1[i+szerokosc/2] = tmp1 - data.comp1[i];
                data.comp2[i+szerokosc/2] = tmp2 - data.comp2[i];
                data.comp3[i+szerokosc/2] = tmp3 - data.comp3[i];
            }
            /*for(int i = 0; i < ((szerokosc / 2) * wysokosc); i++){
                data.comp1[i+(szerokosc/2)] = data.comp1[i+szerokosc/2] - data.comp1[i];
                data.comp2[i+(szerokosc/2)] = data.comp2[i+szerokosc/2] - data.comp2[i];
                data.comp3[i+(szerokosc/2)] = data.comp3[i+szerokosc/2] - data.comp3[i];
            }*/
        break;
        case AVERAGING:
        
        break;
        case PAETH:

        break;
        default:
            std::cout<<"WRONG VALUE TRY AGAIN MY DEAR!!!!"<<std::endl;
        break;
    }
    return data;
}

void unFilterData(dane888* data, FilterType type){

 switch(type){
        case NO_FILTER: //FIXME: NO_MAKEUP
            std::cout<<"to po co tego uzywasz?" << std::endl;
        break;
        case DIFFERENTIAL:
            for(int i = ((szerokosc / 2) * wysokosc) - 1; i > 0; i--){
                data->comp1[i] = data->comp1[i] + data->comp1[i - 1];
                data->comp2[i] = data->comp2[i] + data->comp2[i - 1];
                data->comp3[i] = data->comp3[i] + data->comp3[i - 1];
            }
        break;
        case LINE_DIFFERENCE: 
        for(int i = ((szerokosc / 2) * wysokosc); i > 0; i--){
                data->comp1[i] = data->comp1[i] + data->comp1[i - (szerokosc / 2)];
                data->comp2[i] = data->comp2[i] + data->comp2[i - (szerokosc / 2)];
                data->comp3[i] = data->comp3[i] + data->comp3[i - (szerokosc / 2)];
                //std::cout << "i: " << i << ", i - (szerkosc/2) " << i - (szerokosc / 2) << ", szerkosc/2 " << szerokosc/2 << std::endl;
            }
            //std::cout << "debug" << std::endl;
        break;
        case AVERAGING:
        
        break;
        case PAETH:

        break;
        default:
            std::cout<<"WRONG VALUE TRY AGAIN MY DEAR!!!!"<<std::endl;
        break;
    }

}
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++        Funkcje zmapowane do klawiszy          ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

//paleta narzucona
void Funkcja1() {
    RightToLeft();

    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc / 2; x++){
            setPixel(x + szerokosc/2, y , 0, 0, 0);
        }
    }

    dane888 dataCopy;
    zczytajDane(&dataCopy);
    dane888 data = filterData(FilterType::LINE_DIFFERENCE);
    
    unFilterData(&data, FilterType::LINE_DIFFERENCE);
    int k = 0;
    std::cout << "Finished filter" << std::endl;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc / 2; x++){
            if(dataCopy.comp1[k] != data.comp1[k] or dataCopy.comp2[k] != data.comp2[k] or dataCopy.comp3[k] != data.comp3[k]){
                std::cout << "y: " << y << ", x: " << x << std::endl;
                std::cout << "org1: " << (int)dataCopy.comp1[k] << ", org2: " << (int)dataCopy.comp2[k] << ", org3: " << (int)dataCopy.comp3[k] << std::endl;    
                std::cout << "comp1: " << (int)data.comp1[k] << ", comp2: " << (int)data.comp2[k] << ", comp3: " << (int)data.comp3[k] << std::endl;
            }
            setPixel(x + szerokosc / 2, y, data.comp1[k], data.comp2[k], data.comp3[k]);
            k++;
        }
    }
    std::cout << "Finished painting" << std::endl;
    
    //initial project
    //RightToLeft();
    //HSLSampling();
    //saveRGB888("test.jawa");
    // identyfikator[0] = 'P';
    // identyfikator[1] = 'J';
    // tryb = 1;

    // RightToLeft();

    // if(dithering == 0) paletaNarzucona();
    // if(dithering == 1) ditheringFloyd();
    // if(dithering == 2) ditheringBayer();

    SDL_UpdateWindowSurface(window);
    // zczytajDane8x8(szerokosc/2, 0);
}

//szarosc narzucona
void Funkcja2() {

    RightToLeft();
   /* identyfikator[0] = 'P';
    identyfikator[1] = 'J';
    tryb = 2;

    RightToLeft();

    if(dithering == 0) szaryNarzucony();
    if(dithering == 1) ditheringFloydBW();
    if(dithering == 2) ditheringBayerBW();*/

    SDL_UpdateWindowSurface(window);
   // zczytajDane8x8(szerokosc/2, 0);
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
    zczytajDane8x8(szerokosc/2, 0);
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

