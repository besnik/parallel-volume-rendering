// bezier.h
// bezierova krivka
// by skip 
// version 1.00

#ifndef __BEZIER_H
#define __BEZIER_H

#include <GL/glut.h>
#include "funkcie.h"


struct BOD
{
   float x;
   float y;
}; 



class CBezKrivka
{
public:
	UINT8 c; // maximalny pocet riadiacich vrcholov - na tuto hodnotu sa inicializuje velkost pola
	UINT8 pocet; // pocet - ukazuje na volnu poziciu v poli suradnice
	UINT8 presnost; // ako presne bude krivka vypocitana
	UINT8 presnost_min,presnost_max; // nastavenie maximalnej presnosti
	BOD* suradnice;
	UINT8 bod; // bod, kt. sa bude posuvat
	float dx,dy; // o kolko od zakladnej pozicie sme ho chytili
	short sindex; // tu sa ulozi vysledok vratBod()
	// konstruktory
	CBezKrivka();
	// destruktor
	~CBezKrivka();
};


float bsPol(int n, int i, float t);  // bernsteinov polynom
BOD bKrivka(float t, int p, const CBezKrivka& bk); // bezierova krivka - 1 bod krivky
// funckie na vykreslenie bezierovej krivky - vyzaduju opengl
void kresliBody(const float polomer, const CBezKrivka& bk); // nakresli riadiace body
void kresliSpojnice(const CBezKrivka& bk); // nakresli spojnice riadiacich bodov
void kresliKrivku(const CBezKrivka& bk); // vykresli krivku podla riadiacich bodov
BOD vratKrivku(float t, const CBezKrivka& bk); // jeden konkretny bod krivky
short vratBod(float x, float y, const CBezKrivka& bk); // ak klikol blizko nejakeho bodu vrati jeho index, ak nie vrati -1
void initBKriv(int c, CBezKrivka& bk); // alokuje pole suradnice


	
#endif

