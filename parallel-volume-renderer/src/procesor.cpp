// procesor.cpp - algoritmus binary swap
// by skip 2006
//

#include "../include/procesor.h"
#include "../include/funkcie.h"
#include "../include/init.h"
#include <stdio.h>

extern int err;


// destruktor
Procesor::~Procesor()
{
	if (strom) delete strom; strom=NULL;
	if (plan) delete[] plan; plan=NULL;
	zrusObr(); // uvolnim obrazok z pamate
}


// funkcie

/* Tieto data by sme mali dostat cez UDP/IP protokol */
void Procesor::initScreen(int obr_sirka, int obr_vyska, UINT16 cam_plane_x, UINT16 cam_plane_y)
{
  this->obr_sirka=obr_sirka;
  this->obr_vyska=obr_vyska;
  
  this->cam_plane_x=cam_plane_x;
  this->cam_plane_y=cam_plane_y;

// opacity editor - rozmery a pozicia
  this->edt_sir = 200;
  this->edt_vys = 100;

  this->edt_left = obr_sirka-210;
  this->edt_top  = obr_vyska-110;  
}

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
	
	//this->init_obr(); // vytvori testovaci obrazok
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
	
	
void Procesor::resetOhranicenia() // MUSI BYT pred kazdym volanim BS, pretoze ak volame viac krat po sebebinarySwap() tak sa obr_a,obr_b zmenia
{
        obr_a.x=0; obr_a.y=0;
        obr_b.x=obr_x-1; obr_b.y=obr_y-1;
}
	
	
	
// algoritmus binary swap - obrazok musi byt rozmerov 2^n x 2^n !! kvoli shift optimalizaciam -
void Procesor::binarySwap()
{
  /* VYTVOR PLAN KOMPOZICIE */
 
  vytvorPlan(ID, strom->hlbka, strom->root, CVektor3(0,0,0)-rc.kamera.from, plan);  


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
		
		//MPI_Barrier(MPI_COMM_WORLD);  // MUSI TU BYT, iNAK PROBLEMY S MPI_RECV ked to fici v cykle
		MPI_Sendrecv(&sbuffer[0], sendcount, MPI_FLOAT, this->plan[u].pid, /*TAG1*/ this->ID,
		             &rbuffer[0], recvcount, MPI_FLOAT, this->plan[u].pid, /*TAG2*/ this->plan[u].pid, MPI_COMM_WORLD, &status);
			             
		//this->init_recv(ra,rb); 
		
		obr_a=ra; obr_b=rb;            // uz vlastnime iba polovicu skladaneho obrazku

		this->kompozicia(u);           // kompozicia
		

		u++; // dalsia uroven v kompozicii (strome)
	}
	
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
	   
	    MPI_Recv(&fbuffer[0], bufsize, MPI_FLOAT, MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	    
	    // pozicia prijateho obrazku vo vyslednom - prve 4 floaty
	    Bod2 fa,fb;
	    fa.x=(int)fbuffer[0]; fa.y=(int)fbuffer[1]; fb.x=(int)fbuffer[2]; fb.y=(int)fbuffer[3];
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

    // kvoli vypisu nastavenie realnych rozmerov obrazka (vsetky casti mame)
   	obr_a.x=0; obr_a.y=0;
  	obr_b.x=obr_x-1; obr_b.y=obr_y-1;

	} // if i am zero
	else  { // kazdy procesor <> 0 posle svoju cast vysledneho obrazu
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
    
	  //MPI_Send(&sbuffer[0], sendcount, MPI_FLOAT, this->plan[u].pid, this->ID,
	  MPI_Send(&fbuffer[0], bufsize, MPI_FLOAT,  0, this->ID, MPI_COMM_WORLD);
	}
	// upraceme
	if (fbuffer) { delete[] fbuffer; fbuffer=NULL; }

	this->resetOhranicenia(); 
}



