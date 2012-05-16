// distanceVolume.h
// Distance Volume 3D (DV) structure for RADC algorithm
// by skip 2006
// version 1.1

#include <iostream>
#include <iomanip> // I/O Manipulators

using namespace std;

#define INFINITY 255 // 32767

int dv_getIndex(int i, int j, int k, int sizex, int sizey);				// gets index into 1D representation of 3-D array
void dv_init3x3x3(unsigned short* in, short* out, int x, int y, int z);	// initialize DV for template 3x3x3, necessary to run before compute!
void dv_compute3d(short* p, int x, int y, int z);						// calculates Distance Volume 3D
void dv_celldist3d(short* dv, int x, int y, int z, unsigned char *p);	// calculates distance of cell from objects

template <class T> void dv_print3d(T *p, int x, int y, int z)			// prints values of array
{
	for (int k=0; k<z; k++) {
		for (int j=y-1; j>=0; j--) {
			for (int i=0; i<x; i++) { std::cout << setw(4) <<		(int)p[dv_getIndex(i,j,k, x,y)] << " "; }
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

