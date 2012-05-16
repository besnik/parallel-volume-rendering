// main.cpp
// 3D Distance Volume structure for RADC algorithm
// by skip 2006
//


#include "distanceVolume.h"

const int sizeX = 5;
const int sizeY = 5;
const int sizeZ = 5;

unsigned short *p = NULL;		// input 2D image
short *dvolume = NULL;			// output
unsigned char *bunky = NULL;	// distance of the cells from objects

void init()
{
	// memory allocation
	int i,j,k;
	p = new unsigned short[sizeX*sizeY*sizeZ];
	dvolume = new short[sizeX*sizeY*sizeZ];
	bunky = new unsigned char[(sizeX-1)*(sizeY-1)*(sizeZ-1)];

	
	// initialization of inner values
	for (k=0; k<sizeZ; k++)
	for (j=0; j<sizeY; j++)
	for (i=0; i<sizeX; i++) {
		if ((i>0) && (i<3) && (j>0) && (j<3) && (k>0) && (k<3)) p[dv_getIndex(i,j,k, sizeX,sizeY)]=3;
		else p[dv_getIndex(i,j,k, sizeX,sizeY)]=0;
	}
}



void dispose()
{
	if (p!=NULL) {
		delete[] p;
		p=NULL;
	}

	if (dvolume!=NULL) {
		delete[] dvolume;
		dvolume=NULL;
	}

	if (bunky!=NULL) {
		delete[] bunky;
		bunky=NULL;
	}
}



int main(int argc, char* argv[])
{
	init();
	
	dv_print3d(p, sizeX,sizeY,sizeZ);

	dv_init3x3x3(p,dvolume, sizeX,sizeY,sizeZ);

	dv_compute3d(dvolume, sizeX, sizeY, sizeZ);
	cout << "=================" << endl;
	dv_print3d(dvolume, sizeX,sizeY,sizeZ);

	dv_celldist3d(dvolume, sizeX, sizeY, sizeZ, bunky);
	cout << "=================" << endl;
	dv_print3d(bunky, sizeX-1,sizeY-1,sizeZ-1);

	dispose();
	char c=getchar();
	return 0;
}