// procesor.cpp
// by skip 2006
//

#include "procesor.h"
#include "funkcie.h"
#include <stdio.h>

extern int err;

// konstruktor
Procesor::Procesor(int ID, int pocprc, const TVektor<int>& a, const TVektor<int>& b):ID(ID), pocprc(pocprc),plan(NULL),strom(NULL), a(a), b(b), obr_x(0), obr_y(0), obr(NULL), sbuffer(NULL), rbuffer(NULL)
{
	//printf("init k-d tree...\n",ID, pocprc);
	int hlbka = (int)logn((float)pocprc,(float)2);
	strom=new KDTree(a,b,hlbka,0);
	vytvorPlan(ID, strom->hlbka, strom->root, CVektor3(0,0,0)-CVektor3(2000,2000,2000), plan);
}


// destruktor
Procesor::~Procesor()
{
	if (strom) delete strom; strom=NULL;
	if (plan) delete[] plan; plan=NULL;
	zrusObr(); // uvolnim obrazok z pamate
}


// funkcie

// vypis komunikacneho planu - procesor+nad/pod
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


// uvolnim obrazok z pamate
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
	
	
// algoritmus binary swap - obrazok musi byt rozmerov 2^n x 2^n !! kvoli shift optimalizaciam -
void Procesor::binarySwap()
{
  /* PART I - BINARY SWAP */
  
	int u=0;             // uroven, podla nej % 2 rozhodujeme ci rozdelime obrazok pozdlz osi x alebo y
	Bod2 sa,sb;          // rozmery posielaneho obrazku
	Bod2 ra,rb;          // rozmery prijateho obrazku
	
	// binary swap
	while (u < strom->hlbka)
	{ 
		//this->vypisObr(); 
		this->setRozmery(u, obr_a, obr_b, sa,sb, ra,rb); // nastavi rozmery sub-obrazkov pre mpi_sendrecv
		this->loadBuffer(sa,sb);       // naplni pole sbuffer		
		
		int sendcount = (ABS(sa.x-sb.x)+1)*(ABS(sa.y-sb.y)+1)*4;
		int recvcount = (ABS(ra.x-rb.x)+1)*(ABS(ra.y-rb.y)+1)*4;
		
    // kvoli zamedzeniu deadlocku (sendrecv inak sa pouziva buffer ako fcie send,recv a je to lepsie riesenie ako cez fcie send a recv)
    // TAGy: posielam data so svojim ID, prijimam data s ID presora		
		MPI_Sendrecv(&sbuffer[0], sendcount, MPI_FLOAT, this->plan[u].pid, /*TAG1*/ this->ID,
		             &rbuffer[0], recvcount, MPI_FLOAT, this->plan[u].pid, /*TAG2*/ this->plan[u].pid, MPI_COMM_WORLD, &status);
			             
		//this->init_recv(ra,rb); 
		
		obr_a=ra; obr_b=rb;            // uz vlastnime iba polovicu skladaneho obrazku
		this->kompozicia(u);           // kompozicia
		

		u++; // dalsia uroven v kompozicii (strome)
	}
	this->vypisObr();
	
	
	/* PART II - Final Image Gathering */
	
  // ake velke obrazky posielaju procesory
  int bufsize = (((obr_x*obr_y)/pocprc) * 4) + 4;
  float* fbuffer = new float[bufsize]; // buffer na prijem dat

	// procesor0 zozbiera obrazky a vytvori z nich finalny obrazok na odoslatie na workstation
	if (ID==0) { 
	  float* finalobr = new float[obr_x*obr_y*4+4]; // buffer posielany na workstation, +4=a.x,a.y,b.x,b.y
    
	  // prijem dat od vsetkych procesor okrem seba
	  for (u=1; u < this->pocprc; u++) {
      // prijem dat
	    MPI_Recv(&fbuffer[0], bufsize, MPI_FLOAT, MPI_ANY_SOURCE /*from*/, MPI_ANY_TAG/*tag*/, MPI_COMM_WORLD, &status);
	    // pozicia prijateho obrazku vo vyslednom - prve 4 floaty
	    Bod2 fa,fb;
	    fa.x=fbuffer[0]; fa.y=fbuffer[1]; fb.x=fbuffer[2]; fb.y=fbuffer[3];
	    // zapiseme prijate data do vysledneho obrazku
      int index=4;
	    for (int j=fa.y; j<=fb.y; j++) {
	      for (int i=fa.x; i<=fb.x; i++) {
	        // zistenie hodnot RGBA
	        obr[i][j].r=fbuffer[index++];
	        obr[i][j].g=fbuffer[index++];
	        obr[i][j].b=fbuffer[index++];
	        obr[i][j].a=fbuffer[index++];
	      }
	    }
	    
	  } // get data from all procs
	  // upraceme
	  delete[] finalobr; finalobr=NULL;
	  printf("MASTER: Vsetky data prijate!\n");
    // kvoli vypisu nastavenie realnych rozmerov obrazka (vsetky casti mame)
   	obr_a.x=0; obr_a.y=0;
  	obr_b.x=obr_x-1; obr_b.y=obr_y-1;
  	this->vypisObr();
	} // if i am zero
	else { // kazdy procesor <> 0 posle svoju cast vysledneho obrazu
	  // naplnime buffer
	  // prve 4 floaty je X,Y pozicia vo vyslednom obrazku
	  fbuffer[0]=obr_a.x; fbuffer[1]=obr_a.y;
	  fbuffer[2]=obr_b.x; fbuffer[3]=obr_b.y;	  
    // zvysok RGBA
    int index=4;
    for (int j=obr_a.y; j<=obr_b.y; j++)
      for (int i=obr_a.x; i<=obr_b.x; i++) {
        fbuffer[index++]=obr[i][j].r;
        fbuffer[index++]=obr[i][j].g;
        fbuffer[index++]=obr[i][j].b;
        fbuffer[index++]=obr[i][j].a;
    }    

	  //MPI_Send(&sbuffer[0], sendcount, MPI_FLOAT, this->plan[u].pid, /*TAG1*/ this->ID,
	  MPI_Send(&fbuffer[0], bufsize, MPI_FLOAT, /*TO*/ 0, /*TAG*/ this->ID, MPI_COMM_WORLD);
	}
	// upraceme
	delete[] fbuffer; fbuffer=NULL;
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
      obr[x][y]=CFarba(ID+1.0, ID+1.0, ID+1.0, 0.5);
        
      //if (y>1) obr[x][y]=CFarba(0.5f,0.0f,0.0f,0.5f); else obr[x][y]=CFarba();
      //obr[x][y]=CFarba(x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1), x+y*(ABS(obr_a.x-obr_b.x)+1));
    }
  }
}
	
	
// skombinuje polovicu obrazka a rbuffer	
void Procesor::kompozicia(const int u) // uroven u, pre rozhodnutie ako kombinovat farby
{
  //printf("kompozicia..."); if (plan[u].over) printf("nad\n"); else printf("pod\n");
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
      printf("[%d,%d]=%.2f,%.2f,%.2f,%.2f  ", x,y,obr[x][y].r,obr[x][y].g,obr[x][y].b,obr[x][y].a);
    }
    printf("\n");
  }  
}	
	

void Procesor::init_recv(const Bod2& ra, const Bod2& rb)
{
  printf("generujem data na prijatie...\n");
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
	
