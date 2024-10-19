// funkcje do obs³ugi ditheringu
#ifndef SM2024_DITHERING_H_INCLUDED
#define SM2024_DITHERING_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++                  BAYER                        ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

//float sredniaKrokR();
//float sredniaKrokG();
//float sredniaKrokB();
void ditheringBayer();
void ditheringBayerPaletowy();
void ditheringBayerBW();
void ditheringBayerPaletowyBW();

// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ++              FLOYD-STEINBERG                  ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++

void ditheringFloyd();
void ditheringFloydBW();
void ditheringFloydPaletowy();
void ditheringFloydPaletowyBW();

#endif // SM2024_DITHERING_H_INCLUDED
