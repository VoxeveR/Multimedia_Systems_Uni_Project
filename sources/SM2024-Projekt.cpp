// główny plik projektu
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Paleta.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Dithering.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Modele.h"

#include <exception>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <conio.h>
#include <bitset>

using namespace std;
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                   MAIN                        ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

int main(int argc, char* argv[]) {
    system("cls");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    bool czyBMP = false;
    string nazwa;
    
    while(true){
        if(!window){
            cout << "Zaladuj plik bmp" << endl;
            cout << "Zaladuj plik .jawa" << endl;
            cout << "Wyjdz z programu" << endl;

            int menu[] = {2, 2, 2};
            int counter1 = 1;
            char key;

            do {
                gotoxy(0, 0);
                color(menu[0]);
                key = _getch();
                system("cls");

                if (key == 72) counter1--;
                else if (key == 80) counter1++;


                if (counter1 < 1)counter1 = 3;
                if (counter1 > 3)counter1 = 1;


                if(counter1 ==1)menu[0] = 2;
                else menu[0] = 15;


                if(counter1 ==2) menu[1] = 2;
                else menu[1] = 15;


                if(counter1 ==3) menu[2] = 2;
                else menu[2] = 15;


                color(menu[0]);
                cout << "Zaladuj plik bmp" << endl;
                color(menu[1]);
                cout << "Zaladuj plik .jawa" << endl;
                color(menu[2]);
                cout << "Wyjdz z programu" << endl;


                if (key == 13) {
                    if (counter1 == 1) {
                        cout << "Wybrano opcje: Zaladuj plik bmp" << endl;
                        cout << "Podaj nazwe pliku (uwzgledniajac rozszerzenie): ";
                        cin >> nazwa;
                        if(!odczytajPlik_bmp(nazwa)){
                            cout << "Blad odczytu pilku. Nacisnij dowolny klawisz aby kontynuowac...";
                            _getch();
                        }else{
                            czyBMP = true;
                        }
                        system("cls");

                    }else if (counter1 == 2) {
                        cout << "Wybrano opcje: Zaladuj plik .jawa" << endl;
                        system("cls");

                        cout << "Podaj nazwe pliku: ";
                        cin >> nazwa;

                        if(!read(nazwa)){
                            cout << "Blad odczytu pilku. Nacisnij dowolny klawisz aby kontynuowac...";
                            _getch();
                        }

                        narysujDane(szerokosc/2,0);
                        break;

                    }else if (counter1 == 3) {
                        color(15);
                        cout << "Wybrano opcje: Wyjdz z programu" << endl;
                        return 0;
                    }
                }
            }while(key != 13);
        }
        else
        {
            cout << "=== INFORMACJE O OBRAZIE ===" << endl;
            cout << "ID: " << identyfikator << endl;
            cout << "Rozmiar: " << szerokosc/2 << "x" << wysokosc << " (Szerokosc x Wysokosc)" << endl;
            cout << "Tryb: " << tryb << endl;
            cout << "Dithering: " << (dithering == 0 ? "Brak" : (dithering == 1 ? "Floyd-Steinberg" : "Bayer")) << endl;
            cout << "============================" << endl;

            cout << "=== OPCJE PALETY ===" << endl;
            cout << "1. Zmiana na skale szarosci" << endl;
            cout << "2. Zamiana na 16-bitowy RGB565" << endl;
            cout << "3. Zamiana na 24-bitowy YIQ888" << endl;
            cout << "4. Zamiana na 24-bitowy YIQ888 z próbkowaniem" << endl;
            cout << "5. Algorytm Predykcji typ 2" << endl;
            cout << "6. Kompresja bezstratna LZ77" << endl;
            cout << "7. Kompresja stratna (transformata DCT)" << endl;
            cout << "============================" << endl;

            cout << "=== SUGEROWANY SPOOB UZYCIA ===" << endl;
            cout << "https://github.com/VoxeveR/Multimedia_Systems_Uni_Project/blob/main/sugerowany.jpeg" << endl;
            cout << "===========================" << endl;

            cout << "=== USTAWIENIA DITHERINGU ===" << endl;
            cout << "8. Przelacz Dithering" << endl;
            cout << "   0 - Brak ditheringu" << endl;
            cout << "   1 - Bayer" << endl;      
            cout << "============================" << endl;

            cout << "=== DODATKOWE OPCJE ===" << endl;
            cout << "r - Reset" << endl;
            cout << "s - Zapisz" << endl;
            cout << "ESC - Wyjdz" << endl;
            cout << "============================" << endl;

            SDL_Event event;
            bool option = false;
            while (SDL_WaitEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        SDL_FreeSurface(screen);
                        SDL_DestroyWindow(window);
                        window = nullptr;
                        screen = nullptr;
                        option = true;
                        break;
                    case SDL_KEYDOWN:
                    {
                        if (event.key.keysym.sym == SDLK_ESCAPE)
                        {
                            SDL_FreeSurface(screen);
                            SDL_DestroyWindow(window);
                            window = nullptr;
                            screen = nullptr;
                            option = true;
                            blackandwhite = 0;
                            prediction = 0;
                            bit = 24;
                            compression = 0;
                        }
                        if (event.key.keysym.sym == SDLK_1)
                        {
                            Funkcja1();
                            option = true;
                        }
                        if (event.key.keysym.sym == SDLK_2)
                        {
                            Funkcja2();
                            option = true;
                        }
                        if (event.key.keysym.sym == SDLK_3)
                        {
                            Funkcja3();
                            option = true;
                        }
                        if (event.key.keysym.sym == SDLK_4)
                        {
                            Funkcja4();
                            option = true;
                        }
                        if (event.key.keysym.sym == SDLK_5)
                        {
                            Funkcja5();
                            option = true;
                        }
                        if (event.key.keysym.sym == SDLK_6)
                        {
                            Funkcja6();
                            option = true;
                        }

                        if (event.key.keysym.sym == SDLK_7)
                        {
                            Funkcja7();
                            option = true;
                        }

                        if (event.key.keysym.sym == SDLK_8)
                        {
                            Funkcja8();
                            option = true;
                        }

                        if (event.key.keysym.sym == SDLK_9)
                        {
                            Funkcja9();
                            option = true;
                        }



                        if (event.key.keysym.sym == SDLK_r)
                        {
                            SDL_FreeSurface(screen);
                            SDL_DestroyWindow(window);

                            std::cout << "czyBMP: " << czyBMP << std::endl;
                            if(czyBMP) odczytajPlik_bmp(nazwa);
                            else narysujDane(szerokosc/2,0);
                            option = true;
                            blackandwhite = 0;
                            prediction = 0;
                            bit = 24;
                            compression = 0;
                        }
                        if (event.key.keysym.sym == SDLK_s){
                            system("cls");
                            string nazwa;


                            cout << "Wybierz tryb zapisu: " << endl;
                            gotoxy(0,1);

                            cout << "BMP" << endl;
                            cout << "jawa" << endl;

                            int Set2[] = {2, 2};
                            int counter2 = 1;
                            char key2;
                            //------------------
                            do {
                                gotoxy(0, 1);
                                color(Set2[0]);
                                key2 = _getch();
                                system("cls");

                                if (key2 == 72) counter2--;
                                else if (key2 == 80) counter2++;


                                if (counter2 < 1)counter2 = 2;
                                if (counter2 > 2)counter2 = 1;


                                if(counter2 ==1)Set2[0] = 2;
                                else Set2[0] = 15;


                                if(counter2 ==2) Set2[1] = 2;
                                else Set2[1] = 15;

                                if(counter2 ==3) Set2[2] = 2;
                                else Set2[2] = 15;

                                color(Set2[0]);
                                cout << "BMP" << endl;
                                color(Set2[1]);
                                cout << "jawa" << endl;

                                if (key2 == 13) {
                                    color(15);
                                    cout << "Podaj nazwe pliku (uwzgledniajac rozszerzenie): ";
                                    cin >> nazwa;
                                    if (counter2 == 1) {
                                        saveRightHalfBMP(nazwa);
                                        system("cls");

                                    }else{
                                        save(nazwa);
                                        system("cls");
                                    }
                                }
                            }while(key2 != 13);
                            option = true;
                            break;
                        }else{
                            break;
                        }
                    }
                }
                if (option) break;
            }
        }
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    return 0;
}
