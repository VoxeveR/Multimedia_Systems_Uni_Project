// transformata DCT
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Kompresje.h"
#include "../headers/SM2024-Modele.h"
#include "../headers/SM2024-DCT.h"

using namespace std;

void saveDCT(std::ofstream &wyjscie, int yiq){
    macierz blokDCT_Y;
    macierz blokDCT_I;
    macierz blokDCT_Q;

    macierz blokDane_1;
    macierz blokDane_2;
    macierz blokDane_3;

    for (int by = 0; by < wysokosc; by += rozmiarBloku) {
        for (int bx = szerokosc/2; bx < szerokosc; bx += rozmiarBloku) {
            if(yiq == 1 || yiq == 2){
                for (int y = 0; y < rozmiarBloku; y++) {
                    for (int x = 0; x < rozmiarBloku; x++) {
                        YIQ YIQret = RGBtoYIQ(bx + x, by + y);
                        blokDane_1.dane[x][y] = YIQret.Y;
                        blokDane_1.dct[x][y] = 0;

                        blokDane_2.dane[x][y] = YIQret.I;
                        blokDane_2.dct[x][y] = 0;

                        blokDane_3.dane[x][y] = YIQret.Q;
                        blokDane_3.dct[x][y] = 0;
                    }
                }

            }else{
                for (int y = 0; y < rozmiarBloku; y++) {
                    for (int x = 0; x < rozmiarBloku; x++) {
                        SDL_Color color = getPixel(bx + x, by + y);
                        blokDane_1.dane[x][y] = color.r;
                        blokDane_1.dct[x][y] = 0;

                        blokDane_2.dane[x][y] = color.g;
                        blokDane_2.dct[x][y] = 0;

                        blokDane_3.dane[x][y] = color.b;
                        blokDane_3.dct[x][y] = 0;
                    }
                }
            }

            blokDCT_Y = dct(blokDane_1.dane);
            blokDCT_I = dct(blokDane_2.dane);
            blokDCT_Q = dct(blokDane_3.dane);

            wyjscie.write(reinterpret_cast<char*>(&blokDCT_Y.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);
            wyjscie.write(reinterpret_cast<char*>(&blokDCT_I.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);
            wyjscie.write(reinterpret_cast<char*>(&blokDCT_Q.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);
        }
    }
}


void readDCT(std::ifstream &wejscie, int yiqstatus){
    macierz blokDCT_Y;
    macierz blokDCT_I;
    macierz blokDCT_Q;

    macierz noweDane_1;
    macierz noweDane_2;
    macierz noweDane_3;

    for (int by = 0; by < wysokosc; by += rozmiarBloku) {
        for (int bx = 0; bx < szerokosc/2; bx += rozmiarBloku) {
            wejscie.read(reinterpret_cast<char*>(&blokDCT_Y.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);
            wejscie.read(reinterpret_cast<char*>(&blokDCT_I.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);
            wejscie.read(reinterpret_cast<char*>(&blokDCT_Q.dct), sizeof(float)*rozmiarBloku*rozmiarBloku);

            noweDane_1 = idct(blokDCT_Y.dct);
            noweDane_2 = idct(blokDCT_I.dct);
            noweDane_3 = idct(blokDCT_Q.dct);

            if(yiqstatus == 1 || yiqstatus == 2){
                for (int y = 0; y < rozmiarBloku; y++) {
                    for (int x = 0; x < rozmiarBloku; x++) {
                        setYIQ(bx + x + szerokosc/2, by + y,
                                noweDane_1.dane[x][y], noweDane_2.dane[x][y], noweDane_3.dane[x][y]);
                    }
                }
            }else{
                for (int y = 0; y < rozmiarBloku; y++) {
                    for (int x = 0; x < rozmiarBloku; x++) {
                        setPixel(bx + x + szerokosc/2, by + y,
                                noweDane_1.dane[x][y], noweDane_2.dane[x][y], noweDane_2.dane[x][y]);
                    }
                }
            }

            SDL_UpdateWindowSurface(window);
        }
    }

}


void kwantyzujDCT(macierz& blok) {
    float minVal = blok.dct[0][0];
    float maxVal = blok.dct[0][0];

    for (int i = 0; i < rozmiarBloku; ++i) {
        for (int j = 0; j < rozmiarBloku; ++j) {
            if (blok.dct[i][j] < minVal) minVal = blok.dct[i][j];
            if (blok.dct[i][j] > maxVal) maxVal = blok.dct[i][j];
        }
    }

    for (int i = 0; i < rozmiarBloku; ++i) {
        for (int j = 0; j < rozmiarBloku; ++j) {
            float wartosc = blok.dct[i][j];

                blok.dct[i][j] = Uint8(127 + (wartosc / maxVal) * 128);

        }
    }
}

void dekwantyzujDCT(macierz& blokDCT) {
    float minVal = blokDCT.dct[0][0];
    float maxVal = blokDCT.dct[0][0];

    for (int i = 0; i < rozmiarBloku; i++) {
        for (int j = 0; j < rozmiarBloku; j++) {
            if (blokDCT.dct[j][i] < minVal) minVal = blokDCT.dct[j][i];
            if (blokDCT.dct[j][i] > maxVal) maxVal = blokDCT.dct[j][i];
        }
    }

    for (int i = 0; i < rozmiarBloku; i++) {
        for (int j = 0; j < rozmiarBloku; j++) {
            float wartosc = blokDCT.dct[j][i];
            blokDCT.dct[j][i] = (wartosc / 255.0f) * (maxVal - minVal) + minVal;
        }
    }

}

void wyswietlDane(macierz blok)
{
    cout << "Dane pikselowe w macierzy:"<<endl;
    for(int i = 0; i < rozmiarBloku; i++)
    {
        for(int j = 0; j < rozmiarBloku; j++)
        {
            cout << setw(4) << (int)blok.dane[j][i]<<" ";
        }
        cout << endl;
    }
}

void wyswietlDCT(macierz blok)
{
    cout << "Wspolczynniki transformaty w macierzy:" << endl;
    for(int i = 0; i < rozmiarBloku; i++)
    {
        for(int j = 0; j < rozmiarBloku; j++)
        {
            cout << fixed << setw(6) << setprecision(2) << blok.dct[j][i]<<" ";
        }
        cout << endl;
    }
}

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku])
{
    float wynik[rozmiarBloku][rozmiarBloku];

    for(int v = 0; v < rozmiarBloku; ++v)
    {
        for(int u = 0; u < rozmiarBloku; ++u)
        {
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT = 0;

            for(int y = 0; y < rozmiarBloku; ++y)
            {
                for(int x = 0; x < rozmiarBloku; ++x)
                {
                    double uCosFactor = cos( (double)(2*x+1)*M_PI*(double)u / (2*(double)rozmiarBloku) );
                    double vCosFactor = cos( (double)(2*y+1)*M_PI*(double)v / (2*(double)rozmiarBloku) );
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel*uCosFactor*vCosFactor;
                }

            }
            wspolczynnikDCT *= ( 2.0 / (double)rozmiarBloku ) * cu * cv;
            wynik[u][v] = wspolczynnikDCT;
        }
    }
    macierz rezultat;
    for( int j = 0; j < rozmiarBloku; j++)
    {
        for(int i = 0; i < rozmiarBloku; i++)
        {
            rezultat.dct[i][j] = wynik[i][j];
            rezultat.dane[i][j] = wartosci[i][j];
        }
    }
    return rezultat;
}

template <typename T>
void wypiszWektor(const std::vector<T>& wektor) {
    for (const T& element : wektor) {
        std::cout << element << " ";
    }
    //std::cout << std::endl;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku])
{
    int wynik[rozmiarBloku][rozmiarBloku];

    for(int x = 0; x < rozmiarBloku; ++x)
    {
        for(int y = 0; y < rozmiarBloku; ++y)
        {
            double pixel = 0;
            for(int u = 0; u < rozmiarBloku; ++u)
            {
                for(int v = 0; v < rozmiarBloku; ++v)
                {
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    double uCosFactor = cos( (double)(2*x+1)*M_PI*(double)u / (2*(double)rozmiarBloku) );
                    double vCosFactor = cos( (double)(2*y+1)*M_PI*(double)v / (2*(double)rozmiarBloku) );
                    double wspolczynnikDCT = DCT[u][v];
                    pixel += wspolczynnikDCT * uCosFactor * vCosFactor *cu * cv;
                }
            }
            pixel*= (2.0/(double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }


    macierz rezultat;
    for(int j = 0; j < rozmiarBloku; j++)
    {
        for(int i = 0; i < rozmiarBloku; i++)
        {
            if(wynik[i][j] > 255) wynik[i][j] = 255;
            else if(wynik[i][j] < 0) wynik[i][j] = 0;

            rezultat.dane[i][j] = wynik[i][j];
            rezultat.dct[i][j] = DCT[i][j];
        }
    }
    return rezultat;
}