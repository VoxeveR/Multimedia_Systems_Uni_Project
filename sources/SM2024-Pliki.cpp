// funkcje do operacji na plikach
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Kompresje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Modele.h"
#include "../headers/SM2024-Filtrowanie.h"
#include "../headers/SM2024-DCT.h"

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
        }else{
            //zapis bez kompresja LZ77 dla 16bit
            int rozmiar = static_cast<int>(dane16.comp.size());
            wyjscie.write(reinterpret_cast<char*>(&rozmiar), sizeof(int));

            for(auto& element: dane16.comp){
                wyjscie.write((char*)&element, sizeof(Uint16));
            }
        }

    }else{
        if(yiqstatus == 0 and blackandwhite == 0){
            zczytajDane();
        }
        if(prediction == 1){
            filterData(FilterType::LINE_DIFFERENCE);
        }
        
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
                saveDCT(wyjscie, yiqstatus);
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
                // dct
                saveDCT(wyjscie, yiqstatus);
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
        


        if(compression == 1){
            //dekompresja LZ77
            if(bit == 16){
                //dla 16bit
                int size = 0;
                wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
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
                        int size = 0;
                        wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
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
                        int size1, size2, size3;
                        token<Uint8> comp1, comp2, comp3;
                        std::vector<token<Uint8>> tokenComp1, tokenComp2, tokenComp3;


                        
                        wejscie.read(reinterpret_cast<char*>(&size1), sizeof(int));
                        for (int i = 0; i < size1; ++i) {
                            wejscie.read((char*)&comp1, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp1);
                        }
                        
                        
                        wejscie.read(reinterpret_cast<char*>(&size2), sizeof(int));
                        for (int i = 0; i < size2; ++i) {
                            wejscie.read((char*)&comp2, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp2);
                        }
                        
                        wejscie.read(reinterpret_cast<char*>(&size3), sizeof(int));

                        for (int i = 0; i < size3; ++i) {
                            wejscie.read((char*)&comp3, sizeof(token<Uint8>));
                            tokenComp1.push_back(comp3);
                        }

                        LZ77Dekompresja<Uint8>(tokenComp1, dane24.comp1);
                        LZ77Dekompresja<Uint8>(tokenComp2, dane24.comp2);
                        LZ77Dekompresja<Uint8>(tokenComp3, dane24.comp3);


                        if(prediction == 1){
                            unFilterData(FilterType::LINE_DIFFERENCE);
                        }

                        narysujDane24(szerokosc/2, 0);
                        SDL_UpdateWindowSurface(window);
                    }
                }
        }else if(compression == 2){
            //dekompresja  DCT
            readDCT(wejscie, yiqstatus);
        }else{
            //brak dekompresji
            if(bit == 16){
                int size = 0;
                wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
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
                        int size = 0;
                        wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
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
                        int size = 0;
                        wejscie.read(reinterpret_cast<char*>(&size), sizeof(int));
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

