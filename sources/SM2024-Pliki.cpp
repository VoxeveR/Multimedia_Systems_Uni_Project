// funkcje do operacji na plikach
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Konwersje.h"

using namespace std;
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++            ZAPIS-ODCZYT-PAKOWANIE             ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void zczytajDane(int xStart, int yStart){
    int k = 0;
    for(int y = yStart; y < yStart + wysokosc; y+=8) //wysokoscObrazka zamiast wysokosc wczesniej bylo
    {
        for(int x = xStart; x < xStart + szerokosc/2; x+=8) //szereokoscObrazka
        {
            for(int z = y; z < y+8; z++){
                for(int a = x; a<x+8; a++){
                    dane[k] = getPixel(a,z);
                    k++;
                }
            }
        }
    }
}



void saveRGB888(std::string fileName){
    ofstream output(fileName, ios::binary);
    SDL_Color color;
    dane888 data;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            color = getPixel(x, y);
            data.comp1[k] = color.r;
            data.comp2[k] = color.g;
            data.comp3[k] = color.b;
            k++;
        }
    }
    output.write((char*)&data.comp1, sizeof(data.comp1));
    output.write((char*)&data.comp2, sizeof(data.comp2));
    output.write((char*)&data.comp3, sizeof(data.comp3));

    output.close();
}

void unpack8Colors(Uint8* input, Uint8* output) {
    Uint8 tmp;

    // AAAAABBB
    output[0] = input[0] >> 3;
    output[1] = (input[0] << 5) >> 3;

    // BBCCCCCD
    tmp = input[1] >> 6;
    output[1] |= tmp;

    output[2] = (input[1] << 2) >> 3;

    // DDDDEEEE
    tmp = (input[1] << 7) >> 3;
    output[3] = tmp | (input[2] >> 4);

    // EFFFFFGG
    output[4] = (input[2] << 4) >> 3;
    output[4] |= input[3] >> 7;

    output[5] = (input[3] << 1) >> 3;

    // GGGHHHHH
    tmp = (input[3] << 6) >> 3;
    output[6] = tmp | (input[4] >> 5);

    output[7] = (input[4] << 3) >> 3;
}

void pack8Colors(Uint8* input, Uint8* output) {
    // AAAAABBB
    output[0] = (input[0] << 3) | (input[1] >> 2);

    // BBCCCCCD
    output[1] = (input[1] << 6) | (input[2] << 1) | (input[3] >> 4);

    // DDDDEEEE
    output[2] = (input[3] << 4) | (input[4] >> 1);

    // EFFFFFGG
    output[3] = (input[4] << 7) | (input[5] << 2) | (input[6] >> 3);

    // GGGHHHHH
    output[4] = (input[6] << 5) | input[7];

}

void save(std::string nazwa){
    std::cout << "Zapisujemy plik 'obraz.z23' metoda write()";
    ofstream wyjscie(nazwa, ios::binary);

    szerokoscObrazka = szerokosc/2;
    wysokoscObrazka = wysokosc;

    wyjscie.write( (char*)&identyfikator, sizeof(char)*2 );
    wyjscie.write( (char*)&szerokoscObrazka, sizeof(Uint16) );
    wyjscie.write( (char*)&wysokoscObrazka, sizeof(Uint16) );
    wyjscie.write( (char*)&tryb, sizeof(Uint8) );
    wyjscie.write( (char*)&dithering, sizeof(Uint8) );

    Uint8 imgBuf[8];
    Uint8 outBuf[5];
    dithering = 0;
    if(tryb == 0)
    {
        cout << "PROSZ� WYBRA� OBRAZ DO ZAPISU" << endl;
        return;
    }
    else if(tryb == 1 || tryb == 2){
        for(int i = 0; i < ((szerokosc/2)*wysokosc); i+=8)
        {
            for(int j = 0; j < 8; j++) {
                imgBuf[j] = z24RGBna5RGB(dane[i+j]);
            }
            pack8Colors(imgBuf, outBuf);
            wyjscie.write((char*)&outBuf, sizeof(outBuf));
        }

    }else if(tryb == 3 || tryb == 4 || tryb == 5){

            for(Uint8 i = 0; i < 32; i++)
            {
            wyjscie.write( (char*)&paleta5[i].r, sizeof(Uint8));
            wyjscie.write( (char*)&paleta5[i].g, sizeof(Uint8));
            wyjscie.write( (char*)&paleta5[i].b, sizeof(Uint8));
            }
            Uint8 index;

            for(int i = 0; i < ((szerokosc/2)*wysokosc); i+=8)
            {
                for(int j = 0; j < 8; j++) {
                    imgBuf[j] = (Uint8)znajdzSasiada(dane[i+j]);
                    wyjscie.write((char*)&imgBuf[j], sizeof(imgBuf[j]));
                }
                //pack8Colors(imgBuf, outBuf);
                //wyjscie.write((char*)&outBuf, sizeof(outBuf));
            }
        }
    }

