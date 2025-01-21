// funkcje do operacji na plikach
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Kompresje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Modele.h"
#include "../headers/SM2024-Filtrowanie.h"

using namespace std;
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++            ZAPIS-ODCZYT-PAKOWANIE             ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

Uint16 data555[320*200];

void zczytajDane(){
    for(int y = 0; y < wysokosc; y++){
        for(int x = szerokosc/2; x < szerokosc; x++){
            if(yiqstatus == 0){
                SDL_Color color = getPixel(x, y);
                dane24.comp1.push_back(color.r);
                dane24.comp2.push_back(color.g);
                dane24.comp3.push_back(color.b);
            }else{
                YIQ color = RGBtoYIQ(x, y);
                dane24.comp1.push_back(color.Y);
                dane24.comp2.push_back(color.I);
                dane24.comp3.push_back(color.Q);
            }
        }
    }
}

void zczytajDaneBW(){
    for(int y = 0; y < wysokosc; y++){
        for(int x = szerokosc/2; x < szerokosc; x++){
            SDL_Color color = getPixel(x, y);
            dane8.comp.push_back(color.r);
        }
    }
}

void zczytajDane16(){
    for(int y = 0; y < wysokosc; y++){
        for(int x = szerokosc/2; x < szerokosc; x++){
            Uint16 color = getRGB565_(x, y);
            dane16.comp.push_back(color);
        }
    }
}

void zczytajDane8x8(int xStart, int yStart){
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

void clearVector24(){
    dane24.comp1.clear();
    dane24.comp2.clear();
    dane24.comp3.clear();

}

void clearVector8(){
    dane8.comp.clear();
}

void clearVector16(){
    dane16.comp.clear();
}

void clearArray(){
    //Clear array
    int sizeArray = sizeof(data555)/sizeof(data555[0]);
    fill(data555, data555+sizeArray, 0);
}

void saveBW(std::string fileName, Uint8* dane, int size){
    ofstream output(fileName, ios::binary);
    
    output.write((char*)dane, size);

    output.close();
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


void saveYUV888(std::string fileName){
    ofstream output(fileName, ios::binary);
    dane888 data;
    YUV yuv;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            yuv = RGBtoYUV(x, y);
            data.comp1[k] = yuv.Y;
            data.comp2[k] = yuv.U;
            data.comp3[k] = yuv.V;
            k++;
        }
    }
    output.write((char*)&data.comp1, sizeof(data.comp1));
    output.write((char*)&data.comp2, sizeof(data.comp2));
    output.write((char*)&data.comp3, sizeof(data.comp3));

    output.close();
}

void saveYIQ888(std::string fileName){
    ofstream output(fileName, ios::binary);
    dane888 data;
    YIQ yiq;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            yiq = RGBtoYIQ(x, y);
            data.comp1[k] = yiq.Y;
            data.comp2[k] = yiq.Q;
            data.comp3[k] = yiq.I;
            k++;
        }
    }
    output.write((char*)&data.comp1, sizeof(data.comp1));
    output.write((char*)&data.comp2, sizeof(data.comp2));
    output.write((char*)&data.comp3, sizeof(data.comp3));

    output.close();
}

void saveYCbCr888(std::string fileName){
    ofstream output(fileName, ios::binary);
    dane888 data;
    YCbCr ycbcr;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            ycbcr = RGBtoYCbCr(x, y);
            data.comp1[k] = ycbcr.Y;
            data.comp2[k] = ycbcr.Cr;
            data.comp3[k] = ycbcr.Cb;
            k++;
        }
    }
    output.write((char*)&data.comp1, sizeof(data.comp1));
    output.write((char*)&data.comp2, sizeof(data.comp2));
    output.write((char*)&data.comp3, sizeof(data.comp3));

    output.close();
}

void saveHSL888(std::string fileName){
    ofstream output(fileName, ios::binary);
    dane888 data;
    HSL hsl;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            hsl = RGBtoHSL(x, y);
            data.comp1[k] = hsl.H;
            data.comp2[k] = hsl.S;
            data.comp3[k] = hsl.L;
            k++;
        }
    }
    output.write((char*)&data.comp1, sizeof(data.comp1));
    output.write((char*)&data.comp2, sizeof(data.comp2));
    output.write((char*)&data.comp3, sizeof(data.comp3));

    output.close();
}

