#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Modele.h"
#include "../headers/SM2024-Kompresje.h"


void ByteRunKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName) {
    int i = 0;
    std::vector<Sint8> resultArr;
    
    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            int j = 0;
            while((i+  j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + 1 + j]) && (j < 127)){
                j++;
            }

            //cout<<"("<< -j << "), " << (int)wejscie[i + j] << ", ";


            //// std::cout << "ELement: " << -j << std::endl;
            resultArr.push_back(-j);
            //// std::cout << "Value: " << (int) wejscie[i + j] << std::endl;
            resultArr.push_back(wejscie[i + j]);
            
            i += (j+1);
        } else {
            int j = 0;
            while((i + j < dlugosc - 1) && ((wejscie[i + j] != wejscie[i + j + 1])) && (j < 128)){
                j++;
            }

            if((i+j == dlugosc - 1) && (j < 128)){
                j++;
            }

           // // std::cout<<"("<< (j - 1) << "), ";
            resultArr.push_back(j - 1);

            
            for(int k = 0; k < j; k++){
              //  // std::cout << (int) wejscie[i + k] << ", ";
                resultArr.push_back(wejscie[i + k]);
            }

            i += j;
        }
    }
    // // std::cout << "\n";
    // for (size_t i = 0; i < resultArr.size(); i++) {
    //     // std::cout << (int)resultArr[i] << ", ";
    // }
    // // std::cout << "\n";
    // std::cout << "Kompresja udana\n";
    saveVector<Sint8>(resultArr, fileName);
}


void ByteRunDekompresja(std::string fileName){
    int j = 0;
    std::vector<Sint8> wejscie = readVector<Sint8>(fileName);
    int dlugosc = wejscie.size();
    std::vector<Uint8> output;
    while (j < dlugosc){
        if(wejscie[j] < 0){
            int iters = (-1) * wejscie[j] + 1;
            j++;
            for(int i = 0; i < iters; i ++){
               // // std::cout << (int)wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        } else {
            int iters = wejscie[j] + 1;
            for(int i = 0; i < iters; i ++){
                j++;
                //// std::cout << (int)wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        }
        j++;
    }
    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            setPixel(x, y, output[k], output[k], output[k]);
            k++;
        }
    }
}


void RLEKompresja(std::vector<Uint8> wejscie, int dlugosc, std::string fileName){
    int i = 0;
    std::vector<Uint8> resultArr;

    while (i < dlugosc) {

        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])){
            
            int j = 0;
            while ((i + j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + j + 1]) 
            && (j < 254)){
                j++;
            }

            // std::cout<<"("<<(j + 1)<<"), " << wejscie[i+j]<<", ";

            resultArr.push_back((j+1));
            resultArr.push_back(wejscie[i+j]);

            i += (j+1);
        } else {
            
            int j = 0;

            while ((i + j < dlugosc - 1) && (wejscie[i + j] != wejscie[i + j + 1]) 
            && (j < 254)){
                j++;
            }

            if( (i+j == dlugosc - 1) && (j < 254)){
                j++;
            }

            // std::cout << (int) 0 << ", " << j << ", ";
            resultArr.push_back((int)0);
            resultArr.push_back(j);
            
            for(int k = 0; k < j; k++){
                // std::cout << wejscie[i + k] << ", ";
                resultArr.push_back(wejscie[i + k]);
            }

            if (j % 2 != 0){
                // std::cout << (int) 0 << ", ";
                resultArr.push_back((int)0);
            }

            i += j;
        }
    }

    // std::cout << "Kompresja udana\n";
    saveVector<Uint8>(resultArr, fileName);
}

