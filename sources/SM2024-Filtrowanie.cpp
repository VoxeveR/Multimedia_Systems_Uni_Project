#include "../headers/SM2024-Filtrowanie.h"
#include "../headers/SM2024-Pliki.h"
#include "../headers/SM2024-Zmienne.h"


#include <iostream>

int Peath(Uint8 comp1, Uint8 comp2, Uint8 comp3){
    int w = comp1 + comp2 - comp3;
    return std::min(std::min((comp1 - w), (comp2 - w)), (comp3 - w));
}

void filterData(FilterType type){
    

    if(bit == 24){
        switch(type){
            case NO_FILTER:
                std::cout<<"to po co tego uzywasz?" << std::endl;
            break;
            case DIFFERENTIAL:
                for(int i = 0; i < ((szerokosc / 2) * wysokosc) - 1; i++){
                    Uint8 tmp1 = dane24.comp1[i+1];
                    Uint8 tmp2 = dane24.comp2[i+1];
                    Uint8 tmp3 = dane24.comp3[i+1];

                    dane24.comp1[i+1] = tmp1 - dane24.comp1[i];
                    dane24.comp2[i+1] = tmp2 - dane24.comp2[i];
                    dane24.comp3[i+1] = tmp3 - dane24.comp3[i];
                }
            break;
            case LINE_DIFFERENCE:
                for(int i = szerokosc/2; i < ((szerokosc / 2) * wysokosc); i++){
                    dane24.comp1[i] = dane24.comp1[i] - dane24.comp1[i-szerokosc/2];
                    dane24.comp2[i] = dane24.comp2[i] - dane24.comp2[i-szerokosc/2];
                    dane24.comp3[i] = dane24.comp3[i] - dane24.comp3[i-szerokosc/2];
                }
            break;
            case AVERAGING:
                for(int i = szerokosc/2+1; i < ((szerokosc / 2) * wysokosc); i++){
                    dane24.comp1[i] = dane24.comp1[i] - (dane24.comp1[i-szerokosc/2]+dane24.comp1[i-1])/2;
                    dane24.comp2[i] = dane24.comp2[i] - (dane24.comp2[i-szerokosc/2]+dane24.comp2[i-1])/2;
                    dane24.comp3[i] = dane24.comp3[i] - (dane24.comp3[i-szerokosc/2]+dane24.comp3[i-1])/2;
                }
            
            break;
            case PAETH:
                for(int i = szerokosc/2+1; i < ((szerokosc / 2) * wysokosc); i++){
                    dane24.comp1[i] = dane24.comp1[i] - Peath(dane24.comp1[i-1], dane24.comp2[i-szerokosc/2], dane24.comp3[i - (szerokosc/2) - 1]);
                    dane24.comp2[i] = dane24.comp2[i] - Peath(dane24.comp1[i-1], dane24.comp2[i-szerokosc/2], dane24.comp3[i - (szerokosc/2) - 1]);
                    dane24.comp3[i] = dane24.comp3[i] - Peath(dane24.comp1[i-1], dane24.comp2[i-szerokosc/2], dane24.comp3[i - (szerokosc/2) - 1]);
                }
            break;
            default:
                std::cout<<"WRONG VALUE TRY AGAIN MY DEAR!!!!"<<std::endl;
            break;
        }
    }else{
        switch(type){
            case NO_FILTER:
                std::cout<<"to po co tego uzywasz?" << std::endl;
            break;
            case DIFFERENTIAL:
                for(int i = 0; i < ((szerokosc / 2) * wysokosc) - 1; i++){
                    Uint8 tmp1 = dane16.comp[i+1];

                    dane16.comp[i+1] = tmp1 - dane16.comp[i];
                }
            break;
            case LINE_DIFFERENCE:
                for(int i = szerokosc/2; i < ((szerokosc / 2) * wysokosc); i++){
                    dane16.comp[i] = dane16.comp[i] - dane16.comp[i-szerokosc/2];
                }
            break;
            case AVERAGING:
                for(int i = szerokosc/2+1; i < ((szerokosc / 2) * wysokosc); i++){
                    dane16.comp[i] = dane16.comp[i] - (dane16.comp[i-szerokosc/2]+dane16.comp[i-1])/2;
                }
            
            break;
            case PAETH:
                for(int i = szerokosc/2+1; i < ((szerokosc / 2) * wysokosc); i++){
                    dane16.comp[i] = dane16.comp[i] - Peath(dane16.comp[i-1], dane16.comp[i-szerokosc/2], dane16.comp[i - (szerokosc/2) - 1]);
                }
            break;
            default:
                std::cout<<"WRONG VALUE TRY AGAIN MY DEAR!!!!"<<std::endl;
            break;
    }
}

void unFilterData(dane888* data, FilterType type){
    
    switch(type){
        case NO_FILTER:
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
        for(int i = ((szerokosc / 2) * wysokosc); i > (szerokosc/2); i--){
                data->comp1[i] = data->comp1[i] + data->comp1[i - (szerokosc / 2)];
                data->comp2[i] = data->comp2[i] + data->comp2[i - (szerokosc / 2)];
                data->comp3[i] = data->comp3[i] + data->comp3[i - (szerokosc / 2)];
            }
        break;
        case AVERAGING:
            for(int i = ((szerokosc / 2) * wysokosc) - 1; i > (szerokosc/2); i--){
                data->comp1[i] = data->comp1[i] + (data->comp1[i-szerokosc/2]+data->comp1[i-1])/2;
                data->comp2[i] = data->comp2[i] + (data->comp2[i-szerokosc/2]+data->comp2[i-1])/2;
                data->comp3[i] = data->comp3[i] + (data->comp3[i-szerokosc/2]+data->comp3[i-1])/2;
            }
        break;
        case PAETH: 
            for(int i = ((szerokosc / 2) * wysokosc) - 1; i > (szerokosc/2); i--){
                data->comp1[i] = data->comp1[i] + Peath(data->comp1[i-1], data->comp2[i-szerokosc/2], data->comp3[i - (szerokosc/2) - 1]);
                data->comp2[i] = data->comp2[i] + Peath(data->comp1[i-1], data->comp2[i-szerokosc/2], data->comp3[i - (szerokosc/2) - 1]);
                data->comp3[i] = data->comp3[i] + Peath(data->comp1[i-1], data->comp2[i-szerokosc/2], data->comp3[i - (szerokosc/2) - 1]);
            }
        break;
        default:
            std::cout<<"WRONG VALUE TRY AGAIN MY DEAR!!!!"<<std::endl;
        break;
    }

}