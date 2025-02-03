// funkcje do obsługi probkowania
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-Probkowanie.h"
#include "../headers/SM2024-Modele.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                SAMPLING                       ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void YIQSampling(){

     for (int y = 0; y < wysokosc/2; y += 2) { 
        for (int x = 0; x < szerokosc/2; x += 2) { 
            
            
            YIQ yiq00 = RGBtoYIQ(x,y);
            YIQ yiq01 = RGBtoYIQ(x, y + 1);
            YIQ yiq10 = RGBtoYIQ(x + 1, y);
            YIQ yiq11 = RGBtoYIQ(x + 1, y + 1);

            float Iavg = (yiq00.I + yiq01.I + yiq10.I + yiq11.I) / 4.0;
            float Qavg = (yiq00.Q + yiq01.Q + yiq10.Q + yiq11.Q) / 4.0;
            setYIQ(x + szerokosc / 2, y, yiq00.Y, Iavg, Qavg);
            setYIQ(x + szerokosc / 2, y + 1, yiq01.Y, Iavg, Qavg);
            setYIQ(x + szerokosc / 2  + 1, y, yiq10.Y, Iavg, Qavg);
            setYIQ(x + szerokosc / 2 + 1, y + 1, yiq11.Y, Iavg, Qavg);
        }
    }

}