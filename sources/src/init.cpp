// init.cpp
// by skip

#include "../include/init.h"
#include "../include/loadlsm.h"

extern int err;

bool vrat_velkost(const string& s, TVektor<int>& a, TVektor<int>& b, CLSMFile*& lsmfile)
{
  /*
      Zisti rozmery dat, posiela broadcast o inicializovani renderovania
      Vstup: cesta k suboru
      Vystup: rozmery dat (pociatocny a koncovy bod v priestore)
              pointer na alokovane informacie o datach (aby necitali subor vela krat)
   									               */
  cout << "MASTER: Skumam LSM data...";
  cout.flush();
  
  int erro=0;
  lsmfile=new CLSMFile((char*)s.c_str(),&erro);

  if ((erro==0) && (lsmfile!=NULL))
  {
    // ak je vsetko v poriadku tak nastavime rozmery dat
    a.x=0; a.y=0; a.z=0;
    b.x=lsmfile->sirka-1; b.y=lsmfile->vyska-1; b.z=lsmfile->hlbka-1;
  }
  else {
    // nastala chyba pri nacitavani LSM dat
    switch(erro)
    {
	  case 1:cout << "CHYBA (subor neexistuje)" << endl; cout.flush();
		break;
	  default:cout << "CHYBA (cislo " << erro << ")" <<  endl; cout.flush();
	        break;
    }
    return 0;
  }
  // vsetko v poriadku
  cout << "ok" << endl; cout.flush();
  return 1;
}


void init_master(const string& s, TVektor<int>& a, TVektor<int>& b, CLSMFile*& lsmfile) // inicializacia rozmerov dat na nody; run at master ONLY!
{
    /* konstruktor lsmfile sa vola vo funkcii vrat_velkost() */
    if (lsmfile) { delete lsmfile; lsmfile=NULL; } // pre istotu
    
    // ak je master inicializovany tak posle broadcast s rozmermi dat   
    if (vrat_velkost(s, a,b, lsmfile)) {

      // transfer rozmerov dat medzi ostatne procesory
      int r[3]; r[0]=b.x; r[1]=b.y; r[2]=b.z;  // iba 3 lebo a=0,0,0
      MPI_Bcast(r,3,MPI_INT,0,MPI_COMM_WORLD);
      cout << "MASTER: broadcast vykonany [" << r[0] << "," << r[1] << "," << r[2] << "]" << endl;
    } 

    // inicializacia sa nepodarila, terminating
    else MPI_Abort(MPI_COMM_WORLD, 99);
}


void init_node(TVektor<int>& a, TVektor<int>& b) // inicializacia rozmerov dat z mastera
{
  int r[3];
  MPI_Bcast(r,3,MPI_INT,0,MPI_COMM_WORLD);
  
  // nastavenie ohranicenia dat
  a.x=0; a.y=0; a.z=0;
  b.x=r[0]; b.y=r[1]; b.z=r[2];
}


void posli_data(UINT16* buf, int pocet, int kam, int tag)
{
  const int buf_size = 1024;    // velkost buffra pre posli/prijmi
  UINT16 send_buffer[buf_size]; // buffer
 
  int n;                     // pocet interacii cyklu
  if (pocet % buf_size == 0) n = pocet / buf_size; else n = (pocet / buf_size) + 1;
  int kolko;                 // kolko dat skutocne poslem (kvoli mod 2 <> 0) 
  
  // posielam data cez buffer
  for (int i = 0; i<n; i++) {
    kolko = buf_size;          // defaulte posielame buf_size, ak ju nezmenime neskor
    for (int index = 0; index < buf_size; index++) {
      if (i*buf_size+index+1>pocet) { kolko = index; break; } // ak sme presli vsetky data tak stopneme
      send_buffer[index]=buf[i*buf_size+index];
    }
    // buffer ready, mozeme posielat
    MPI_Send(send_buffer, kolko, MPI_UNSIGNED_SHORT, kam, tag, MPI_COMM_WORLD); 
  }
}


// prijem velkeho bloku dat
void prijmi_data(UINT16* buf, int pocet, int odkial, int tag, int& prijate) // posledny parameter ako MPI_Status
{
  const int buf_size = 1024;    // velkost buffra pre posli/prijmi
  UINT16 send_buffer[buf_size]; // buffer
  
  int n;                     // pocet interacii cyklu
  if (pocet % buf_size == 0) n = pocet / buf_size; else n = (pocet / buf_size) + 1;
  int recv;    // pocet prijatych dat po MPI_Recv
  MPI_Status status;
  prijate=0;  

  for (int i = 0; i<n; i++) {
    // prijem dat
    MPI_Recv(send_buffer, buf_size, MPI_UNSIGNED_SHORT, odkial, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_UNSIGNED_SHORT, &recv);
    // zapis do velkeho pola z buffra
    prijate+=recv;
    for (int index = 0; index < recv; index++) {
      buf[i*buf_size+index]=send_buffer[index];
    }
  }
}