void saveRGB555(std::string fileName){
    clearArray();
    ofstream output(fileName, ios::binary);
    SDL_Color color;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            data555[k] = getRGB555_(x, y);
            k++;
        }
    }
    output.write((char*)&data555, sizeof(data555));

    output.close();
}

void saveRGB565(std::string fileName){
    clearArray();
    ofstream output(fileName, ios::binary);
    SDL_Color color;
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            data555[k] = getRGB565_(x, y);
            k++;
        }
    }
    output.write((char*)&data555, sizeof(data555));

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

void save(std::string nazwa) {
    std::ofstream wyjscie(nazwa, std::ios::binary);

    if (!wyjscie.is_open()) {
        std::cerr << "Błąd: nie można otworzyć pliku do zapisu " << nazwa << std::endl;
        return;
    }

    // Nagłówek pliku
    char identyfikator[2] = {'P', 'J'};
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc;

    wyjscie.write(reinterpret_cast<char*>(&identyfikator), sizeof(char) * 2);
    wyjscie.write(reinterpret_cast<char*>(&szerokoscObrazka), sizeof(Uint16));
    wyjscie.write(reinterpret_cast<char*>(&wysokoscObrazka), sizeof(Uint16));
    wyjscie.write(reinterpret_cast<char*>(&dithering), sizeof(int));
    wyjscie.write(reinterpret_cast<char*>(&blackandwhite), sizeof(int));
    wyjscie.write(reinterpret_cast<char*>(&yiqstatus), sizeof(int));
    wyjscie.write(reinterpret_cast<char*>(&bit), sizeof(int));
    wyjscie.write(reinterpret_cast<char*>(&prediction), sizeof(int));
    wyjscie.write(reinterpret_cast<char*>(&compression), sizeof(int));
    
    if(bit == 16){
        if(prediction == 1){
            filterData(FilterType::LINE_DIFFERENCE);
        }

        if(compression == 1){
            //zapis z kompresja LZ77 dla 16bit
            std::vector<token<Uint16>> tokenComp = LZ77Kompresja<Uint16>(dane16.comp, dane16.comp.size());

            int rozmiar = static_cast<int>(tokenComp.size());
            wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));
            for(token<Uint16> t: tokenComp){
                wyjscie.write(reinterpret_cast<char*>(&t), sizeof(token<Uint16>));
            }
        }else if(compression == 2){

        }else{
            //zapis bez kompresja LZ77 dla 16bit
            int rozmiar = static_cast<int>(dane16.comp.size());
            wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));

            for(auto& element: dane16.comp){
                wyjscie.write((char*)&element, sizeof(Uint16));
            }
        }

    }else{
        cout << "Wywalone przed filtracji" << endl;
        if(yiqstatus == 0 and blackandwhite == 0){
            zczytajDane();
        }
        if(prediction == 1){
            filterData(FilterType::LINE_DIFFERENCE);
        }
        cout << "Wywalone po filtracji" << endl;
        if (blackandwhite == 1) {
            if(compression == 1){
                //zapis z kompresja LZ77 dla czarnobiałego
                std::vector<token<Uint8>> tokenComp = LZ77Kompresja<Uint8>(dane8.comp, dane8.comp.size());

                int rozmiar = static_cast<int>(tokenComp.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));
                for(token<Uint8> t: tokenComp){
                    wyjscie.write(reinterpret_cast<char*>(&t), sizeof(token<Uint8>));
                }

            }else if(compression == 2){

            }else{
                //zapis bez kompresji dla czanobiałego
                int rozmiar = static_cast<int>(dane8.comp.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));

                for (auto& element : dane8.comp) {
                    wyjscie.write((char*)&element, sizeof(Uint8));
                }
            }
        }else{
            if(compression == 1){
                //zapis z kompresja LZ77 dla 24bit
                std::vector<token<Uint8>> tokenComp1 = LZ77Kompresja<Uint8>(dane24.comp1, dane24.comp1.size());
                std::vector<token<Uint8>> tokenComp2 = LZ77Kompresja<Uint8>(dane24.comp2, dane24.comp2.size());
                std::vector<token<Uint8>> tokenComp3 = LZ77Kompresja<Uint8>(dane24.comp3, dane24.comp3.size());

                int rozmiar1 = static_cast<int>(tokenComp1.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar1), sizeof(int));
                for(token<Uint8> t: tokenComp1){
                    wyjscie.write(reinterpret_cast<char*>(&t), sizeof(token<Uint8>));
                }

                int rozmiar2 = static_cast<int>(tokenComp2.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar2), sizeof(int));
                for(token<Uint8> t: tokenComp2){
                    wyjscie.write(reinterpret_cast<char*>(&t), sizeof(token<Uint8>));
                }

                int rozmiar3 = static_cast<int>(tokenComp3.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar3), sizeof(int));
                for(token<Uint8> t: tokenComp3){
                    wyjscie.write(reinterpret_cast<char*>(&t), sizeof(token<Uint8>));
                }
            }else if(compression == 2){

            }else{
                //zapis bez kompresja LZ77 dla 24bit
                int rozmiar = static_cast<int>(dane24.comp1.size());
                wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));

                for (size_t i = 0; i < rozmiar; ++i) {
                    wyjscie.write((char*)&dane24.comp1[i], sizeof(Uint8));
                    wyjscie.write((char*)&dane24.comp2[i], sizeof(Uint8));
                    wyjscie.write((char*)&dane24.comp3[i], sizeof(Uint8));
                }
            }
        }
    }

    if (!wyjscie) {
        std::cerr << "Błąd podczas zapisywania pliku!" << std::endl;
    }

    wyjscie.close();
}

