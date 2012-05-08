// binswap.cpp
// by skip 2006
//

#include "../include/kdtree.h"
#include "../include/vektor3.h"
#include "../include/definicie.h"
#include "../include/binswap.h"

#include <stdio.h>
#include <math.h>



bool inInterv(int co, int a, int b)			// je cislo v intervale <a,b> ?
{
	//if ((a<=co) && (co<=b)) return true; else return false;
	return ((a<=co) && (co<=b)) ? true : false;
}



void vytvorPlan(int ID, int hlbka, Node* n, const CVektor3& v, Plan* out)	// pre kazdy procesor urci vymenny plan
{	
if (hlbka==0) {
	printf("one processor. no communication.\n");
} else {

	//printf("computing comm schedule for processor #%d...\n",ID);
	// pre kazdu uroven stromu, pocnuc najnizsou vyrata plan kompozicie (id procesora, nad/pod)
	for (int i=0; i<hlbka; i++)						// s N procesormi bude procesor komunikovat
	{
		int okolie = (int)pow(2,i);					// pre kazdu uroven sa procesor nachadza vo vzdialenost 2^uroven
		Node* t = n;
		for (int j=0; j<hlbka-i-1; j++)			// mame 2 moznosti, tu spravnu urcime traverzovanim k-D stromu
		{
			if (ID-okolie>=t->i1) // ak su obe moznosti kladne
			{
				if ( (inInterv(ID-okolie, t->back->i1, t->back->i2)) && 
						 (inInterv(ID, t->back->i1, t->back->i2)) )
					 t=t->back; else t=t->front;
			} else	// ak je jedna moznost zaporna tak testujeme len kladu
			{
				if ( (inInterv(ID+okolie, t->back->i1, t->back->i2)) && 
						 (inInterv(ID, t->back->i1, t->back->i2)) )
					 t=t->back; else t=t->front;
			}
		}
		// zisti ci kombinovat F-B alebo B-F
		CVektor3 normala;
		// vypocitame normalu pre dane rozdelenie dat
		switch (t->delenie)
		{
			case 0: normala = CVektor3(0,1,0); break;
			case 1: normala = CVektor3(1,0,0); break;
			case 2: normala = CVektor3(0,0,1); break;
		}

		// zistime s ktorym procesorom budeme komunikovat
		if (inInterv(ID-okolie, t->i1, t->i2)) {
			out[i].pid=ID-okolie;
		} else {
			out[i].pid=ID+okolie;
		}

		// v akom poradi zlozime obrazky
		float s = skalarsucin(normala,v);
		if (s<0.0f) // front-to-back usporiadanie synov, tj front je front a naopak
		{
			if (inInterv(ID, t->back->i1, t->back->i2))  { out[i].over=0; } else { out[i].over=1; }
		} else			// back-to-front
		{
			if (inInterv(ID, t->back->i1, t->back->i2))  { out[i].over=1; } else { out[i].over=0; }
		}
	}
} // if hlbka==0
}



void traverzuj(Node* n)  // vypise ID jednotlivych dat, traverzovanie front-to-back
{
	if ((n->front==NULL) && (n->back==NULL)) printf("Data c.%d, a: %d,%d,%d, b: %d,%d,%d\n",n->i1,   n->a.x, n->a.y, n->a.z,    n->b.x, n->b.y, n->b.z);
	else
	{
		traverzuj(n->front);
		traverzuj(n->back);
	}
}


/*
int main(int argc, char* argv[])
{
	printf("k-D Tree Test\n");
	
	TVektor<int> a,b;
	a.x=0; a.y=0; a.z=0;
	b.x=15; b.y=31; b.z=7;

	CVektor3 kamera = CVektor3(2000,2000,2000);
	KDTree tree = KDTree(a,b,2,0);
	
	traverzuj(tree.root);
	for (int i=0; i<(int)pow(2,tree.hlbka); i++) vytvorPlan(i, tree.hlbka, tree.root, CVektor3(0,0,0) - kamera);

	char c=getchar();
	return 0;
}
*/
