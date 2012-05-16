// procesor.cpp
// by skip 2006
//

#include "procesor.h"
#include "funkcie.h"
#include <stdio.h>


// constructor
Procesor::Procesor(int ID, int pocprc, const TVektor<int>& a, const TVektor<int>& b):ID(ID), pocprc(pocprc),plan(NULL),strom(NULL), a(a), b(b), obr_x(0), obr_y(0), obr(NULL), sbuffer(NULL), rbuffer(NULL)
{
	printf("init k-d tree...\n",ID, pocprc);
	int hlbka = (int)logn((float)pocprc,(float)2);
	strom=new KDTree(a,b,hlbka,0);
	vytvorPlan(ID, strom->hlbka, strom->root, CVektor3(0,0,0)-CVektor3(2000,2000,2000), plan);
}


// destructor
Procesor::~Procesor()
{
	if (strom) delete strom; strom=NULL;
	if (plan) delete[] plan; plan=NULL;
	zrusObr(); // free image from memory
}


// methods

// print communication plan- processor+up/down
void Procesor::vypisPlan()
{
	for (int i=0; i<strom->hlbka; i++) { 
		printf("komunikacia s procesorom c.%d. ",plan[i].pid);
		if (plan[i].over) printf("Nad.\n"); else printf("Pod.\n");
	}
}


// inicializujem rozmery obrazka a alokujem polia
void Procesor::initObr(int obr_x, int obr_y)
{
	this->obr_x=obr_x; this->obr_y=obr_y; // nastavim rozmery
	// alokovanie 2D pola v pamati
	obr=new CFarba*[obr_x];
	for (int x=0; x<obr_x; x++) obr[x]=new CFarba[obr_y]();
	// nastavenie ohranicenia obrazu
	obr_a.x=0; obr_a.y=0;
	obr_b.x=obr_x-1; obr_b.y=obr_y-1;
	// alokovanie pamate pre buffre (s&r)
	int size = (obr_x / 2)*(obr_y)*4; // v najhorsom (prvy sendrecv) pripade sa posiela vertikalna polovica obrazku, 4 - RGBA
	sbuffer=new float[size];
	rbuffer=new float[size];
	
	this->init_obr(); // vytvori testovaci obrazok
}


// free image from memory
void Procesor::zrusObr()
{
	if (obr) {
		for (int x=0; x<obr_x; x++) if (obr[x]) {
			delete obr[x]; obr[x]=NULL;
		}
		delete[] obr; obr=NULL;
	}
	if (sbuffer) delete[] sbuffer; sbuffer=NULL;
	if (rbuffer) delete[] rbuffer; rbuffer=NULL;
}
	
	
// algorithm binary swap - image must be of size 2^n x 2^n !! because of shift optimalizations !!
void Procesor::binarySwap()
{
	int u=0;             // uroven, podla nej % 2 rozhodujeme ci rozdelime obrazok pozdlz osi x alebo y
	Bod2 sa,sb;          // rozmery posielaneho obrazku
	Bod2 ra,rb;          // rozmery prijateho obrazku
	
	while (u < strom->hlbka)
	{ 
		this->vypisObr();
		this->setRozmery(u, obr_a, obr_b, sa,sb, ra,rb); // nastavi rozmery sub-obrazkov pre mpi_sendrecv
		this->loadBuffer(sa,sb);       // naplni pole sbuffer		
		
		/* MPI_Sendrecv(...) */        // poslanie a prijem udajov
		this->init_recv(ra,rb);
		
		obr_a=ra; obr_b=rb;            // uz vlastnime iba polovicu skladaneho obrazku
		this->kompozicia(u);           // kompozicia
		

		u++; // dalsia uroven v kompozicii (strome)
	}
	this->vypisObr();
}



