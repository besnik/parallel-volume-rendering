// procesor.h
// by skip 2006
//

#ifndef __PROCESOR_H_
#define __PROCESOR_H_

#include "vektor3.h"
#include "binswap.h"
#include "kdtree.h"
#include "farba.h"

#include "mpi.h"


typedef struct
{
	int x, y;
} Bod2;


class Procesor	// spolocne premenne procesora
{
public:
	int ID;	// id procesora
	int pocprc;	// celkovy pocet procesorov
	MPI_Status status; // pri mpi_recv - kolko dat sme realne prijali
	
	Plan* plan;	// binary swap plan, k ktorym procesorom ci vymenim data a ako
	KDTree* strom;	// kd-strom, rozdeli cele data a vdaka nemu vytvorime kompozicny plan pre alg. binary swap
	TVektor<int> a,b; // data ohranicene bodmi A,B
	
	CFarba** obr; // vyrenderovany obrazok
	int obr_x, obr_y; // rozmery obrazku
	Bod2 obr_a,obr_b; // ohranicenie obrazka ktory kombinujeme
	
	float *sbuffer; // send
	float *rbuffer; // receive
	
	// konstruktory
	Procesor():ID(0),pocprc(0),plan(NULL),strom(NULL), obr_x(0), obr_y(0), obr(NULL), sbuffer(NULL), rbuffer(NULL) {};
	Procesor(int ID, int pocprc, const TVektor<int>& a, const TVektor<int>& b);
	// destruktor
	~Procesor();
	
	// funkcie
	void initObr(int obr_x, int obr_y); // inicializuje rozmery obrazka a pamat
	void zrusObr(); // uvolni obrazok z pamate
	void binarySwap(); // skombinuje sub-obrazky medzi procesormi a nakoniec bude procesor drzat cast vysledneho obrazku
	void setRozmery(const int u, const Bod2& a, const Bod2& b, Bod2& sa, Bod2& sb, Bod2& ra, Bod2& rb); // do sbuffra nacita prislusnu cast obrazku, ktora sa potom posle druhemu procesoru // a,b ohranicenie povodneho obrazu, sx - send ohranicenie, rx, receive ohranicenie
	void loadBuffer(const Bod2& sa, const Bod2& sb); // naplni sbuffer
	void kompozicia(const int u); // skompinuje cast obrazka so rbuffer
	// debug stuff
	void vypisAB(); // vypise na stdout obr_a obr_b
	void vypisPlan(); // vypise obsah pola plan
	void vypisObr(); // zobrazi aktualne data obrazku
	void init_obr(); // inicializuje hodnoty obrazka na testovacie 
	void init_recv(const Bod2& ra, const Bod2& rb); // inicializuje rbuffer na dake testovacie hodnoty
};

#endif

/*
premenne, ktore ziskame cez MPI:
ID procesora
pocet procesorov
rozmery dat
data

*/