void Procesor::setRozmery(const int u, const Bod2& a, const Bod2& b, Bod2& sa, Bod2& sb, Bod2& ra, Bod2& rb) // do sbuffra nacita prislusnu cast obrazku, ktora sa potom posle druhemu procesoru	
{
  ra=sa=a; rb=sb=b;
  // ak je parna uroven tak delime pozdlz Y-osi, ak neparna tak pozdlz X-osi
  if ((u % 2)==0) {
    // ak nasa cast obrazku bude *nad*, tak si nechame *lavu* cast obrazu
    if (plan[u].over) { 
      ra.x=a.x + ((b.x-a.x+1)/2); // b je neparne, indexujeme o 0
      sb.x=a.x + ((b.x-a.x+1)/2) - 1;
    } else { 
      sa.x=a.x + ((b.x-a.x+1)/2); 
      rb.x=a.x + ((b.x-a.x+1)/2) - 1;
    }  
  } else {  // neparna uroven, delime pozdlz X-ovej osi
    if (plan[u].over) {
      rb.y=a.y + ((b.y-a.y+1)/2) - 1;
      sa.y=a.y + ((b.y-a.y+1)/2);
    } else {
      sb.y=a.y + ((b.y-a.y+1)/2) - 1;
      ra.y=a.y + ((b.y-a.y+1)/2);
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


/* vytvori testovaci obrazok */
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
	

/* Inicializacia renderovania */

void Procesor::init_rend() // inicializacia k-d strom a planu kompozicie
{
  /* SPOLOCNE OPERACIE */
  if (ID==0) {
    cout << "TASK " << this->ID << ": Inicializacia k-D stromu a vytvorenie kompozicneho planu...";
    cout.flush();
  }
  int hlbka = (int)logn((float)pocprc,(float)2);
  strom=new KDTree(a,b,hlbka,0);
  
  // alokovanie pamate pre plan kompozicie
  if (hlbka==0) {
	  printf("one processor. no communication.\n");
  } else {
	  this->plan = new Plan[hlbka];
	}
  
  if (ID==0) cout << "ok" << endl;
  cout.flush();
}


/* Vrati velkost sub-dat a poziciu sub-dat v priestore
   Funkcia vrati spravnu velkost (strany su mocniny 2), pretoze loadlsm.cpp uz upravuje rozmery */
int Procesor::data_size()
{
	if (!this->strom->root) return -1; // ak neinicializovany strom tak kill
	Node* t = this->strom->root;       // zaciname traverzovat z roota

  while (t->back!=NULL && t->front!=NULL) {
    if (inInterv(ID, t->front->i1, t->front->i2)) t=t->front; else t=t->back;
  }
  
	// nasli sme prislusny list k-D stromu
	a=t->a; b=t->b;	

	return (b.x - a.x + 1)*(b.y - a.y + 1)*(b.z - a.z + 1);
}



void Procesor::setOpacEdt() // opac edt default settings
{
	this->rc.kriv.pocet=2;
	this->rc.kriv.suradnice[0].x=edt_left+39;
	this->rc.kriv.suradnice[0].y=edt_top+edt_vys-1;
	this->rc.kriv.suradnice[1].x=edt_left+edt_sir-1;
	this->rc.kriv.suradnice[1].y=edt_top+70;
}



/* volame konstruktor CRaycaster */
void Procesor::init_raycaster() 
{
  if (ID==0) { cout << "MASTER: Init raycaster..."; cout.flush(); }
  
  // vytvorenie celej sceny a rendera
  this->rc = CRaycaster(cam_plane_x, cam_plane_y);
  // alokovanie pola pre suradnice krivky
	this->rc.initBKriv();
	// nastavenie rozmerov editora
  this->rc.initOpacEdt(edt_sir,edt_vys,edt_left, edt_top);
	// vytvorenie defalut krivky
	this->setOpacEdt();
	// vytvorenie lookup table
	this->rc.vytvorEdtLut();
	// alokovanie pola pre texturu
	this->rc.initTexture();
  if (ID==0) { cout << "ok" << endl; cout.flush(); }
}



void Procesor::init_data()
{
  if (ID==0) { cout << "MASTER: Init data" << endl; cout.flush(); }
  if (!this->strom->root) return; // ak neinicializovany strom tak kill

  int ds = data_size(); // velkost sub-dat, spravne nastavenie a,b
  this->rc.data.data = new UINT16[ds]; // alokovanie pamate
  
  
  // master bude postupne nacitavat subdata a posielat ich nodom
   /* MASTER */
  if (ID==0) {
    TVektor<int> node_a, node_b; // rozmery dat pre aktualne sub-data
    for (int i=1; i<this->pocprc; i++) {
     	
      Node* t = this->strom->root;       // zaciname traverzovat z roota
      while (t->back!=NULL && t->front!=NULL) {
        if (inInterv(i, t->front->i1, t->front->i2)) t=t->front; else t=t->back;
      }
  
      // nasli sme prislusny list k-D stromu a rozmery sub-dat
      node_a=t->a; node_b=t->b;	
      
      master_load_data(node_a,node_b);
      posli_data(rc.data.data, ds, i, i);
      cout << "MASTER: data uspesne poslane na node" << endl;
//      for (int i=0; i<3; i++) cout << "M" << " " << rc.data.data[i] << endl;      
    }
    // master teraz nacita svoje data
    Node* t = this->strom->root;       // zaciname traverzovat z roota
    while (t->back!=NULL && t->front!=NULL) {
      if (inInterv(0, t->front->i1, t->front->i2)) t=t->front; else t=t->back;
    }
    node_a=t->a; node_b=t->b;	
    master_load_data(node_a,node_b);
    cout << "MASTER: vlastne data nacitane a nastavene" << endl; cout.flush();
  }
  
  /* NODE */
  else 
  {
    int prijate;
    prijmi_data(rc.data.data, ds, 0, this->ID, prijate);
    //cout << "NODE: data uspesne prijate z mastera a nastavene" << endl; cout.flush();
//    for (int i=0; i<3; i++) cout << ID << " " << rc.data.data[i] << endl;
  }
  

  /* SPOLOCNE NASTAVENIA DAT */
   
  // nastavenie rozmerov
  rc.data.x = b.x-a.x+1; rc.data.y = b.y-a.y+1; rc.data.z = b.z-a.z+1;
  // rozmery povodnych dat
  TVektor<int> r;
 	r.x=strom->root->b.x - strom->root->a.x + 1; 
 	r.y=strom->root->b.y - strom->root->a.y + 1;
 	r.z=strom->root->b.z - strom->root->a.z + 1;
 	// posun
 	rc.data.posunx=r.x/-2.0f + a.x;  rc.data.posuny=r.y/-2.0f + a.y;	rc.data.posunz=r.z/-2.0f + a.z;
  // nastavenie rozmerov medzi voxelmi mriezky
	rc.data.dx = 1; 	rc.data.dy = 1; 	rc.data.dz = 1;
  	// nastavime obal
	rc.data.nastavObal();
}



void Procesor::master_load_data(TVektor<int> a, TVektor<int> b)
{
  cout << "MASTER Loading sub-data..."; cout.flush();
  
  // ak nebol otvoreny lsm subor tak kill
  if (!this->rc.data.lsmfile) {
    MPI_Abort(MPI_COMM_WORLD, err);
    exit(1);
  }
  
  // nacitanie sub-dat
	CImageDirectory *adir=NULL; // pointer na aktivny objekt (image directory) spajaneho zoznamu 
	adir=this->rc.data.lsmfile->head; // zaciatok spajaneho zoznamu
	int pocdir = 0; // kolko imagedir sme uz presli
	int index = 0; // index do pola dat

  while (adir) {
		adir->LoadPixelData(this->rc.data.lsmfile->f);    // nacitame rez

		if ((pocdir%2)==1 && pocdir>=a.z) {  // spajany zoznam obsahuje rezy odzadu - prvy ide teraz nahlad

			  // prechadzame rez a nacitavame prislusny vysek
				for (int y=a.y; y<=b.y; y++)
					for (int x=a.x; x<=b.x; x++) {
							this->rc.data.data[index++] = adir->chead->pixeldata16[x][adir->chead->vyska-1-y];

					}
		}
		adir->DeletePixelData();						// uvolnime rez z memory
		
		pocdir++;
		if (pocdir>b.z) break; // ak sme uz presli vsetky potrebne rezy tak leftujeme
		adir=adir->dalsi;	
	}
	adir=NULL;  
   
  cout << "ok" << endl;
}


/* inicializuje optimalizacie CRaycastera na datach */
void Procesor::init_opti()
{
  if (ID==0) { cout << "MASTER: Init pomocne datove struktury"; cout.flush(); }
  rc.initCamRot(this->strom->root->b.x - this->strom->root->a.x + 1,
                this->strom->root->b.y - this->strom->root->a.y + 1,
                this->strom->root->b.z - this->strom->root->a.z + 1);       // prisposobime vzdialenost kamery datam
  if (ID==0) { cout << "."; cout.flush(); }
  rc.data.initDVolume(); // vypocet distance volume
  if (ID==0) { cout << "."; cout.flush(); }
  rc.data.initBrick();   // vytvorenie inak adresovanych dat
  if (ID==0) { cout << "."; cout.flush(); }
  rc.data.initHomog();   // vytvorenie homog lookup dat
  if (ID==0) { cout << "."; cout.flush(); }
  if (!rc.overNastavenia())
  {
    cout << "Overenie nastaveni renderera zlyhalo!" << endl;
    MPI_Abort(MPI_COMM_WORLD, err);
  }  
  MPI_Barrier(MPI_COMM_WORLD);
  if (ID==0) { cout << "done" << endl; cout.flush(); }
  
  rc.kamera.zoom+=0.0f;
}



void Procesor::renderuj()
{
  // renderovanie
  rc.renderuj();
  
/*  
  // rend0.out
  string subor;
  subor = "rend";
  char c[4];
  sprintf(c,"%d", ID);
  subor = subor + c + ".out"; 
  
  FILE* vystup;
  vystup = fopen(subor.c_str(), "w");
  UINT8 buf[4];
*/    

  // vysledny obrazok pripravime na binary swap

// !!!! tieto casti treba odkomentovat pre sfunkcnenie kompletneho algoritmu

  int index=0;
  for (register int y=0; y<obr_y; y++)
  for (register int x=0; x<obr_x; x++) {
    obr[x][y].r = float(rc.textura.data[index+0])/255.0f;
    obr[x][y].g = float(rc.textura.data[index+1])/255.0f;
    obr[x][y].b = float(rc.textura.data[index+2])/255.0f;
    obr[x][y].a = float(rc.textura.data[index+3])/255.0f;
    


/*    buf[0] = rc.textura.data[index+0];
    buf[1] = rc.textura.data[index+1];
    buf[2] = rc.textura.data[index+2];
    buf[3] = rc.textura.data[index+3]; */
    
   index+=4;

//    fwrite(buf, sizeof(UINT8), 4, vystup);
 }
  
//  fclose(vystup);
  
}



void Procesor::uloz_na_disk()
{
  if (ID==0)
  {
    UINT8 buf[4];
    FILE* f;
    f = fopen("final.out","w");

    for (register int y=0; y<obr_y; y++)
    for (register int x=0; x<obr_x; x++) {

      buf[0] = UINT8(obr[x][y].r*255.0f);
      buf[1] = UINT8(obr[x][y].g*255.0f);
      buf[2] = UINT8(obr[x][y].b*255.0f);
      buf[3] = UINT8(obr[x][y].a*255.0f);

      fwrite(buf, sizeof(UINT8), 4, f);
    }    

    fclose(f);
    cout << "MASTER: data ulozene na lokalnom disku" << endl;
  }
}