void RLEDekompresja(std::string fileName){
    int j = 0;
    std::vector<Uint8> wejscie = readVector<Uint8>(fileName);
    int dlugosc = wejscie.size();
    std::vector<Uint8> output;
    while (j < dlugosc){
        //// std::cout <<"W" << wejscie[j] << " ";
        if(wejscie[j] > 0){
            int iter = wejscie[j];
           // // std::cout <<"I" << iter << " ";
            j++;
            for(int i = 0; i < iter; i++){
                // std::cout << wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }
        } else if ( wejscie[j] == 0){
            j++;
            int iter = wejscie[j];
            for(int i = 0; i < iter; i++){
                j++;
                // std::cout << wejscie[j] << ", ";
                output.push_back(wejscie[j]);
            }

            if(iter % 2 != 0){
                j++;
            }
        }
        j++;
    }


    int k = 0;
    for(int y = 0; y < wysokosc; y++){
        for(int x = 0; x < szerokosc/2; x++){
            setPixel(x, y, output[k], output[k], output[k]);
            k++;
        }
    }
} 

//LZW

int rozmiarSlownika = 0;
slowo slownik[65535];


slowo noweSlowo() {
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.dlugosc = 0;
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo noweSlowo(Uint8 znak){
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.dlugosc = 1;
    noweSlowo.element[0] = znak;
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak){
    slowo noweSlowo;

    if(aktualneSlowo.dlugosc < 4096){
        noweSlowo.kod = 0;
        noweSlowo.dlugosc = aktualneSlowo.dlugosc + 1;
        noweSlowo.wSlowniku = false;
        std::copy(std::begin(aktualneSlowo.element), std::end(aktualneSlowo.element), std::begin(noweSlowo.element));
        noweSlowo.element[aktualneSlowo.dlugosc] = znak;
        return noweSlowo;
    } else {
        std::cout << "UWAGA! Przepelnienie rozmiar uznakow w pojedynczym slowie" << std::endl;
        noweSlowo.kod = 0;
        noweSlowo.dlugosc = 0;
        noweSlowo.wSlowniku = false;
        noweSlowo.element[0] = znak;
        return noweSlowo;
    }
}

bool porownajSlowa(slowo slowo1, slowo slowo2){
    if (slowo1.dlugosc != slowo2.dlugosc) return false;
    for (int s = 0; s < slowo1.dlugosc; s++){
        if(slowo1.element[s] != slowo2.element[s]) return false;
    }
    return true;
}

int znajdzWSlowniku(slowo szukany){
    for (int nr = 0; nr < rozmiarSlownika; nr++){
        if (porownajSlowa(slownik[nr], szukany)) return nr;
    }
    return -1;
}

void wyswietlSlowo(slowo aktualneSlowo){
    if(aktualneSlowo.wSlowniku){
        std::cout << "[" << aktualneSlowo.kod << "] ";
    } else {
        std::cout << "[X]";
    }
    for (int s = 0; s < aktualneSlowo.dlugosc; s++){
        std::cout << (int)aktualneSlowo.element[s];
        if(s < aktualneSlowo.dlugosc - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

void wyswietlSlownik() {
    std::cout << "Slownik:" << std::endl;
    for (int i = 0; i < rozmiarSlownika; i++) {
        wyswietlSlowo(slownik[i]);
    }
    std::cout << "Liczba slow w slowniku: " << rozmiarSlownika << std::endl;
}

int dodajDoSlownika(slowo nowy, bool czyWyswietlac = true){
    if (rozmiarSlownika < 65536){
        Uint16 nr = rozmiarSlownika;
        slownik[nr].kod = nr;
        slownik[nr].dlugosc = nowy.dlugosc;
        std::copy(std::begin(nowy.element), std::end(nowy.element), std::begin(slownik[nr].element));
        slownik[nr].wSlowniku = true;
        if (czyWyswietlac) wyswietlSlowo(slownik[nr]);
        rozmiarSlownika++;
        return nr;
    }
    return -1;
}

void LZWinicjalizacja(){
    rozmiarSlownika = 0;

    for(int s = 0; s < 65536; s++){
        slownik[s].kod = 0;
        slownik[s].dlugosc = 0;
        slownik[s].wSlowniku = false;
        memset(slownik[s].element, 0, sizeof(slownik[s].element));
    }

    slowo noweSlowo;
    for(int s = 0; s < 4; s++){
        noweSlowo.dlugosc = 1;
        noweSlowo.element[0] = s;
        noweSlowo.kod = dodajDoSlownika(noweSlowo);
    }
}
void LZWKompresja(int wejscie[], int dlugosc){
    LZWinicjalizacja();
    slowo aktualneSlowo = noweSlowo();
    slowo slowoZnak;
    Uint8 znak;
    int kod;
    int i = 0;

    std::vector<Uint16> resultArray;

    while (i < dlugosc){
        znak = wejscie[i];
        std::cout << "Pobieramy znak" << (int)znak << " z pozycji " << i << std::endl;
        slowoZnak = polaczSlowo(aktualneSlowo, znak);
        std::cout << "Aktualne slowo: ";
        wyswietlSlowo(aktualneSlowo);
        std::cout << "Slowo + znak: ";
        wyswietlSlowo(slowoZnak);
        kod = znajdzWSlowniku(slowoZnak);
        std::cout << "Czy w slowniku? ";
        if (kod < 0){
            std::cout << "NIE" <<std::endl;
            std::cout << "Na wyjscie: [" << aktualneSlowo.kod << "]" << std::endl;
            resultArray.push_back(aktualneSlowo.kod);
            dodajDoSlownika(slowoZnak, false);
            if(znajdzWSlowniku(slowoZnak) > 0){
                slowoZnak.kod = znajdzWSlowniku(slowoZnak);
                std::cout << "Dodajemy do slownika ";
                wyswietlSlowo(slownik[slowoZnak.kod]);
            }
            aktualneSlowo = noweSlowo(znak);
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        } else {
            std::cout << "TAK: [" << kod << "]" << std::endl;
            aktualneSlowo = slowoZnak;
            aktualneSlowo.kod = znajdzWSlowniku(aktualneSlowo);
            aktualneSlowo.wSlowniku = true;
        }
        i++;
    }
    std::cout << "Koniec danych" << std::endl;
    std::cout << "Na wyjscie: [" << aktualneSlowo.kod << "]" << std::endl;
    resultArray.push_back(aktualneSlowo.kod);
    std::cout << std::endl;
    std::cout << "Aktualny slownik:" << std::endl;
    wyswietlSlownik();

    saveVector<Uint16>(resultArray, "test.jawa");
}

//maybe works
void LZWDekompresja(int wejscie[], int dlugosc) {
    LZWinicjalizacja();

    slowo poprzednieSlowo;
    slowo aktualneSlowo;

    for (int i = 0; i < dlugosc; i++) {
        int kod = wejscie[i];
        std::cout << "na wyjscie: [" << kod << "] ";

        if (kod < rozmiarSlownika) {
            aktualneSlowo = slownik[kod];
        } else if (kod == rozmiarSlownika && poprzednieSlowo.dlugosc > 0) {
            aktualneSlowo = polaczSlowo(poprzednieSlowo, poprzednieSlowo.element[0]);
        } else {
            std::cout << "Blad: nieprawidlowy kod" << std::endl;
            return;
        }

        for (int j = 0; j < aktualneSlowo.dlugosc; j++) {
            std::cout << (int)aktualneSlowo.element[j];
            if (j < aktualneSlowo.dlugosc - 1) std::cout << ", ";
        }
    
        std::cout << std::endl;

        if (poprzednieSlowo.dlugosc > 0) {
            slowo noweSlowo = polaczSlowo(poprzednieSlowo, aktualneSlowo.element[0]);
            std::cout << "Nowy element slownika: ";
            wyswietlSlowo(noweSlowo);
            dodajDoSlownika(noweSlowo, true);
        }

        poprzednieSlowo = aktualneSlowo;

        std::cout << std::endl;
    }

    std::cout << "koniec danych" << std::endl;
    std::cout << std::endl;
    std::cout << "aktualny slownik:" << std::endl;
    wyswietlSlownik();
}