void Procesor::vypisAB()
{
	printf("Ohranicenie: A[%d,%d], B[%d,%d]\n",obr_a.x,obr_a.y,obr_b.x,obr_b.y);
}
	
	
void Procesor::setRozmery(const int u, const Bod2& a, const Bod2& b, Bod2& sa, Bod2& sb, Bod2& ra, Bod2& rb) // do sbuffra nacita prislusnu cast obrazku, ktora sa potom posle druhemu procesoru	
{
  ra=sa=a; rb=sb=b;
  // ak je parna uroven tak delime pozdlz Y-osi, ak neparna tak pozdlz X-osi
  if ((u % 2)==0) {
    // ak nasa cast obrazku bude *nad*, tak si nechame *pravu* cast obrazu
    if (plan[u].over) { 
      ra.x=(b.x+1) >> 1; // (b+1)/2  // b je neparne, indexujeme o 0
      sb.x=((b.x+1) >> 1)-1;
    } else { 
      sa.x=(b.x+1) >> 1; 
      rb.x=((b.x+1) >> 1)-1;
    }  // (obr_a+1)/2 - 1
  } else {  // neparna uroven, delime pozdlz X-ovej osi
    if (plan[u].over) {
      rb.y=((b.y+1) >> 1)-1;
      sa.y=(b.y+1) >> 1;
    } else {
      sb.y=((b.y+1) >> 1)-1;
      ra.y=(b.y+1) >> 1;
    } 
  }
}
	
	
void Procesor::loadBuffer(const Bod2& sa, const Bod2& sb)
{
  int index=0;
  for (int j=sa.y; j<=sb.y; j++)
    for (int i=sa.x; i<=sb.x; i++) {
      sbuffer[index++]=obr[i][j].r;
      sbuffer[index++]=obr[i][j].g;
      sbuffer[index++]=obr[i][j].b;
      sbuffer[index++]=obr[i][j].a;
  }
}


void Procesor::init_obr()
{
  for (int y=obr_a.y; y<=obr_b.y; y++)	{
    for (int x=obr_a.x; x<=obr_b.x; x++) {
      if (y>1) obr[x][y]=CFarba(0.5f,0.0f,0.0f,0.5f); else obr[x][y]=CFarba();
      //obr[x][y]=CFarba(x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1));
    }
  }
}
	
	
// skombinuje polovicu obrazka a rbuffer	
void Procesor::kompozicia(const int u) // uroven u, pre rozhodnutie ako kombinovat farby
{
  printf("kompozicia..."); if (plan[u].over) printf("nad\n"); else printf("pod\n");
  int index = 0; // index do pola rbuffer
  
  for (int y=obr_a.y; y<=obr_b.y; y++)	{
    for (int x=obr_a.x; x<=obr_b.x; x++) {  
      if (plan[u].over) { // nase data su front
	
	obr[x][y].r=obr[x][y].r + rbuffer[index+0]*rbuffer[index+3]*(1.0f-obr[x][y].a); // Red     acc = acc + color*alpha(1-acc_alpha)
	obr[x][y].g=obr[x][y].g + rbuffer[index+1]*rbuffer[index+3]*(1.0f-obr[x][y].a); // Green
	obr[x][y].b=obr[x][y].b + rbuffer[index+2]*rbuffer[index+3]*(1.0f-obr[x][y].a); // Blue
	obr[x][y].a=obr[x][y].a + rbuffer[index+3]*(1.0f-obr[x][y].a);			// Alpha
	
      } else { // prijate data su 'nad'

        obr[x][y].r=rbuffer[index+0] + obr[x][y].r*obr[x][y].a*(1.0f-rbuffer[index+3]); // Red 
	obr[x][y].g=rbuffer[index+1] + obr[x][y].g*obr[x][y].a*(1.0f-rbuffer[index+3]); // Green
	obr[x][y].b=rbuffer[index+2] + obr[x][y].b*obr[x][y].a*(1.0f-rbuffer[index+3]); // Blue 
        obr[x][y].a=rbuffer[index+3] + obr[x][y].a*(1.0f-rbuffer[index+3]);		// Alpha    
	
      }

      index+=4;
    }
  }	
}
	
	
void Procesor::vypisObr()
{
  for (int y=obr_a.y; y<=obr_b.y; y++)	{
    for (int x=obr_a.x; x<=obr_b.x; x++) {
      printf("[%d,%d]= %.2f, %.1f, %.1f, %.2f  ", x,y,obr[x][y].r,obr[x][y].g,obr[x][y].b,obr[x][y].a);
    }
    printf("\n");
  }  
}	
	

void Procesor::init_recv(const Bod2& ra, const Bod2& rb)
{
  printf("generating data to accept...\n");
  int index=0;
  for (int j=ra.y; j<=rb.y; j++) {
    for (int i=ra.x; i<=rb.x; i++) {  
      if (j<3) {
        rbuffer[index+0]=0.7f;
	rbuffer[index+3]=0.5f;
      } else {
        rbuffer[index+0]=0.0f;
	rbuffer[index+3]=0.0f;
      }
      rbuffer[index+1]=0.0f;
      rbuffer[index+2]=0.0f;
      printf("%.2f %.2f   ",rbuffer[index+0], rbuffer[index+3]);
      index+=4;
    }
    printf("\n");
  }
}
	
