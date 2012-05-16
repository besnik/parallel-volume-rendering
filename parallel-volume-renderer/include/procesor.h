// procesor.h
// by skip 2006
// ver 1.1
// 27.06.2006
//
// Controller on a processor. 
// Master loads data, calculates kD tree and sends portions of data to the nodes in the farm. 
// Manages initial setup of nodes. Triggers the rendering.
// Saves data to the disc.
// Binary swap algorith + Image gathering and merging


#ifndef __PROCESOR_H_
#define __PROCESOR_H_

#include "vektor3.h"
#include "binswap.h"
#include "kdtree.h"
#include "farba.h"
#include "raycaster.h"

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
	TVektor<int> a,b; // data ohranicene bodmi A,B, ohranicenia pre vsetky proc su aj tak v k-D strome
	
	CRaycaster rc; // raycaster
	
	CFarba** obr; // vyrenderovany obrazok
	int obr_x, obr_y; // rozmery obrazku
	Bod2 obr_a,obr_b; // ohranicenie obrazka ktory kombinujeme
	
	float *sbuffer; // send
	float *rbuffer; // receive

	
	// obrazovka 
	int obr_sirka;
  int obr_vyska;

// rozmery camera plane vo world coord., tiez rozmery vyslednej textury
  UINT16 cam_plane_x;
  UINT16 cam_plane_y;

// opacity editor - rozmery a pozicia
  int edt_sir;
  int edt_vys;

  int edt_left;
  int edt_top;

	Procesor():plan(NULL),strom(NULL), obr_x(0), obr_y(0), obr(NULL), sbuffer(NULL), rbuffer(NULL)
        {
		/* Pocet procesorov a vlastne ID */
		MPI_Comm_rank(MPI_COMM_WORLD, &this->ID);
		MPI_Comm_size(MPI_COMM_WORLD, &this->pocprc);
        };
        
	// destruktor
	~Procesor();
	
	// funkcie
	void initScreen(int obr_sirka, int obr_vyska, UINT16 cam_plane_x, UINT16 cam_plane_y);
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
	void init_rend(); // inicializacia k-d strom a planu kompozicie
	int data_size(); // vrati velkost sub-dat a ich poziciu v celych datach
	void init_data(); // alokuje pamat pre data podla data_size()
	void init_raycaster(); // inicializacia CRaycaster
	void setOpacEdt(); // opac edt default settings
	void master_load_data(TVektor<int> a, TVektor<int> b); // nacita prislusne sub-data
	void init_opti(); // init optimalizacie CRaycaster na datach
	void renderuj(); // CRaycaster renderuj
	void resetOhranicenia(); // musi byt po kazdom vola binarySwap(), algoritmus modifikuje obr_a, obr_b
	void uloz_na_disk(); // ulozi vysledny obrazok na disk, resp bude sa posielat cez UDP/IP na klienta
};

#endif

/*
premenne, ktore ziskame cez MPI:
ID procesora
pocet procesorov
rozmery dat
data

*/
