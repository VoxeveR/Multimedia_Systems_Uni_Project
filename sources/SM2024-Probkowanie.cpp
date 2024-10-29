// funkcje do obsługi probkowania
#include "../headers/SM2024-Zmienne.h"
#include "../headers/SM2024-Funkcje.h"
#include "../headers/SM2024-Konwersje.h"
#include "../headers/SM2024-MedianCut.h"
#include "../headers/SM2024-Paleta.h"
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

void YCbCrSampling(){
    for (int y = 0; y < wysokosc/2; y += 2) { 
        for (int x = 0; x < szerokosc/2; x += 2) { 

            YCbCr ycbcr00 = RGBtoYCbCr(x, y);
            YCbCr ycbcr01 = RGBtoYCbCr(x, y + 1);
            YCbCr ycbcr10 = RGBtoYCbCr(x + 1, y);
            YCbCr ycbcr11 = RGBtoYCbCr(x + 1, y + 1);

            float Cbavg = (ycbcr00.Cb + ycbcr01.Cb + ycbcr10.Cb + ycbcr11.Cb) / 4.0;
            float Cravg = (ycbcr00.Cr + ycbcr01.Cr + ycbcr10.Cr + ycbcr11.Cr) / 4.0;
            setYCbCr(x + szerokosc / 2, y, ycbcr00.Y, Cbavg, Cravg);
            setYCbCr(x + szerokosc / 2, y + 1, ycbcr01.Y, Cbavg, Cravg);
            setYCbCr(x + szerokosc / 2  + 1, y, ycbcr10.Y, Cbavg, Cravg);
            setYCbCr(x + szerokosc / 2 + 1, y + 1, ycbcr11.Y, Cbavg, Cravg);
        }
    }
}


void YUVSampling(){
    for (int y = 0; y < wysokosc/2; y += 2) { 
        for (int x = 0; x < szerokosc/2; x += 2) { 

            YUV yuv00 = RGBtoYUV(x, y);
            YUV yuv01 = RGBtoYUV(x, y + 1);
            YUV yuv10 = RGBtoYUV(x + 1, y);
            YUV yuv11 = RGBtoYUV(x + 1, y + 1);

            float Uavg = (yuv00.U + yuv01.U + yuv10.U + yuv11.U) / 4.0;
            float Vavg = (yuv00.V + yuv01.V + yuv10.V + yuv11.V) / 4.0;

            setYUV(x + szerokosc / 2, y, yuv00.Y, Uavg, Vavg);
            setYUV(x + szerokosc / 2, y + 1, yuv01.Y, Uavg, Vavg);
            setYUV(x + szerokosc / 2  + 1, y, yuv10.Y, Uavg, Vavg);
            setYUV(x + szerokosc / 2 + 1, y + 1, yuv11.Y, Uavg, Vavg);
        }
    } 
}


void HSLSampling(){
    for (int y = 0; y < wysokosc/2; y += 2) { 
        for (int x = 0; x < szerokosc/2; x += 2) { 

            HSL hsl00 = RGBtoHSL(x, y);
            HSL hsl01 = RGBtoHSL(x, y + 1);
            HSL hsl10 = RGBtoHSL(x + 1, y);
            HSL hsl11 = RGBtoHSL(x + 1, y + 1);

            float Lavg = (hsl00.L + hsl01.L + hsl10.L + hsl11.L) / 4.0;

            setHSL(x + szerokosc / 2, y, hsl00.H, hsl00.S, Lavg);
            setHSL(x + szerokosc / 2, y + 1, hsl01.H, hsl01.S, Lavg);
            setHSL(x + szerokosc / 2  + 1, y, hsl10.H, hsl10.S, Lavg);
            setHSL(x + szerokosc / 2 + 1, y + 1, hsl11.H, hsl11.S, Lavg);
        }
    } 
}