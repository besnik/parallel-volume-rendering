// holds volume data


#ifndef __DATA_H
#define __DATA_H


#include "kvader.h"
#include "funkcie.h"
#include "loadlsm.h"
#include "distanceVolume.h"

class CData	// regular grid, lavotocivy suradnicovy system
{
public:
	UINT16* data;	// samotne data
	UINT32* homog; // homog lookup data
	UINT8*  bunky; // distance volume
	int x,y,z;	// rozmery mriezky
	int dx,dy,dz;	// vzdialenosti medzi voxelmi mriezky
	float posunx, posuny, posunz;	// posunutie mriezky dat v priestore
	CKvader obal;	// abstraktny obal na testovanie prieniku luca s kockou dat
	UINT16* data2; // inak usporiadane data	
	// sakas addressing scheme
	UINT8 Caddr;
	UINT32* Gaddr, *Haddr, *Iaddr;
	// bricking
	UINT16 bd_x, bd_y, bd_z; // rozmery kocky (brick dimension)
	UINT16 bvd_x, bvd_y, bvd_z; // pocet brickov v jednotlivych smeroch (brick volume dim)
	UINT16 i2,j2,k2; // ktory brick (n div BD)
	UINT8 i1,j1,k1; // offset v ramci bricku (n mod BD)
	UINT32 lut[8][7];
	// lsm file
	CLSMFile *lsmfile; // pomocou tohto objektu nacitame subor
	CImageDirectory *imdirAkt; // pointer na aktivny objekt spajaneho zoznamu
	// konstruktory
	CData();
	CData(int x, int y, int z);
	CData(const CVektor3& v);
	// destruktor
	~CData();
	void init(); // vygeneruje synteticke data
	void init2(); // testovacie data kvoli lsm datam
	void initLSM(); // nacita data zo suboru
	void initHomog(); // z dat vygeneruje pole "homog"
	void nastavObal(); // nastavit vektory min a max podla x,y,z a posunx,posuny,posunz
	void initDVolume(); // urcenie vzdialenosti buniek od zaujimavych objektov, ulozene v poli uint8 *bunky
	// addressing scheme
	void initSakas(); // sakas adr. scheme. inicializuje polia G,H,I,-addr
	void initBrick(); // inicializuje premenne pre bricking, alokuje pole "data2" a preindexuje tam linearne usporiadane data
};


// addressing scheme
int getIndex(int i, int j, int k, const CData& d);	// vrati index do 1D pola "data" z 3D suradnic [i,j,k]
int getIndexSak(int i, int j, int k, const CData& d); // sakas adr.vrati index do sakas alokovaneho pola
// set and check homog bit
void setHBit(int i, int j, int k, const CData& d); // nastavi prislusny bit v "homog" na 1 ak je blok homogenny
bool overHBit(int i, int j, int k, const CData& d); // overi ci blok je homogenny
// bricking
void nastavIcka(int i, int j, int k, CData& d); // nastavi i2 a i1
int blkOffset(int i, int j, int k, const CData& d); // ktory brick
int offsetInBlk(int i, int j, int k, const CData& d); // offset v bricku
int getIndexBrick(int i, int j, int k, CData& d); // vrati index do bricking alokovaneho pola
void getIndexLut(int i, int j, int k, CData& d, UINT32* v); // vrati index za pomoci look


#endif


/*

Version history:

	1.03
	  - 16.1.2006
		- universal bricking algorithm
		- bugs fixed

	1.02
		- 5.1.2006
		- homog lookup data added
		- check and set homogenous bit functions added

	1.01
		- 4.1.2006
		- some bugs fixed

	1.00
		- 2.1.2006
		- initial commit, first methods



*/