void narysujDane(int xStart, int yStart){
    int k = 0;
    for(int y = yStart; y < yStart + wysokosc; y+=8)
    {
        for(int x = xStart; x < xStart + szerokosc/2; x+=8)
        {
            for(int z = y; z < y+8; z++){
                for(int a = x; a<x+8; a++){
                    setPixel(a,z,dane[k].r,dane[k].g,dane[k].b);
                    k++;
                }
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

bool read(string nazwa){
    ifstream wejscie(nazwa, ios::binary);

    if(wejscie){
        wejscie.read( (char*)&identyfikator, sizeof(char)*2 );
        wejscie.read( (char*)&szerokoscObrazka, sizeof(Uint16) );
        wejscie.read( (char*)&wysokoscObrazka, sizeof(Uint16) );
        wejscie.read( (char*)&tryb, sizeof(Uint8) );
        wejscie.read( (char*)&dithering, sizeof(Uint8) );

        if(identyfikator[0] == 'P' && identyfikator[1] == 'J'){
            wysokosc = wysokoscObrazka;
            szerokosc = szerokoscObrazka*2;
            dane = new SDL_Color[szerokoscObrazka*wysokoscObrazka];

            window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc*2, wysokosc*2, SDL_WINDOW_SHOWN);

            if (window == NULL){
                printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                return false;
            }

            screen = SDL_GetWindowSurface(window);

            if (screen == NULL) {
                fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
                return false;
            }

            SDL_UpdateWindowSurface(window);

            Uint8 inBuf[5] = {};
            Uint8 outBuf[8];
            if(tryb == 1 || tryb == 2){
                for(int i = 0; i < ((szerokosc/2)*wysokosc); i+=8) {
                    wejscie.read((char*)&inBuf, sizeof(inBuf));
                    unpack8Colors(inBuf, outBuf);

                    for(int j = 0; j < 8; j++) {
                            SDL_Color kolor = z5RGBna24RGB(outBuf[j]);
                            dane[i+j] = kolor;
                    }
                }
                narysujDane(szerokosc/2,0);
                return true;
            }
            else if(tryb == 3 || tryb == 4 || tryb ==5){
                    for(Uint8 i = 0; i < 32; i++){
                        wejscie.read( (char*)&paleta5[i].r, sizeof(Uint8));
                        wejscie.read( (char*)&paleta5[i].g, sizeof(Uint8));
                        wejscie.read( (char*)&paleta5[i].b, sizeof(Uint8));
                    }

                    narysujPalete(0, 0, paleta5);

                    for(int i = 0; i < ((szerokosc/2)*wysokosc); i+=8) {
                        //wejscie.read((char*)&inBuf, sizeof(inBuf));
                        //unpack8Colors(inBuf, outBuf);
                            for(int j = 0; j < 8; j++) {
                                wejscie.read((char*)&outBuf[j], sizeof(outBuf[j]));
                                dane[i+j] = paleta5[outBuf[j]];
                        }
                    }
                    narysujDane(szerokosc/2,0);
                    return true;
                }
            }
        wejscie.close();
        return true;
    }
    else return false;
}

bool odczytajPlik_bmp(string nazwa){
    nazwa = "pictures/" + nazwa;
    SDL_Surface* bmp = SDL_LoadBMP(nazwa.c_str());
    if (!bmp)
    {
        return false;
    }

    szerokosc = bmp->w*2;
    wysokosc = bmp->h;

    dane = new SDL_Color[szerokosc/2*wysokosc];

    int szerokosc2 = bmp->w*4;
    int wysokosc2 = bmp->h*2;

    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc2, wysokosc2, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }

    screen = SDL_GetWindowSurface(window);

    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_UpdateWindowSurface(window);


    SDL_Color kolor;
    for (int yy=0; yy<bmp->h; yy++)
    {
        for (int xx=0; xx<bmp->w; xx++)
        {
            kolor = getPixelSurface(xx, yy, bmp);
            setPixel(xx+szerokosc/2, yy, kolor.r, kolor.g, kolor.b);
        }
    }

    SDL_FreeSurface(bmp);
    SDL_UpdateWindowSurface(window);
    return true;
}

void saveRightHalfBMP(string nazwa){
    int rightHalfWidth = szerokosc;

    SDL_Surface* rightHalfSurface = SDL_CreateRGBSurface(0, rightHalfWidth, wysokosc*2, 32, 0, 0, 0, 0);

    SDL_Rect rightHalfRect = {szerokosc, 0, rightHalfWidth, wysokosc*2};
    SDL_BlitSurface(screen, &rightHalfRect, rightHalfSurface, NULL);

    SDL_SaveBMP(rightHalfSurface, nazwa.c_str());

    SDL_FreeSurface(rightHalfSurface);
}

void ladujBMP(char const* nazwa, int x, int y) {
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy=0; yy<bmp->h; yy++) {
			for (int xx=0; xx<bmp->w; xx++) {
				kolor = getPixelSurface(xx, yy, bmp);
				setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
			}
        }
		SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}

