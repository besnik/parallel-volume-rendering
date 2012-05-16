// main.cpp
// by skip 2006
// 

#include "procesor.h"
#include <stdio.h>

Procesor* p=NULL;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	
	//const int ID=0;
	//const int pocprc=4;
	
	int ID; MPI_Comm_rank( MPI_COMM_WORLD, &ID );
	int pocprc; MPI_Comm_size(MPI_COMM_WORLD,&pocprc);
	

	// rozmery dat
	TVektor<int> a, b;
	a.x=0; a.y=0; a.z=0;
	b.x=15; b.y=31; b.z=7;	
	
        // vytvori kD strom a plan kompozicie
	p=new Procesor(ID,pocprc, a,b);
	// alokuje data pre obrazok a vytvori testovaci obrazok (ID+1)
	p->initObr(256,256);
	// vypise plan kompozicie
	//p->vypisPlan();
	p->binarySwap();
	
	delete p; p=NULL;
	
	MPI_Finalize();
	return 0;
}
