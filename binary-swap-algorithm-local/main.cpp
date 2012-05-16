// main.cpp
// by skip 2006
// 

#include "procesor.h"
#include <stdio.h>

Procesor* p=NULL;

int main(int argc, char* argv[])
{
	const int ID=0;
	const int pocprc=2;
	
	TVektor<int> a, b;
	a.x=0; a.y=0; a.z=0;
	b.x=15; b.y=31; b.z=7;	
	
	p=new Procesor(ID,pocprc, a,b);
	p->initObr(4,4);
	p->vypisPlan();
	p->binarySwap();

	traverzuj(p->strom->root);
	
	delete p; p=NULL;
	return 0;
}