void narysujDane8(int xStart, int yStart){
    int k = 0;
    for(int y = yStart; y < yStart + wysokosc; y++)
    {
        for(int x = xStart; x < xStart + szerokosc/2; x++)
        {
            if(k!=64000){
                setPixel(x, y, dane8.comp[k], dane8.comp[k], dane8.comp[k]);
                ++k;
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void narysujDane24(int xStart, int yStart){
    int k = 0;
    for(int y = yStart; y < yStart + wysokosc; y++)
    {
        for(int x = xStart; x < xStart + szerokosc/2; x++)
        {
            if(k!=64000){
                if(yiqstatus == 0){
                    setPixel(x,y,dane24.comp1[k], dane24.comp2[k], dane24.comp3[k]);
                }else{
                    setYIQ(x,y,dane24.comp1[k], dane24.comp2[k], dane24.comp3[k]);
                }
                
                k++;
            }
        }
    }
    SDL_UpdateWindowSurface(window);
}

void narysujDane16(int xStart, int yStart){
    int k = 0;
    for(int y = yStart; y < yStart + wysokosc; y++)
    {
        for(int x = xStart; x < xStart + szerokosc/2; x++)
        {
            if(k!=64000){
                setRGB565(x, y, dane16.comp[k]);
                k++;
            }
        }
    }
    SDL_UpdateWindowSurface(window);
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

bool read(std::string nazwa) {
    clearVector24();
    clearVector8();
    clearVector16();
    std::ifstream wejscie(nazwa, std::ios::binary);

    if (!wejscie.is_open()) {
        std::cerr << "Błąd: nie można otworzyć pliku " << nazwa << std::endl;
        return false;
    }

    wejscie.read(reinterpret_cast<char*>(&identyfikator), sizeof(char) * 2);
    wejscie.read(reinterpret_cast<char*>(&szerokoscObrazka), sizeof(Uint16));
    wejscie.read(reinterpret_cast<char*>(&wysokoscObrazka), sizeof(Uint16));
    wejscie.read(reinterpret_cast<char*>(&dithering), sizeof(int));
    wejscie.read(reinterpret_cast<char*>(&blackandwhite), sizeof(int));
    wejscie.read(reinterpret_cast<char*>(&yiqstatus), sizeof(int));
    wejscie.read(reinterpret_cast<char*>(&bit), sizeof(int));
    wejscie.read(reinterpret_cast<char*>(&prediction), sizeof(int));
    wejscie.read(reinterpret_cast<char*>(&compression), sizeof(int));

    if (!wejscie) {
        std::cerr << "Błąd podczas odczytu nagłówka pliku!" << std::endl;
        wejscie.close();
        return false;
    }

    if (identyfikator[0] == 'P' && identyfikator[1] == 'J') {
        int wysokosc = wysokoscObrazka;
        int szerokosc = szerokoscObrazka * 2;

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

        // Odczytaj rozmiar danych
        int size = 0;
        wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
        if (size <= 0) {
            std::cerr << "Błąd: rozmiar danych (color) jest nieprawidłowy: " << size << std::endl;
            wejscie.close();
            return false;
        }

        if(compression == 1){
            //dekompresja LZ77
            if(bit == 16){
                //dla 16bit
                token<Uint16> zmienna;
                std::vector<token<Uint16>> token16;

                for(int i = 0; i < size; i++){
                    wejscie.read((char*)&zmienna, sizeof(token<Uint16>));
                    token16.push_back(zmienna);
                }

                LZ77Dekompresja<Uint16>(token16, dane16.comp);

                if(prediction == 1){
                    unFilterData(FilterType::LINE_DIFFERENCE);
                }

                narysujDane16(szerokosc/2, 0);
                
                SDL_UpdateWindowSurface(window);
            }else{
                //dla 24
                    if (blackandwhite == 1) {
                        //dla czarnobiałego
                        token<Uint8> zmienna;
                        std::vector<token<Uint8>> token8;
                        
                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&zmienna, sizeof(token<Uint8>));
                            token8.push_back(zmienna);
                        }

                        LZ77Dekompresja<Uint8>(token8, dane8.comp);

                        if(prediction == 1){
                            unFilterData(FilterType::LINE_DIFFERENCE);
                        }

                        narysujDane8(szerokosc/2, 0);
                        
                        SDL_UpdateWindowSurface(window);
                    } else {
                        //dla kolor RGB i YIQ
                        token<Uint8> comp1, comp2, comp3;
                        std::vector<token<Uint8>> tokenComp1, tokenComp2, tokenComp3;

                        std::cout << "Dupa1" << std::endl;
                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&comp1, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp1);
                        }
                        std::cout << "Dup2" << std::endl;

                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&comp2, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp2);
                        }
                        std::cout << "Dupa3" << std::endl;

                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&comp3, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp3);
                        }
                        std::cout << "Dupa4" << std::endl;

                        LZ77Dekompresja<Uint8>(tokenComp1, dane24.comp1);
                                                std::cout << "Dupa5" << std::endl;

                       LZ77Dekompresja<Uint8>(tokenComp2, dane24.comp2);
                                                std::cout << "Dupa6" << std::endl;

                        LZ77Dekompresja<Uint8>(tokenComp3, dane24.comp3);
                                                std::cout << "Dupa7" << std::endl;



                        if(prediction == 1){
                            unFilterData(FilterType::LINE_DIFFERENCE);
                        }

                        narysujDane24(szerokosc/2, 0);
                        SDL_UpdateWindowSurface(window);
                    }
                }
        }else if(compression == 2){
            //dekompresja  DCT
        }else{
            //brak dekompresji
            if(bit == 16){
                Uint16 zmienna;

                for(int i = 0; i < size; i++){
                    wejscie.read((char*)&zmienna, sizeof(Uint16));
                    dane16.comp.push_back(zmienna);
                }
                if(prediction == 1){
                    unFilterData(FilterType::LINE_DIFFERENCE);
                }

                narysujDane16(szerokosc/2, 0);
                
                SDL_UpdateWindowSurface(window);
            }else{
                    if (blackandwhite == 1) {
                        Uint8 zmienna;
                        
                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&zmienna, sizeof(Uint8));
                            dane8.comp.push_back(zmienna);
                        }

                        if(prediction == 1){
                            unFilterData(FilterType::LINE_DIFFERENCE);
                        }

                        narysujDane8(szerokosc/2, 0);
                        
                        SDL_UpdateWindowSurface(window);
                    } else {
                        Uint8 r, g, b;

                        for (int i = 0; i < size; ++i) {
                            wejscie.read((char*)&r, sizeof(Uint8));
                            wejscie.read((char*)&g, sizeof(Uint8));
                            wejscie.read((char*)&b, sizeof(Uint8));

                            dane24.comp1.push_back(r);
                            dane24.comp2.push_back(g);
                            dane24.comp3.push_back(b);
                        }

                        if(prediction == 1){
                            unFilterData(FilterType::LINE_DIFFERENCE);
                        }

                        narysujDane24(szerokosc/2, 0);
                        SDL_UpdateWindowSurface(window);
                    }
                }
            }
        wejscie.close();
        return true;
    } else {
        std::cerr << "Błąd: nieprawidłowy identyfikator pliku." << std::endl;
        wejscie.close();
        return false;
    }
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

