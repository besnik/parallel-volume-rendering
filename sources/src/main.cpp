// main.cpp
// by skip 
// jul 2006
// CLUSTER REQUIRED !! :-)
// ver 1.0

#include "../include/procesor.h"
#include "../include/init.h"
#include "../include/timer.h"
#include <string>
#include <iostream>

using namespace std;

/* 
   Tieto data ziskame z klienta a MPI
   Jedine data z klienta bude subor, ktory sa ma renderovat
   Pocet procesorov zapojenych do vypoctu urcuje nastavenie servera

					*/

/* Spolocne premenne pre procesory */

int err;          // MPI error
Procesor* p=NULL; // udaje o procesore

/* Docasne premenne */

string s = "../data/data.lsm"; // ziskame z klienta


int main(int argc, char* argv[])
{
  /* Inicializacia */
  err=MPI_Init(&argc, &argv);       

  if (err!=MPI_SUCCESS)
  {
    cout << "Chyba pocas inicializacie MPI. Koncim." << endl;
    MPI_Abort(MPI_COMM_WORLD, err);
  }

  
  p = new Procesor(); //zistenie ID a pocprc
  p->initScreen(512,512, 64,64);    // nastavenie rozmerov obrazovky a pozicia editora pokrytia
  p->init_raycaster(); // inicializacia CRaycastera
  if (p->ID==0) init_master(s,p->a,p->b, p->rc.data.lsmfile); else init_node(p->a,p->b); // nastavenie rozmerov dat
  p->init_rend(); // vytvori k-D strom a plan kompozicie
  p->initObr(p->cam_plane_x, p->cam_plane_y);
  p->init_data(); // alokuje pamat pre data
  p->init_opti(); // inicializuje vsetky pomocne datove struktury CRaycastera na datach


  
  double t0,t1;
  float fps, avg_fps=0.0f, total_fps=0.0f, max_fps=-1;
  const int poc_testov = 20;

  if (p->ID==0) { cout << "TESTING IN PROGRESS..." << endl; cout.flush(); }
  
  
  
  for (int test=0; test<poc_testov; test++)
  {
    MPI_Barrier(MPI_COMM_WORLD);  // MUSI TU BYT, iNAK PROBLEMY S MPI_RECV ked to fici v cykle
    t0=timer();
    
    p->renderuj();   // CRaycaster renderuj
    p->binarySwap(); // CProcesor  binary swap
//    p->uloz_na_disk();  // Vysledok na HDD
    
    t1=timer();
    fps=1000.0f/(float)(t1-t0);
    if (fps>max_fps) max_fps=fps;
    total_fps+=fps;
    
  }
    
  avg_fps=total_fps/(float)poc_testov;
  if (p->ID==0) cout << "ID=" << p->ID << " AVG FPS: " << avg_fps << ", MAX FPS: " << max_fps << endl;
  
  // free
  if (p) { delete p; p=NULL; }

  /* Zatvorime MPI */
  err=MPI_Finalize();
  return EXIT_SUCCESS;
}
