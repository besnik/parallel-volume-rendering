// main.cpp
// Distance Volume structure for RADC algorithm
// by skip 2006
//


#include "distanceVolume.h"

const int sizeX = 8;
const int sizeY = 8;

short **p=NULL; // input 2D obrazok
short **dvolume=NULL; // output
unsigned char **bunky=NULL; // distance of cells from the objects


void init()
{
	int i,j;

	// memory allocation
	p = new short*[sizeX];
	for (i=0; i<sizeX; i++) p[i]=new short[sizeY];

	dvolume = new short*[sizeX];
	for (i=0; i<sizeX; i++) dvolume[i]=new short[sizeY];

	bunky = new unsigned char*[sizeX-1];
	for (i=0; i<sizeX-1; i++) bunky[i]=new unsigned char[sizeY-1];

	// initialization of inner values
	for (j=0; j<sizeY; j++)
		for (i=0; i<sizeX; i++) {
			if ((i>4) && (i<7) && (j>0) && (j<3)) p[i][j]=1; else p[i][j]=0;
		}
}



void dispose()
{
	if (p!=NULL) {
		for (int i=0; i<sizeX; i++) if (p[i]!=NULL) { delete[] p[i]; p[i]=NULL; }
		delete[] p;
		p=NULL;
	}

	if (dvolume!=NULL) {
		for (int i=0; i<sizeX; i++) if (dvolume[i]!=NULL) { delete[] dvolume[i]; dvolume[i]=NULL; }
		delete[] dvolume;
		dvolume=NULL;
	}

	if (bunky!=NULL) {
		for (int i=0; i<sizeX-1; i++) if (bunky[i]!=NULL) { delete[] bunky[i]; bunky[i]=NULL; }
		delete[] bunky;
		bunky=NULL;
	}

}



int main(int argc, char* argv[])
{
	init();
	
	dv_init3x3(p,dvolume, sizeX,sizeY);
	dv_compute(dvolume, sizeX, sizeY);
	dv_print(dvolume, sizeX, sizeY);
	
	cout << endl;
	
	dv_celldist(dvolume, sizeX, sizeY, bunky);
	dv_print(bunky, sizeX-1, sizeY-1);

	dispose();
	char c=getchar();
	return 0;
}