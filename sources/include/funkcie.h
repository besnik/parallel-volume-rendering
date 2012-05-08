// usefull not only math functions

#ifndef __FUNKCIE_H
#define __FUNCKIE_H

#include "vektor3.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

float logn(float x, float base);
float znamienko(float x);
float mocnina(float x);
float odmocnina(float x);
float orez(float x, float min, float max);
float zaokruhli(float x);
float random(float min, float max);
float diV(float x, float y, float c);
float exP(float x);
UINT32 dvaNaX32(UINT8 x); // x iba v rozpati 0-31!
float linInt(UINT16 a, UINT16 b, float fa, float fb, UINT16 x); // linearna interpolacia

// trilinearna interpolacia v kocke o velkosti hrany 1.0f
float triInt(const float& v000, const float& v100,
						 const float& v101, const float& v001,
						 const float& v010, const float& v110,
						 const float& v111, const float& v011,
						 float x, float y, float z);
// trilinearna interpolacia v kvadri o vzdialenostiach medzi voxelmi dx,dy,dz
float triIntAnySize(const float& v000, const float& v100,
										const float& v101, const float& v001,
										const float& v010, const float& v110,
										const float& v111, const float& v011,
										float x, float y, float z, int dx, int dy, int dz);

void vypis(const CVektor3& v); // na stdout vypise obsah vektora
// shading
float schlick(float x, float n); // aproximacia x^n pre osvetlovaci model
CVektor3 cent_dif(float fx1, float fx2, float fy1, float fy2, float fz1, float fz2);
CVektor3 getAnalGrad(const float& v000, const float& v100,
							       const float& v010, const float& v110,
							       const float& v001, const float& v101,
							       const float& v011, const float& v111,
										 const CVektor3& v);
UINT32 faktorial(UINT32 n); // int udrzi maximalne 12! unsigned mozno o 1-2 viac
UINT32 komCis(UINT32 n,UINT32 i); // kombinacne cislo



#endif




